# ⚙️ FUNCTIONS (Stored Functions / UDFs)

## What is it?
A **named, reusable block of SQL/procedural code** that accepts inputs, performs logic, and **returns a value**. Must return something. Cannot commit/rollback transactions.

---

## PostgreSQL Function Syntax

```sql
CREATE [OR REPLACE] FUNCTION function_name(param1 type, param2 type)
RETURNS return_type
LANGUAGE plpgsql   -- or sql, python, etc.
AS $$
DECLARE
    variable_name type;
BEGIN
    -- logic here
    RETURN value;
END;
$$;
```

---

## Language Options

| Language | Use when |
|----------|----------|
| `sql` | Simple query, no variables needed |
| `plpgsql` | Variables, loops, conditionals |
| `plpython3u` | Complex logic, Python libraries |

---

## Return Types

```sql
-- Scalar: returns single value
RETURNS integer
RETURNS text
RETURNS boolean

-- Returns a row (composite)
RETURNS employees  -- returns one row matching table structure

-- Returns multiple rows
RETURNS TABLE(col1 int, col2 text)
RETURNS SETOF employees

-- Returns nothing (use PROCEDURE instead, but if needed)
RETURNS void
```

---

## Examples

```sql
-- Simple SQL function
CREATE OR REPLACE FUNCTION get_employee_count(dept text)
RETURNS integer
LANGUAGE sql
AS $$
    SELECT COUNT(*) FROM employees WHERE department = dept;
$$;

-- Call it
SELECT get_employee_count('Engineering');


-- PL/pgSQL with logic
CREATE OR REPLACE FUNCTION calculate_bonus(emp_id int)
RETURNS numeric
LANGUAGE plpgsql
AS $$
DECLARE
    emp_salary numeric;
    bonus      numeric;
BEGIN
    SELECT salary INTO emp_salary FROM employees WHERE id = emp_id;

    IF emp_salary > 100000 THEN
        bonus := emp_salary * 0.15;
    ELSE
        bonus := emp_salary * 0.10;
    END IF;

    RETURN bonus;
END;
$$;


-- Returns TABLE (set-returning function)
CREATE OR REPLACE FUNCTION get_dept_employees(dept text)
RETURNS TABLE(emp_id int, emp_name text, emp_salary numeric)
LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
    SELECT id, name, salary FROM employees WHERE department = dept;
END;
$$;

-- Call set-returning function
SELECT * FROM get_dept_employees('Engineering');
```

---

## Variables & Control Flow (PL/pgSQL)

```sql
DECLARE
    v_count   integer := 0;     -- := for assignment
    v_name    text;
    v_date    date DEFAULT CURRENT_DATE;

-- Conditionals
IF condition THEN ...
ELSIF condition THEN ...
ELSE ...
END IF;

-- Loops
LOOP
    EXIT WHEN condition;
END LOOP;

FOR i IN 1..10 LOOP ... END LOOP;

WHILE condition LOOP ... END LOOP;

-- Query into variable
SELECT col INTO v_name FROM table WHERE ...;
SELECT col INTO STRICT v_name FROM ...;  -- STRICT raises error if 0 or 2+ rows
```

---

## Drop & Info

```sql
DROP FUNCTION function_name(param_types);
DROP FUNCTION IF EXISTS get_employee_count(text);

-- Must include param types due to overloading
DROP FUNCTION calculate_bonus(int);

-- List functions (PostgreSQL)
\df
SELECT routine_name, routine_type FROM information_schema.routines
WHERE routine_schema = 'public';
```

---

## Function vs Procedure

| | Function | Procedure |
|--|----------|-----------|
| Returns value | ✅ Must | ❌ Optional (OUT params) |
| Call with | `SELECT` or in expressions | `CALL` |
| Transaction control | ❌ Cannot | ✅ Can `COMMIT`/`ROLLBACK` |
| Use in WHERE/JOIN | ✅ Yes | ❌ No |

---

## Key Notes

- Functions are **deterministic** or **volatile** — mark with `IMMUTABLE` (no DB access), `STABLE` (reads only), or `VOLATILE` (default, can change data)
- `IMMUTABLE` / `STABLE` allow **query optimizer** to cache/inline calls
- PostgreSQL supports **function overloading** (same name, different param types)
- `SECURITY DEFINER` — runs with **creator's** permissions (like sudo)
- `SECURITY INVOKER` — runs with **caller's** permissions (default)

```sql
CREATE FUNCTION ... SECURITY DEFINER;  -- careful: privilege escalation risk
```