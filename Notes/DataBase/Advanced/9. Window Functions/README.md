# 🪟 WINDOW FUNCTIONS

## What is it?
Functions that perform calculations **across a set of rows related to the current row**, without collapsing rows like `GROUP BY`. The window is defined by `OVER(...)`.

---

## Syntax

```sql
function_name([args]) OVER (
    [PARTITION BY col1, col2]   -- group rows (like GROUP BY but keeps all rows)
    [ORDER BY col3 ASC/DESC]    -- order within partition
    [frame_clause]              -- which rows in the window
)
```

---

## Function Categories

### Ranking Functions

```sql
-- ROW_NUMBER: unique sequential number (no ties)
SELECT name, salary,
    ROW_NUMBER() OVER (ORDER BY salary DESC) AS row_num
FROM employees;

-- RANK: ties get same rank, next rank skips (1,2,2,4)
SELECT name, salary,
    RANK() OVER (ORDER BY salary DESC) AS rnk
FROM employees;

-- DENSE_RANK: ties same rank, no gaps (1,2,2,3)
SELECT name, salary,
    DENSE_RANK() OVER (ORDER BY salary DESC) AS dense_rnk
FROM employees;

-- NTILE: divide into N buckets
SELECT name, salary,
    NTILE(4) OVER (ORDER BY salary) AS quartile
FROM employees;
```

### Offset Functions

```sql
-- LAG: access previous row's value
SELECT name, salary,
    LAG(salary, 1, 0) OVER (ORDER BY hire_date) AS prev_salary
FROM employees;
-- LAG(col, offset, default)

-- LEAD: access next row's value
SELECT name, salary,
    LEAD(salary, 1) OVER (ORDER BY hire_date) AS next_salary
FROM employees;

-- FIRST_VALUE / LAST_VALUE: first/last value in window
SELECT name, salary,
    FIRST_VALUE(salary) OVER (
        PARTITION BY department ORDER BY salary DESC
    ) AS max_salary_in_dept
FROM employees;
```

### Aggregate as Window Functions

```sql
-- Running total (cumulative sum)
SELECT name, salary,
    SUM(salary) OVER (ORDER BY hire_date) AS running_total

-- Total per partition (no aggregation)
SELECT name, department, salary,
    SUM(salary) OVER (PARTITION BY department) AS dept_total,
    AVG(salary) OVER (PARTITION BY department) AS dept_avg,
    COUNT(*) OVER (PARTITION BY department) AS dept_count
FROM employees;

-- Percentage of department total
SELECT name, department, salary,
    ROUND(salary * 100.0 / SUM(salary) OVER (PARTITION BY department), 2) AS pct_of_dept
FROM employees;
```

---

## PARTITION BY

```sql
-- Rank within each department (resets rank per department)
SELECT name, department, salary,
    RANK() OVER (PARTITION BY department ORDER BY salary DESC) AS dept_rank
FROM employees;

-- No PARTITION BY = entire table is one window
SELECT name, salary,
    RANK() OVER (ORDER BY salary DESC) AS overall_rank
FROM employees;
```

---

## Frame Clause

Defines which rows are included in the calculation relative to the current row.

```sql
ROWS BETWEEN start AND end
RANGE BETWEEN start AND end

-- Boundary keywords:
UNBOUNDED PRECEDING  -- from the very first row
n PRECEDING          -- n rows before current
CURRENT ROW          -- current row
n FOLLOWING          -- n rows after current
UNBOUNDED FOLLOWING  -- to the very last row
```

```sql
-- Running total (default frame = RANGE BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW)
SUM(amount) OVER (ORDER BY date)

-- Moving average (last 7 days)
AVG(amount) OVER (ORDER BY date ROWS BETWEEN 6 PRECEDING AND CURRENT ROW)

-- Full window sum (same value for all rows in partition)
SUM(amount) OVER (PARTITION BY dept ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING)

-- Previous 3 rows + current
SUM(amount) OVER (ORDER BY date ROWS BETWEEN 3 PRECEDING AND CURRENT ROW)
```

---

## Common Patterns

### Top N per group

```sql
-- Top 3 earners per department
WITH ranked AS (
    SELECT name, department, salary,
        ROW_NUMBER() OVER (PARTITION BY department ORDER BY salary DESC) AS rn
    FROM employees
)
SELECT * FROM ranked WHERE rn <= 3;
```

### Month-over-month change

```sql
SELECT month, revenue,
    LAG(revenue) OVER (ORDER BY month) AS prev_month,
    revenue - LAG(revenue) OVER (ORDER BY month) AS change,
    ROUND((revenue - LAG(revenue) OVER (ORDER BY month)) * 100.0
          / LAG(revenue) OVER (ORDER BY month), 2) AS pct_change
FROM monthly_revenue;
```

### Deduplication (keep latest)

```sql
WITH deduped AS (
    SELECT *,
        ROW_NUMBER() OVER (PARTITION BY email ORDER BY created_at DESC) AS rn
    FROM users
)
SELECT * FROM deduped WHERE rn = 1;
```

---

## Key Notes

- Window functions are computed **after** WHERE, GROUP BY, HAVING — but **before** ORDER BY and LIMIT
- You **cannot use** window functions in WHERE clause — wrap in CTE or subquery
- Multiple `OVER()` clauses in same query are fine — each defines its own window
- `ROWS` vs `RANGE`: `ROWS` is physical (row count), `RANGE` is logical (value-based — careful with ties)
- `LAST_VALUE` needs explicit frame clause: default frame stops at current row, so you'd always get current row's value