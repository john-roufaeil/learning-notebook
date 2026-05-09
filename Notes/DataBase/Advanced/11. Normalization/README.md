# 📐 NORMALIZATION

## What is it?
The process of **organizing a database to reduce redundancy and improve data integrity** by applying a series of normal forms. Each higher normal form builds on the previous.

---

## Quick Summary Table

| Form | Eliminates |
|------|-----------|
| **1NF** | Repeating groups, non-atomic values |
| **2NF** | Partial dependencies (composite PK issues) |
| **3NF** | Transitive dependencies |
| **BCNF** | Any anomaly where non-key determines key part |
| **4NF** | Multi-valued dependencies |
| **5NF** | Join dependencies |

> In practice: **3NF is the target** for most OLTP systems. BCNF is stricter but rarely needed.

---

## 1NF — First Normal Form

**Rule:** Each column has **atomic** (indivisible) values. No repeating groups. Each row is unique (has a primary key).

```
❌ VIOLATES 1NF:
employee_id | name  | phones
1           | Alice | 555-1234, 555-5678   ← multiple values in one cell

✅ 1NF:
employee_id | name  | phone
1           | Alice | 555-1234
1           | Alice | 555-5678
```

---

## 2NF — Second Normal Form

**Rule:** Must be in 1NF + **no partial dependencies** (every non-key attribute depends on the WHOLE primary key, not just part of it).

*Only relevant when the PK is composite.*

```
❌ VIOLATES 2NF:
order_id | product_id | quantity | product_name
  ↑            ↑           ↑            ↑
  └────────────┘           │            └── depends only on product_id, not (order_id, product_id)
    composite PK        depends on full PK

✅ 2NF — split into:
order_items:  order_id, product_id, quantity
products:     product_id, product_name
```

---

## 3NF — Third Normal Form

**Rule:** Must be in 2NF + **no transitive dependencies** (non-key columns must not depend on other non-key columns).

```
❌ VIOLATES 3NF:
employee_id | department_id | department_name
                ↑                    ↑
              PK col           depends on department_id (non-key) → transitive

✅ 3NF — split into:
employees:    employee_id, department_id
departments:  department_id, department_name
```

---

## BCNF — Boyce-Codd Normal Form

**Rule:** Must be in 3NF + for every functional dependency `A → B`, A must be a **super key**. Stricter than 3NF — handles edge cases with multiple overlapping candidate keys.

```
Rare in practice — most 3NF tables are already BCNF.
Violation occurs when: non-trivial FD where the determinant is NOT a superkey.
```

---

## Denormalization

Intentionally **relaxing normalization** for performance. Common in:
- **OLAP / Data Warehouses** — wide flat tables for fast analytics
- **High-read-throughput APIs** — fewer JOINs = faster queries
- **Caching layers**

```sql
-- Normalized (3NF): join required for every query
SELECT e.name, d.department_name
FROM employees e JOIN departments d ON e.department_id = d.id;

-- Denormalized: store department_name directly on employees
-- ↑ Faster reads, but department name can go out of sync
```

**Denormalization techniques:**
- Store computed aggregates (`total_orders`, `avg_rating`)
- Duplicate columns to avoid joins
- Use `JSONB` for semi-structured data
- Materialized views for precomputed results

---

## Functional Dependencies (FD)

The theoretical basis of normalization.

```
A → B means: knowing A, you always know B
employee_id → name      (✅ each employee has one name)
department_id → budget  (✅ each dept has one budget)
name → employee_id      (❌ names aren't unique)
```

---

## Key Notes

- **Over-normalization** leads to too many JOINs → performance problems
- **Under-normalization** leads to update anomalies, data inconsistency
- The sweet spot for OLTP: **3NF** with strategic denormalization where proven necessary
- Data warehouses use **star schema** (fact tables + dimension tables) — intentionally denormalized
- `JSONB` in PostgreSQL is a pragmatic escape hatch — for truly flexible/variable schemas
- Normalization is a **design decision**, not a rigid rule — always consider your query patterns