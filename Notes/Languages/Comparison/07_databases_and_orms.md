# Module 07 — Databases, ORMs & the Data Layer

> **Series:** From Junior to Senior — Languages, Runtimes & Frameworks Explained  
> **Level:** Intermediate → Advanced  
> **Goal:** Understand how frameworks talk to databases, what ORMs actually do, and how data layer choices affect performance.

---

## The Data Layer Is Where Your Performance Actually Lives

Here's a counterintuitive truth: **for most web applications, the programming language and framework barely affect performance. The database does.**

A request that takes 200ms often looks like this:
- Framework overhead: ~2ms
- Business logic: ~3ms
- Database queries: ~195ms

You could switch from Python to Go and cut the 5ms overhead to 0.5ms. That's a 4.5ms improvement on a 200ms request — barely measurable. But fix the database query (add an index, remove an N+1 query) and you might go from 200ms to 5ms — a 40x improvement.

This module covers how frameworks talk to databases, the patterns that work well, and the patterns that silently kill performance.

---

## How Web Applications Talk to Databases

Every framework eventually needs to send SQL (or queries in another query language) to a database and get results back. There's a spectrum of how much the framework helps you with this:

```
Raw SQL ←──────────────────────────────────────────────────────► Full ORM

"SELECT * FROM users    Query Builder        User.find(42)
 WHERE id = $1"         User.where('id = ?', 42)  ← hides SQL completely
```

### Level 1: Raw SQL

The most direct approach. You write SQL strings, pass them to a database driver, get results back.

```python
# Python with psycopg2
cursor.execute("SELECT id, email, name FROM users WHERE id = %s AND active = TRUE", [user_id])
row = cursor.fetchone()
if row:
    user = {"id": row[0], "email": row[1], "name": row[2]}
```

```go
// Go with database/sql
row := db.QueryRowContext(ctx, 
    "SELECT id, email, name FROM users WHERE id = $1 AND active = TRUE", 
    userID)
var user User
row.Scan(&user.ID, &user.Email, &user.Name)
```

**Pros:** Maximum control. You see exactly what's sent to the database. Easy to optimize.  
**Cons:** Verbose. You manually map result rows to objects. Repetitive for CRUD operations.

### Level 2: Query Builders

A thin layer that lets you build SQL programmatically without strings, with type checking and composability.

```javascript
// Knex.js (Node.js query builder)
const users = await knex('users')
    .select('id', 'email', 'name')
    .where('active', true)
    .where('created_at', '>', cutoffDate)
    .orderBy('name')
    .limit(20)
    .offset(page * 20);

// Generated SQL: SELECT id, email, name FROM users 
//               WHERE active = true AND created_at > ? 
//               ORDER BY name LIMIT 20 OFFSET 40
```

```java
// JOOQ (Java query builder)
List<UserRecord> users = dsl
    .selectFrom(USERS)
    .where(USERS.ACTIVE.eq(true))
    .and(USERS.CREATED_AT.gt(cutoffDate))
    .orderBy(USERS.NAME)
    .fetch();
```

**Pros:** Type-safe query construction, composable, SQL is still visible/debuggable.  
**Cons:** Less automatic than ORM (no relationship loading, no model objects by default).

### Level 3: Full ORM (Object-Relational Mapper)

An ORM maps your database tables to classes/objects in your language. You work with objects and the ORM generates SQL automatically.

```python
# Django ORM
class User(models.Model):
    email = models.EmailField()
    name = models.CharField(max_length=200)
    active = models.BooleanField(default=True)
    created_at = models.DateTimeField(auto_now_add=True)

# Query using Python objects, not SQL:
users = User.objects.filter(active=True, created_at__gt=cutoff).order_by('name')[:20]
# Django generates: SELECT * FROM users WHERE active=1 AND created_at > ? ORDER BY name LIMIT 20
```

```java
// Spring Data JPA
@Entity
public class User {
    @Id Long id;
    String email;
    String name;
    boolean active;
    LocalDateTime createdAt;
}

public interface UserRepository extends JpaRepository<User, Long> {
    // Spring generates the SQL from the method name:
    List<User> findByActiveAndCreatedAtAfterOrderByName(boolean active, LocalDateTime after);
}
```

**Pros:** Rapid development. No SQL for simple queries. Automatic migrations (mostly). Relationship handling.  
**Cons:** Can generate inefficient SQL. The N+1 query problem. Less control over exact SQL.

---

## The N+1 Query Problem: The Most Common Performance Bug

This is the single most important concept in this module. The N+1 problem is so common and so impactful that every senior engineer must understand it deeply.

**The scenario:**

```python
# Django: Get all users and their post counts
users = User.objects.filter(active=True)  # Query 1: SELECT * FROM users

for user in users:
    print(f"{user.name}: {user.posts.count()} posts")  
    # This runs a new query for EACH user!
    # If you have 100 users: 100 more queries = 101 total
```

**The math:** With 100 users, this runs 101 database queries. Each query has ~2ms overhead. That's 200ms of query latency just from overhead. For 1,000 users: 1,001 queries = 2 seconds.

**Why it happens:** ORMs make accessing relationships feel like accessing a local object property. `user.posts` looks free. It's not — it's a database query.

**The fix — eager loading:**

```python
# Django: load users AND their posts in 2 queries, not N+1
users = User.objects.filter(active=True).prefetch_related('posts')

# Django runs:
# Query 1: SELECT * FROM users WHERE active = TRUE
# Query 2: SELECT * FROM posts WHERE user_id IN (1, 2, 3, ..., 100)
# Then links them in memory

for user in users:
    print(f"{user.name}: {user.posts.all().count()} posts")
    # Now accesses in-memory data — no additional queries
```

```ruby
# Rails: same pattern
users = User.where(active: true).includes(:posts)
# includes triggers eager loading:
# SELECT * FROM users WHERE active = 1
# SELECT * FROM posts WHERE user_id IN (1,2,3,...,100)
```

```java
// Spring JPA: JPQL with JOIN FETCH
List<User> users = em.createQuery(
    "SELECT DISTINCT u FROM User u LEFT JOIN FETCH u.posts WHERE u.active = true", 
    User.class
).getResultList();
```

**Tools that detect N+1:**
- **Rails:** Bullet gem — logs N+1 queries and suggests fixes in development
- **Django:** Django Debug Toolbar — shows every query per request
- **Node.js/Prisma:** Logging `prisma.$on('query', ...)` shows all queries

---

## ORMs in Each Framework

### ActiveRecord (Rails)

Rails' ORM is famous for its expressiveness. The model IS the table — no separate mapper:

```ruby
class Post < ApplicationRecord
  belongs_to :user
  has_many :comments, dependent: :destroy
  has_many :tags, through: :post_tags
  
  validates :title, presence: true, length: { minimum: 3 }
  validates :body, presence: true
  
  scope :published, -> { where(published: true) }
  scope :recent,    -> { order(created_at: :desc) }
  
  before_save :generate_slug
end

# Powerful query composition:
Post.published.recent.includes(:user, :tags).limit(10)
# SELECT posts.*, users.*, tags.* with joins in 2–3 queries (not 1+N)

# Raw SQL when needed:
Post.find_by_sql("SELECT * FROM posts WHERE MATCH(title) AGAINST(?)", search_term)
```

**Migrations:** Rails generates schema changes as Ruby code:

```ruby
# db/migrate/20240101_add_status_to_posts.rb
class AddStatusToPosts < ActiveRecord::Migration[7.1]
  def change
    add_column :posts, :status, :string, default: 'draft'
    add_index :posts, [:status, :created_at]
  end
end
```

### Eloquent (Laravel)

Very similar to ActiveRecord in philosophy — models are active records:

```php
class Post extends Model
{
    protected $fillable = ['title', 'body', 'user_id'];
    protected $casts = ['published_at' => 'datetime'];
    
    public function user()
    {
        return $this->belongsTo(User::class);
    }
    
    public function scopePublished($query)
    {
        return $query->whereNotNull('published_at');
    }
}

// Usage:
$posts = Post::published()
    ->with(['user', 'tags'])    // eager loading
    ->latest()
    ->paginate(20);
```

### Django ORM

Unique in having relationships defined with explicit field declarations, and a powerful queryset API:

```python
class Post(models.Model):
    title = models.CharField(max_length=200)
    body = models.TextField()
    user = models.ForeignKey(User, on_delete=models.CASCADE, related_name='posts')
    tags = models.ManyToManyField(Tag)
    published_at = models.DateTimeField(null=True, blank=True)
    
    class Meta:
        ordering = ['-created_at']

# Querysets are lazy — SQL runs when you iterate or slice
posts = Post.objects.filter(
    published_at__isnull=False,
    user__is_active=True         # join to users table in one query
).select_related('user')         # eager load user (JOIN)
 .prefetch_related('tags')       # eager load tags (separate query)
 .annotate(comment_count=Count('comments'))  # aggregate
 .order_by('-published_at')[:10]
```

### Spring Data JPA (Hibernate)

Java's ORM is the most powerful and complex. Hibernate generates SQL but can be opaque:

```java
@Entity
@Table(name = "posts")
public class Post {
    @Id @GeneratedValue Long id;
    
    @Column(nullable = false, length = 200)
    String title;
    
    @ManyToOne(fetch = FetchType.LAZY)    // lazy loading = N+1 risk!
    @JoinColumn(name = "user_id")
    User user;
    
    @ManyToMany
    @JoinTable(name = "post_tags", 
               joinColumns = @JoinColumn(name = "post_id"),
               inverseJoinColumns = @JoinColumn(name = "tag_id"))
    Set<Tag> tags;
}

// Spring Data repository:
public interface PostRepository extends JpaRepository<Post, Long> {
    // Method name → SQL:
    List<Post> findByPublishedAtNotNullOrderByPublishedAtDesc(Pageable pageable);
    
    // JPQL with eager loading to avoid N+1:
    @Query("SELECT DISTINCT p FROM Post p LEFT JOIN FETCH p.user LEFT JOIN FETCH p.tags WHERE p.publishedAt IS NOT NULL")
    List<Post> findPublishedWithRelations(Pageable pageable);
}
```

### Prisma (TypeScript/Node.js)

Modern TypeScript ORM with a schema-first approach:

```prisma
// schema.prisma
model Post {
  id          Int      @id @default(autoincrement())
  title       String
  body        String
  publishedAt DateTime?
  user        User     @relation(fields: [userId], references: [id])
  userId      Int
  tags        Tag[]
}
```

```typescript
// TypeScript usage — fully type-safe
const posts = await prisma.post.findMany({
    where: { publishedAt: { not: null } },
    include: { user: true, tags: true },  // eager loading
    orderBy: { publishedAt: 'desc' },
    take: 10,
});
// TypeScript knows exactly what shape `posts` has — no runtime surprises
```

### GORM (Go)

Go's most popular ORM, though many Go developers prefer raw SQL + sqlx:

```go
type Post struct {
    gorm.Model
    Title       string
    Body        string
    UserID      uint
    User        User     // relationship
    Tags        []Tag    `gorm:"many2many:post_tags;"`
    PublishedAt *time.Time
}

// Query
var posts []Post
db.Where("published_at IS NOT NULL").
    Preload("User").           // eager load user
    Preload("Tags").           // eager load tags
    Order("published_at DESC").
    Limit(10).
    Find(&posts)
```

---

## Database Connections and Pooling

### Why Connection Pooling Matters

Opening a database connection is expensive (~5–50ms depending on the database and network). Without pooling, every request opens a new connection:

```
Without pooling:
Request → open connection (~20ms) → query (~5ms) → close connection → Response
Each request wastes 20ms just on connection overhead.

With pooling:
Startup: open 10 connections (pool)
Request → borrow connection (0ms) → query (5ms) → return connection → Response
Connection overhead: ~0ms (already open)
```

Every serious framework has connection pooling:

| Framework | Pool Library | Default Pool Size |
|---|---|---|
| Spring Boot | HikariCP | 10 connections |
| Django | psycopg2 pool (or none; needs CONN_MAX_AGE) | per-request by default |
| Rails | ActiveRecord ConnectionPool | 5 connections |
| Laravel | PDO connection pool | 5 connections |
| Go (GORM/sqlx) | database/sql built-in pool | configurable |
| Node.js (pg/knex) | node-postgres Pool | 10 connections |

### The Pool Size Question

Bigger pool size is not always better. Each connection uses memory on the DATABASE SERVER. PostgreSQL defaults to 100 max connections. If you have 10 application servers each with a 20-connection pool, that's 200 connections to PostgreSQL — over the limit.

Rule of thumb: pool size = (CPU cores × 2) + effective spindle count. For most web apps: 10–20 connections per application instance, with PgBouncer as a proxy to reduce the connections PostgreSQL sees.

---

## Database Types and When to Use Them

### Relational Databases (PostgreSQL, MySQL, SQLite)

Store data in tables with rows and columns. Relationships between tables via foreign keys. Enforce constraints (NOT NULL, UNIQUE, FOREIGN KEY). ACID transactions.

**Use when:** Data has clear relationships. You need transactions. Reporting and complex queries are common.

**PostgreSQL** is the default choice for most new applications — advanced features (JSON, full-text search, CTEs, window functions), excellent performance, reliable.

### Document Databases (MongoDB)

Store documents (JSON-like objects). No enforced schema. Nested objects in one document rather than multiple tables.

**Use when:** Data structure varies per record. Rapid iteration on schema. Content management with irregular structures.

**Watch out for:** No built-in joins. Transactions more complex. Easy to create an unmaintainable schema over time.

### Key-Value Stores (Redis)

Simple: store a value under a key. Extremely fast (in-memory). Often used for caching, sessions, queues.

**Redis in practice:**
- Cache expensive DB query results
- Store user sessions
- Rate limiting counters
- Pub/sub for real-time notifications
- Queue for background jobs (Sidekiq, BullMQ, Celery)

### Time-Series Databases (InfluxDB, TimescaleDB)

Optimized for data points with timestamps: metrics, logs, IoT sensor data.

**Use when:** Storing logs, metrics, analytics events with timestamps as the primary dimension.

---

## Database Migrations: How Schema Changes Work

When you need to add a column, rename a table, or add an index, you need a **migration** — a versioned change to the database schema that can be run in order to bring any database to the current state.

**Rails migrations:**
```ruby
class AddIndexToUsersEmail < ActiveRecord::Migration[7.1]
  def change
    add_index :users, :email, unique: true
    # rollback: remove_index :users, :email
    # Rails knows how to undo add_index
  end
end
```

**Django migrations:**
```
$ python manage.py makemigrations  # Django detects changes to models.py
# Creates: 0015_add_index_to_users_email.py

$ python manage.py migrate         # Applies pending migrations
```

**Flyway/Liquibase (Java):** Plain SQL migration files versioned numerically. Each file runs once.

**Why migrations matter for senior engineers:**
- Migrations are run in production — bad migrations can lock tables and cause downtime
- `ALTER TABLE` can be very slow on large tables (PostgreSQL can lock the table)
- Zero-downtime schema changes require specific strategies (add nullable column, backfill, then add constraint)

---

## Summary

| Concept | What It Is | Key Insight |
|---|---|---|
| **Raw SQL** | Direct SQL strings sent to DB driver | Most control; verbose |
| **Query builder** | Programmatic SQL construction | Type-safe, composable |
| **ORM** | Objects mapped to database tables | Fastest to write; N+1 risk |
| **N+1 problem** | Query per row instead of one JOIN query | Most common performance bug |
| **Eager loading** | Load related data in advance | The N+1 fix |
| **Connection pool** | Reuse open DB connections | Essential for performance |
| **Migration** | Versioned schema change | How you safely evolve schema in production |
| **Redis** | In-memory key-value store | Caching, sessions, queues |

---

*Previous: [Module 06 — Type Systems, Paradigms & Code Architecture](./06_type_systems_and_paradigms.md)*  
*Next: [Module 08 — Choosing the Right Stack: A Decision Framework](./08_choosing_the_right_stack.md)*
