# 🔐 LOCKING & CONCURRENCY

## What is it?
Mechanisms that control how **concurrent transactions access shared data** to prevent conflicts and ensure consistency.

---

## Lock Types (PostgreSQL)

### Table-Level Locks (weakest → strongest)

| Lock | Acquired by | Blocks |
|------|-------------|--------|
| `ACCESS SHARE` | `SELECT` | ACCESS EXCLUSIVE only |
| `ROW SHARE` | `SELECT FOR UPDATE/SHARE` | EXCLUSIVE, ACCESS EXCLUSIVE |
| `ROW EXCLUSIVE` | `INSERT`, `UPDATE`, `DELETE` | SHARE, SHARE ROW EXCLUSIVE, EXCLUSIVE, ACCESS EXCLUSIVE |
| `SHARE UPDATE EXCLUSIVE` | `VACUUM`, `ANALYZE`, `CREATE INDEX CONCURRENTLY` | Itself + heavier locks |
| `SHARE` | `CREATE INDEX` | ROW EXCLUSIVE and heavier |
| `SHARE ROW EXCLUSIVE` | Rarely used manually | SHARE and heavier |
| `EXCLUSIVE` | Rare | All except ACCESS SHARE |
| `ACCESS EXCLUSIVE` | `DROP TABLE`, `TRUNCATE`, `ALTER TABLE` | **Everything** |

> ⚠️ `ALTER TABLE` takes `ACCESS EXCLUSIVE` — it blocks even `SELECT`. Plan maintenance windows carefully.

### Row-Level Locks

```sql
FOR UPDATE          -- exclusive row lock, blocks other writers
FOR NO KEY UPDATE   -- weaker exclusive, doesn't block FK checks
FOR SHARE           -- shared lock, blocks writers, allows readers
FOR KEY SHARE       -- weakest shared, allows FK updates
```

---

## Row-Level Locking Syntax

```sql
-- Lock rows for update (blocks other transactions from updating same rows)
SELECT * FROM orders WHERE status = 'pending' FOR UPDATE;

-- Don't wait — fail immediately if lock unavailable
SELECT * FROM orders WHERE id = 1 FOR UPDATE NOWAIT;

-- Skip locked rows — great for parallel job workers
SELECT * FROM jobs WHERE status = 'queued'
ORDER BY created_at
FOR UPDATE SKIP LOCKED
LIMIT 10;

-- Share lock — allow other reads, block writes
SELECT * FROM accounts WHERE id = 1 FOR SHARE;
```

---

## MVCC (Multi-Version Concurrency Control)

PostgreSQL's approach to concurrency — **no read/write conflicts**.

```
Key Idea:
- Writers create NEW versions of rows (don't overwrite)
- Readers see a SNAPSHOT from transaction start
- Dead versions cleaned up by VACUUM

Result:
- Readers never block writers ✅
- Writers never block readers ✅
- Only write/write conflicts need locking
```

### Snapshot Isolation

```sql
-- T1 starts, sees snapshot at time T
BEGIN;
SET TRANSACTION ISOLATION LEVEL REPEATABLE READ;
SELECT * FROM accounts;  -- sees snapshot

-- T2 commits new data while T1 is open
-- T1 still sees old snapshot
SELECT * FROM accounts;  -- same result as before

COMMIT;
```

---

## Deadlocks

A **deadlock** occurs when two transactions each hold a lock the other needs.

```
T1: locks row A, waits for row B
T2: locks row B, waits for row A
→ Neither can proceed → deadlock
```

PostgreSQL **detects deadlocks automatically** and kills one transaction:
```
ERROR: deadlock detected
DETAIL: Process 1234 waits for ShareLock on transaction 5678;
        blocked by process 5678.
        Process 5678 waits for ShareLock on transaction 1234;
        blocked by process 1234.
```

**Prevention strategies:**
```sql
-- Always lock resources in the SAME ORDER across transactions
-- T1 and T2 both: lock A first, then B

-- Use shorter transactions
-- Lock all needed rows at once
SELECT * FROM accounts
WHERE id IN (1, 2) ORDER BY id FOR UPDATE;
--           ↑ order by id ensures consistent lock order
```

---

## Advisory Locks

Application-level locks not tied to any table row.

```sql
-- Session-level (held until released or session ends)
SELECT pg_advisory_lock(12345);        -- exclusive
SELECT pg_advisory_lock_shared(12345); -- shared
SELECT pg_advisory_unlock(12345);
SELECT pg_advisory_unlock_all();

-- Transaction-level (auto-released at COMMIT/ROLLBACK)
SELECT pg_advisory_xact_lock(12345);

-- Non-blocking (returns false if can't acquire)
SELECT pg_try_advisory_lock(12345);
SELECT pg_try_advisory_xact_lock(12345);
```

**Use case:** Ensuring only one worker runs a specific job, distributed locking.

```sql
-- Job queue: one worker per job type
BEGIN;
IF pg_try_advisory_xact_lock(job_type_id) THEN
    -- process job
    UPDATE jobs SET status = 'done' WHERE ...;
END IF;
COMMIT;  -- advisory lock auto-released
```

---

## Monitoring Locks

```sql
-- See all locks
SELECT pid, locktype, relation::regclass, mode, granted
FROM pg_locks;

-- Find blocking chains
SELECT
    blocked.pid AS blocked_pid,
    blocked.query AS blocked_query,
    blocking.pid AS blocking_pid,
    blocking.query AS blocking_query
FROM pg_stat_activity blocked
JOIN pg_stat_activity blocking
    ON blocking.pid = ANY(pg_blocking_pids(blocked.pid))
WHERE blocked.wait_event_type = 'Lock';

-- Kill a blocking process
SELECT pg_terminate_backend(pid);
SELECT pg_cancel_backend(pid);  -- softer: cancel current query only
```

---

## Optimistic vs Pessimistic Locking

| | Pessimistic | Optimistic |
|--|-------------|------------|
| **How** | Lock rows on read | Detect conflict on write |
| **Use when** | High contention, must prevent conflicts | Low contention, most ops succeed |
| **SQL** | `SELECT FOR UPDATE` | Check version/timestamp on UPDATE |
| **Cost** | Locks held longer | Possible retry on conflict |

```sql
-- Optimistic locking with version column
-- Read
SELECT id, balance, version FROM accounts WHERE id = 1;

-- Write: only succeeds if version hasn't changed
UPDATE accounts
SET balance = 950, version = version + 1
WHERE id = 1 AND version = 5;   -- ← check version

-- If 0 rows affected → someone else updated it → retry
```

---

## Key Notes

- **Lock contention** is a major performance bottleneck — keep transactions short
- `FOR UPDATE SKIP LOCKED` is the standard pattern for **parallel queue workers**
- MVCC dead tuples cause **table bloat** — regular `VACUUM` is essential
- `VACUUM FULL` reclaims disk space but takes `ACCESS EXCLUSIVE` lock — avoid in production
- `autovacuum` runs automatically — tune aggressively for write-heavy tables
- Deadlocks are rare if you: keep transactions short, lock in consistent order, avoid long-held locks