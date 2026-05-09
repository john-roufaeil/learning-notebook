# 🔧 STORED PROCEDURES

## What is it?
A **named block of procedural code** stored in the DB. Unlike functions, procedures can **control transactions** (`COMMIT`/`ROLLBACK`) and don't need to return a value. Called with `CALL`.

---

## Syntax (PostgreSQL 11+)

```sql
CREATE [OR REPLACE] PROCEDURE procedure_name(
    param1 type,
    INOUT param2 type,    -- for output values
    OUT param3 type
)
LANGUAGE plpgsql
AS $$
DECLARE
    v_var type;
BEGIN
    -- logic
    COMMIT;   -- ✅ allowed in procedures
END;
$$;

-- Call
CALL procedure_name(arg1, arg2);
```

---

## Parameter Modes

| Mode | Description |
|------|-------------|
| `IN` | Input only (default) |
| `OUT` | Output only — caller gets value back |
| `INOUT` | Both input and output |

```sql
CREATE OR REPLACE PROCEDURE transfer_funds(
    IN  from_account int,
    IN  to_account   int,
    IN  amount       numeric,
    OUT success      boolean
)
LANGUAGE plpgsql
AS $$
BEGIN
    UPDATE accounts SET balance = balance - amount WHERE id = from_account;
    UPDATE accounts SET balance = balance + amount WHERE id = to_account;
    success := true;
    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        success := false;
END;
$$;

-- Call with OUT param
CALL transfer_funds(1, 2, 500.00, NULL);
```

---

## Transaction Control in Procedures

```sql
CREATE OR REPLACE PROCEDURE batch_process()
LANGUAGE plpgsql
AS $$
BEGIN
    INSERT INTO log_table VALUES ('start', NOW());
    COMMIT;   -- commit the log entry independently

    -- do heavy work
    UPDATE big_table SET processed = true WHERE ...;
    COMMIT;   -- commit work

    INSERT INTO log_table VALUES ('done', NOW());
    COMMIT;
EXCEPTION
    WHEN OTHERS THEN
        ROLLBACK;
        RAISE;
END;
$$;
```

> ⚠️ If a procedure is called inside an existing transaction, `COMMIT`/`ROLLBACK` inside it will commit/rollback **that outer transaction too**.

---

## Error Handling

```sql
BEGIN
    -- risky code
EXCEPTION
    WHEN unique_violation THEN
        RAISE NOTICE 'Duplicate key: %', SQLERRM;
    WHEN foreign_key_violation THEN
        RAISE EXCEPTION 'FK error: %', SQLERRM;
    WHEN OTHERS THEN
        RAISE;  -- re-raise the original error
END;
```

### RAISE Levels

```sql
RAISE DEBUG   'msg';   -- only shown at debug level
RAISE INFO    'msg';
RAISE NOTICE  'msg';   -- printed to client
RAISE WARNING 'msg';
RAISE EXCEPTION 'msg'; -- aborts the block (like throwing error)

-- With substitution
RAISE NOTICE 'Employee % has salary %', emp_id, salary;
```

---

## Drop & Info

```sql
DROP PROCEDURE procedure_name(param_types);
DROP PROCEDURE IF EXISTS transfer_funds(int, int, numeric, boolean);

-- List procedures
SELECT routine_name FROM information_schema.routines
WHERE routine_type = 'PROCEDURE';
```

---

## Procedure vs Function Summary

| | Procedure | Function |
|--|-----------|----------|
| Called with | `CALL` | `SELECT` / expression |
| Transaction control | ✅ Yes | ❌ No |
| Return value | OUT params only | Must `RETURN` |
| Use in SQL expressions | ❌ | ✅ |
| Main use case | Business logic, batch ops | Computation, data transformation |

---

## Key Notes

- Procedures are ideal for **ETL**, **batch jobs**, and **multi-step business workflows**
- Unlike functions, procedures **cannot be used** in `SELECT`, `WHERE`, or `JOIN`
- Calling `COMMIT` inside a procedure makes sub-transactions harder to control — design carefully
- In **MySQL/SQL Server**, procedures predate functions and are more commonly used — PostgreSQL added procedures in v11
- Always handle exceptions — an unhandled error leaves transactions in undefined state