# Advanced DB

- Joins are time expensive because tables could be not in the same place on hard disk

- **Serialization**: one string data retrieval instead of each piece in a place

- hierarchical model -> no relations mapping
- network model -> developer issues
- OODBMS -> 

- query optimizer

- lookup vs join

- relational db is good for many to many relationships - pro from other models

- newSQL

- charset & collate

## Data Modeling Evolution

### 1) Hierarchical Model

**Structure:** Tree (parent → child, 1-to-many)

```
Company
 └── Department
      └── Employee
```

**Pros**

* Very fast traversal (predefined paths)
* Simple mental model

**Cons**

* No many-to-many relationships
* Rigid structure (changing tree = painful)
* Data duplication if relationships don’t fit tree

👉 Why it exists: early systems optimized for **sequential access**

---

### 2) Network Model

**Structure:** Graph-like (records + pointers, many-to-many)

```
Employee ↔ WorksOn ↔ Project
```

**Pros**

* Supports complex relationships
* More flexible than hierarchical

**Cons**

* Pointer-based → very hard to manage
* Tight coupling between data and access logic
* Poor developer productivity

👉 Key idea: **you navigate manually**, not declaratively

---

### 3) Relational Model (baseline for comparison)

**Structure:** Tables + relations (keys)

**Why it replaced the above**

* You say *what* you want (SQL), not *how to traverse*
* Decouples data from access path

---

### 4) Object-Oriented DBMS (OODBMS)

**Structure:** Objects (like in OOP: classes, inheritance, methods)

```java
class User {
  String name;
  List<Order> orders;
}
```

**Pros**

* Natural fit with OOP languages (no impedance mismatch)
* Supports inheritance, encapsulation

**Cons**

* Weak standardization
* Querying is harder than SQL
* Not widely adopted

👉 Key idea: **store objects as they are in code**

---

### 5) Object-Relational Model (ORDBMS)

**Structure:** Relational + object features

* Tables + custom types
* Supports JSON, arrays, user-defined types

**Pros**

* Best of both worlds (structure + flexibility)
* Easier transition from relational

**Cons**

* Added complexity
* Not fully object-oriented

👉 Example mindset: relational core + extensions

---

### Core Comparison (what actually matters)

| Model        | Relationship Handling | Flexibility | Ease of Query | Coupling  |
| ------------ | --------------------- | ----------- | ------------- | --------- |
| Hierarchical | 1 → many (tree)       | Low         | Low           | High      |
| Network      | Many ↔ many           | Medium      | Low           | Very High |
| Relational   | Keys + joins          | Medium      | High (SQL)    | Low       |
| OODBMS       | Native objects        | High        | Medium        | Medium    |
| ORDBMS       | Hybrid                | High        | High          | Low       |

---

### The Real Insight (this is what most people miss)

The evolution solves one problem:

> **Who controls navigation? Developer or database?**

* Hierarchical / Network → **YOU control navigation**
* Relational → **DB figures it out (query planner)**
* OODBMS → **objects control structure**
* Modern NoSQL → **optimize access patterns**

---

### Now think like a system designer

If you used:

* Hierarchical → your app logic becomes rigid
* Network → your code becomes messy pointer navigation
* OODBMS → tight coupling with your language
* Relational → flexibility but joins cost
* NoSQL → speed but duplication

---
## Schema levels

Database schema design is split into three abstraction levels to separate concerns:

* **External** → what each user sees
* **Conceptual** → overall logical structure
* **Physical** → how data is actually stored

This separation is called **data independence**.

---

### External Schema (View Level)

**What it is:**
User-specific views of the database.

**Structure:**

* Subsets of tables
* Virtual tables (views)
* Renamed/filtered data
* DCL (Grant, Revoke)

**Example:**

* Student sees: `name, grades`
* Admin sees: full student record

**Pros**

* Security (hide sensitive data)
* Simplicity for users
* Multiple tailored views

**Cons**

* Can become complex with many views
* Performance overhead (view resolution)

**Key Idea:**
Different users see **different representations** of the same data.

---

### Conceptual Schema (Logical Level)

**What it is:**
The complete logical model of the database.

**Structure:**

* Entities (tables)
* Relationships
* Constraints (PK, FK, rules)
* DDL (Create, Alter, Drop, ...)

**Example:**

```
User(id, name)
Lesson(id, title)
Progress(user_id, lesson_id)
```

**Pros**

* Central source of truth
* Independent of storage details
* Easier to design and reason about

**Cons**

* Doesn’t consider performance/storage
* Changes affect all external views

**Key Idea:**
Defines **what data exists and how it relates**.

---

### Physical Schema (Internal Level)

**What it is:**
How data is stored on disk.

**Structure:**

* Indexes
* File organization (B-Trees, hashing)
* Partitioning, storage formats
* DML (Insert, Update, Delete, ...)

**Example:**

* Index on `user_id`
* Table stored as B+ tree

**Pros**

* Performance optimization
* Efficient storage and retrieval

**Cons**

* Complex and DB-specific
* Hard to modify without impact

**Key Idea:**
Defines **how data is actually stored and accessed**.

---

### Core Comparison

| Aspect         | External Schema      | Conceptual Schema      | Physical Schema          |
| -------------- | -------------------- | ---------------------- | ------------------------ |
| Focus          | User view            | Logical structure      | Storage & performance    |
| Audience       | End users / apps     | Designers / architects | DB engineers             |
| Abstraction    | Highest              | Medium                 | Lowest                   |
| Changes Impact | Minimal (isolated)   | Affects all views      | Affects performance only |
| Example        | View (SELECT subset) | ER diagram / tables    | Indexes, file layout     |

---

### Data Independence (Why this matters)

* **Logical Independence:**
  Change conceptual schema without affecting external views

* **Physical Independence:**
  Change storage without affecting conceptual schema

👉 This is why modern databases scale and evolve without breaking applications.

---

### Think Like a Designer

If you mix these layers:

* You leak storage logic into business code → tight coupling
* You break flexibility → hard to scale

---

### Challenge

You add a new index to speed up queries:

* Which schema changes?
* Why should the API layer NOT care?

If you can answer that instantly, you understand the architecture.

---

## Subqueries vs Join vs CTE

All three are ways to **combine or reuse data**, but they differ in **readability, reuse, and execution behavior**.

---

### Subqueries

**What it is:**
A query inside another query.

**Structure:**

```sql
SELECT name
FROM users
WHERE id IN (SELECT user_id FROM orders);
```

**Types:**

* Scalar (returns one value)
* Row
* Table (used in `FROM`)

**Pros**

* Simple for small logic
* Good for filtering (`IN`, `EXISTS`)

**Cons**

* Hard to read when nested
* Can be inefficient if repeated
* Limited reuse

**Key Insight:**
Good for **localized logic**, not complex pipelines.

---

### Joins

**What it is:**
Combine tables based on relationships.

**Structure:**

```sql
SELECT u.name
FROM users u
JOIN orders o ON u.id = o.user_id;
```

**Types:**

* INNER, LEFT, RIGHT, FULL

**Pros**

* Efficient (optimized by DB engine)
* Clear relational logic
* Best for combining datasets

**Cons**

* Can get messy with many joins
* Duplicates if not careful

**Key Insight:**
Primary tool for **relational data combination**.

---

### CTE (Common Table Expression)

**What it is:**
Named temporary result set using `WITH`.

**Structure:**

```sql
WITH user_orders AS (
  SELECT user_id, COUNT(*) AS total
  FROM orders
  GROUP BY user_id
)
SELECT u.name, uo.total
FROM users u
JOIN user_orders uo ON u.id = uo.user_id;
```

**Pros**

* Improves readability
* Reusable within query
* Supports recursion (unique feature)

**Cons**

* Sometimes not optimized (depends on DB)
* Can be slower than inline queries

**Key Insight:**
Think of it as **temporary named steps** in a query pipeline.

---

### Core Comparison

| Feature      | Subquery          | Join           | CTE                       |
| ------------ | ----------------- | -------------- | ------------------------- |
| Purpose      | Nested logic      | Combine tables | Structure complex queries |
| Readability  | Low (when nested) | Medium         | High                      |
| Reusability  | None              | None           | Yes (within query)        |
| Performance  | Can degrade       | Usually best   | Depends on DB             |
| Advanced Use | Limited           | Standard       | Recursive queries         |

---

### When to Use What

* Use **JOIN** → when combining related tables (default choice)
* Use **Subquery** → for simple filters (`EXISTS`, `IN`)
* Use **CTE** → when query becomes complex and needs structure

---

### The Real Insight (this is the “why”)

All three can often produce the **same result**.

The difference is:

* Subquery → **inline thinking**
* Join → **relational thinking**
* CTE → **step-by-step pipeline thinking**

---

### Common Mistake

Writing this:

```sql
SELECT *
FROM users
WHERE id IN (SELECT user_id FROM orders);
```

Instead of:

```sql
SELECT DISTINCT u.*
FROM users u
JOIN orders o ON u.id = o.user_id;
```

👉 Why is the JOIN often better here?
Think about:

* Execution plan
* Index usage
* Duplicate handling

---

### Challenge

You need:

* Top 3 users with most orders

Would you:

1. Use subquery with `ORDER BY`
2. Use join + `GROUP BY`
3. Use CTE

👉 Which is most readable?
👉 Which is most efficient?
👉 Why might CTE be the best teaching tool but not always the fastest?

---

## Materialized vs Simple Views

Both are **saved queries**, but the key difference is:

* **View** → stores the query only
* **Materialized View** → stores the **result of the query**

---

### Simple View (Virtual View)

**What it is:**
A stored SQL query executed every time you use it.

**Structure:**

```sql
CREATE VIEW user_orders AS
SELECT u.name, o.id
FROM users u
JOIN orders o ON u.id = o.user_id;
```

**How it works:**

* No data stored
* Runs the query on access

**Pros**

* Always up-to-date
* No extra storage
* Simple to maintain

**Cons**

* Slow for complex queries
* Recomputes every time

**Key Insight:**
A view is just a **saved abstraction**, not data.

---

### Materialized View

**What it is:**
A precomputed, stored result of a query.

**Structure:**

```sql
CREATE MATERIALIZED VIEW user_orders_mv AS
SELECT u.name, COUNT(o.id) AS total
FROM users u
JOIN orders o ON u.id = o.user_id
GROUP BY u.name;
```

**How it works:**

* Data is physically stored
* Needs manual or scheduled refresh

**Pros**

* Very fast reads
* Great for heavy aggregations
* Reduces repeated computation

**Cons**

* Data can become stale
* Requires storage
* Refresh cost (can be expensive)

**Key Insight:**
Materialized view = **cached query result**

---

### Core Comparison

| Feature     | View                | Materialized View    |
| ----------- | ------------------- | -------------------- |
| Stores data | ❌ No                | ✅ Yes                |
| Performance | Slower (recomputed) | Faster (precomputed) |
| Freshness   | Always fresh        | Can be stale         |
| Storage     | None                | Requires storage     |
| Maintenance | Minimal             | Needs refresh        |

---

### When to Use What

* Use **View**
  → when data changes frequently and must always be correct
  → when query is simple

* Use **Materialized View**
  → when query is expensive (joins, aggregates)
  → when slight staleness is acceptable

---

### The Real Insight (the “why”)

This is a classic tradeoff:

> **Compute on read vs compute on write**

* View → compute on **read**
* Materialized View → compute on **write (refresh time)**

---

### Design Thinking

If your query:

* Runs every request
* Aggregates millions of rows

👉 Why recompute it every time?

Cache it → materialized view.

But:

* If data updates every second

👉 Why cache something that becomes wrong immediately?

Use a normal view.

---

### Challenge

You have a dashboard:

* Shows total sales per day
* Updated every 5 seconds

👉 Would you use:

* View
* Materialized view with refresh

If materialized:

* How often do you refresh?
* What happens during refresh (lock vs concurrent)?

Answer that → you understand real-world tradeoffs.
