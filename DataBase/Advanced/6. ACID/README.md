# 🔒 ACID

## What is it?
Four properties that **guarantee database transactions are processed reliably**. ACID is the contract a database makes to protect data integrity.

---

## The Four Properties

### A — Atomicity
> **"All or nothing"**

A transaction is treated as a single unit. If any part fails, the **entire transaction is rolled back**. There is no partial commit.

```
Transfer $500: debit account A + credit account B
→ If credit fails → debit is also undone
→ You never get a state where only one happened
```

**How it's implemented:** Write-Ahead Logging (WAL) / undo logs. On crash, DB replays or undoes incomplete transactions.

---

### C — Consistency
> **"Data moves from one valid state to another"**

A transaction must leave the database in a **valid state** — all constraints, rules, and invariants must hold before and after.

```
Constraint: account balance >= 0
→ A transaction that would make balance negative is rejected
→ Consistency is enforced by: CHECK constraints, FK constraints, triggers, app logic
```

**Note:** Consistency is partly the DB's job (constraints) and partly the **application's job** (business rules).

---

### I — Isolation
> **"Concurrent transactions don't interfere with each other"**

Transactions executing concurrently produce the same result as if they ran **serially** (one after another). Controlled by **isolation levels**.

```
T1: reads balance (100), plans to deduct 50
T2: reads balance (100), plans to deduct 80
Without isolation → both could "succeed" → balance = -30  ❌
With isolation → one wins, one retries or fails
```

**How it's implemented:** Locks, MVCC (Multi-Version Concurrency Control)

**MVCC (PostgreSQL):**
- Each transaction sees a **snapshot** of the DB at its start time
- Writers don't block readers, readers don't block writers
- Old versions of rows are kept until no transaction needs them (vacuum cleans them)

---

### D — Durability
> **"Committed data survives crashes"**

Once a transaction is **committed**, it is permanently saved even if the system crashes immediately after.

```
COMMIT; → power goes out
→ After restart, the committed data is still there
```

**How it's implemented:** Write-Ahead Log (WAL)
- Changes are written to WAL on disk **before** being written to data files
- On crash recovery, WAL is replayed to restore committed transactions

---

## ACID Summary Table

| Property | Guarantee | Broken by |
|----------|-----------|-----------|
| **Atomicity** | All or nothing | App crash mid-transaction |
| **Consistency** | Valid state always | Bad constraints, app bugs |
| **Isolation** | No interference | Weak isolation level |
| **Durability** | Commits survive crashes | Disk failure (mitigated by replication) |

---

## ACID vs BASE

NoSQL systems often trade ACID for **BASE** (eventual consistency):

| ACID | BASE |
|------|------|
| Strong consistency | **B**asically **A**vailable |
| Transactions guaranteed | **S**oft state |
| Isolation enforced | **E**ventual consistency |
| PostgreSQL, MySQL | Cassandra, DynamoDB, MongoDB (partially) |

---

## How PostgreSQL Implements ACID

| Property | Mechanism |
|----------|-----------|
| Atomicity | WAL + rollback segments |
| Consistency | Constraints, triggers, FK checks |
| Isolation | MVCC + lock manager |
| Durability | WAL flushed to disk before COMMIT returns |

```sql
-- WAL-related settings (postgresql.conf)
wal_level = replica          -- minimal, replica, logical
synchronous_commit = on      -- ensures WAL flushed before COMMIT
fsync = on                   -- ensures data actually hits disk
```

---

## Key Notes

- **Isolation is the most nuanced** — full serializability is expensive, so most systems default to `READ COMMITTED`
- MVCC makes **reads fast and non-blocking** — PostgreSQL's key performance feature
- **Durability cost**: `synchronous_commit = off` gives speed but risks data loss on crash (last ~few ms of commits)
- `fsync = off` — NEVER do in production (data corruption risk)
- ACID violations are subtle — test under concurrent load