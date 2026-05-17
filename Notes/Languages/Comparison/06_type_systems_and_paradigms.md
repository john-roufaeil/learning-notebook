# Module 06 — Type Systems, Paradigms & Code Architecture Patterns

> **Series:** From Junior to Senior — Languages, Runtimes & Frameworks Explained  
> **Level:** Intermediate → Advanced  
> **Goal:** Understand how languages think about types, how paradigms shape your code, and what architectural patterns each framework enforces.

---

## Part 1: Type Systems

### What is a type system and why does it matter?

A type system is the set of rules a language uses to categorize values (integers, strings, objects) and verify that operations on them make sense. "You can't add a number to a shopping cart" is a type constraint.

Type systems matter because:
- They catch bugs at compile time instead of at 3am in production
- They serve as machine-checked documentation
- They guide IDEs to give you useful autocomplete
- They make large codebase refactoring safer

### The Two Most Important Axes

**Axis 1: Static vs. Dynamic**

**Static:** Types are known at compile time. If you try to call `.toUpperCase()` on an integer, the compiler catches it before the program runs.

```typescript
// TypeScript (static)
let name: string = "Alice";
name.toUpperCase(); // ✓ OK
name.toFixed(2);    // ✗ COMPILE ERROR: Property 'toFixed' does not exist on type 'string'
```

**Dynamic:** Types are checked at runtime. The program crashes (or does something weird) when it encounters a type error during execution.

```python
# Python (dynamic)
name = "Alice"
name.upper()      # works fine
name.is_integer() # AttributeError at RUNTIME — no compile-time warning
```

**Axis 2: Strong vs. Weak**

**Strong:** The language refuses to implicitly convert between incompatible types.

```python
# Python (strong dynamic)
1 + "1"   # TypeError: can only concatenate str (not "int") to str
           # Python refuses to guess what you meant
```

**Weak:** The language silently coerces between types.

```javascript
// JavaScript (weak dynamic)
1 + "1"   // "11" — JavaScript coerces 1 to string "1", then concatenates
1 == "1"  // true — JavaScript coerces types for comparison
1 === "1" // false — strict equality, no coercion
```

JavaScript's weak typing is the source of famous bugs and the entire reason TypeScript was created.

### Combining the Axes

This gives us four quadrants:

```
                    STATIC                    DYNAMIC
            ┌──────────────────────┬──────────────────────┐
            │                      │                      │
 STRONG     │  Java, C#, Rust, Go  │  Python, Ruby        │
            │  TypeScript          │  (strong but dynamic)│
            │                      │                      │
            ├──────────────────────┼──────────────────────┤
            │                      │                      │
 WEAK       │  C, C++              │  JavaScript          │
            │  (implicit casts)    │  PHP (historically)  │
            │                      │                      │
            └──────────────────────┴──────────────────────┘
```

**The ideal for large codebases:** Static + Strong (top-left). You get compile-time checking AND no surprising implicit conversions. Java, Rust, Go, and TypeScript (mostly) live here.

### Nominal vs. Structural Typing

This is a subtler distinction that matters when you start using interfaces and defining APIs.

**Nominal typing:** A type is compatible with an interface only if it explicitly declares that it implements that interface.

```java
// Java (nominal)
interface Printable {
    void print();
}

class Document implements Printable {  // must EXPLICITLY declare
    public void print() { ... }
}

class Report {  // does NOT implement Printable (even though it has print())
    public void print() { ... }
}

void display(Printable p) { p.print(); }

display(new Document()); // ✓ works
display(new Report());   // ✗ COMPILE ERROR: Report doesn't implement Printable
```

**Structural typing:** A type is compatible with an interface if it has the right shape — the right methods with the right signatures — regardless of what it explicitly declares.

```go
// Go (structural interfaces)
type Printable interface {
    Print()
}

type Document struct{}
func (d Document) Print() { ... }  // no explicit "implements Printable"

type Report struct{}
func (r Report) Print() { ... }    // also has Print()

func Display(p Printable) { p.Print() }

Display(Document{}) // ✓ works (has Print method)
Display(Report{})   // ✓ also works (has Print method)
// Neither declares "implements Printable" — Go infers compatibility from shape
```

Go's structural typing is loved for its flexibility. TypeScript also uses structural typing. Java's nominal typing is more explicit and easier to understand in large teams.

### TypeScript: A Special Case

TypeScript deserves special attention because it's widely used but often misunderstood.

TypeScript is a **typed superset of JavaScript** that compiles to plain JavaScript. The TypeScript compiler removes all type annotations and produces `.js` files that run in Node.js or browsers.

```typescript
// TypeScript code:
function greet(name: string): string {
    return `Hello, ${name}`;
}

// Compiled JavaScript output:
function greet(name) {
    return `Hello, ${name}`;
}
```

**Critical implication:** TypeScript types DO NOT EXIST at runtime. There is no runtime type checking. TypeScript is a development-time tool only.

```typescript
// TypeScript thinks this is safe:
const user = JSON.parse(response) as User;  // "as User" is just a type assertion
                                             // no runtime check happens
                                             
user.email.toUpperCase();  // if response.email is null, this crashes at RUNTIME
                           // TypeScript had no way to know — you asserted the type
```

This means TypeScript's type safety is only as good as your type definitions and your discipline. Libraries like **Zod** or **io-ts** add actual runtime validation:

```typescript
import { z } from 'zod';

const UserSchema = z.object({
    email: z.string().email(),
    name: z.string(),
});

// This ACTUALLY validates at runtime:
const user = UserSchema.parse(JSON.parse(response));
// Throws if the data doesn't match — runtime safety
```

---

## Part 2: Programming Paradigms

A **paradigm** is a style of organizing and thinking about code. Most modern languages support multiple paradigms, but each has a dominant style.

### Object-Oriented Programming (OOP)

**Core idea:** Organize code around objects that combine data (fields) and behavior (methods). Objects interact by calling each other's methods.

**Pillars:**
- **Encapsulation:** Hide internal details; expose a public interface
- **Inheritance:** A class can extend another, inheriting its behavior
- **Polymorphism:** Different classes can respond to the same message differently

```python
# OOP in Python
class Animal:
    def __init__(self, name):
        self.name = name
    
    def speak(self):
        raise NotImplementedError

class Dog(Animal):   # inherits from Animal
    def speak(self):
        return f"{self.name} says Woof!"

class Cat(Animal):
    def speak(self):
        return f"{self.name} says Meow!"

animals = [Dog("Rex"), Cat("Whiskers")]
for animal in animals:
    print(animal.speak())  # polymorphism: same method, different behavior
```

**Languages that emphasize OOP:** Java, C#, Ruby, Python

**Java is especially OOP:** Java requires everything to be inside a class. Until Java 21, you couldn't write a function outside of a class. This strict OOP model is both its strength (clear organization) and criticism (boilerplate for simple tasks).

### Functional Programming (FP)

**Core idea:** Build programs by composing pure functions. Avoid mutable state. A pure function always returns the same output for the same input and has no side effects.

```javascript
// Imperative (OOP-style): mutate state
const numbers = [1, 2, 3, 4, 5];
const result = [];
for (let n of numbers) {
    if (n % 2 === 0) {
        result.push(n * 2);
    }
}

// Functional: compose transformations, no mutation
const result = numbers
    .filter(n => n % 2 === 0)   // select even numbers
    .map(n => n * 2);            // double them
// original array unchanged; result is a new array
```

**Languages that emphasize FP:** Haskell, Elixir, Clojure, F#  
**Languages with strong FP support:** Scala, Rust, JavaScript, Python, Kotlin, C#

**Why FP matters for senior engineers:**
- Pure functions are trivially testable (no mocking needed)
- Immutability prevents an entire class of bugs (shared mutable state bugs)
- FP patterns (map, filter, reduce, flatMap) appear everywhere in modern codebases — React, LINQ, Java Streams, Python comprehensions

### Procedural Programming

**Core idea:** Write a sequence of instructions. Use functions/procedures to organize code.

```c
// Procedural C
int add(int a, int b) {
    return a + b;
}

int main() {
    int sum = add(3, 4);
    printf("Sum: %d\n", sum);
    return 0;
}
```

C is procedural. Go is mostly procedural with some OOP features. Many languages support procedural style for simpler code.

### How Paradigms Appear in Frameworks

| Framework | Dominant Paradigm | Notes |
|---|---|---|
| **Rails** | OOP (MVC) | Heavy OOP + ActiveRecord pattern |
| **Django** | OOP (MVC) | Models as OOP, class-based views, mixins |
| **Spring Boot** | OOP + dependency injection | Annotation-heavy OOP |
| **Express** | Procedural / FP | Middleware is function composition |
| **NestJS** | OOP + decorators | Angular-style OOP with DI |
| **FastAPI** | Procedural + FP | Function-based routes, Pydantic models |
| **React (Next.js)** | FP + component model | Functional components, immutable state |
| **Gin** | Procedural | Functions, no classes |

---

## Part 3: Architectural Patterns

These are the patterns that frameworks impose or encourage — the structural decisions that shape entire codebases.

### MVC: Model-View-Controller

The most common architecture in traditional web frameworks.

```
┌──────────┐    user action     ┌─────────────┐
│  Browser │ ─────────────────► │  Controller │
│  (View)  │                    │  (routes,   │
└────▲─────┘                    │   logic)    │
     │                          └──────┬──────┘
     │  HTML/JSON                      │ reads/writes
     │                          ┌──────▼──────┐
     └──────────────────────────│    Model    │
                                │  (database  │
                                │   access)   │
                                └─────────────┘
```

**Controller:** Receives HTTP requests, orchestrates logic, calls models, returns responses  
**Model:** Represents data and database access  
**View:** Templates/serializers that format the response

**Rails, Django, Laravel, Spring Boot** all implement MVC (though they call it different things — Spring calls controllers "Controllers", views "Templates/ResponseBody", models "Entities/Repositories").

### Dependency Injection (DI) and IoC Containers

**The problem without DI:**

```java
// Tightly coupled — hard to test, hard to change
class UserService {
    private final DatabaseClient db;
    
    UserService() {
        this.db = new PostgreSQLClient("jdbc:postgresql://localhost/mydb");
        // UserService creates its own DatabaseClient
        // To test UserService, you need a real database
        // To change the database, you change UserService
    }
}
```

**With DI:**

```java
// Loosely coupled — testable, flexible
class UserService {
    private final DatabaseClient db;
    
    UserService(DatabaseClient db) {  // dependency is injected from outside
        this.db = db;
    }
}

// In tests:
UserService service = new UserService(new InMemoryDatabase()); // fake DB

// In production:
UserService service = new UserService(new PostgreSQLClient(config));

// With Spring Boot IoC container:
@Service
class UserService {
    @Autowired DatabaseClient db;  // Spring creates and injects this
}
```

Spring Boot, NestJS, and Laravel all have IoC containers that automatically create objects and inject their dependencies. This pattern makes large codebases testable and modular.

### Repository Pattern

Separates data access logic from business logic:

```typescript
// Without repository (data access in business logic):
async function deleteUser(userId: number) {
    const user = await db.query('SELECT * FROM users WHERE id = $1', [userId]);
    if (user.subscription_type === 'premium') {
        throw new Error("Cannot delete premium users");
    }
    await db.query('DELETE FROM users WHERE id = $1', [userId]);
    await db.query('DELETE FROM sessions WHERE user_id = $1', [userId]);
}

// With repository pattern:
class UserRepository {
    async findById(id: number): Promise<User> {
        return db.query('SELECT * FROM users WHERE id = $1', [id]);
    }
    async delete(id: number): Promise<void> {
        await db.query('DELETE FROM users WHERE id = $1', [id]);
    }
}

class UserService {
    constructor(private repo: UserRepository) {}
    
    async deleteUser(userId: number): Promise<void> {
        const user = await this.repo.findById(userId);
        if (user.subscriptionType === 'premium') {
            throw new Error("Cannot delete premium users");
        }
        await this.repo.delete(userId);
    }
}
```

**Benefits:** Business logic doesn't depend on database specifics. You can swap PostgreSQL for MySQL by changing the Repository, not the Service. You can test the Service with a mock Repository.

Spring Data, NestJS TypeORM, and Go's common patterns all use this.

### Middleware Pattern

Already covered in Module 05, but worth naming explicitly as an architectural pattern. The middleware pattern chains functions that each:
1. Receive the request (and some context)
2. Do some work
3. Either respond (short-circuit) or pass to the next middleware

This is how Express, Gin, ASP.NET Core, and Django implement their request pipelines.

### Event-Driven Architecture

Rather than calling functions directly, components emit **events** and other components subscribe to them.

```javascript
// Direct coupling (tight):
class OrderService {
    placeOrder(order) {
        this.save(order);
        emailService.sendConfirmation(order);    // direct call
        inventoryService.updateStock(order);     // direct call
        analyticsService.trackPurchase(order);   // direct call
    }
}

// Event-driven (loose coupling):
class OrderService {
    placeOrder(order) {
        this.save(order);
        eventBus.emit('order.placed', order);    // just emit an event
        // OrderService doesn't know who's listening or care
    }
}

// Separately, somewhere else:
eventBus.on('order.placed', (order) => emailService.sendConfirmation(order));
eventBus.on('order.placed', (order) => inventoryService.updateStock(order));
eventBus.on('order.placed', (order) => analyticsService.trackPurchase(order));
```

This pattern is used heavily in:
- Message queues (Kafka, RabbitMQ, SQS) — for distributed systems
- Spring's `@EventListener` — within a Spring application
- Node.js's EventEmitter — the basis of Node's I/O model
- Sidekiq/Resque/Celery — background job processing

### REST vs. GraphQL vs. gRPC

These are API architecture styles — the contract between your backend and its clients.

**REST (Representational State Transfer):**  
Resources are nouns (URLs), actions are HTTP verbs (GET, POST, PUT, DELETE). Dominant style.

```
GET    /users/42        → get user 42
POST   /users           → create user
PUT    /users/42        → update user 42
DELETE /users/42        → delete user 42
GET    /users/42/posts  → get posts for user 42
```

**GraphQL:**  
Client specifies exactly what data it needs in a query. Single endpoint. Great for complex frontends that need flexible data shapes.

```graphql
# Client asks for exactly what it needs
query {
    user(id: 42) {
        name
        email
        posts(limit: 5) {
            title
            publishedAt
        }
    }
}
```

**gRPC:**  
Google's high-performance RPC (Remote Procedure Call) protocol. Uses Protocol Buffers for serialization (much smaller than JSON). Strongly typed. Used heavily for service-to-service communication where performance matters.

```protobuf
// Service definition
service UserService {
    rpc GetUser (GetUserRequest) returns (User);
    rpc CreateUser (CreateUserRequest) returns (User);
}

message GetUserRequest { int64 id = 1; }
message User {
    int64 id = 1;
    string email = 2;
    string name = 3;
}
```

**When to use what:**
- **REST:** Default choice for HTTP APIs — universal, simple, well-understood
- **GraphQL:** When frontend teams need flexibility and you have complex, nested data (social graph, e-commerce catalog)
- **gRPC:** Service-to-service communication in microservices; when you need performance and strict contracts

---

## Part 4: How These Concepts Connect

Here's the synthesis — how Module 01–06 concepts connect to explain framework behavior:

```
Language execution model (Module 01)
    ↓ determines
Startup time, peak performance, cold start behavior
    ↓ shapes
Deployment model (binary, container, serverless)

Memory management (Module 02)
    ↓ determines
GC pause behavior, memory footprint per instance
    ↓ shapes
Cost at scale, P99 latency behavior

Concurrency model (Module 03)
    ↓ determines
How many concurrent requests can be handled
    ↓ shapes
Framework design (thread pool size, async/sync API)

Type system (Module 06 Part 1)
    ↓ determines
When errors are caught, how safe large codebases are
    ↓ shapes
How productive teams are during refactoring

Paradigm (Module 06 Part 2)
    ↓ determines
Code organization, testability, reusability
    ↓ shapes
How maintainable the codebase is over time
```

---

## Summary

| Language | Type System | Primary Paradigm | Null Safety? | Key Architectural Pattern |
|---|---|---|---|---|
| **C** | Static, weak | Procedural | No | Manual resource management |
| **C++** | Static, weak | OOP + procedural | No | RAII, ownership via smart ptrs |
| **Rust** | Static, strong | Multi-paradigm | Yes (Option<T>) | Ownership, no shared mutable state |
| **Go** | Static, structural | Procedural + interfaces | No | Composition, explicit error handling |
| **Java** | Static, nominal | OOP | No (NullPointerException) | DI/IoC, Repository, MVC |
| **C#** | Static, nominal | OOP + FP | Yes (C#8+ nullable) | DI/IoC, async/await, LINQ |
| **Python** | Dynamic, strong | OOP + FP + procedural | No | Duck typing, protocols |
| **JavaScript** | Dynamic, weak | FP + OOP + procedural | No | Event-driven, callbacks, promises |
| **TypeScript** | Static (structural, unsound) | FP + OOP | Yes (strict mode) | Same as JS + interfaces/generics |
| **Ruby** | Dynamic, strong | OOP | No | Convention, metaprogramming, DSLs |
| **PHP** | Dynamic (improving) | OOP + procedural | No | MVC, ActiveRecord (Eloquent) |

---

*Previous: [Module 05 — Frameworks In Depth](./05_frameworks_in_depth.md)*  
*Next: [Module 07 — Databases, ORMs & the Data Layer](./07_databases_and_orms.md)*
