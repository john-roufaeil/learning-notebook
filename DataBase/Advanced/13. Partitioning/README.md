# 🗂️ PARTITIONING

## What is it?
**Splitting a large table into smaller physical pieces** (partitions) while maintaining a single logical table interface. Improves query performance, manageability, and maintenance on large datasets.

---

## Partition Types

| Type | Best for |
|------|----------|
| **RANGE** | Date/time ranges, sequential numeric ranges |
| **LIST** | Discrete known values (country, status, region) |
| **HASH** | Distributing rows evenly when no natural range |

---

## RANGE Partitioning (most common)

```sql
-- Create partitioned table
CREATE TABLE orders (
    id          BIGINT,
    created_at  TIMESTAMP NOT NULL,
    customer_id INT,
    total       NUMERIC
) PARTITION BY RANGE (created_at);

-- Create partitions
CREATE TABLE orders_2023 PARTITION OF orders
    FOR VALUES FROM ('2023-01-01') TO ('2024-01-01');

CREATE TABLE orders_2024 PARTITION OF orders
    FOR VALUES FROM ('2024-01-01') TO ('2025-01-01');

CREATE TABLE orders_2025 PARTITION OF orders
    FOR VALUES FROM ('2025-01-01') TO ('2026-01-01');

-- Default partition (catches anything not matched)
CREATE TABLE orders_default PARTITION OF orders DEFAULT;
```

---

## LIST Partitioning

```sql
CREATE TABLE customers (
    id      BIGINT,
    region  TEXT NOT NULL,
    name    TEXT
) PARTITION BY LIST (region);

CREATE TABLE customers_us PARTITION OF customers
    FOR VALUES IN ('US', 'CA', 'MX');

CREATE TABLE customers_eu PARTITION OF customers
    FOR VALUES IN ('UK', 'DE', 'FR', 'IT');

CREATE TABLE customers_other PARTITION OF customers DEFAULT;
```

---

## HASH Partitioning

```sql
CREATE TABLE events (
    id      BIGINT,
    user_id INT NOT NULL,
    data    JSONB
) PARTITION BY HASH (user_id);

-- MODULUS = total partitions, REMAINDER = which one
CREATE TABLE events_0 PARTITION OF events FOR VALUES WITH (MODULUS 4, REMAINDER 0);
CREATE TABLE events_1 PARTITION OF events FOR VALUES WITH (MODULUS 4, REMAINDER 1);
CREATE TABLE events_2 PARTITION OF events FOR VALUES WITH (MODULUS 4, REMAINDER 2);
CREATE TABLE events_3 PARTITION OF events FOR VALUES WITH (MODULUS 4, REMAINDER 3);
```

---

## Partition Pruning

The optimizer **skips irrelevant partitions** when the partition key is in WHERE clause.

```sql
-- Only scans orders_2024 partition
SELECT * FROM orders WHERE created_at BETWEEN '2024-01-01' AND '2024-12-31';

-- Check pruning with EXPLAIN
EXPLAIN SELECT * FROM orders WHERE created_at = '2024-06-15';
-- Look for: "Partitions selected: 1 out of 3"
```

> ⚠️ Partition pruning only works if WHERE clause includes the **partition key**.

---

## Indexes on Partitions

```sql
-- Index on parent automatically applies to ALL partitions
CREATE INDEX idx_orders_customer ON orders (customer_id);

-- Index on specific partition only
CREATE INDEX idx_2024_total ON orders_2024 (total);

-- Each partition's index is independent and smaller → faster
```

---

## Managing Partitions

```sql
-- Add new partition
CREATE TABLE orders_2026 PARTITION OF orders
    FOR VALUES FROM ('2026-01-01') TO ('2027-01-01');

-- Detach partition (becomes standalone table)
ALTER TABLE orders DETACH PARTITION orders_2023;
-- Now orders_2023 is a regular table — can archive/drop safely

-- Attach existing table as partition
ALTER TABLE orders ATTACH PARTITION orders_2022
    FOR VALUES FROM ('2022-01-01') TO ('2023-01-01');

-- Drop old partition (fast — no row-by-row DELETE)
DROP TABLE orders_2022;  -- instant vs DELETE FROM orders WHERE year=2022 (slow)
```

---

## Sub-partitioning

```sql
CREATE TABLE logs (
    id         BIGINT,
    created_at TIMESTAMP NOT NULL,
    region     TEXT NOT NULL
) PARTITION BY RANGE (created_at);

CREATE TABLE logs_2024 PARTITION OF logs
    FOR VALUES FROM ('2024-01-01') TO ('2025-01-01')
    PARTITION BY LIST (region);   -- sub-partitioned!

CREATE TABLE logs_2024_us PARTITION OF logs_2024 FOR VALUES IN ('US');
CREATE TABLE logs_2024_eu PARTITION OF logs_2024 FOR VALUES IN ('EU');
```

---

## Key Notes

- Partitioning is best for tables **> ~10GB** or that need **lifecycle management** (archive old partitions)
- **Partition key must be in WHERE** for pruning to work — without it, all partitions are scanned
- Foreign keys **cannot reference** a partitioned table (only individual partitions)
- UNIQUE constraints must include the partition key
- Partition pruning happens at **plan time** for literals, **execution time** for parameters
- `DETACH + DROP` is the fast way to archive old data vs slow `DELETE`
- Use `pg_partman` extension for automated time-based partition creation and management
- Check partition info: `SELECT * FROM pg_partitions WHERE tablename = 'orders';`