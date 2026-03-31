# 🔍 QUERY OPTIMIZATION & EXPLAIN

## What is it?
The process of making SQL queries run faster by understanding how the **query planner** executes them and removing bottlenecks.

---

## EXPLAIN

```sql
-- Show execution plan (estimates only, doesn't run query)
EXPLAIN SELECT * FROM employees WHERE salary > 80000;

-- Run query AND show actual timing + row counts
EXPLAIN ANALYZE SELECT * FROM employees WHERE salary > 80000;

-- Full detail: buffers, memory
EXPLAIN (ANALYZE, BUFFERS, FORMAT TEXT) SELECT ...;

-- JSON output (for tools)
EXPLAIN (ANALYZE, FORMAT JSON) SELECT ...;
```

---

## Reading EXPLAIN Output

```
Seq Scan on employees  (cost=0.00..458.00 rows=100 width=64)
                             ↑          ↑      ↑         ↑
                         startup    total   estimated  avg row
                           cost     cost     rows      size

-- With ANALYZE:
(actual time=0.023..1.234 rows=97 loops=1)
                  ↑         ↑       ↑        ↑
               start      end    actual    times
               time      time     rows    executed
```

**Key metrics:**
- `cost`: arbitrary planner units (not ms). Relative comparison only.
- `rows`: estimated row count — if wildly off → run `ANALYZE` on table
- `actual time`: milliseconds (with ANALYZE only)
- `loops`: how many times this node ran

---

## Node Types to Know

| Node | Meaning |
|------|---------|
| `Seq Scan` | Full table scan — fine for small tables |
| `Index Scan` | Uses index, fetches rows from heap |
| `Index Only Scan` | Uses index only — no heap access (fastest) |
| `Bitmap Heap Scan` | Uses index bitmap, then fetches in block order |
| `Nested Loop` | Join: for each outer row, scan inner (good for small sets) |
| `Hash Join` | Builds hash table from one side (good for larger sets) |
| `Merge Join` | Both sides sorted, merge them (good for large sorted sets) |
| `Sort` | Explicit sort — look for this on large datasets |
| `Hash Aggregate` | GROUP BY using hash table |
| `Limit` | Stops after N rows |

---

## Statistics & Planner

```sql
-- Update table statistics (planner uses these)
ANALYZE table_name;
ANALYZE;  -- all tables

-- Check when last analyzed
SELECT relname, last_analyze, last_autoanalyze, n_live_tup, n_dead_tup
FROM pg_stat_user_tables;

-- Increase stats target for column with high cardinality
ALTER TABLE employees ALTER COLUMN department SET STATISTICS 500;
-- Default is 100. Higher = better estimates, more ANALYZE time.
```

---

## Query Optimization Techniques

### 1. Use indexes appropriately

```sql
-- ❌ Function on indexed column kills index
WHERE UPPER(name) = 'BOB'
-- ✅ Expression index or rewrite
WHERE name = 'Bob'  -- or CREATE INDEX ON (UPPER(name))
```

### 2. SELECT only needed columns

```sql
-- ❌ Fetches everything, blocks index-only scans
SELECT * FROM orders WHERE status = 'pending';
-- ✅ Enables index-only scan if columns are covered
SELECT id, created_at FROM orders WHERE status = 'pending';
```

### 3. Avoid implicit type casting

```sql
-- ❌ id is integer but compared to string → cast → no index
WHERE id = '123'
-- ✅
WHERE id = 123
```

### 4. Use LIMIT with ORDER BY

```sql
-- With proper index, this is extremely fast
SELECT * FROM events ORDER BY created_at DESC LIMIT 20;
```

### 5. Optimize JOINs

```sql
-- Join on indexed foreign keys
-- Filter BEFORE joining when possible (use subquery/CTE)
-- Use HASH JOIN hint for large tables (optimizer usually figures it out)
```

### 6. EXISTS vs IN vs JOIN

```sql
-- EXISTS is often faster for "does it exist" checks
SELECT * FROM orders o
WHERE EXISTS (SELECT 1 FROM payments p WHERE p.order_id = o.id);

-- IN with subquery (bad if subquery returns NULLs)
WHERE id IN (SELECT id FROM ...)

-- JOIN for when you need columns from both tables
```

### 7. Avoid SELECT in loop (N+1)

```sql
-- ❌ N+1: one query per row
FOR each_dept IN departments:
    SELECT * FROM employees WHERE department = each_dept

-- ✅ One query
SELECT d.name, e.* FROM departments d JOIN employees e ON e.department = d.name;
```

---

## Planner Config (for debugging)

```sql
-- Disable specific scan types to test alternatives
SET enable_seqscan = off;
SET enable_indexscan = off;
SET enable_hashjoin = off;
SET enable_nestloop = off;

-- Don't leave these off in production!
RESET enable_seqscan;
```

---

## Useful Diagnostic Queries

```sql
-- Slowest queries (requires pg_stat_statements)
SELECT query, mean_exec_time, calls, total_exec_time
FROM pg_stat_statements
ORDER BY mean_exec_time DESC
LIMIT 20;

-- Table sizes
SELECT relname, pg_size_pretty(pg_total_relation_size(relid)) AS total_size
FROM pg_stat_user_tables
ORDER BY pg_total_relation_size(relid) DESC;

-- Cache hit ratio (should be > 95%)
SELECT sum(heap_blks_hit) / (sum(heap_blks_hit) + sum(heap_blks_read)) AS cache_hit_ratio
FROM pg_statio_user_tables;

-- Long running queries
SELECT pid, now() - pg_stat_activity.query_start AS duration, query
FROM pg_stat_activity
WHERE state = 'active' AND now() - query_start > interval '5 seconds';

-- Locks and blocking
SELECT pid, locktype, relation::regclass, mode, granted
FROM pg_locks
WHERE NOT granted;
```

---

## Key Notes

- Always use `EXPLAIN ANALYZE` on real data, not empty tables
- **Estimates vs Actuals**: if rows estimates are very wrong → run `ANALYZE`
- `BUFFERS` option shows cache hits vs disk reads — high disk reads = slow
- The planner picks the plan with **lowest estimated cost**, not always the fastest real-world plan
- `pg_stat_statements` extension is essential for production query analysis
- Tools: **pgBadger**, **pgAdmin**, **explain.depesz.com**, **explain.tensor.ru** for visual EXPLAIN