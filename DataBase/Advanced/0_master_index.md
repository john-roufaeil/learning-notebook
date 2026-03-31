# 🗂️ DB ADVANCED CONCEPTS — MASTER INDEX

Quick-reference index for all notes. Each topic has its own file.

---

## Files in This Set

| # | File | Topic | One-liner |
|---|------|-------|-----------|
| 01 | `01_views.md` | **Views** | Virtual tables / stored queries |
| 02 | `02_functions.md` | **Functions** | Named reusable logic that returns a value |
| 03 | `03_procedures.md` | **Procedures** | Named logic with transaction control |
| 04 | `04_ctes.md` | **CTEs** | Named temp result sets; enables recursion |
| 05 | `05_transactions.md` | **Transactions** | All-or-nothing units of work |
| 06 | `06_acid.md` | **ACID** | The four guarantees of a reliable DB |
| 07 | `07_triggers.md` | **Triggers** | Auto-executed code on table events |
| 08 | `08_indexing.md` | **Indexing** | Speed up reads with data structures |
| 09 | `09_window_functions.md` | **Window Functions** | Row calculations without collapsing groups |
| 10 | `10_query_optimization.md` | **Query Optimization** | EXPLAIN, stats, plan tuning |
| 11 | `11_normalization.md` | **Normalization** | 1NF → BCNF, reduce redundancy |
| 12 | `12_locking_concurrency.md` | **Locking & Concurrency** | MVCC, row locks, deadlocks |
| 13 | `13_partitioning.md` | **Partitioning** | Split large tables into physical pieces |

---

## Ultra-Quick Recall Cheatsheet

### Views
```sql
CREATE [OR REPLACE] VIEW v AS SELECT ...;
CREATE MATERIALIZED VIEW mv AS SELECT ...;
REFRESH MATERIALIZED VIEW mv;
```

### Functions
```sql
CREATE OR REPLACE FUNCTION f(x int) RETURNS int LANGUAGE plpgsql AS $$ BEGIN RETURN x*2; END; $$;
SELECT f(5);
```

### Procedures
```sql
CREATE OR REPLACE PROCEDURE p(x int) LANGUAGE plpgsql AS $$ BEGIN ...; COMMIT; END; $$;
CALL p(5);
```

### CTEs
```sql
WITH cte AS (SELECT ...), cte2 AS (SELECT ... FROM cte)
SELECT * FROM cte2;

WITH RECURSIVE r AS (SELECT ... UNION ALL SELECT ... FROM r WHERE ...) SELECT * FROM r;
```

### Transactions
```sql
BEGIN; ... COMMIT;   -- or ROLLBACK;
SAVEPOINT sp; ROLLBACK TO sp; RELEASE SAVEPOINT sp;
SELECT ... FOR UPDATE [NOWAIT | SKIP LOCKED];
```

### ACID
```
A = Atomicity   → all or nothing (WAL)
C = Consistency → constraints always hold
I = Isolation   → concurrent txns don't interfere (MVCC + locks)
D = Durability  → committed data survives crash (WAL flush)
```

### Triggers
```sql
CREATE FUNCTION f() RETURNS TRIGGER AS $$ BEGIN RETURN NEW; END; $$ LANGUAGE plpgsql;
CREATE TRIGGER t BEFORE|AFTER INSERT|UPDATE|DELETE ON tbl FOR EACH ROW EXECUTE FUNCTION f();
-- Special vars: NEW, OLD, TG_OP, TG_TABLE_NAME
```

### Indexing
```sql
CREATE INDEX idx ON tbl (col);               -- B-tree (default)
CREATE UNIQUE INDEX idx ON tbl (col);
CREATE INDEX idx ON tbl (col) WHERE active;  -- partial
CREATE INDEX idx ON tbl (LOWER(col));        -- expression
CREATE INDEX idx ON tbl USING GIN (jsonb_col); -- JSONB/arrays
CREATE INDEX CONCURRENTLY idx ON tbl (col);  -- no lock
```

### Window Functions
```sql
ROW_NUMBER() / RANK() / DENSE_RANK() OVER (PARTITION BY x ORDER BY y)
SUM(col) OVER (ORDER BY date ROWS BETWEEN 6 PRECEDING AND CURRENT ROW)
LAG(col, 1, default) / LEAD(col, 1) OVER (ORDER BY ...)
```

### Query Optimization
```sql
EXPLAIN ANALYZE SELECT ...;
EXPLAIN (ANALYZE, BUFFERS) SELECT ...;
ANALYZE table_name;  -- update stats
```

### Normalization
```
1NF: atomic values, no repeating groups
2NF: no partial deps (non-key depends on WHOLE PK)
3NF: no transitive deps (non-key → non-key)
BCNF: every FD determinant is a superkey
```

### Locking
```sql
SELECT ... FOR UPDATE;           -- exclusive row lock
SELECT ... FOR UPDATE NOWAIT;    -- fail if locked
SELECT ... FOR UPDATE SKIP LOCKED; -- skip locked rows
SELECT pg_advisory_lock(id);    -- advisory lock
SELECT pg_advisory_unlock(id);
```

### Partitioning
```sql
CREATE TABLE t (...) PARTITION BY RANGE|LIST|HASH (col);
CREATE TABLE t_2024 PARTITION OF t FOR VALUES FROM ('2024-01-01') TO ('2025-01-01');
ALTER TABLE t DETACH PARTITION t_2023;  -- for archiving
DROP TABLE t_2023;                       -- fast delete
```

---

## Common Patterns Quick Lookup

| Goal | Pattern | File |
|------|---------|------|
| Rank rows within group | `ROW_NUMBER() OVER (PARTITION BY ... ORDER BY ...)` | 09 |
| Top N per group | ROW_NUMBER in CTE, filter `WHERE rn <= N` | 09, 04 |
| Running total | `SUM(x) OVER (ORDER BY date)` | 09 |
| Moving average | `AVG(x) OVER (ORDER BY date ROWS BETWEEN N PRECEDING AND CURRENT ROW)` | 09 |
| Month-over-month | `LAG(value) OVER (ORDER BY month)` | 09 |
| Deduplicate, keep latest | ROW_NUMBER() OVER (PARTITION BY id ORDER BY created_at DESC) WHERE rn=1 | 09 |
| Hierarchy/tree traversal | Recursive CTE | 04 |
| Auto-set updated_at | BEFORE UPDATE trigger | 07 |
| Audit trail | AFTER INSERT/UPDATE/DELETE trigger → audit table | 07 |
| Fast job queue | `FOR UPDATE SKIP LOCKED LIMIT N` | 05, 12 |
| Archive old data fast | Partition + DETACH + DROP | 13 |
| Case-insensitive search | Expression index on LOWER(col) | 08 |
| JSONB search | GIN index + `@>` operator | 08 |
| Prevent concurrent duplicates | Advisory lock or unique index | 12 |