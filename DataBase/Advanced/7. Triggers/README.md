# ⚡ TRIGGERS

## What is it?
A **function automatically executed** in response to a specific event on a table or view (INSERT, UPDATE, DELETE, TRUNCATE). Used for auditing, validation, derived data, and enforcing business rules.

---

## Anatomy of a Trigger

A trigger has **two parts**:
1. **Trigger function** — the logic (returns `TRIGGER` type)
2. **Trigger definition** — when/what/where to fire it

```sql
-- Step 1: Create the trigger function
CREATE OR REPLACE FUNCTION trigger_function_name()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$
BEGIN
    -- logic using NEW and OLD
    RETURN NEW;  -- or OLD, or NULL
END;
$$;

-- Step 2: Attach trigger to a table
CREATE TRIGGER trigger_name
    BEFORE | AFTER | INSTEAD OF
    INSERT | UPDATE | DELETE | TRUNCATE
    ON table_name
    FOR EACH ROW | FOR EACH STATEMENT
    [WHEN (condition)]
    EXECUTE FUNCTION trigger_function_name();
```

---

## Timing Options

| Timing | Description |
|--------|-------------|
| `BEFORE` | Fires before the operation — can modify or cancel it |
| `AFTER` | Fires after the operation — sees committed state |
| `INSTEAD OF` | Replaces the operation — only on **views** |

---

## Row vs Statement Level

| | `FOR EACH ROW` | `FOR EACH STATEMENT` |
|--|----------------|----------------------|
| Fires | Once per affected row | Once per SQL statement |
| Access to NEW/OLD | ✅ Yes | ❌ No |
| Use case | Auditing, row transforms | Bulk logging |

---

## NEW and OLD

| Variable | Available when |
|----------|---------------|
| `NEW` | INSERT (new row), UPDATE (new values) |
| `OLD` | DELETE (deleted row), UPDATE (old values) |

```sql
-- Audit trigger example
CREATE OR REPLACE FUNCTION audit_changes()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO audit_log(table_name, operation, old_data, new_data, changed_at, changed_by)
    VALUES (
        TG_TABLE_NAME,
        TG_OP,
        row_to_json(OLD),
        row_to_json(NEW),
        NOW(),
        current_user
    );
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER employee_audit
AFTER INSERT OR UPDATE OR DELETE ON employees
FOR EACH ROW EXECUTE FUNCTION audit_changes();
```

---

## Special Trigger Variables

| Variable | Description |
|----------|-------------|
| `TG_OP` | Operation: `'INSERT'`, `'UPDATE'`, `'DELETE'`, `'TRUNCATE'` |
| `TG_TABLE_NAME` | Name of the table that fired the trigger |
| `TG_TABLE_SCHEMA` | Schema of the table |
| `TG_WHEN` | `'BEFORE'` or `'AFTER'` |
| `TG_LEVEL` | `'ROW'` or `'STATEMENT'` |
| `TG_NARGS` | Number of trigger arguments |
| `TG_ARGV` | Array of trigger arguments |

---

## Return Values

| Return | Effect |
|--------|--------|
| `RETURN NEW` | Proceed with the (possibly modified) new row |
| `RETURN OLD` | Proceed with the old row (useful for UPDATE/DELETE) |
| `RETURN NULL` | **Cancel** the operation (BEFORE triggers only) |

```sql
-- Validation trigger: cancel if salary is negative
CREATE OR REPLACE FUNCTION validate_salary()
RETURNS TRIGGER AS $$
BEGIN
    IF NEW.salary < 0 THEN
        RAISE EXCEPTION 'Salary cannot be negative: %', NEW.salary;
        -- or silently cancel: RETURN NULL;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;
```

---

## Modifying Data in BEFORE Trigger

```sql
-- Auto-set updated_at timestamp
CREATE OR REPLACE FUNCTION set_updated_at()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = NOW();   -- modify before save
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER set_timestamp
BEFORE UPDATE ON employees
FOR EACH ROW EXECUTE FUNCTION set_updated_at();
```

---

## INSTEAD OF Trigger (on Views)

```sql
CREATE OR REPLACE FUNCTION insert_into_view()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO employees(name, department)
    VALUES (NEW.name, NEW.department);
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER view_insert
INSTEAD OF INSERT ON employees_view
FOR EACH ROW EXECUTE FUNCTION insert_into_view();
```

---

## Conditional Trigger

```sql
-- Only fire when salary actually changes
CREATE TRIGGER salary_change_alert
AFTER UPDATE OF salary ON employees
FOR EACH ROW
WHEN (OLD.salary IS DISTINCT FROM NEW.salary)
EXECUTE FUNCTION notify_salary_change();
```

---

## Managing Triggers

```sql
-- Drop trigger
DROP TRIGGER trigger_name ON table_name;
DROP TRIGGER IF EXISTS employee_audit ON employees;

-- Disable/enable
ALTER TABLE employees DISABLE TRIGGER employee_audit;
ALTER TABLE employees ENABLE TRIGGER employee_audit;
ALTER TABLE employees DISABLE TRIGGER ALL;
ALTER TABLE employees ENABLE TRIGGER ALL;

-- List triggers
SELECT trigger_name, event_manipulation, action_timing
FROM information_schema.triggers
WHERE event_object_table = 'employees';
```

---

## Key Notes

- Trigger functions **must return TRIGGER type** in PostgreSQL
- Multiple triggers on same event fire in **alphabetical order** by name
- Triggers can call other functions, insert into other tables, send notifications
- **AFTER triggers** can't cancel the operation — use BEFORE for that
- Triggers add **overhead** — use judiciously, not for everything
- `TRUNCATE` trigger only supports `FOR EACH STATEMENT` (not row level)
- Be careful with triggers that fire triggers (cascading triggers — hard to debug)
- Use `pg_notify()` inside triggers for **event-driven** architectures