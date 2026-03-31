# 🔁 CTEs (Common Table Expressions)

## What is it?
A **named temporary result set** defined with `WITH` that exists only for the duration of a single query. Makes complex queries readable and allows recursion.

---

## Basic Syntax

```sql
WITH cte_name AS (
    SELECT ...
),
another_cte AS (
    SELECT ... FROM cte_name   -- can reference previous CTEs
)
SELECT * FROM another_cte;
```

---

## Types

| Type | Description |
|------|-------------|
| **Regular CTE** | Named subquery, improves readability |
| **Recursive CTE** | Calls itself, used for hierarchies/graphs |
| **Materialized CTE** | Forced to execute once and cache (PostgreSQL 12+) |

---

## Regular CTE Examples

```sql
-- Readable alternative to nested subqueries
WITH dept_avg AS (
    SELECT department, AVG(salary) AS avg_salary
    FROM employees
    GROUP BY department
),
high_earners AS (
    SELECT e.name, e.salary, d.avg_salary
    FROM employees e
    JOIN dept_avg d ON e.department = d.department
    WHERE e.salary > d.avg_salary * 1.2
)
SELECT * FROM high_earners ORDER BY salary DESC;


-- CTE with DML (writeable CTEs)
WITH updated AS (
    UPDATE employees
    SET salary = salary * 1.1
    WHERE performance_score > 90
    RETURNING id, name, salary
)
SELECT * FROM updated;   -- see what was updated
```

---

## Recursive CTE

```sql
WITH RECURSIVE cte_name AS (
    -- 1. Base case (anchor): non-recursive, runs once
    SELECT ...

    UNION ALL   -- or UNION

    -- 2. Recursive case: references cte_name itself
    SELECT ... FROM cte_name WHERE stop_condition
)
SELECT * FROM cte_name;
```

### Employee Hierarchy Example

```sql
WITH RECURSIVE org_chart AS (
    -- Anchor: start from top (no manager)
    SELECT id, name, manager_id, 1 AS level
    FROM employees
    WHERE manager_id IS NULL

    UNION ALL

    -- Recursive: find direct reports
    SELECT e.id, e.name, e.manager_id, oc.level + 1
    FROM employees e
    JOIN org_chart oc ON e.manager_id = oc.id
)
SELECT level, name FROM org_chart ORDER BY level, name;
```

### Number Series

```sql
WITH RECURSIVE nums AS (
    SELECT 1 AS n
    UNION ALL
    SELECT n + 1 FROM nums WHERE n < 100
)
SELECT * FROM nums;
```

### Path Tracking (avoid cycles)

```sql
WITH RECURSIVE paths AS (
    SELECT id, name, ARRAY[id] AS path
    FROM nodes WHERE id = 1

    UNION ALL

    SELECT n.id, n.name, p.path || n.id
    FROM nodes n
    JOIN paths p ON n.parent_id = p.id
    WHERE NOT n.id = ANY(p.path)   -- cycle prevention
)
SELECT * FROM paths;
```

---

## Materialization Control (PostgreSQL 12+)

```sql
-- Force CTE to execute once and store result
WITH cte AS MATERIALIZED (
    SELECT expensive_computation()
)
SELECT * FROM cte JOIN cte c2 ON ...;  -- uses cached result both times


-- Force inline (let optimizer fold it into main query)
WITH cte AS NOT MATERIALIZED (
    SELECT * FROM employees
)
SELECT * FROM cte WHERE department = 'HR';
```

> ⚠️ In PostgreSQL < 12, CTEs were **always materialized** (optimization fence). In 12+, the optimizer decides unless you specify.

---

## CTE vs Subquery vs Temp Table

| | CTE | Subquery | Temp Table |
|--|-----|----------|------------|
| Readability | ✅ Great | ❌ Nested mess | ✅ OK |
| Reusable in query | ✅ Yes | ❌ Must repeat | ✅ Yes |
| Recursive | ✅ Yes | ❌ No | ❌ No |
| Persists after query | ❌ No | ❌ No | ✅ Yes |
| Can be indexed | ❌ No | ❌ No | ✅ Yes |
| Performance (large data) | ⚠️ Varies | ⚠️ Varies | ✅ Better |

---

## Key Notes

- CTEs run in the **scope of a single statement** only
- Multiple CTEs separated by commas, each can reference previous ones
- Recursive CTEs **must** use `UNION ALL` (or `UNION` to deduplicate)
- Without `WHERE` stop condition in recursive part → **infinite loop**
- Use `LIMIT` or depth counter to guard against infinite recursion
- Writeable CTEs (`INSERT`/`UPDATE`/`DELETE` inside CTE) are powerful for chained DML
- CTEs are great for **readability refactoring** — no performance gain by default