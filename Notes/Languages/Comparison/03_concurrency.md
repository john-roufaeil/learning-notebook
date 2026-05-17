# Module 03 — Concurrency: How Languages Do Multiple Things at Once

> **Series:** From Junior to Senior — Languages, Runtimes & Frameworks Explained  
> **Level:** Intermediate  
> **Goal:** Understand threads, event loops, goroutines, and async/await — and why this determines how frameworks handle load.

---

## The Problem: A Web Server Needs to Handle Many Users Simultaneously

Imagine you're running a restaurant. One customer orders food, and the chef starts cooking. While that order is being prepared, other customers are waiting to order. If the chef refused to take new orders until the first plate was completely done, everyone would walk out.

Web servers face the same problem. A request arrives, the server starts processing it (maybe waiting 50ms for a database query), and then another request arrives. If the server is locked on the first request, the second user waits. At 1,000 users per second, this becomes catastrophic.

Every language and framework has a strategy for this. The strategy chosen fundamentally shapes the entire framework design.

---

## Two Types of "Waiting"

Before comparing strategies, understand the two types of work a server does:

**I/O-bound work:** Waiting for something external to respond.
- Waiting for a database query to return
- Waiting for a network request to another API
- Waiting for a file to be read from disk

During I/O-bound waits, your code is literally doing nothing — just waiting for the database to respond. The CPU is idle.

**CPU-bound work:** Actually computing something.
- Calculating a hash
- Parsing JSON
- Resizing an image
- Running a machine learning model

During CPU-bound work, the CPU is busy. You can't do two CPU-bound tasks on one CPU core simultaneously.

The difference matters enormously because different concurrency models handle these two types differently. A great solution for I/O-bound work may be terrible for CPU-bound work and vice versa.

---

## Strategy 1: One Thread Per Request (Traditional Model)

**Used by:** Java (pre-2021), PHP-FPM, Ruby on Rails (Puma), ASP.NET (traditional)

The simplest approach: when a request arrives, give it its own thread. The thread handles that request from start to finish.

```
Request 1 → Thread 1 → [handles req 1] → [done]
Request 2 → Thread 2 → [handles req 2] → [done]
Request 3 → Thread 3 → [handles req 3] → [done]
...
Request N → Thread N → [waiting for a free thread]
```

**OS Threads:** These are real threads managed by the operating system. The OS can run multiple threads simultaneously on multiple CPU cores. One thread can block (waiting for DB) while another runs.

**Why this works:** Simple to understand. Each request is fully isolated. If Thread 2 crashes, Threads 1 and 3 continue.

**Why this has limits:** OS threads are expensive. Each thread needs memory for its stack (~1MB by default). If you have 10,000 concurrent users, you need 10,000 threads, which requires ~10GB of RAM just for stacks. Plus the OS has to context-switch between thousands of threads — constantly saving and restoring state.

**Practical ceiling:** ~500–2,000 concurrent threads before performance degrades. This is why Spring Boot's default Tomcat configuration has a thread pool of 200 threads. If 201 requests come in simultaneously, the 201st waits for one of the 200 threads to become free.

**The "thread pool exhaustion" failure mode:**  
Imagine your DB is running slow (100ms queries instead of 10ms). 200 threads, all blocked waiting for DB responses. New requests queue up. The queue fills. The server appears hung. This is one of the most common production incidents in Java/thread-based systems.

---

## Strategy 2: The Event Loop (Single Thread, Non-Blocking)

**Used by:** Node.js, Python (asyncio/FastAPI), Ruby (EventMachine)

Instead of many threads, use ONE thread that never blocks. When there's I/O to wait for, the thread registers interest and moves on to the next task. When the I/O completes, the thread comes back to handle it.

```
Single thread running an event loop:

Event: Request 1 arrives
  → Start handling req 1
  → Send DB query (non-blocking: "tell me when done, I'll do other work")
  
Event: Request 2 arrives (while DB query for req 1 is in flight)
  → Start handling req 2
  → Send DB query for req 2 (non-blocking)
  
Event: DB responded for Request 1
  → Return to handling req 1
  → Send response to user 1
  
Event: DB responded for Request 2
  → Return to handling req 2
  → Send response to user 2
```

The thread is constantly doing work — never blocking, always moving. I/O callbacks wake it up when data is ready.

**Why this works for I/O-bound servers:**  
Node.js can handle 50,000 concurrent connections on a single thread because "concurrent" means "waiting for I/O simultaneously," not "doing CPU work simultaneously." The single thread processes events as they complete.

**The critical limitation:** CPU-bound work blocks everything.

```
// DISASTER in Node.js: CPU-intensive code on the event loop
app.get('/process', (req, res) => {
    // This 500ms computation blocks ALL other requests
    const result = heavyCPUComputation();  // 500ms of pure CPU work
    res.json(result);
    
    // While this runs, ZERO other requests can be handled
    // If you have 1,000 users, they all wait
});
```

If you put 500ms of CPU work on Node.js's event loop, your entire server stalls for 500ms. Every single user waits. This is the event loop's Achilles heel.

**The Node.js model in detail:**

Node.js uses a library called **libuv** that manages the event loop. libuv uses the operating system's efficient I/O notification mechanisms (epoll on Linux, kqueue on macOS) to monitor thousands of file descriptors simultaneously. When any of them has data ready, libuv adds a callback to the event queue.

```
User code (JavaScript, single thread)
        │
        │ delegates I/O to...
        ▼
   libuv (C library)
        │
        ├── epoll/kqueue: monitors network sockets, files
        ├── Thread pool: handles file I/O, crypto (4 threads by default)
        └── Timers: setTimeout, setInterval

When I/O completes → callback queued → JS thread processes it
```

---

## Strategy 3: Async / Await — Making Async Code Readable

**Used by:** JavaScript/TypeScript, Python (asyncio), C#, Rust, Dart

The event loop model requires writing callbacks — functions that run when something completes. This leads to "callback hell":

```javascript
// Callback hell (old style)
db.query('SELECT * FROM users', function(err, users) {
    if (err) {
        return handleError(err);
    }
    getPermissions(users[0].id, function(err, permissions) {
        if (err) {
            return handleError(err);
        }
        sendNotification(users[0].email, permissions, function(err) {
            // ... nested forever
        });
    });
});
```

`async/await` is syntactic sugar that makes this readable while keeping the non-blocking behavior. Under the hood, it still runs on an event loop — it just looks like synchronous code:

```javascript
// Same logic with async/await
async function processUser() {
    const users = await db.query('SELECT * FROM users');
    const permissions = await getPermissions(users[0].id);
    await sendNotification(users[0].email, permissions);
}
```

The `await` keyword means "pause THIS function here and let other tasks run until the result is ready." It doesn't block the thread — it suspends the current async function and resumes it when done.

**This is the same event loop model, just with much better syntax.**

---

## Strategy 4: Goroutines — Lightweight Threads at Scale

**Used by:** Go

Go invented a new approach: **goroutines**. These look like threads (you spawn them the same way) but they're managed by Go's runtime, not the OS.

```go
// Spawning 10,000 goroutines is perfectly fine
for i := 0; i < 10000; i++ {
    go handleRequest(request) // spawns a goroutine
}
```

**What makes goroutines different from OS threads:**

OS threads are expensive (~1MB stack, managed by OS). Goroutines start with a tiny 2KB stack that grows as needed. Go's runtime manages thousands of goroutines on a handful of OS threads, automatically switching between them.

```
Go Runtime:

  Goroutine 1 (2KB stack) ─┐
  Goroutine 2 (2KB stack) ─┤  scheduled by Go runtime
  Goroutine 3 (2KB stack) ─┤  onto OS threads
  ...
  Goroutine 100,000        ─┘
        │
        ▼
  OS Thread 1 ─┐
  OS Thread 2 ─┤  one per CPU core (typically)
  OS Thread 3 ─┤
  OS Thread 4 ─┘
        │
        ▼
        CPUs
```

**The key behavior:** When a goroutine does I/O (waits for DB, network), Go's runtime automatically parks it and runs a different goroutine on that OS thread. When the I/O completes, the goroutine is scheduled to run again. You write code that looks blocking, but it's non-blocking under the hood.

```go
// Go code that looks blocking but isn't:
func handleRequest(w http.ResponseWriter, r *http.Request) {
    user, err := db.Query("SELECT * FROM users WHERE id = ?", id) // looks blocking!
    // but: Go parks this goroutine while waiting, runs other goroutines
    // when DB responds, this goroutine resumes automatically
    sendJSON(w, user)
}
```

**Why Go scales well:** 500,000 goroutines × 2KB each = 1GB of stack memory. Compare to 500,000 OS threads × 1MB each = 500GB. Goroutines can handle enormous concurrency.

---

## Strategy 5: The GIL — Python's Concurrency Ceiling

**Specific to:** CPython (the standard Python implementation)

Python has a mechanism called the **GIL (Global Interpreter Lock)** that prevents true parallel execution of Python code. At any moment, only one thread can execute Python bytecode, even if you have 8 CPU cores.

```
Python with 4 threads on 4 CPU cores:

What you expect:
Core 1: Thread 1 ████████████
Core 2: Thread 2 ████████████
Core 3: Thread 3 ████████████
Core 4: Thread 4 ████████████

What actually happens (CPU-bound Python):
Core 1: Thread 1 ██░░████░░██
Core 2: Thread 2 ░░██░░██░░░░  (only one runs at a time!)
Core 3: Thread 3 waiting
Core 4: Thread 4 waiting
```

**Why does the GIL exist?** Python's reference counting (remember from Module 02) isn't thread-safe. If two threads simultaneously increment/decrement the same object's reference count, they can corrupt it. The GIL prevents this by only letting one thread run at a time.

**The workaround for I/O:** The GIL is released during I/O operations! When your Python thread is waiting for a database response, it releases the GIL, and another thread can run Python code. This is why Python threading works for I/O-bound workloads — threads genuinely run concurrently during I/O waits.

**For CPU-bound work:** Python threads give you no parallelism. The standard solution is `multiprocessing` — separate Python processes each with their own GIL. Or using libraries like NumPy that release the GIL in their C code.

**Python's asyncio:** Python's async solution is like Node.js — single-threaded, event loop, works great for I/O-bound work. FastAPI is built on this. But unlike Node.js's V8 engine (which has JIT), Python's event loop is still running through the CPython interpreter, so it's slower per operation.

---

## Comparing Concurrency Models for a Web Server

Let's say 1,000 requests arrive simultaneously, each needing a 100ms database query.

### Thread-per-request (Java/Spring, 200 thread pool):
- Threads 1–200 start immediately
- Requests 201–1,000 queue up waiting for a thread
- As threads finish and return to pool, queued requests start
- If DB slows to 500ms, all 200 threads are occupied, everything queues → service appears unresponsive

### Event loop (Node.js/Express):
- Single thread sends 1,000 async DB queries simultaneously
- All 1,000 queries are "in flight" at once (just waiting for responses)
- As responses arrive, the event loop handles each one and sends the response
- 1,000 concurrent DB connections required — but Node.js handles this fine
- Works great until there's CPU work — then everything blocks

### Goroutines (Go/Gin):
- 1,000 goroutines spawned, each handling one request
- Each goroutine makes a blocking DB call (but Go makes it non-blocking internally)
- Go runtime parks goroutines while waiting, runs CPU work on OS threads
- Best of both worlds: handles I/O AND CPU efficiently

### Python asyncio (FastAPI):
- Similar to Node.js — single thread, async DB calls
- All 1,000 queries in flight simultaneously
- GIL not relevant here (we're not using threads)
- Works for I/O, but slower per-operation than Node.js due to CPython overhead

---

## What This Means for Frameworks

| Framework | Language | Concurrency Model | Good For | Watch Out For |
|---|---|---|---|---|
| **Spring Boot** | Java | Thread pool (default) or reactive | Enterprise APIs | Thread pool exhaustion under slow DB |
| **Spring WebFlux** | Java | Reactive (event-loop-like via Netty) | High-concurrency, streaming | Complex programming model |
| **Express.js** | Node.js | Event loop (single-threaded) | I/O-heavy APIs | CPU-bound code blocks everything |
| **NestJS** | Node.js | Event loop (Express or Fastify) | Structured Node APIs | Same as Express |
| **FastAPI** | Python | Async event loop (asyncio) | I/O-heavy APIs | CPU-bound blocks all requests |
| **Django** | Python | Sync (Gunicorn workers) | CRUD apps, content | Scale = more processes |
| **Flask** | Python | Sync (Gunicorn workers) | Microservices, simple APIs | Same as Django |
| **Gin** | Go | Goroutines | High-performance APIs | Less mature ecosystem than Java/Node |
| **Rails** | Ruby | Thread pool (Puma) | CRUD, feature-rich apps | Memory-heavy, GVL limits CPU parallelism |
| **Laravel** | PHP | Prefork (process per request) | Web apps, CMS | Process overhead, no in-memory state |
| **ASP.NET Core** | C# | Async/await, thread pool | Enterprise, high-performance | Windows-first history (though Linux now fine) |
| **Next.js** | Node.js | Event loop (SSR/API routes) | Full-stack web, React | SSR CPU work on event loop |

---

## The GVL: Ruby's GIL Equivalent

Ruby's MRI interpreter (the standard one) has a **GVL (Global VM Lock)** that works exactly like Python's GIL. Only one thread runs Ruby code at a time.

This is why Puma (Rails' server) uses a mix of processes AND threads:
- Multiple processes: each has its own GVL, can run in parallel on multiple CPU cores
- Multiple threads per process: useful when threads are waiting on I/O (DB queries, API calls)

Ruby 3.0+ introduced **Ractors** — an experimental model for true parallelism without the GVL. They're like separate mini-Ruby runtimes that communicate by passing messages instead of sharing memory.

---

## Java Virtual Threads: The Newer Answer

Java 21 introduced **Virtual Threads** (Project Loom). These are managed by the JVM rather than the OS — similar to goroutines. You can have millions of virtual threads. When a virtual thread does blocking I/O, the JVM transparently parks it and runs another.

This means Spring Boot can now handle high concurrency without complex reactive programming, and without the thread pool exhaustion problem. You write normal blocking code, but the JVM makes it non-blocking behind the scenes.

This is a major shift in the Java ecosystem and brings Java closer to Go's model while keeping the simpler blocking programming style.

---

## The Key Mental Model: Concurrency vs. Parallelism

These two words are often confused:

**Concurrency:** Handling multiple tasks by interleaving them. A single barista at a coffee shop — takes one order, starts the machine, takes another order while the machine runs. Not doing two things simultaneously, but making progress on many things.

**Parallelism:** Doing multiple tasks simultaneously. Multiple baristas — each one independently serving a customer at the same time.

- **Event loop (Node.js, asyncio):** Concurrency without parallelism. One thread, many tasks interleaved.
- **OS Thread pool:** Parallelism (multiple CPU cores doing work) + concurrency (threads waiting on I/O).
- **Goroutines:** Both — thousands of concurrent tasks, automatically scheduled for parallelism across CPU cores.
- **Python GIL + threads:** Concurrency (I/O interleaved) but NOT parallelism for CPU work.

For web APIs that are mostly I/O-bound (waiting for DB, waiting for other services), concurrency without parallelism is often sufficient. Node.js and FastAPI work well here. For CPU-heavy workloads, you need true parallelism — Go, Java, C# are better fits.

---

## Summary

| Model | How It Works | Best For | Limitation |
|---|---|---|---|
| **Thread per request** | OS thread for each request | Simple, isolated handling | Memory + OS overhead at scale |
| **Thread pool** | Fixed pool of reusable threads | Most traditional web servers | Pool exhaustion under load |
| **Event loop** | Single thread, non-blocking I/O callbacks | High-concurrency I/O | CPU work blocks everything |
| **async/await** | Looks synchronous, runs on event loop | Readable async code | Still event loop limitations |
| **Goroutines** | Lightweight, Go-managed "threads" | Both I/O and CPU work | Go ecosystem only |
| **Virtual threads** | JVM-managed lightweight threads | Java async without complexity | Java 21+ required |
| **GIL/GVL** | Lock ensuring one thread runs at a time | N/A (limitation) | Prevents CPU parallelism in Python/Ruby |

---

*Previous: [Module 02 — Memory: Where Your Data Lives and Who Cleans It Up](./02_memory_management.md)*  
*Next: [Module 04 — Languages In Depth: A Tour of Every Major Language](./04_languages_in_depth.md)*
