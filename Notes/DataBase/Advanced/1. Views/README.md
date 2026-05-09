# 📋 VIEWS

## What is it?
A **virtual table** based on a SELECT query. Stores the query definition, not the data (unless materialized). Acts like a table but dynamically fetches data each time it's queried.

---

## Types

| Type | Description |
|------|-------------|
| **Simple View** | Based on one table, no aggregation — usually updatable |
| **Complex View** | Joins, aggregations, subqueries — NOT updatable |
| **Materialized View** | Physically stores result on disk, must be refreshed |

---

## Core Syntax

```sql
-- Create
CREATE VIEW view_name AS
SELECT col1, col2 FROM table WHERE condition;

-- Create or replace
CREATE OR REPLACE VIEW view_name AS
SELECT ...;

-- Drop
DROP VIEW view_name;
DROP VIEW IF EXISTS view_name;

-- See definition (PostgreSQL)
\d+ view_name
SELECT definition FROM pg_views WHERE viewname = 'view_name';
```

---

## Materialized Views (PostgreSQL)

```sql
-- Create
CREATE MATERIALIZED VIEW mat_view AS
SELECT department, COUNT(*), AVG(salary)
FROM employees
GROUP BY department;

-- Refresh (re-run the query)
REFRESH MATERIALIZED VIEW mat_view;

-- Refresh without locking reads
REFRESH MATERIALIZED VIEW CONCURRENTLY mat_view;

-- Drop
DROP MATERIALIZED VIEW mat_view;
```

> ⚠️ `CONCURRENTLY` requires a **unique index** on the materialized view.

---

## Updatable Views

A view is updatable when:
- Based on a **single table**
- No `DISTINCT`, `GROUP BY`, `HAVING`, `UNION`, `LIMIT`
- No aggregate functions
- No subqueries in SELECT

```sql
-- This view IS updatable
CREATE VIEW active_employees AS
SELECT id, name, salary FROM employees WHERE active = true;

UPDATE active_employees SET salary = 90000 WHERE id = 5;

-- WITH CHECK OPTION — prevents updates that would make row invisible to view
CREATE VIEW active_employees AS
SELECT * FROM employees WHERE active = true
WITH CHECK OPTION;
```

---

## Key Notes

- Views **do not store data** (except materialized)
- Every query on a view **re-executes** the underlying query
- Use views for: **security** (hide columns), **simplification** (complex joins), **consistency** (shared logic)
- Materialized views trade **freshness** for **speed**
- In PostgreSQL, you can `CREATE INDEX` on materialized views
- `WITH CHECK OPTION` enforces view filter on INSERT/UPDATE
- Views can be **nested** (view on a view) — avoid deep nesting, kills performance

---

## Quick Mental Model

```
Regular View  → alias for a query, runs fresh every time
Materialized  → snapshot stored on disk, must be manually refreshed
Updatable     → passthrough to base table (simple views only)
```