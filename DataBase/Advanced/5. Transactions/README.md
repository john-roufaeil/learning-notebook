# 💳 TRANSACTIONS

## What is it?
A **unit of work** that groups one or more SQL operations into an all-or-nothing block. Either everything succeeds (`COMMIT`) or everything is undone (`ROLLBACK`).

---

## Core Commands

```sql
BEGIN;              -- start transaction (also: START TRANSACTION)
COMMIT;             -- save all changes permanently
ROLLBACK;           -- undo all changes since BEGIN
SAVEPOINT sp_name;  -- create a checkpoint within a transaction
ROLLBACK TO sp_name;-- undo to savepoint (transaction still open)
RELEASE SAVEPOINT sp_name;  -- remove savepoint

-- PostgreSQL also accepts:
BEGIN TRANSACTION;
END;  -- alias for COMMIT
```

---

## Basic Flow

```sql
BEGIN;

UPDATE accounts SET balance = balance - 500 WHERE id = 1;
UPDATE accounts SET balance = balance + 500 WHERE id = 2;

-- If both succeed:
COMMIT;

-- If something went wrong:
ROLLBACK;
```

---

## Savepoints

```sql
BEGIN;

INSERT INTO orders VALUES (1, 'Item A');
SAVEPOINT after_first_insert;

INSERT INTO orders VALUES (2, 'Item B');
-- Oops, item B was wrong
ROLLBACK TO after_first_insert;   -- undoes only Item B

INSERT INTO orders VALUES (2, 'Item C');
COMMIT;  -- commits Item A + Item C
```

---

## Autocommit

- By default, most DB clients run in **autocommit mode** — each statement is its own transaction
- `BEGIN` disables autocommit for that block
- In **psql**: autocommit is ON by default
- In **application code**: depends on driver/ORM settings

```sql
-- PostgreSQL: check/set autocommit in psql
\set AUTOCOMMIT off
\set AUTOCOMMIT on
```

---

## Isolation Levels

Controls what a transaction can **see** from other concurrent transactions.

```sql
-- Set for current transaction
BEGIN;
SET TRANSACTION ISOLATION LEVEL READ COMMITTED;

-- Set for session
SET SESSION CHARACTERISTICS AS TRANSACTION ISOLATION LEVEL SERIALIZABLE;
```

| Level | Dirty Read | Non-Repeatable Read | Phantom Read |
|-------|-----------|---------------------|--------------|
| `READ UNCOMMITTED` | ✅ possible | ✅ possible | ✅ possible |
| `READ COMMITTED` *(PG default)* | ❌ prevented | ✅ possible | ✅ possible |
| `REPEATABLE READ` | ❌ | ❌ prevented | ✅ possible (❌ in PG) |
| `SERIALIZABLE` | ❌ | ❌ | ❌ all prevented |

### Anomaly Definitions

| Anomaly | What happens |
|---------|-------------|
| **Dirty Read** | Reading uncommitted changes from another transaction |
| **Non-Repeatable Read** | Reading same row twice, getting different values (another tx committed between reads) |
| **Phantom Read** | Re-running a query returns different rows (another tx inserted/deleted rows) |
| **Serialization Anomaly** | Concurrent txns produce result impossible in any serial order |

---

## Transaction States

```
IDLE → BEGIN → ACTIVE → COMMIT → IDLE
                      ↘ ROLLBACK ↗
                      → ABORTED (error occurred, must ROLLBACK)
```

> ⚠️ In PostgreSQL, once a transaction hits an error, it enters **ABORTED** state. All further commands are rejected until you `ROLLBACK`.

---

## Locking Within Transactions

```sql
-- Explicit row lock (block other writers)
SELECT * FROM accounts WHERE id = 1 FOR UPDATE;

-- Lock without waiting
SELECT * FROM accounts WHERE id = 1 FOR UPDATE NOWAIT;

-- Skip locked rows (queue-like processing)
SELECT * FROM jobs WHERE status = 'pending'
FOR UPDATE SKIP LOCKED
LIMIT 10;

-- Share lock (allow reads, block writes)
SELECT * FROM accounts WHERE id = 1 FOR SHARE;
```

---

## Key Notes

- **Never leave transactions open** — locks are held until COMMIT/ROLLBACK
- Long-running transactions cause **lock contention** and bloat (PostgreSQL: dead tuples)
- `FOR UPDATE SKIP LOCKED` is the pattern for **job queues** / parallel workers
- Nested transactions → use **savepoints**, not nested `BEGIN`
- In PostgreSQL, DDL (`CREATE TABLE`, `ALTER TABLE`) is **transactional** — can be rolled back
- Always handle exceptions and rollback in application code:

```python
# Python example
try:
    conn.autocommit = False
    cur.execute("UPDATE ...")
    cur.execute("INSERT ...")
    conn.commit()
except Exception:
    conn.rollback()
    raise
```