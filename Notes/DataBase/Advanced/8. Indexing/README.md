# 📇 INDEXING

## What is it?
A **data structure** that allows the database to find rows quickly without scanning the entire table. Trades **write speed and storage** for **read speed**.

---

## Index Types

| Type | Best for |
|------|----------|
| **B-Tree** | Default. Equality, range, ORDER BY, LIKE 'prefix%' |
| **Hash** | Equality only (`=`). Faster than B-tree for pure equality |
| **GIN** | Full-text search, arrays, JSONB, `@>`, `&&` |
| **GiST** | Geometric types, full-text, range overlap |
| **BRIN** | Very large tables with naturally ordered data (timestamps, sequential IDs) |
| **SP-GiST** | Non-balanced structures: IP ranges, phone trees |

---

## Core Syntax

```sql
-- Basic index
CREATE INDEX idx_name ON table_name (column_name);

-- Unique index (also enforces uniqueness)
CREATE UNIQUE INDEX idx_name ON table_name (column_name);

-- Multi-column (composite) index
CREATE INDEX idx_name ON table_name (col1, col2, col3);

-- Partial index (only indexes rows matching condition)
CREATE INDEX idx_active ON employees (id) WHERE active = true;

-- Functional / expression index
CREATE INDEX idx_lower_email ON users (LOWER(email));

-- Concurrent (doesn't lock table)
CREATE INDEX CONCURRENTLY idx_name ON table_name (col);

-- Drop
DROP INDEX idx_name;
DROP INDEX CONCURRENTLY idx_name;  -- no lock
```

---

## B-Tree Index (Default)

```sql
CREATE INDEX idx_salary ON employees (salary);

-- Works with: =, <, >, <=, >=, BETWEEN, IN, IS NULL
SELECT * FROM employees WHERE salary > 80000;
SELECT * FROM employees WHERE salary BETWEEN 50000 AND 100000;
SELECT * FROM employees ORDER BY salary;  -- index scan possible

-- LIKE only works for prefix matching
SELECT * FROM employees WHERE name LIKE 'Jo%';   -- ✅ uses index
SELECT * FROM employees WHERE name LIKE '%ohn';  -- ❌ full scan
```

---

## Composite Index — Column Order Matters

```sql
CREATE INDEX idx_dept_salary ON employees (department, salary);

-- ✅ Uses index (leading column present)
WHERE department = 'Engineering'
WHERE department = 'Engineering' AND salary > 80000

-- ❌ Cannot use index (skips leading column)
WHERE salary > 80000
```

> **Rule:** The index is useful when queries filter/sort on the **leftmost prefix** of the indexed columns.

---

## Partial Index

```sql
-- Index only active users — smaller, faster
CREATE INDEX idx_active_users ON users (email) WHERE active = true;

-- Index only recent orders
CREATE INDEX idx_recent_orders ON orders (created_at)
WHERE created_at > '2024-01-01';
```

---

## Expression / Functional Index

```sql
-- Case-insensitive email lookup
CREATE INDEX idx_lower_email ON users (LOWER(email));
-- Query MUST use same expression
SELECT * FROM users WHERE LOWER(email) = 'test@example.com';  -- ✅ uses index
SELECT * FROM users WHERE email = 'test@example.com';          -- ❌ doesn't
```

---

## JSONB Indexes (GIN)

```sql
-- Index all keys in JSONB column
CREATE INDEX idx_data ON events USING GIN (data);

-- Index specific path
CREATE INDEX idx_data_type ON events USING GIN ((data -> 'type'));

-- Queries that use it
SELECT * FROM events WHERE data @> '{"type": "click"}';
SELECT * FROM events WHERE data ? 'user_id';
```

---

## Full-Text Search Index

```sql
-- GIN index on tsvector
CREATE INDEX idx_fts ON articles USING GIN (to_tsvector('english', content));

-- Or store tsvector in a column
ALTER TABLE articles ADD COLUMN tsv tsvector;
UPDATE articles SET tsv = to_tsvector('english', content);
CREATE INDEX idx_tsv ON articles USING GIN (tsv);

-- Query
SELECT * FROM articles WHERE tsv @@ to_tsquery('english', 'database & performance');
```

---

## Index Scan Types (PostgreSQL)

| Scan Type | Description |
|-----------|-------------|
| **Index Scan** | Follows index to fetch rows from heap |
| **Index Only Scan** | All needed data in index — no heap access (fastest) |
| **Bitmap Index Scan** | Builds bitmap, then fetches heap in block order (multi-condition) |
| **Sequential Scan** | Full table scan — used when index isn't helpful |

```sql
-- Force explain to see which scan is used
EXPLAIN ANALYZE SELECT * FROM employees WHERE salary > 80000;
```

---

## Covering Index (Index-Only Scan)

```sql
-- Include non-filter columns so heap access is avoided
CREATE INDEX idx_covering ON employees (department) INCLUDE (name, salary);

-- This query can be satisfied entirely from the index
SELECT name, salary FROM employees WHERE department = 'Engineering';
```

---

## Index Maintenance

```sql
-- Rebuild index (removes bloat)
REINDEX INDEX idx_name;
REINDEX TABLE table_name;
REINDEX DATABASE db_name;

-- Concurrent rebuild (pg14+)
REINDEX INDEX CONCURRENTLY idx_name;

-- Check index usage
SELECT indexrelname, idx_scan, idx_tup_read, idx_tup_fetch
FROM pg_stat_user_indexes
WHERE relname = 'employees';

-- Find unused indexes
SELECT indexrelname, idx_scan
FROM pg_stat_user_indexes
WHERE idx_scan = 0;

-- Index size
SELECT pg_size_pretty(pg_relation_size('idx_name'));
```

---

## When Indexes Are NOT Used

- Querying a small table (seq scan is cheaper)
- Non-selective column (e.g., boolean flag with 90% true)
- `LIKE '%suffix'` or `LIKE '%middle%'`
- Wrapping column in function: `WHERE UPPER(name) = 'BOB'` (unless expression index exists)
- Type mismatch: `WHERE id = '123'` when id is integer
- `OR` across different columns (use `UNION` or GIN instead)

---

## Key Notes

- Indexes speed up **reads**, slow down **writes** (INSERT/UPDATE/DELETE must maintain index)
- **Don't over-index** — every index costs maintenance on writes
- `CREATE INDEX CONCURRENTLY` — safe for production, takes longer but no table lock
- Unique indexes **enforce uniqueness** at the data level (more reliable than just constraints)
- Regularly check `pg_stat_user_indexes` for unused indexes — drop them
- PostgreSQL planner uses **statistics** (`ANALYZE`) to decide when to use indexes — run `ANALYZE` after bulk loads
- `VACUUM` reclaims dead tuples and keeps index bloat under control