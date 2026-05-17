# Module 02: Memory Management, Allocators, and Storage Internals

> **Series:** Principal Engineer's Deep-Dive into Languages, Runtimes, and Frameworks  
> **Prerequisites:** Module 01 (Compilation, Execution Models, and Runtimes)

---

## Table of Contents

1. [The Memory Hierarchy: A Mental Model](#1-the-memory-hierarchy)
2. [Stack vs. Heap: Mechanics Across Runtimes](#2-stack-vs-heap)
3. [Manual Memory Management: C and C++](#3-manual-memory-management)
4. [Compile-Time Memory Safety: Rust's Borrow Checker](#4-rusts-borrow-checker)
5. [Reference Counting: Python, PHP, Swift](#5-reference-counting)
6. [Tracing Garbage Collection: Java, C#, Go](#6-tracing-garbage-collection)
7. [GC Algorithm Deep Dives](#7-gc-algorithm-deep-dives)
8. [Object Layout and Memory Overhead](#8-object-layout-and-memory-overhead)
9. [Memory Footprint, Cache Locality, and Throughput](#9-cache-locality-and-throughput)
10. [Allocator Internals](#10-allocator-internals)
11. [Comparative Summary](#11-comparative-summary)

---

## 1. The Memory Hierarchy

Before comparing strategies, internalize the hardware reality every memory manager is fighting against:

```
Memory Hierarchy (approximate latency, 2024 hardware):

  ┌──────────────────────────────────────────────────────────┐
  │  CPU Registers         ~0.3ns    ~1 cycle                │
  ├──────────────────────────────────────────────────────────┤
  │  L1 Cache (32–64KB)    ~1ns      ~4 cycles               │
  ├──────────────────────────────────────────────────────────┤
  │  L2 Cache (256KB–1MB)  ~4ns      ~12 cycles              │
  ├──────────────────────────────────────────────────────────┤
  │  L3 Cache (8–64MB)     ~12–40ns  ~40 cycles              │
  ├──────────────────────────────────────────────────────────┤
  │  DRAM (main memory)    ~60–100ns ~200 cycles             │
  ├──────────────────────────────────────────────────────────┤
  │  NVMe SSD              ~100µs    ~300,000 cycles         │
  └──────────────────────────────────────────────────────────┘
```

**The critical insight:** A cache miss to DRAM costs ~200 cycles. A function call costs ~1–5 cycles. Your GC's pointer-chasing through fragmented heap objects isn't slow because GC is expensive — it's slow because every pointer dereference into a non-resident cache line is a 200-cycle stall.

Every memory management strategy in this module is, at its core, an attempt to maximize cache residency of hot data.

---

## 2. Stack vs. Heap

### 2.1 Stack Allocation: The Fast Path

The stack is a contiguous block of memory managed by the CPU's stack pointer register (`rsp` on x86-64). Allocation is a single `SUB rsp, N` instruction. Deallocation is a `ADD rsp, N` or `RET`. There is no free list, no fragmentation, no GC involvement.

```
Stack Frame Layout (x86-64 System V ABI):

  High addresses
  ┌──────────────────┐ ← previous frame's rsp
  │ Return address   │  8 bytes
  │ Saved rbp        │  8 bytes
  │ Local var: x     │  8 bytes (int64)
  │ Local var: buf   │  64 bytes (char[64])
  │ Padding          │  (alignment to 16 bytes)
  └──────────────────┘ ← current rsp
  Low addresses

  Allocation: SUB rsp, 88   → 1 cycle
  Deallocation: RET          → 1 cycle (+ branch prediction)
```

**Stack allocation is the fastest possible allocation.** Languages that can push more data to the stack (Rust, C, Go) reduce GC pressure and improve cache locality.

### 2.2 What Forces Heap Allocation

| Condition | Why Stack Is Insufficient |
|---|---|
| Size unknown at compile time | Stack frame size must be fixed at compile time |
| Lifetime exceeds function scope | Stack frame is destroyed on return |
| Data shared across threads | Each thread has its own stack |
| Recursive data structures | Unbounded depth |
| Large allocations | Default stack size: 1–8MB; stack overflow risk |

### 2.3 Language-by-Language Stack Usage

```
Stack Generosity by Language:

  C/C++:       Maximum. VLAs (C99), alloca(). Nearly anything can be stack-allocated.
  Rust:        Maximum. Borrow checker ensures stack references never outlive frame.
               Box<T> explicitly opts into heap.
  Go:          Automatic via escape analysis. Goroutine stacks start at 2–8KB and GROW.
  Java:        Primitive locals on stack. ALL objects on heap. No stack-allocated objects.
  C#:          Primitives + structs on stack. Classes on heap.
               stackalloc Span<T> for explicit stack buffers.
  Python:      All objects on heap. Stack holds only PyObject* pointers.
  Ruby:        All objects on heap (with some VALUE tagging tricks for small ints).
  PHP:         zval structs; post-PHP7 copy-on-write value semantics, mostly heap.
  JavaScript:  All objects on heap. V8 escape analysis may stack-allocate some.
```

---

## 3. Manual Memory Management

### 3.1 C: malloc/free

```c
// C memory lifecycle — entirely manual
#include <stdlib.h>
#include <string.h>

typedef struct {
    int   id;
    char* name;   // pointer to heap-allocated string
    float score;
} Record;

Record* create_record(int id, const char* name) {
    Record* r = malloc(sizeof(Record));  // allocate struct on heap
    if (!r) return NULL;                 // malloc returns NULL on OOM

    r->id    = id;
    r->name  = strdup(name);            // allocates another heap block for string
    r->score = 0.0f;
    return r;
}

void destroy_record(Record* r) {
    if (!r) return;
    free(r->name);  // must free inner allocation first
    free(r);        // then free the struct
    // r is now a dangling pointer — undefined behavior to dereference
    // Discipline: set r = NULL after free (doesn't help with other aliases)
}

// Common failure modes:
// 1. Use-after-free: accessing r after destroy_record(r)
// 2. Double-free: calling destroy_record(r) twice
// 3. Memory leak: forgetting to call destroy_record
// 4. Buffer overflow: writing past allocated bounds
```

The C allocator (`ptmalloc2` in glibc, `jemalloc`, `tcmalloc`) maintains a **free list** of previously freed blocks, segregated by size class. `malloc(n)` searches the appropriate size class's free list; if empty, calls `brk()` or `mmap()` to request more pages from the kernel.

### 3.2 C++: RAII and Smart Pointers

C++ extends C with **Resource Acquisition Is Initialization (RAII)**: resource lifetime is tied to object lifetime, and destructors run deterministically when objects go out of scope.

```cpp
#include <memory>
#include <string>

struct Record {
    int         id;
    std::string name;   // std::string manages its own heap buffer via RAII
    float       score;
    
    Record(int id, std::string name) 
        : id(id), name(std::move(name)), score(0.0f) {}
    
    ~Record() = default;  // std::string destructor handles name buffer
};

// unique_ptr: sole ownership — exactly one owner at a time
std::unique_ptr<Record> make_record(int id, std::string name) {
    return std::make_unique<Record>(id, std::move(name));
    // unique_ptr destructor calls delete when it goes out of scope
    // No manual free() required. No memory leak possible.
}

// shared_ptr: reference-counted shared ownership
std::shared_ptr<Record> shared = std::make_shared<Record>(1, "Alice");
std::shared_ptr<Record> alias  = shared;  // ref count: 2
// shared_ptr uses atomic reference counting — thread-safe but with overhead

// weak_ptr: non-owning reference (breaks cycles)
std::weak_ptr<Record> weak = shared;
if (auto locked = weak.lock()) {
    // use locked safely — guarantees object still alive
}
```

**`std::make_shared` optimization:** Allocates the control block (ref count) and the object in a **single allocation**, improving cache locality when the shared_ptr and the object are accessed together.

### 3.3 The Cost of Manual Management

| Issue | Consequence |
|---|---|
| Use-after-free | Undefined behavior; exploitable security vulnerability |
| Double-free | Heap corruption; crash or exploit |
| Memory leak | RSS grows unboundedly; OOM kill |
| Buffer overflow | Stack smashing; RCE |
| Fragmentation | Allocator overhead; cache thrashing |

Tools: AddressSanitizer (ASan), Valgrind, LeakSanitizer detect these at dev/test time. In production, they are invisible until they crash your process or make headlines.

---

## 4. Rust's Borrow Checker

Rust eliminates the entire class of manual memory management bugs through a **compile-time ownership system** with zero runtime overhead.

### 4.1 Ownership Rules

```
Three Rules (enforced at compile time, erased before codegen):

  1. Every value has exactly ONE owner.
  2. When the owner goes out of scope, the value is dropped (destructor runs).
  3. There can be EITHER:
       - Any number of immutable references (&T), OR
       - Exactly ONE mutable reference (&mut T)
     at any given time. Never both simultaneously.
```

```rust
fn demonstrate_ownership() {
    // --- Move Semantics ---
    let s1 = String::from("hello");  // s1 owns the heap buffer
    let s2 = s1;                     // MOVE: ownership transferred to s2
    // println!("{}", s1);           // COMPILE ERROR: s1 is moved — use-after-move
    println!("{}", s2);              // OK

    // --- Borrowing ---
    let s3 = String::from("world");
    let len = calculate_length(&s3); // immutable borrow — s3 retains ownership
    println!("{} has length {}", s3, len); // s3 still valid

    // --- Mutable Borrow ---
    let mut s4 = String::from("hello");
    change(&mut s4);                 // exactly one mutable borrow
    // let r1 = &mut s4;
    // let r2 = &mut s4;            // COMPILE ERROR: two mutable borrows
}

fn calculate_length(s: &String) -> usize {
    s.len()
    // s goes out of scope, but doesn't drop — it's a borrow, not ownership
}

fn change(s: &mut String) {
    s.push_str(", world");
}
```

### 4.2 Lifetimes: The Borrow Checker's Scope Analysis

```rust
// Lifetime annotations make borrow duration explicit
// The compiler infers most lifetimes; annotations required when ambiguous

fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
    // 'a means: the returned reference lives as long as the SHORTER of x and y
    if x.len() > y.len() { x } else { y }
}

// Without lifetimes, this would be impossible to verify:
// Does the returned reference point into x or y?
// If x is dropped, is the returned ref dangling?
// The borrow checker answers these questions at compile time.
```

### 4.3 Memory Layout in Rust

Rust gives fine-grained control over memory layout:

```rust
use std::mem;

// Default layout: compiler may reorder for alignment optimization
struct Optimized {
    a: u8,    // 1 byte
    b: u64,   // 8 bytes
    c: u8,    // 1 byte
    // compiler inserts padding: a(1) + pad(7) + b(8) + c(1) + pad(7) = 24 bytes
}

// Repr(C): deterministic C-compatible layout (no reordering)
#[repr(C)]
struct CLayout {
    a: u8,    // offset 0
    // 7 bytes padding
    b: u64,   // offset 8
    c: u8,    // offset 16
    // 7 bytes padding
}             // total: 24 bytes — matches C struct layout

// Repr(packed): no padding — potentially unaligned, unsafe to dereference
#[repr(packed)]
struct Packed {
    a: u8,    // offset 0
    b: u64,   // offset 1 (UNALIGNED — UB to take reference on some architectures)
    c: u8,    // offset 9
}             // total: 10 bytes

println!("{}", mem::size_of::<Optimized>());  // 24
println!("{}", mem::size_of::<Packed>());     // 10
```

### 4.4 The Drop Trait: Deterministic Destruction

```rust
struct DatabaseConnection {
    fd: i32,
}

impl Drop for DatabaseConnection {
    fn drop(&mut self) {
        // Called DETERMINISTICALLY when the value goes out of scope
        // No GC pause, no finalizer queue, no uncertainty
        unsafe { libc::close(self.fd); }
        println!("Connection {} closed", self.fd);
    }
}

fn use_connection() {
    let conn = DatabaseConnection { fd: 5 };
    // ... use conn ...
}   // conn.drop() called HERE — guaranteed, synchronous, immediate
    // Compare: Java's finalize() — called by GC, nondeterministic timing
```

---

## 5. Reference Counting

### 5.1 Python: CPython's Reference Counting + Cycle Collector

Every CPython object contains an `ob_refcnt` field. When it reaches zero, the object is immediately freed. This is **not** a tracing GC for the common case.

```c
// CPython object header (simplified from Include/object.h):
typedef struct _object {
    Py_ssize_t ob_refcnt;   // reference count (8 bytes on 64-bit)
    PyTypeObject *ob_type;  // pointer to type object (8 bytes)
    // ... type-specific data follows
} PyObject;

// Every INCREF/DECREF is an atomic or non-atomic increment/decrement:
#define Py_INCREF(op)  (((PyObject*)(op))->ob_refcnt++)
#define Py_DECREF(op)  if (--((PyObject*)(op))->ob_refcnt == 0) _Py_Dealloc(op)
```

**Immediate deallocation** is the key advantage: when you `del x` or exit a scope, the object and its memory are reclaimed immediately if `refcnt` drops to zero. No GC pause.

**The cycle problem:** Reference counting cannot free reference cycles:

```python
# Memory leak in CPython without cycle collector:
class Node:
    def __init__(self):
        self.next = None

a = Node()
b = Node()
a.next = b   # a's refcnt for b: 1
b.next = a   # b's refcnt for a: 1

del a        # a's external refcnt drops to 0... wait, b.next still holds a ref!
del b        # b's external refcnt drops to 0... but a.next still holds b!
# Both a and b have refcnt=1 due to the cycle. Neither is freed.
# CPython's cyclic garbage collector (gc module) handles this separately.
```

**CPython's cycle collector** runs periodically, tracking containers (lists, dicts, objects) and performing a modified mark-and-sweep to detect cycles. It has three generations (gen0, gen1, gen2) with different collection frequencies.

```python
import gc
gc.collect()       # manually trigger cycle collection
gc.disable()       # disable cycle collector (safe if you guarantee no cycles)
gc.get_threshold() # (700, 10, 10) — collection thresholds per generation
```

**The GIL and reference counting:** The GIL exists partly because `ob_refcnt` manipulation is not atomic in the standard build. Without the GIL, two threads decrementing the same object's refcnt concurrently could both read `1`, both decrement to `0`, and both attempt `_Py_Dealloc` — double-free. Python 3.13's free-threaded mode makes refcnt ops atomic (with biased reference counting for thread-local objects).

### 5.2 PHP: zval and Copy-on-Write

PHP 7+ uses a restructured `zval` (Zend value) with reference counting baked into the refcounted heap values:

```c
// PHP 7+ zval (16 bytes on 64-bit):
typedef struct _zval_struct {
    zend_value value;          // 8 bytes: union of long/double/pointer
    union {
        struct {
            zend_uchar type;       // IS_LONG, IS_STRING, IS_ARRAY, etc.
            zend_uchar type_flags;
            uint16_t   extra;
        } v;
        uint32_t type_info;
    } u1;
    union {
        uint32_t next;         // hash collision chain
        uint32_t cache_slot;
        uint32_t opline_num;
        uint32_t lineno;
    } u2;
} zval;
```

**Copy-on-Write (CoW):** PHP arrays and strings use CoW. When you assign `$b = $a`, both variables point to the same zval with `refcount=2`. Only when `$b` is mutated does PHP perform a deep copy ("separation"). This makes reads cheap and avoids unnecessary copies.

```php
$a = ['x' => 1, 'y' => 2];  // zval refcount: 1
$b = $a;                      // refcount: 2, NO copy yet (CoW)
$b['z'] = 3;                  // NOW PHP copies $a's array — refcount[original]: 1, new array: 1
// $a is unchanged: ['x' => 1, 'y' => 2]
// $b is:          ['x' => 1, 'y' => 2, 'z' => 3]
```

---

## 6. Tracing Garbage Collection

Tracing GC works by periodically identifying all **live** objects (those reachable from roots: globals, stack, registers) and reclaiming everything else. Unlike reference counting, it handles cycles naturally.

### 6.1 Root Set

```
GC Root Set (starting points for reachability traversal):

  ┌─────────────────────────────────────────────────────┐
  │ GC Roots                                            │
  │  ├── CPU Registers (may hold object references)     │
  │  ├── Thread stacks (local variables)                │
  │  ├── Static/global variables                        │
  │  ├── JNI global refs (Java Native Interface)        │
  │  └── Class loader references                        │
  └────────────────────┬────────────────────────────────┘
                       │ Traverse object graph
                       ▼
  ┌─────────────────────────────────────────────────────┐
  │ LIVE objects: all objects reachable from roots      │
  │ DEAD objects: everything else → reclaim             │
  └─────────────────────────────────────────────────────┘
```

### 6.2 Generational Hypothesis

The generational hypothesis, empirically validated across decades of production systems: **most objects die young**. A newly allocated string in a request handler is almost certainly dead by the time the response is sent. A connection pool object may live for the process lifetime.

This justifies dividing the heap into generations and collecting the young generation (where most garbage is) far more frequently than the old generation.

---

## 7. GC Algorithm Deep Dives

### 7.1 Java: G1GC (Default since JDK 9)

G1 (Garbage First) divides the heap into equal-sized **regions** (~1–32MB each), labeled as Eden, Survivor, Old, or Humongous (large objects). This region-based design enables:

- **Incremental collection:** Only collect a subset of regions per GC cycle
- **Predictable pause targets:** G1 estimates collection time and selects regions to meet a pause goal (default: 200ms)
- **Concurrent marking:** Most of the live-object tracing happens concurrently with application threads

```
G1GC Heap Layout:

  ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
  │ E │ E │ S │ O │ O │ H │ H │ E │ O │ S │ E │ O │
  └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
    E = Eden  S = Survivor  O = Old  H = Humongous

  Minor GC (Young Collection):
    - Collects all Eden + Survivor regions
    - Stop-The-World pause: 5–200ms typical
    - Copies live young objects to Survivor or promotes to Old

  Mixed GC (Concurrent + STW):
    Phase 1: Initial Mark (STW ~1ms) — mark roots
    Phase 2: Concurrent Mark (concurrent) — traverse object graph
    Phase 3: Remark (STW ~1ms) — handle objects modified during concurrent phase
    Phase 4: Cleanup (STW ~1ms) — reclaim empty regions
    Phase 5: Mixed Collection (STW) — collect Old regions with most garbage
```

**Remembered Sets (RSets):** Since G1 may collect a subset of regions, it must track cross-region references. Every region has an RSet recording which other regions hold pointers into it. When collecting a region, the GC doesn't scan the entire heap — it consults the RSet. **RSets are a significant memory overhead** (typically 5–20% of heap).

**Write Barriers:** Every reference write in Java application code is intercepted by a write barrier (inserted by the JIT) to update RSets:

```java
obj.field = newValue;
// JIT-compiled to (approximately):
// *(obj + field_offset) = newValue;
// g1_write_barrier(obj, field_offset, newValue);  ← extra work per write
```

### 7.2 Java: ZGC (Ultra-Low Latency)

ZGC (Z Garbage Collector, production-ready since JDK 15) targets **sub-millisecond** STW pauses regardless of heap size (tested up to 16TB).

**Key technique: Colored Pointers**

ZGC encodes GC metadata directly in the object reference pointer bits (on 64-bit systems, only 48 bits are used for addressing, leaving 16 bits for metadata):

```
ZGC Colored Pointer (64-bit):

  Bit 63-48: Unused
  Bit 47-46: Finalizable | Remapped | Marked1 | Marked0  ← GC metadata
  Bit 45-0:  Actual heap address (46-bit address space = 64TB)

  The "color" of a pointer tells the GC its status in the current GC cycle.
  Load barriers check the color on EVERY object reference load.
```

**Load Barriers:** ZGC inserts a **load barrier** on every reference load from heap. This is the "tax" for concurrent relocation:

```java
// Application code:
Object x = obj.field;

// ZGC-compiled code (approximately):
Object x = obj.field;
if (x.is_bad_color()) {         // check pointer color
    x = zgc_barrier_slow_path(x); // remap/heal pointer
}
```

This barrier cost is ~5–10% throughput overhead compared to no GC, but eliminates multi-second STW pauses.

**ZGC Phases (all concurrent except tiny STW points):**
```
ZGC Cycle:

  STW: Pause Mark Start    (~0.5ms) — scan thread stacks, mark roots
  Concurrent: Mark         — trace object graph concurrently
  STW: Pause Mark End      (~0.5ms) — handle concurrent mutation races  
  Concurrent: Mark+Remap   — finish marking, prepare relocation sets
  STW: Pause Relocate Start (~0.5ms) — select relocation sets
  Concurrent: Relocate     — copy objects to new locations concurrently
  (next cycle's load barriers heal stale pointers lazily)
```

### 7.3 Go: Concurrent Tri-Color Mark-and-Sweep

Go's GC is designed for **low latency** (target: <1ms STW) with moderate throughput. It uses the tri-color mark-and-sweep algorithm with concurrent execution.

```
Tri-Color Invariant:

  White: Not yet visited — potentially garbage
  Gray:  Discovered but children not yet scanned
  Black: Fully scanned — all references processed

  Invariant: No black object points directly to a white object
             (the "tri-color invariant")

  Algorithm:
  1. Mark all GC roots GRAY
  2. While gray set is non-empty:
       a. Take a gray object
       b. Mark all white objects it references → GRAY
       c. Mark the object itself → BLACK
  3. All remaining WHITE objects are garbage → reclaim

  Concurrency challenge: Application threads ("mutators") may create
  new gray→white pointers while the GC is running.
  Solution: Write barriers maintain the tri-color invariant during concurrent marking.
```

**Go's Write Barrier (Dijkstra + Yuasa hybrid):**

```go
// When application writes: *slot = ptr
// Go runtime (simplified):
func writeBarrier(slot *unsafe.Pointer, ptr unsafe.Pointer) {
    shade(*slot) // shade the OLD value gray (Yuasa deletion barrier)
    shade(ptr)   // shade the NEW value gray (Dijkstra insertion barrier)
    *slot = ptr
}
// "shade" = mark gray if currently white
// This maintains the invariant even with concurrent mutations
```

**Go GC Phases:**
```
Go GC Cycle (concurrent, target: <1ms STW):

  Phase 1: Mark Setup (STW ~0.1ms)
    - Enable write barriers
    - Scan goroutine stacks
    - Mark global roots

  Phase 2: Mark (Concurrent)
    - Background goroutines trace heap
    - Assists: mutator goroutines help mark if allocating too fast
    - GC paces itself to ~25% of CPU budget

  Phase 3: Mark Termination (STW ~0.1ms)
    - Flush remaining work
    - Disable write barriers
    - Calculate next GC trigger (GOGC env var, default 100 = double heap before GC)

  Phase 4: Sweep (Concurrent)
    - Return white objects to allocator's free lists
    - Happens lazily as new allocations occur
```

**GOGC tuning:** `GOGC=100` means: trigger next GC when heap size doubles from previous live set. `GOGC=off` disables GC entirely (dangerous for long-running services). `GOMEMLIMIT` (Go 1.19+) sets a hard memory ceiling — GC runs more aggressively as usage approaches the limit.

### 7.4 C#/.NET: Generational GC with Server Mode

.NET's GC has two modes:
- **Workstation GC:** Single background GC thread; optimized for interactive latency
- **Server GC:** One GC thread per logical CPU core; optimized for throughput — default for ASP.NET Core

```
.NET Server GC Heap Layout (per CPU core):

  Core 0: [Gen0][Gen1][Gen2/LOH]
  Core 1: [Gen0][Gen1][Gen2/LOH]
  ...
  Core N: [Gen0][Gen1][Gen2/LOH]

  LOH = Large Object Heap (objects > 85,000 bytes; never compacted by default)

  Gen0 collection: ~0.1–1ms STW
  Gen1 collection: ~1–10ms STW
  Gen2 collection: Concurrent (background GC) or blocking full GC
```

**.NET's `Span<T>` and Stack Allocation:**

```csharp
// stackalloc: allocate on stack, no GC involvement
Span<byte> buffer = stackalloc byte[256]; // stack allocated, zero-init
// Span<T> is a ref struct — cannot escape to heap (borrow-checker-like guarantee)
// Used extensively in ASP.NET Core HTTP parsing for zero-allocation request handling

// ArrayPool<T>: pool-based allocation to reduce Gen0 pressure
var pool = ArrayPool<byte>.Shared;
byte[] rented = pool.Rent(1024);  // get from pool
try {
    // use rented
} finally {
    pool.Return(rented);  // return to pool — no GC required
}
```

---

## 8. Object Layout and Memory Overhead

### 8.1 Java Object Headers

Every Java object carries a mandatory overhead header:

```
Java Object Header (HotSpot JVM, 64-bit with Compressed OOPs):

  ┌─────────────────────────────────────────────────────┐
  │ Mark Word (8 bytes)                                 │
  │   ├── Hash code (31 bits)                           │
  │   ├── Age (GC generation, 4 bits)                   │
  │   ├── Lock state (biased/thin/fat lock bits)        │
  │   └── GC forwarding pointer (during GC relocation)  │
  ├─────────────────────────────────────────────────────┤
  │ Klass Pointer (4 bytes, compressed) or 8 bytes      │
  │   └── Pointer to class metadata (Metaspace)         │
  ├─────────────────────────────────────────────────────┤
  │ Array Length (4 bytes, only for arrays)             │
  ├─────────────────────────────────────────────────────┤
  │ Instance Fields (aligned to 8 bytes)                │
  └─────────────────────────────────────────────────────┘

Minimum object size: 16 bytes (header only, no fields)

Example: Integer wrapper vs. primitive
  int primitive:     4 bytes (in stack frame or array)
  Integer object:   16 bytes header + 4 bytes field + 4 bytes padding = 16 bytes
                    (4x overhead for a heap-allocated integer)

Example: Object[] vs. int[]
  int[100]:    16 bytes header + 400 bytes data = 416 bytes
  Integer[100]: 16 bytes header + 400 bytes pointers + 100 * 16 byte Integer objects
               = 416 + 1600 = 2016 bytes (nearly 5x overhead)
```

**This is why Java collections of primitives are expensive.** `ArrayList<Integer>` boxes every int. Libraries like Eclipse Collections, Trove, and JDK's upcoming Valhalla value types (Project Valhalla) address this.

### 8.2 Go Struct Layout

```go
// Go: Flat, cache-friendly layout
type Record struct {
    ID    int64   // 8 bytes
    Score float64 // 8 bytes
    Name  string  // 16 bytes (pointer + length — no separate heap alloc for small strings)
}
// sizeof(Record) = 32 bytes. No header. No GC object overhead on the struct itself.
// If Record is on the stack, zero GC involvement.
// If on heap (escaped), GC tracks the pointer to Record, not the struct fields individually.

// Slice: 24-byte header (pointer + len + cap), data on heap
// Map: pointer to runtime.hmap struct
// Interface: 2 words (type pointer + data pointer or value for small types)
```

### 8.3 Python Object Overhead

```python
import sys

sys.getsizeof(1)          # 28 bytes (PyLongObject: ob_refcnt + ob_type + digit)
sys.getsizeof(1.0)        # 24 bytes (PyFloatObject: ob_refcnt + ob_type + double)
sys.getsizeof("hello")    # 54 bytes (PyUnicodeObject + 5 chars)
sys.getsizeof([])         # 56 bytes (PyListObject: header + pointer array, 0 elements)
sys.getsizeof([1,2,3])    # 88 bytes (56 + 3 * 8-byte pointers, plus each int is 28 bytes)
# Total for [1,2,3]: 88 + 3*28 = 172 bytes vs. 12 bytes for C int[3]
# 14x overhead for a list of three integers.
```

### 8.4 Comparative Object Overhead Table

| Data Structure | C/Rust | Go | Java | Python |
|---|---|---|---|---|
| 64-bit integer | 8 bytes | 8 bytes | 16 bytes (boxed) / 8 bytes (primitive) | 28 bytes |
| Array of 100 int64s | 800 bytes | 800 bytes | 816 bytes (long[]) / 1616+ bytes (Long[]) | ~2,856 bytes |
| String "hello" | 5 bytes + metadata | 16 bytes header + 5 bytes | 48–56 bytes | 54 bytes |
| Struct/Object (3 fields) | 24 bytes | 24 bytes | 40 bytes | 120+ bytes (dict) |

---

## 9. Cache Locality and Throughput

### 9.1 Array of Structs vs. Struct of Arrays

This is one of the most impactful layout decisions in high-throughput systems:

```c
// Array of Structs (AoS) — common, object-oriented style
struct Particle {
    float x, y, z;       // position
    float vx, vy, vz;    // velocity
    float mass;
};
struct Particle particles[1000];

// To update all positions: particles[i].x += particles[i].vx * dt
// Memory access pattern:
// [x,y,z,vx,vy,vz,mass][x,y,z,vx,vy,vz,mass]...
// Loading x also loads y,z,vx,vy,vz,mass into cache line — wasted bandwidth
// if only x is needed for this loop.
```

```c
// Struct of Arrays (SoA) — data-oriented design
struct Particles {
    float x[1000], y[1000], z[1000];
    float vx[1000], vy[1000], vz[1000];
    float mass[1000];
};

// To update all x positions: particles.x[i] += particles.vx[i] * dt
// Memory access pattern: x[0],x[1],x[2]...x[7] fits in one cache line
// SIMD vectorization: compiler can process 8 floats per AVX2 instruction
```

**Impact:** SoA layouts can be 3–8x faster for batch operations due to:
- Better cache line utilization (no wasted loaded bytes)
- Auto-vectorization (contiguous same-type data → SIMD)
- Prefetcher effectiveness (sequential access pattern)

### 9.2 How GC Affects Cache Locality

**Compacting GC (Java G1, .NET):** Moves objects during collection. After compaction, related objects may be adjacent in memory — good for cache. But compaction itself causes STW pauses.

**Non-compacting GC (Go's mark-sweep, CPython refcount):** Objects stay in place. Over time, the heap fragments. Pointer-heavy object graphs may scatter related objects across many pages — poor cache locality.

**Rust/C (manual):** You control layout completely. Arenas and pool allocators let you place related objects contiguously:

```rust
// Arena allocation in Rust: all nodes contiguous in memory
struct Arena {
    nodes: Vec<Node>,  // all nodes in a single contiguous allocation
}

impl Arena {
    fn alloc(&mut self, data: i32) -> usize {
        let idx = self.nodes.len();
        self.nodes.push(Node { data, next: None });
        idx  // return index, not pointer — avoids GC tracing entirely
    }
}
// All nodes in nodes[] are cache-local. Traversal = sequential memory access.
```

### 9.3 False Sharing: The Multicore Memory Trap

When two threads write to different variables that happen to share a cache line (64 bytes), the CPU cache coherence protocol forces the cache line to bounce between cores — **false sharing**:

```
Cache Line (64 bytes):
  [thread0_counter (8 bytes)][thread1_counter (8 bytes)][padding...]

  Thread 0 writes thread0_counter → invalidates the cache line on ALL cores
  Thread 1 writes thread1_counter → invalidates the cache line on ALL cores
  Both threads compete for the SAME cache line despite touching DIFFERENT data.
  Result: Cache line ping-pong between CPU cores → 10-100x slowdown.
```

```rust
// Rust solution: pad to cache line boundary
#[repr(align(64))]  // align struct to 64-byte boundary
struct PaddedCounter {
    value: u64,
    _pad: [u8; 56],  // fill rest of cache line
}
// Now each counter lives on its own cache line — no false sharing
```

Java's `@Contended` annotation (JDK 8+) does the same via JVM padding.

---

## 10. Allocator Internals

### 10.1 Thread-Local Caches (TLABs and TCMalloc)

**JVM Thread-Local Allocation Buffers (TLABs):**

```
TLAB Allocation (JVM):

  Each thread has a private TLAB (typically 1% of Eden, ~512KB)
  
  Allocation: ptr = tlab.top; tlab.top += size; return ptr;
  ↑ This is a SINGLE memory write — ~1–2 ns allocation cost
  
  TLAB exhausted → request new TLAB from Eden (synchronized)
  Eden exhausted → Minor GC

  No synchronization on fast path — allocation is thread-local.
```

**TCMalloc (Thread-Caching Malloc — used by Go, some C++ apps):**

```
TCMalloc Architecture:

  Thread Cache:
    Per-thread free lists for size classes (8, 16, 32, 48, 64, ... 256KB)
    Allocation: pop from thread-local free list → ~1 cycle, no lock
    
  Central Cache:
    Shared pool per size class; thread caches refill from here
    Requires lock, but infrequent
    
  Page Heap:
    Manages large allocations (>256KB) via OS mmap/brk
    
  Go's allocator is derived from TCMalloc with GC integration:
    - 67 size classes (from 8 bytes to 32KB)
    - mcache (per-P, where P = processor in Go scheduler)
    - mcentral (shared per size class)
    - mheap (global; manages 8KB pages in 64-page "spans")
```

### 10.2 Python's Memory Allocator

CPython has a layered allocator:

```
CPython Allocator Layers:

  Layer 3 (Object-specific): PyLong_New, PyList_New — type-specific free lists
           ↓
  Layer 2 (pymalloc): Manages 8-byte-aligned blocks up to 512 bytes
                      Allocates from 256KB "arenas" divided into "pools"
                      (avoids glibc malloc overhead for small objects)
           ↓
  Layer 1 (malloc): System allocator for objects > 512 bytes
           ↓
  Layer 0 (OS): mmap / brk

  Small integer cache: -5 to 256 are pre-allocated singletons
  String interning: Short strings may be interned (shared across program)
```

---

## 11. Comparative Summary

### 11.1 Memory Management Strategy Matrix

| Language | Strategy | STW Pauses | Deterministic Free | Cycle Handling | Overhead |
|---|---|---|---|---|---|
| **C** | Manual | None | Yes | Manual | Minimal |
| **C++** | RAII + smart ptrs | None | Yes (deterministic dtors) | weak_ptr | Minimal |
| **Rust** | Borrow checker + RAII | None | Yes | N/A (compile error) | None |
| **Go** | Concurrent tri-color GC | <1ms | No | Yes | ~10–25% |
| **Java** | Generational GC (G1/ZGC) | ms–sub-ms | No | Yes | ~20–40% |
| **C#** | Generational GC | ms–sub-ms | No (but Dispose pattern) | Yes | ~15–30% |
| **Python** | Refcount + cycle collector | Cycle GC only | Mostly (refcount) | Cycle collector | High |
| **Ruby** | Tracing GC (YJIT-aware) | Yes | No | Yes | High |
| **PHP** | Refcount + cycle collector | Per-request reset | Mostly | Cycle collector | Medium |
| **JavaScript** | V8 generational GC | Incremental/concurrent | No | Yes | ~15–25% |

### 11.2 The Senior Engineering Trade-off

```
Memory Management Trade-off Spectrum:

  Control ◄──────────────────────────────────────► Safety & Velocity
  
  C ─── C++ ─── Rust ─── Go ─── Java/C# ─── Python/Ruby/PHP/JS

  Left side: You control every byte. Maximum performance possible.
             You can corrupt memory in ways that are exploitable.
             
  Right side: Runtime handles reclamation. Productivity high.
              GC pauses are real. Memory overhead is real.
              You cannot control when objects are freed.

  Rust: Achieves left-side performance with right-side safety.
        Cost: steep learning curve, longer compile times,
              occasionally fighting the borrow checker.
```

### 11.3 Choosing Based on Workload

| Workload | Recommended Memory Model | Rationale |
|---|---|---|
| Ultra-low latency (HFT, real-time) | C++/Rust (manual/RAII) | Zero GC pauses; deterministic latency |
| High-throughput backend | Go or Java (ZGC) | GC pauses acceptable; developer productivity |
| Batch data processing | Java/C# (tune GC for throughput) | Amortize GC cost over large batches |
| Scripting/glue code | Python/Ruby | Memory efficiency irrelevant; velocity matters |
| Web request handling | PHP/Python/Node.js | Share-nothing or GC per-request; acceptable |
| Embedded/systems | C/Rust | No runtime; deterministic |

---

## Key Mental Models

1. **Every heap allocation is a GC liability.** The goal of high-performance code is to maximize stack allocation, pool reuse, and minimize pointer indirection.

2. **Cache miss > GC pause for sequential code.** A fragmented heap with pointer-chasing graph traversal will underperform a compact array with a 10ms GC pause.

3. **Reference counting is not "no GC"** — it is a form of incremental GC with O(1) work per deallocation, but it cannot handle cycles and has high per-write overhead (INCREF/DECREF on every assignment).

4. **Rust's borrow checker is not a limitation — it is a proof system.** It proves at compile time that your program has no use-after-free, no data races, no double-free. No runtime required.

5. **Java's boxing is a tax on generics.** Every `List<Integer>` pays 4x memory overhead vs. `int[]`. Project Valhalla (JDK 23+) introduces value types to eliminate this. Until then, `int[]` beats `ArrayList<Integer>` for numeric data.

---

*End of Module 02.*

---

> **Next:** Module 03 covers **Concurrency, Threading, and I/O Paradigms** — OS kernel threads vs. goroutines, Node.js's event loop mechanics (libuv, epoll), the GIL in CPython and YARV, PHP-FPM's prefork model, and exactly how each system behaves under I/O-bound vs. CPU-bound load.
