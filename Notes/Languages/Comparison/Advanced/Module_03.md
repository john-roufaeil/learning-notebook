# Module 03: Concurrency, Threading, and I/O Paradigms

> **Series:** Principal Engineer's Deep-Dive into Languages, Runtimes, and Frameworks  
> **Prerequisites:** Module 01 (Execution Models), Module 02 (Memory Management)

---

## Table of Contents

1. [The Concurrency Problem Space](#1-the-concurrency-problem-space)
2. [OS Kernel Threads: Java, C, C++](#2-os-kernel-threads)
3. [Go's M:N Scheduler and Goroutines](#3-go-goroutines-and-mn-scheduler)
4. [Node.js: Single-Threaded Event Loop with libuv](#4-nodejs-event-loop-and-libuv)
5. [Python's GIL: The Concurrency Ceiling](#5-pythons-gil)
6. [Ruby YARV: GIL and YJIT Interactions](#6-ruby-yarv-and-the-gil)
7. [PHP-FPM: The Prefork Shared-Nothing Model](#7-php-fpm-prefork-model)
8. [Async/Await: C#, Rust, Python, JavaScript](#8-asyncawait-across-runtimes)
9. [I/O Multiplexing: epoll, kqueue, IOCP](#9-io-multiplexing-internals)
10. [Failure Modes Under Load](#10-failure-modes-under-load)
11. [Comparative Concurrency Matrix](#11-comparative-matrix)

---

## 1. The Concurrency Problem Space

Before comparing implementations, precisely define what we are solving:

```
Concurrency vs. Parallelism:

  Concurrency: Dealing with multiple things at once (structure)
               A single-core system running a web server is concurrent —
               it interleaves handling multiple connections.

  Parallelism:  Doing multiple things at once (execution)
               A multi-core system running Go goroutines is parallel —
               goroutines execute simultaneously on different CPUs.

  A system can be concurrent without being parallel (Node.js on 1 core).
  A system can be parallel without structured concurrency (fork/join pools).
```

**Two fundamental bottleneck types** drive all concurrency design decisions:

```
I/O-Bound workload:
  Thread spends majority of time WAITING for:
    - Network socket reads/writes
    - Disk reads/writes
    - Database query responses
    - External API calls
  CPU utilization: low
  Optimal strategy: maximize concurrent waiters; minimize threads-per-waiter

CPU-Bound workload:
  Thread spends majority of time COMPUTING:
    - JSON parsing
    - Cryptographic operations
    - Image processing
    - Machine learning inference
  CPU utilization: high
  Optimal strategy: one thread per CPU core; minimize context switching
```

**The golden rule:** Every concurrency model in this module is an attempt to efficiently handle one or both of these bottleneck types.

---

## 2. OS Kernel Threads

### 2.1 The 1:1 Threading Model

Java, C, and C++ (by default) use **OS kernel threads** — each language-level "thread" is mapped 1:1 to an OS-scheduled thread.

```
OS Thread Architecture:

  User Space              Kernel Space
  ─────────────────────   ──────────────────────────────
  Thread A ─────────────► OS Thread (kernel stack ~1MB)
  Thread B ─────────────► OS Thread (kernel stack ~1MB)
  Thread C ─────────────► OS Thread (kernel stack ~1MB)
  ...N threads            ...N kernel threads
                                      │
                                      ▼
                          CPU Scheduler (preemptive)
                          Assigns threads to CPU cores
```

**OS thread costs:**
- **Memory:** Each kernel thread requires ~64KB–8MB of kernel stack (default 1–8MB on Linux). 10,000 threads = 10–80GB of stack space.
- **Context switch:** Saving/restoring full CPU register state, TLB flush (if different process), scheduler decision — typically **1–10µs** on modern hardware.
- **Scheduling:** The OS scheduler uses priority queues and time slices. It has no knowledge of what your thread is doing (waiting for I/O or computing).

### 2.2 Java Threading

```java
// Java: explicit thread creation (legacy pattern)
Thread t = new Thread(() -> {
    // runs in an OS kernel thread
    System.out.println("Thread: " + Thread.currentThread().getName());
});
t.start();

// Java thread pool (ExecutorService — preferred)
ExecutorService pool = Executors.newFixedThreadPool(
    Runtime.getRuntime().availableProcessors()  // one thread per CPU core
);
pool.submit(() -> processRequest(request));

// Java 21: Virtual Threads (Project Loom)
// Creates a "virtual thread" — lightweight, scheduled by JVM, not OS
Thread vt = Thread.ofVirtual().start(() -> {
    // Blocking I/O here does NOT block an OS thread
    // JVM parks the virtual thread, unmounts from carrier thread
    // Carrier thread (OS thread) handles another virtual thread
    InputStream is = socket.getInputStream();
    byte[] buf = is.readAllBytes();  // "blocking" — actually non-blocking under the hood
});
```

**Java Virtual Threads (Loom — JDK 21+):** This is the most significant Java concurrency change in a decade. Virtual threads are M:N scheduled (millions of virtual threads on a handful of OS carrier threads). When a virtual thread blocks on I/O, the JVM:
1. Detects the blocking call (via instrumented JDK I/O classes)
2. Suspends the virtual thread (saves its stack to heap)
3. Unmounts it from the carrier OS thread
4. Carrier thread picks up another ready virtual thread

This is architecturally equivalent to Go's goroutine model, now available in Java.

### 2.3 C++ Thread Synchronization Primitives

```cpp
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

// Mutex: mutual exclusion (blocking)
std::mutex mtx;
std::vector<int> shared_data;

void producer() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII lock — unlocks on scope exit
    shared_data.push_back(42);
}

// Atomic: lock-free for simple types
std::atomic<int> counter{0};
counter.fetch_add(1, std::memory_order_relaxed);  // no memory fence
counter.fetch_add(1, std::memory_order_seq_cst);  // sequential consistency (default)
// Memory ordering is critical for correctness on weakly-ordered architectures (ARM)

// Condition variable: blocking wait for condition
std::condition_variable cv;
bool ready = false;

void waiter() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, []{ return ready; });  // atomically releases lock and blocks
    // lock re-acquired when woken
}

void notifier() {
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_one();  // wake one waiter
}
```

**C++ memory model (C++11+):** Defines a formal memory model specifying when writes in one thread become visible to other threads. `std::memory_order` gives fine-grained control over compiler and CPU instruction reordering — critical for writing correct lock-free data structures.

---

## 3. Go Goroutines and M:N Scheduler

### 3.1 The M:N Model

Go's scheduler implements **M:N threading**: M goroutines multiplexed onto N OS threads (where N typically equals the number of CPU cores, controlled by `GOMAXPROCS`).

```
Go M:N Scheduler Components:

  G (Goroutine): A lightweight coroutine with a small, growable stack (2–8KB initial)
  M (Machine):  An OS thread (M maps to CPU execution resource)
  P (Processor): A scheduling context; holds the run queue

  Architecture:

  ┌─────────────────────────────────────────────────────────────┐
  │  Global Run Queue: [G5] [G6] [G7]                          │
  └──────────────┬──────────────────────────────────────────────┘
                 │ (steal when local queue empty)
  ┌──────────────┴──────────────────────────────────────────────┐
  │  P0              P1              P2              P3          │
  │  Local Queue:    Local Queue:    Local Queue:    Local Queue:│
  │  [G1][G2][G3]    [G4][G8]        [G9]            []         │
  │      │               │               │                      │
  │      M0              M1              M2              M3      │ ← OS threads
  │  (running G1)    (running G4)   (running G9)   (idle/spin) │
  └─────────────────────────────────────────────────────────────┘
```

### 3.2 Goroutine Scheduling: Cooperative with Preemption

Early Go used purely cooperative scheduling — goroutines yielded at function call boundaries. Go 1.14+ added **asynchronous preemption** via OS signals (SIGURG on Linux/macOS): the runtime can preempt a goroutine at any safe point, not just at function calls. This prevents CPU-bound goroutines from starving others.

```go
// Goroutine creation: dirt cheap (~2KB stack, ~200ns to create)
go func() {
    // runs concurrently, possibly in parallel (different OS thread)
    result := computeHeavyWork()
    resultChan <- result
}()

// Channel: typed, goroutine-safe communication (CSP model)
ch := make(chan int, 10)  // buffered channel, capacity 10

go func() {
    for i := 0; i < 100; i++ {
        ch <- i  // send; blocks if channel full
    }
    close(ch)
}()

for v := range ch {
    fmt.Println(v)  // receive; blocks if channel empty; terminates on close
}

// Select: multiplex on multiple channels (like epoll for channels)
select {
case msg := <-incoming:
    handleMessage(msg)
case <-timeout:
    return errors.New("timed out")
case <-ctx.Done():
    return ctx.Err()
}
```

### 3.3 Goroutine Stack Growth

This is one of Go's most elegant mechanisms:

```
Goroutine Stack Growth:

  Initial stack: 2KB (Go 1.4+) or 8KB (older)

  Stack overflow detection: Each function prologue checks:
    if sp < stackguard0 { runtime.morestack() }
  
  runtime.morestack():
    1. Allocates a new, LARGER stack (typically 2x current size)
    2. Copies the current stack frame to the new stack
    3. Updates all pointers that pointed into the old stack
    4. Frees the old stack
    5. Returns to user code on the new stack

  Why this matters:
    - You can have 1,000,000 goroutines each with 2KB stack = 2GB total
    - vs. 1,000,000 OS threads each with 1MB stack = 1TB total (impossible)
    - Goroutines scale to millions; OS threads plateau at ~10,000
```

### 3.4 I/O and Goroutine Parking

When a goroutine performs blocking I/O (network read, file read, syscall), the Go runtime does NOT block the OS thread:

```
Goroutine I/O Lifecycle:

  G1 calls conn.Read(buf) // "blocking" from user's perspective
    │
    ▼ runtime: Check if data is ready (non-blocking syscall)
    │
    ├── Data ready → return data immediately, G1 continues on M
    │
    └── Data not ready:
          1. Register fd with netpoller (epoll/kqueue)
          2. Park G1 (remove from run queue, save state)
          3. M picks up next G from run queue (G2, G3, etc.)
          4. Network data arrives → OS signals netpoller
          5. netpoller wakes G1, adds to run queue
          6. G1 resumes on any available M
```

This is why Go can handle **hundreds of thousands of concurrent connections** with a fixed number of OS threads — parked goroutines consume only heap memory (goroutine struct + stack), not an OS thread.

---

## 4. Node.js Event Loop and libuv

### 4.1 The Single-Threaded Model

Node.js runs JavaScript in a **single OS thread** using V8. There is never more than one piece of JavaScript executing simultaneously (ignoring Worker Threads). Concurrency is achieved purely through **non-blocking I/O callbacks** via the event loop.

```
Node.js Process Architecture:

  ┌───────────────────────────────────────────────────────────┐
  │  V8 Heap + Call Stack (1 JavaScript thread)               │
  │                                                           │
  │  ┌────────────────────────────────────────────────────┐   │
  │  │ Call Stack                                         │   │
  │  │  main()                                            │   │
  │  │  handleRequest()                                   │   │
  │  │  ...                                               │   │
  │  └────────────────────────────────────────────────────┘   │
  └──────────────────────────┬────────────────────────────────┘
                             │
  ┌──────────────────────────▼────────────────────────────────┐
  │  libuv Event Loop (C library)                             │
  │                                                           │
  │  Phase 1: Timers          (execute setTimeout/setInterval)│
  │  Phase 2: Pending I/O     (I/O callbacks deferred from    │
  │                            previous iteration)            │
  │  Phase 3: Idle/Prepare    (internal use)                  │
  │  Phase 4: Poll ◄──────── epoll_wait() / kqueue()         │
  │           │               (block waiting for I/O events)  │
  │           │               (timeout = next timer deadline) │
  │  Phase 5: Check           (execute setImmediate())        │
  │  Phase 6: Close callbacks (socket.on('close', ...) etc.) │
  │                                                           │
  │  Between each phase: process microtasks                   │
  │    (Promise.resolve, queueMicrotask, process.nextTick)    │
  └───────────────────────────────────────────────────────────┘
                             │
  ┌──────────────────────────▼────────────────────────────────┐
  │  libuv Thread Pool (default: 4 threads, max: 1024)        │
  │  Handles: fs.readFile, crypto, dns.lookup, zlib           │
  │  (Things that don't have async kernel interfaces)         │
  └───────────────────────────────────────────────────────────┘
```

### 4.2 The Poll Phase: The Heart of the Event Loop

```
libuv Poll Phase Detail:

  1. Calculate timeout for epoll_wait:
       - If there are pending timers: timeout = time until next timer
       - If there are pending setImmediate callbacks: timeout = 0 (don't block)
       - Otherwise: timeout = -1 (block until I/O event)

  2. Call epoll_wait(epfd, events, maxevents, timeout)
       - OS kernel: suspend thread, monitor registered file descriptors
       - Returns when: I/O event ready, timeout expires, or signal received

  3. For each returned event:
       - Identify associated callback from libuv's fd→callback map
       - Queue callback for execution

  4. Return control to event loop main dispatcher
       - Execute queued I/O callbacks (they run in JavaScript!)
```

### 4.3 Microtask Queue vs. Macrotask Queue

This is a major source of confusion and bugs in Node.js:

```javascript
// Execution order demonstration:
console.log('1: synchronous');

setTimeout(() => console.log('5: setTimeout (macrotask)'), 0);

Promise.resolve()
  .then(() => console.log('3: Promise microtask'))
  .then(() => console.log('4: chained microtask'));

process.nextTick(() => console.log('2: nextTick (priority microtask)'));

console.log('1b: also synchronous');

// Output order:
// 1: synchronous
// 1b: also synchronous
// 2: nextTick (process.nextTick drains COMPLETELY before Promises)
// 3: Promise microtask
// 4: chained microtask
// 5: setTimeout (macrotask — only after microtask queue is empty)
```

**Priority order within an event loop tick:**
1. Current synchronous code runs to completion
2. `process.nextTick` queue drains completely
3. Promise microtask queue drains completely (including newly added microtasks)
4. One macrotask from event loop phase executes
5. Repeat from step 2

### 4.4 What Blocks the Event Loop

Since there is only one JavaScript thread, **any synchronous CPU work blocks ALL requests**:

```javascript
// CATASTROPHIC: blocks event loop for all concurrent connections
app.get('/process', (req, res) => {
    const result = JSON.parse(req.body.largePayload);  // 50ms CPU
    const processed = heavyComputation(result);        // 200ms CPU
    res.json(processed);
    // During these 250ms, NO other requests can be handled — 0 req/s throughput
});

// CORRECT: offload CPU work to Worker Thread
const { Worker } = require('worker_threads');
app.get('/process', async (req, res) => {
    const result = await runInWorker('./compute.js', req.body.largePayload);
    res.json(result);
    // Event loop is FREE during computation
    // Other requests handled concurrently
});
```

**Node.js Worker Threads** (not the libuv thread pool): Dedicated JS threads with their own V8 instance and event loop. Share ArrayBuffers via zero-copy transfer. Ideal for CPU-bound parallelism.

### 4.5 The C10K Problem and libuv's Solution

Traditional servers spawned one thread per connection. At 10,000 connections, this requires ~10GB of thread stacks — impractical. **libuv** (and its platform equivalents: epoll on Linux, kqueue on macOS/BSD, IOCP on Windows) solve this by registering interest in I/O events and blocking a single thread until any event is ready:

```
epoll-based Connection Handling:

  epfd = epoll_create1(0);   // create epoll instance

  // Register each accepted connection:
  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;  // edge-triggered: notify on new data
  ev.data.fd = client_fd;
  epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

  // Event loop:
  while (1) {
      n = epoll_wait(epfd, events, MAX_EVENTS, timeout_ms);
      for (int i = 0; i < n; i++) {
          handle_event(events[i].data.fd);  // only handle READY fds
      }
  }

  // Result: 1 thread monitors 100,000 connections efficiently
  // OS kernel does the waiting; our thread only wakes for ready fds
```

---

## 5. Python's GIL

### 5.1 What the GIL Is

The **Global Interpreter Lock** is a mutex in CPython that ensures only one thread executes Python bytecode at a time, even on multi-core hardware.

```
CPython GIL Mechanics:

  Thread A (holds GIL) ──► executes bytecodes
  Thread B (waiting)   ──► blocked on GIL acquisition
  Thread C (waiting)   ──► blocked on GIL acquisition

  Every 5ms (sys.getswitchinterval()):
    1. Running thread releases GIL
    2. Signal other waiting threads
    3. Waiting threads race to acquire GIL
    4. Winner continues; others wait again

  GIL is released during:
    - C extension I/O calls (socket.recv, file.read)
    - Explicit Py_BEGIN_ALLOW_THREADS in C extensions
    - time.sleep()
  
  GIL is held during:
    - All Python bytecode execution
    - Reference count manipulation (INCREF/DECREF)
    - dict/list operations
    - All pure-Python code
```

### 5.2 The GIL's Impact on CPU-Bound vs. I/O-Bound Work

```python
import threading
import time

def cpu_bound_task():
    # Pure Python computation — GIL held entire time
    count = 0
    for i in range(10_000_000):
        count += i
    return count

def io_bound_task():
    # I/O call releases GIL — true concurrency possible
    import urllib.request
    urllib.request.urlopen('https://example.com')

# CPU-bound: threads SLOWER than single-threaded (GIL thrashing overhead)
start = time.time()
t1 = threading.Thread(target=cpu_bound_task)
t2 = threading.Thread(target=cpu_bound_task)
t1.start(); t2.start(); t1.join(); t2.join()
print(f"2 threads CPU: {time.time()-start:.2f}s")  # ~2x SLOWER than sequential

# I/O-bound: threads benefit (GIL released during I/O wait)
# Both threads can wait concurrently — true parallelism for I/O
```

**The critical failure mode:** A CPU-intensive endpoint in a multi-threaded Python web server (Gunicorn threads mode) will cause GIL contention — threads compete for the GIL and actually slow each other down. Gunicorn's default **multi-process** (prefork) mode avoids this — each process has its own GIL.

### 5.3 Escaping the GIL

```python
# Option 1: multiprocessing — multiple processes, no shared GIL
from multiprocessing import Pool

with Pool(processes=4) as pool:
    results = pool.map(cpu_bound_task, range(4))
    # True parallelism — 4 separate CPython processes
    # Cost: IPC overhead (pickle serialization), higher memory

# Option 2: C extensions with Py_BEGIN_ALLOW_THREADS
# NumPy, TensorFlow, OpenCV release the GIL during heavy computation:
import numpy as np
large_array = np.random.rand(10000, 10000)
result = np.dot(large_array, large_array)  # releases GIL internally — parallel BLAS

# Option 3: concurrent.futures (ThreadPoolExecutor for I/O, ProcessPoolExecutor for CPU)
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor

with ThreadPoolExecutor(max_workers=10) as executor:
    futures = [executor.submit(io_bound_task) for _ in range(100)]
    # Works well for I/O-bound; GIL released during I/O

# Option 4: asyncio — single-threaded cooperative concurrency (no GIL contention)
import asyncio

async def async_io_task():
    async with aiohttp.ClientSession() as session:
        async with session.get('https://example.com') as response:
            return await response.text()
# asyncio doesn't release the GIL — it avoids threading entirely
# All I/O is non-blocking; event loop handles concurrency
```

### 5.4 Python's Concurrency Frameworks

```
Python Concurrency Decision Tree:

  Task type?
  ├── I/O-bound (network, disk, database)
  │     └── High concurrency needed?
  │           ├── Yes → asyncio + async/await (FastAPI, aiohttp)
  │           └── No  → threading or ThreadPoolExecutor
  │
  └── CPU-bound (computation, data processing)
        └── Need parallelism?
              ├── Yes → multiprocessing or ProcessPoolExecutor
              │         or use NumPy/C extensions that release GIL
              └── No  → single-threaded (simplest, no overhead)
```

---

## 6. Ruby YARV and the GIL

### 6.1 Ruby's Global VM Lock (GVL)

Ruby MRI (CRuby) has a **Global VM Lock** (functionally identical to CPython's GIL). Ruby threads are real OS threads but only one runs Ruby code at a time.

```ruby
# Ruby GVL behavior mirrors Python GIL
require 'thread'

# CPU-bound: threads don't help (GVL prevents parallelism)
threads = 4.times.map do
  Thread.new { 10_000_000.times { } }
end
threads.each(&:join)
# Runs sequentially despite 4 threads — GVL held

# I/O-bound: threads help (GVL released during blocking I/O)
threads = 10.times.map do
  Thread.new { Net::HTTP.get(URI('https://example.com')) }
end
threads.each(&:join)
# All 10 requests sent concurrently — GVL released during socket read
```

**Ractors (Ruby 3.0+):** Ruby's answer to the GVL limitation. Each Ractor is a concurrency primitive with its own GVL — objects are passed between Ractors explicitly (no shared mutable state). This enables true parallelism for CPU-bound work but requires programming model changes.

```ruby
# Ractor: true parallelism
ractors = 4.times.map do
  Ractor.new do
    10_000_000.times { }  # runs in parallel on different CPU cores!
    Ractor.yield :done
  end
end
results = ractors.map(&:take)
```

---

## 7. PHP-FPM: The Prefork Model

### 7.1 Architecture

PHP-FPM (FastCGI Process Manager) uses a fundamentally different concurrency model from all others in this module: **one OS process per concurrent request**.

```
PHP-FPM Architecture:

  Nginx (or Apache)
       │  HTTP request
       ▼
  PHP-FPM Master Process
       │  (listens on Unix socket or TCP port)
       ├── Worker 1 (idle)       ← ready to accept
       ├── Worker 2 (busy)       ← handling request A
       ├── Worker 3 (busy)       ← handling request B
       ├── Worker 4 (idle)       ← ready to accept
       └── Worker N (max_children: 50 default)

  Per-Worker request lifecycle:
    1. Accept connection from master
    2. Initialize PHP environment (post-OPcache)
    3. Execute script: index.php → route → controller → DB → view
    4. Send response
    5. RESET all global state (superglobals, static vars, object instances)
    6. Return to idle pool, ready for next request
    7. After pm.max_requests: worker self-terminates (prevents memory leaks)
```

### 7.2 PHP-FPM Pool Configuration

```ini
; /etc/php-fpm.d/www.conf
[www]
pm = dynamic                    ; dynamic worker count (vs. static/ondemand)
pm.max_children = 50            ; absolute maximum workers
pm.start_servers = 10           ; workers at startup
pm.min_spare_servers = 5        ; keep at least 5 idle
pm.max_spare_servers = 15       ; don't keep more than 15 idle
pm.max_requests = 500           ; restart worker after 500 requests (memory leak protection)

; Memory implications:
; Each worker: ~40–80MB RSS (loaded code + OPcache shared pages + per-request heap)
; 50 workers × 60MB = 3GB RAM dedicated to PHP-FPM
; This is the fundamental scaling ceiling of prefork PHP
```

### 7.3 PHP-FPM vs. Swoole / ReactPHP

**Swoole** and **ReactPHP** bring async I/O to PHP by implementing event loops in PHP or C:

```php
// Traditional PHP-FPM: new process per request
// Swoole: persistent process, event loop, coroutines

use Swoole\Http\Server;
use Swoole\Coroutine;

$server = new Server('0.0.0.0', 9501);

$server->on('request', function ($request, $response) {
    // Swoole coroutine: cooperative multitasking within one process
    $result = Coroutine\run(function() {
        // This MySQL query is NON-BLOCKING — coroutine yields while waiting
        $client = new Coroutine\MySQL();
        $client->connect(['host' => 'localhost', 'user' => 'root', 'password' => '']);
        return $client->query('SELECT * FROM users LIMIT 1');
    });
    
    $response->end(json_encode($result));
});

$server->start();
// Result: single persistent process handling thousands of concurrent connections
// No per-request process fork overhead
// State persists between requests — requires discipline (memory leaks, shared state bugs)
```

**The trade-off:** Traditional PHP-FPM's share-nothing model makes state isolation free and crash isolation total. Swoole's persistent process model requires disciplined state management and careful memory hygiene — exactly the problems PHP's architecture was designed to avoid.

---

## 8. Async/Await Across Runtimes

Async/await is **syntactic sugar** over state machines. The compiler transforms an async function into a state machine that can be suspended and resumed without blocking a thread.

### 8.1 JavaScript/TypeScript (Promises and async/await)

```javascript
// These are equivalent:

// Promise chain (explicit)
function fetchUser(id) {
    return fetch(`/api/users/${id}`)
        .then(response => response.json())
        .then(user => {
            return fetch(`/api/posts?userId=${user.id}`);
        })
        .then(response => response.json());
}

// async/await (syntactic sugar over Promises)
async function fetchUser(id) {
    const response = await fetch(`/api/users/${id}`);  // suspends here
    const user = await response.json();                // suspends here
    const postResponse = await fetch(`/api/posts?userId=${user.id}`); // suspends here
    return postResponse.json();
}

// Under the hood: the JS engine creates a state machine:
// State 0: call fetch, register callback, return to event loop
// State 1 (resumed when fetch resolves): call .json(), register callback
// State 2 (resumed when json resolves): call fetch again...
// Each "await" is a yield point where the event loop can run other tasks
```

### 8.2 Rust async/await: Zero-Cost Futures

Rust's async model generates a **state machine struct** at compile time — no heap allocation for the async machinery itself:

```rust
use tokio; // Tokio: the dominant async runtime for Rust

// Async function: compiler generates a state machine struct
async fn fetch_user(id: u64) -> Result<User, Error> {
    // Each await point is a state in the machine
    let response = reqwest::get(format!("/api/users/{}", id)).await?;
    let user: User = response.json().await?;
    Ok(user)
}

// The compiler generates approximately:
enum FetchUserFuture {
    State0 { id: u64 },                    // before first await
    State1 { future: reqwest::Future },    // waiting on HTTP request
    State2 { future: JsonFuture<User> },   // waiting on JSON parse
    Done,
}

impl Future for FetchUserFuture {
    type Output = Result<User, Error>;
    
    fn poll(&mut self, cx: &mut Context) -> Poll<Self::Output> {
        match self {
            State0 { id } => {
                // Start HTTP request, transition to State1
            }
            State1 { future } => {
                match future.poll(cx) {
                    Poll::Ready(resp) => { *self = State2 { ... }; }
                    Poll::Pending => Poll::Pending,  // yield to executor
                }
            }
            // ...
        }
    }
}
```

**Key difference from Go goroutines:** Rust futures are **lazy** — they do nothing until polled by an executor (Tokio's thread pool). Go goroutines are **eager** — spawning a goroutine immediately schedules it for execution.

### 8.3 Python asyncio

```python
import asyncio
import aiohttp

async def fetch_all(urls: list[str]) -> list[str]:
    async with aiohttp.ClientSession() as session:
        # asyncio.gather: run all coroutines concurrently (not parallel!)
        tasks = [fetch_one(session, url) for url in urls]
        return await asyncio.gather(*tasks)

async def fetch_one(session: aiohttp.ClientSession, url: str) -> str:
    async with session.get(url) as response:
        return await response.text()

# Run the event loop
results = asyncio.run(fetch_all(['https://a.com', 'https://b.com', 'https://c.com']))
```

**CPython asyncio limitation:** asyncio is single-threaded (GIL not relevant — no threads). CPU-bound code inside an async function **blocks the event loop**. Unlike Go, there is no preemption. A 100ms CPU-heavy coroutine starves all other coroutines for 100ms.

### 8.4 C# async/await: State Machine + Thread Pool

```csharp
// C# async/await — similar to Rust in state machine generation
// but uses the CLR thread pool for execution

public async Task<User> FetchUserAsync(int id)
{
    // HttpClient.GetAsync: returns immediately (non-blocking), 
    // continuation scheduled on thread pool when I/O completes
    var response = await httpClient.GetAsync($"/api/users/{id}");
    response.EnsureSuccessStatusCode();
    var json = await response.Content.ReadAsStringAsync();
    return JsonSerializer.Deserialize<User>(json);
}

// ConfigureAwait(false): avoid capturing SynchronizationContext
// (critical in library code to avoid deadlocks in ASP.NET classic)
var data = await dbContext.Users.FindAsync(id).ConfigureAwait(false);
```

**ASP.NET Core:** Every request gets an async pipeline. When an async operation is awaited, the OS thread is returned to the thread pool — no thread wasted on waiting. This is how ASP.NET Core handles high concurrency with few threads.

---

## 9. I/O Multiplexing Internals

### 9.1 epoll (Linux)

```c
// epoll: Linux's scalable I/O event notification
// Used by: libuv (Node.js), Go's netpoller, Java NIO, Tokio (Rust)

int epfd = epoll_create1(EPOLL_CLOEXEC);

// Register file descriptors:
struct epoll_event ev = {
    .events = EPOLLIN | EPOLLET,  // EPOLLET = edge-triggered (more efficient)
    .data.fd = client_fd
};
epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &ev);

// Event loop:
struct epoll_event events[MAX_EVENTS];
while (1) {
    // Block until events ready (or timeout)
    int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
    
    for (int i = 0; i < n; i++) {
        if (events[i].events & EPOLLIN) {
            handle_readable(events[i].data.fd);
        }
        if (events[i].events & EPOLLOUT) {
            handle_writable(events[i].data.fd);
        }
    }
}

// Scalability: O(1) per event notification (vs. O(n) for select/poll)
// epoll uses a red-black tree internally for O(log n) fd management
// and a ready list for O(1) event retrieval
```

**Level-triggered vs. Edge-triggered:**
- **Level-triggered (LT):** epoll_wait returns as long as data is available. Simpler but less efficient (repeated notifications for unconsumed data).
- **Edge-triggered (ET):** epoll_wait returns only when state *changes* (new data arrives). More efficient but requires reading all available data in a loop (using non-blocking reads).

### 9.2 kqueue (macOS/BSD) and IOCP (Windows)

```
Platform I/O Multiplexing:

  Linux:   epoll    - edge/level triggered, O(1) event retrieval
  macOS:   kqueue   - more general (files, processes, signals, timers)
                      kevent() system call
  Windows: IOCP     - Completion-based (not readiness-based)
                      I/O operations post completion notices to port
                      More aligned with async model — no need to re-read after event
  Solaris: /dev/poll, event ports

libuv abstracts all of these behind a unified API.
Go's netpoller implements per-platform backends.
Tokio uses mio (Metal I/O), which wraps epoll/kqueue/IOCP.
```

---

## 10. Failure Modes Under Load

### 10.1 I/O-Bound Overload

```
System: Java Spring Boot (thread-per-request, Tomcat, 200 threads)
Load: 300 concurrent requests, each requiring a 100ms DB query

  Threads 1–200: Assigned to requests 1–200
                 Each blocked on DB query (100ms wait)
  Requests 201–300: Queued in Tomcat's accept queue

  If DB is slow (500ms instead of 100ms):
    200 threads × 500ms = all threads consumed for 500ms
    Requests 201–300: queue fills, new requests see TIMEOUT or REJECTION
    ↑ Thread pool exhaustion — the #1 cause of Spring Boot service degradation

  Fix: Use reactive stack (Spring WebFlux / Netty) or Virtual Threads (JDK 21+)
       Non-blocking DB driver (R2DBC) + WebFlux = no thread-per-request
```

```
System: Node.js Express
Load: 300 concurrent requests, each requiring a 100ms DB query

  Single event loop thread handles all 300 connections
  Each DB query is async (e.g., pg driver uses libuv for network I/O)
  100ms queries all inflight concurrently — event loop NOT blocked
  Throughput: excellent

  FAILURE MODE: If handler has 10ms of CPU work per request:
    300 concurrent × 10ms CPU = 3 seconds of blocked event loop
    All other requests stall during computation
    → Node.js degrades catastrophically under CPU load
```

### 10.2 CPU-Bound Overload

```
System: Go HTTP server (Gin)
Load: CPU-intensive endpoint (JSON processing, 50ms CPU each)

  GOMAXPROCS = 8 (8 CPU cores)
  8 goroutines running in parallel = optimal throughput for CPU work
  Additional goroutines queued, preemptively scheduled

  Go's preemptive scheduler prevents starvation:
    If a goroutine runs >10ms without a yield point,
    SIGURG preempts it and schedules another goroutine

  Result: Graceful degradation; latency increases linearly with load
          No catastrophic collapse (unlike GIL languages or single-threaded models)
```

```
System: Python Gunicorn (4 worker processes, 4 threads each)
Load: CPU-intensive endpoint, 50ms Python computation each

  4 processes × 4 threads = 16 "concurrent" workers
  BUT: Each process has its own GIL
       Within each process, only 1 thread runs at a time
       Effective CPU parallelism: 4 (one thread per process does real work)

  GIL thrashing: 3 threads per process compete for GIL and slow each other down
  Optimal config for CPU-bound: 1 worker per CPU core, 1 thread each
```

### 10.3 PHP-FPM Saturation

```
System: PHP-FPM, pm.max_children = 50
Load: 100 concurrent requests, each taking 200ms

  Workers 1–50: handling requests 1–50
  Workers available: 0

  Requests 51–100: sit in Nginx's fastcgi_pass queue
  If queue depth exceeded: Nginx returns 502 Bad Gateway

  PHP-FPM saturation is HARD — no async fallback
  Solution: Increase max_children (requires proportional RAM)
            Add horizontal scaling (more servers)
            Reduce request duration (caching, query optimization)
```

### 10.4 The "Thundering Herd" Problem

When a blocked resource (DB connection, cache miss, downstream API) recovers after an outage, all queued requests slam it simultaneously, potentially causing another failure:

```
Thundering Herd Mitigation:

  Go: Use errgroup + semaphore to limit concurrency:
    sem := semaphore.NewWeighted(int64(maxConcurrent))
    sem.Acquire(ctx, 1)
    defer sem.Release(1)

  Node.js: Use p-limit or bottleneck to throttle concurrent async calls:
    const limit = pLimit(10);
    const results = await Promise.all(urls.map(url => limit(() => fetch(url))));

  Java: Use bulkhead pattern (Resilience4j):
    @Bulkhead(name = "backendA", type = Bulkhead.Type.THREADPOOL)
    CompletableFuture<String> callBackend() { ... }
```

---

## 11. Comparative Concurrency Matrix

### 11.1 Concurrency Model Comparison

| Language/Runtime | Model | Parallelism | I/O Handling | Thread Cost | Max Concurrency |
|---|---|---|---|---|---|
| **C/C++ (pthreads)** | 1:1 OS threads | True | Blocking or epoll/async | High (~1MB/thread) | ~10K threads |
| **Java (pre-Loom)** | 1:1 OS threads | True | Blocking + NIO | High (~1MB/thread) | ~10K threads |
| **Java (Loom, JDK 21+)** | M:N virtual threads | True | Non-blocking (transparent) | Low (~KB/vthread) | Millions |
| **Go** | M:N goroutines | True | Non-blocking (netpoller) | Very Low (~2KB) | Millions |
| **Node.js** | Single-thread event loop | I/O only | Non-blocking (libuv) | N/A (no threads) | ~10K–100K connections |
| **Python (CPython threads)** | 1:1 OS threads + GIL | I/O only (CPU blocked by GIL) | Blocking or asyncio | Medium | GIL-limited |
| **Python (asyncio)** | Single-thread event loop | I/O only | Non-blocking (selectors) | N/A | ~10K connections |
| **Python (multiprocessing)** | Multiple processes | True | Blocking | Very High (full process) | CPU cores count |
| **Ruby (MRI threads)** | 1:1 OS threads + GVL | I/O only | Blocking or EventMachine | Medium | GVL-limited |
| **PHP-FPM** | 1:1 process per request | True (separate processes) | Blocking | Very High (full process) | pm.max_children |
| **Rust (Tokio)** | M:N async tasks | True | Non-blocking (mio/epoll) | Very Low (Future state machine) | Millions |
| **C# (ASP.NET Core)** | Thread pool + async | True | Non-blocking (IOCP/epoll) | Low (thread pool) | ~100K connections |

### 11.2 Workload Recommendation Matrix

| Workload | Best Choice | Why |
|---|---|---|
| **High-concurrency I/O (100K+ connections)** | Go, Rust (Tokio), Node.js, C# | Non-blocking I/O; low per-connection overhead |
| **CPU-bound parallel processing** | Rust, C++, Go, Java | True parallelism; no GIL; efficient thread/goroutine scheduling |
| **Mixed I/O + CPU** | Go, Rust, Java (Loom) | Can parallelize both; efficient scheduling |
| **Rapid development, I/O-heavy web API** | Node.js (TypeScript), Python (FastAPI+asyncio) | Event loop efficiency; excellent ecosystem |
| **Scripting / Data pipelines** | Python (multiprocessing + NumPy) | GIL escaped via C extensions and processes |
| **Ultra-low latency (microsecond)** | C++, Rust | Zero GC; deterministic; CPU-pinnable threads |
| **Legacy PHP web apps** | PHP-FPM | Share-nothing simplicity; horizontal scaling |

---

## Key Mental Models

1. **Concurrency is not parallelism.** Node.js is highly concurrent (thousands of open connections) but never parallel (one JS thread). Go is both concurrent AND parallel (millions of goroutines across all CPU cores).

2. **The GIL is a serialization point.** CPython's GIL means that Python threads are useful only for I/O-bound concurrency. For CPU-bound parallelism, you must leave the interpreter (multiprocessing, Cython, ctypes, NumPy).

3. **Thread cost is not just stack memory.** OS threads carry: kernel stack, scheduler state, TLB entries, open file descriptor inheritance. Goroutines and virtual threads eliminate most of this overhead.

4. **Blocking I/O wastes threads.** A thread blocked on a 100ms DB query is an OS thread doing nothing but consuming memory. Event loops and async I/O reclaim this waste — the thread does other work while I/O completes.

5. **CPU-bound work in an event loop is catastrophic.** Any synchronous CPU work in Node.js or Python asyncio blocks all other coroutines for its duration. The event loop assumes coroutines yield frequently. Violate this and throughput collapses.

6. **Go's preemptive goroutine scheduler is a safety net.** Unlike Node.js (cooperative) or Python asyncio (cooperative), Go will preempt a CPU-hogging goroutine and run others. This makes Go's performance degrade gracefully under unexpected CPU load.

---

*End of Module 03.*

---

> **Next:** Module 04 covers **Low-Level Framework Architecture & Request Lifecycles** — the TCP-to-response pipeline inside Spring Boot (Netty), ASP.NET Core (Kestrel), Gin, Rails (Puma), and Node.js. Middleware pipeline mechanics, ORM overhead, and connection pool architecture.
