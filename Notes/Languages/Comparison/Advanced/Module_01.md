# Module 01: Compilation, Execution Models, and Runtimes

> **Series:** Principal Engineer's Deep-Dive into Languages, Runtimes, and Frameworks  
> **Level:** Junior → Senior Elevation  
> **Prerequisites:** Familiarity with at least one compiled or interpreted language; basic understanding of CPU registers and OS processes.

---

## Table of Contents

1. [The Fundamental Question: What Happens Before `main()`?](#1-the-fundamental-question)
2. [Ahead-of-Time (AOT) Compilation: C, C++, Rust, Go](#2-aot-compilation)
3. [Virtual Machines and JIT Compilation: JVM and CLR](#3-jit-and-virtual-machines)
4. [Dynamic Interpreted Engines: CPython, YARV, Zend, V8](#4-dynamic-interpreted-engines)
5. [V8's Optimization Pipeline: A Deep Dive](#5-v8-optimization-pipeline)
6. [Intermediate Representations (IR): The Compiler's Internal Language](#6-intermediate-representations)
7. [Runtime Startup, Warmup, and the Serverless Problem](#7-startup-warmup-and-serverless)
8. [Comparative Architecture Summary](#8-comparative-architecture-summary)
9. [Module Summary & Key Mental Models](#9-summary)

---

## 1. The Fundamental Question

Before a single line of your application logic runs, the runtime environment has already made profound, irreversible architectural decisions on your behalf. These decisions govern:

- **When** machine code is produced (at build time vs. at runtime)
- **Who** manages memory (you, the compiler, or the GC)
- **How** your code maps to CPU instructions (directly or through an interpreter loop)
- **What** optimizations are legal (static type-provable vs. speculative, profile-guided)

Understanding these decisions is the difference between a senior engineer who *knows* why their Java service has a 3-second cold start, and a junior who merely observes it.

The three dominant paradigms are:

```
┌─────────────────────────────────────────────────────────────────┐
│  PARADIGM 1: AOT                                                │
│  Source Code ──► Compiler ──► Native Machine Binary             │
│  (C, C++, Rust, Go)                                             │
├─────────────────────────────────────────────────────────────────┤
│  PARADIGM 2: VM / JIT                                           │
│  Source Code ──► Compiler ──► Bytecode ──► JIT ──► Native Code  │
│  (Java/JVM, C#/CLR)                                             │
├─────────────────────────────────────────────────────────────────┤
│  PARADIGM 3: Interpreted / Dynamic Engine                       │
│  Source Code ──► Tokenize ──► Parse ──► AST ──► Execute         │
│  (CPython, YARV, Zend, V8*)                                     │
│  (* V8 compiles to bytecode; see §4–5 for nuance)              │
└─────────────────────────────────────────────────────────────────┘
```

---

## 2. AOT Compilation

### 2.1 The Core Model

In Ahead-of-Time compilation, the entire translation from source to native machine instructions happens **before** deployment. By the time the OS loads your binary, every instruction is already a CPU opcode.

```
C/C++ Pipeline:
  .c / .cpp
    │
    ▼ Preprocessor (macro expansion, #includes)
    │
    ▼ Compiler Frontend (lexing, parsing → AST)
    │
    ▼ Compiler Middle-end (IR generation, optimizations: DCE, inlining, loop unrolling)
    │
    ▼ Compiler Backend (instruction selection, register allocation, code generation)
    │
    ▼ Assembler → .o object files
    │
    ▼ Linker (symbol resolution, relocation) → ELF/PE/Mach-O binary
    │
    ▼ OS Loader (maps segments into virtual address space)
    │
    ▼ CPU executes native opcodes directly
```

**Key implication:** The compiler has **unlimited time** to optimize. It can perform whole-program analysis (link-time optimization / LTO), profile-guided optimization (PGO), and vectorization — transformations that a JIT compiler cannot afford because they would add intolerable runtime latency.

### 2.2 C and C++: Zero-Overhead Abstraction (with a Cost)

- **Memory model:** Manual. `malloc`/`free` in C; `new`/`delete` plus RAII in C++.
- **Type system:** Statically typed, but `void*` and `reinterpret_cast` allow escape hatches that sacrifice safety for performance.
- **Runtime:** Essentially none. The C runtime (`crt0.s`) initializes the stack, sets up `argc`/`argv`, calls global constructors, then jumps to `main`. This takes **microseconds**.
- **Undefined Behavior (UB):** The compiler is permitted to assume UB never happens. This enables aggressive optimizations (e.g., removing null-checks the compiler proves unreachable) but is a major source of security vulnerabilities (buffer overflows, use-after-free).

```c
// C: Direct memory layout. No hidden overhead.
struct Packet {
    uint32_t seq;    // offset 0, 4 bytes
    uint16_t flags;  // offset 4, 2 bytes
    uint8_t  data[1024]; // offset 6, 1024 bytes
};
// sizeof(Packet) == 1030 bytes. No vtable. No GC header. No reference count.
// The compiler can pass this struct in registers or lay it out in cache lines.
```

### 2.3 Rust: AOT with Compile-Time Memory Safety

Rust uses the same LLVM backend as Clang, producing identical-quality machine code — but the borrow checker enforces memory safety **at compile time**, with **zero runtime cost**.

```
Rust Compilation Pipeline:
  .rs
    │
    ▼ rustc Frontend (HIR: High-level IR — type checking, borrow checking)
    │
    ▼ MIR (Mid-level IR — borrow check enforcement, optimization, drop elaboration)
    │
    ▼ LLVM IR (hand-off to LLVM for codegen)
    │
    ▼ LLVM Backend (same as Clang: vectorization, LTO, PGO)
    │
    ▼ Native binary (comparable performance to optimized C++)
```

**The borrow checker's runtime cost:** Zero. All lifetime and ownership checks are erased before LLVM sees the IR. The generated assembly is indistinguishable from equivalent safe C++.

**Key senior insight:** Rust's `unsafe` blocks do not add overhead — they simply disable borrow checker enforcement for that lexical scope, matching C semantics.

### 2.4 Go: AOT with a Managed Runtime

Go occupies a unique middle ground. It is compiled AOT to native code, but ships with a **built-in runtime** that provides:

- A concurrent garbage collector (tri-color mark-and-sweep)
- A goroutine scheduler (M:N, covered in Module 3)
- A stack-growing allocator (goroutine stacks start at 2–8 KB and grow dynamically)

```
Go Compilation Pipeline:
  .go
    │
    ▼ gc compiler (Go's own compiler, NOT GCC/LLVM by default)
    │   - Parses → SSA (Static Single Assignment) IR
    │   - Escape analysis (determines heap vs. stack allocation)
    │   - Inlining, bounds-check elimination
    │
    ▼ Plan 9 assembler → native binary + embedded Go runtime
    │
    ▼ At process start: runtime.main() initializes GC, scheduler, then calls user main()
```

**Startup time:** ~5–10ms (runtime initialization). Significantly faster than JVM, but measurably slower than a bare C binary.

**Escape analysis** is critical to understand: the Go compiler decides at compile time whether a variable lives on the goroutine's stack (cheap, no GC pressure) or escapes to the heap (requires GC tracking).

```go
// go build -gcflags='-m' reports escape decisions:

func noEscape() *int {
    x := 42       // x does NOT escape; lives on stack frame
    return &x     // Wait — returning a pointer means x DOES escape to heap
}
// The compiler detects the escape and allocates x on the heap automatically.
// No dangling pointer. No manual free. GC handles it.
```

---

## 3. JIT and Virtual Machines

### 3.1 The JVM: Write Once, Compile Twice

Java's execution model is a two-phase compilation: `javac` produces **bytecode** (`.class` files, a JVM-specific instruction set), and the **JVM** compiles that bytecode to native machine code at runtime using a JIT compiler.

```
Java Execution Pipeline:
  .java
    │
    ▼ javac (Java Compiler — produces bytecode, not native code)
    │
    ▼ .class files (JVM bytecode — a stack-based virtual instruction set)
    │
    ▼ JVM Startup:
    │   ├── Class loading (bootstrap → extension → application classloaders)
    │   ├── Bytecode verification (safety guarantee — no illegal memory access)
    │   ├── Interpreter phase (C1 tier 0: execute bytecode directly)
    │   │
    │   ├── Profiling (JVM instruments hot methods with counters)
    │   │
    │   ├── C1 JIT (Client Compiler — fast, lightly optimized native code)
    │   │       Kicks in at ~1,000 method invocations
    │   │
    │   └── C2 JIT (Server Compiler — heavily optimized, speculative)
    │           Kicks in at ~10,000–15,000 invocations (configurable)
    │           Performs: escape analysis, scalar replacement, inlining,
    │                     loop vectorization, intrinsics for SIMD
    │
    ▼ Native code in Code Cache (64MB–256MB by default)
```

**The critical insight here is speculative optimization.** The C2 compiler can make assumptions that are **not provable from bytecode alone**, such as:

- *"This virtual method dispatch always resolves to `FooImpl.process()` — I'll inline it."*
- *"This field is always non-null at this call site."*

These are **speculative deoptimization guards** (uncommon traps). If a guard is violated at runtime (e.g., a second class implementation is loaded), the JVM **deoptimizes** — abandoning the compiled code and falling back to the interpreter for that frame, then recompiling with the new information.

```
JVM Tier System (HotSpot tiered compilation):

Tier 0: Interpreter
  ↓ (invocation count threshold)
Tier 1: C1 - no profiling (simple methods)
  ↓
Tier 2: C1 - limited profiling
  ↓
Tier 3: C1 - full profiling (collects type profiles, branch histograms)
  ↓ (OSR: On-Stack Replacement or method re-entry)
Tier 4: C2 - fully optimized native code
  ↑ (deoptimize if speculation fails → back to interpreter → recompile)
```

### 3.2 GraalVM Native Image: JVM Goes AOT

GraalVM's Native Image compiles Java to a standalone native binary using **closed-world assumption** (all reachable code must be known at build time — no dynamic class loading). The result: startup in **tens of milliseconds**, lower memory footprint, but **no JIT warmup gains**. This is the technology behind Quarkus and Micronaut's "fast startup" mode for serverless.

### 3.3 The CLR: C#'s JIT Runtime

The .NET CLR is architecturally similar to the JVM but with notable differences:

- **IL (Intermediate Language / CIL):** Like JVM bytecode but richer in type information (value types, generics without erasure — see Module 5).
- **RyuJIT:** The .NET 8+ JIT compiler. Performs aggressive inlining, SIMD vectorization, and `Span<T>`-based stack allocation optimizations.
- **AOT Option:** .NET Native / NativeAOT allows AOT compilation with closed-world assumptions, similar to GraalVM Native Image.
- **Value Types:** Unlike Java, C# structs are true value types — no boxing overhead when used in generic collections (due to reified generics on the CLR).

```csharp
// C#: No boxing. Generic constraint resolves at JIT time to value type path.
List<int> ints = new List<int>();  // CLR emits specialized int[] backed storage
ints.Add(42);                      // No heap allocation for the integer 42

// Java: Type erasure means ArrayList<Integer> boxes every int to a heap object
ArrayList<Integer> ints = new ArrayList<>();
ints.add(42);  // Autoboxes: allocates Integer(42) on heap — GC pressure
```

**ASP.NET Core on Kestrel** exploits CLR value types aggressively: HTTP headers are parsed into stack-allocated `ReadOnlySpan<byte>` slices, avoiding heap allocation per request. This is why ASP.NET Core consistently leads TechEmpower benchmarks.

---

## 4. Dynamic Interpreted Engines

### 4.1 CPython: The Reference Interpreter

CPython is the canonical Python implementation. It is **not** a JIT compiler. It compiles Python source to CPython bytecode (`.pyc`) and then **interprets** that bytecode in an **eval loop**.

```
CPython Execution Pipeline:
  .py
    │
    ▼ Tokenizer (produces token stream)
    │
    ▼ Parser (produces AST — Abstract Syntax Tree)
    │
    ▼ Compiler (AST → CPython bytecode, stored in .pyc via marshal module)
    │
    ▼ Eval Loop (ceval.c: the heart of CPython)
    │   ┌─────────────────────────────────────────────┐
    │   │  while (1) {                                │
    │   │    opcode = *pc++;                          │
    │   │    switch (opcode) {                        │
    │   │      case LOAD_FAST: ...                    │
    │   │      case BINARY_OP: ...  ← dispatches to  │
    │   │      case CALL: ...          C functions    │
    │   │    }                                        │
    │   │  }                                          │
    │   └─────────────────────────────────────────────┘
    │
    ▼ Every Python object is a PyObject* (heap-allocated C struct)
      with ob_refcnt (reference count) and ob_type (type pointer)
```

**Every Python integer, string, list, and function is a heap-allocated `PyObject`.** There is no "unboxed int" in CPython. Even `x = 1` results in a heap object (though CPython caches small integers -5 to 256 as singletons).

**Python 3.11+ Adaptive Specialization:** CPython 3.11 introduced "specializing adaptive interpreter" — opcodes that self-modify after observing types (e.g., `BINARY_OP` → `BINARY_OP_ADD_INT` after seeing integer operands). This is **not** a full JIT but provides ~10–60% speedup on select workloads.

**Python 3.13 Free-Threaded Mode (PEP 703):** Experimental removal of the GIL, enabling true thread-level parallelism. Still carries significant per-object locking overhead.

### 4.2 Ruby's YARV

YARV (Yet Another Ruby VM, introduced Ruby 1.9) follows a similar architecture to CPython: compile to bytecode, interpret in an eval loop. Ruby 3.x added:

- **MJIT:** Method-based JIT (compiles Ruby bytecode to C, then uses GCC/Clang to compile to native). High quality but high latency (compilation in background thread).
- **YJIT:** Rust-based lazy basic block versioning JIT (from Shopify). Lower latency than MJIT, more production-ready. Ships in Ruby 3.1+. Delivers 10–70% throughput improvement on Rails workloads.

### 4.3 Zend Engine (PHP)

PHP's execution model is unique in web development: by default, **each HTTP request spawns a fresh interpreter execution**.

```
PHP (FPM) Request Lifecycle:
  HTTP Request arrives at PHP-FPM worker (pre-forked process)
    │
    ▼ Zend Engine: .php → opcodes (per-request unless OPcache active)
    │
    ▼ [OPcache]: compiled opcodes stored in shared memory → skip recompile
    │
    ▼ Execute opcodes in Zend Executor
    │
    ▼ Request completes → worker resets all state (globals, static vars)
    │
    ▼ Worker available for next request (no persistent in-process state)
```

**OPcache** (enabled by default in PHP 7+) caches the compiled opcode tree in shared memory across workers, eliminating repeated compilation. Without it, every request re-parses every `.php` file — catastrophically slow.

**PHP's "share-nothing" architecture** means state isolation is free: workers are prefork processes. This is simultaneously PHP's greatest reliability feature (a crashed worker doesn't affect others) and its greatest scalability limitation (each worker consumes ~30–80MB RSS).

### 4.4 JavaScript and V8: The Outlier

V8 defies simple categorization. It is a **multi-tier JIT compiler** that produces native machine code. Modern JavaScript execution is often faster than Python or Ruby by 5–50x due to V8's aggressive JIT.

See §5 for the full V8 pipeline.

---

## 5. V8's Optimization Pipeline

V8 is architecturally one of the most sophisticated execution engines in production software. Understanding it is essential for any engineer working with Node.js or browser-side JavaScript.

```
V8 Execution Pipeline (V8 v9+, post-Ignition/TurboFan era):

  JavaScript Source (.js / .ts transpiled)
    │
    ▼ Scanner (byte-level tokenizer, streaming)
    │
    ▼ Parser (produces AST)
    │   ├── Eager parsing: top-level code
    │   └── Lazy parsing: function bodies (deferred until called)
    │
    ▼ Ignition (Bytecode Compiler + Interpreter)
    │   - Compiles AST → V8 Bytecode (compact, register-based)
    │   - Interprets bytecode immediately (no cold-start penalty)
    │   - Collects type feedback: "this LOAD_PROPERTY always returns a Smi (small integer)"
    │   - Inline Cache (IC) slots embedded in bytecode stream
    │
    ▼ [Threshold: function called ~1000–10000x, or bytecode budget]
    │
    ▼ TurboFan (Optimizing JIT Compiler)
    │   ├── Input: Bytecode + Type Feedback from Ignition ICs
    │   ├── Sea-of-Nodes IR (combines control flow + data flow)
    │   ├── Typed lowering (specialize operations for observed types)
    │   ├── Escape analysis (stack-allocate objects that don't escape)
    │   ├── Inlining (aggressive: may inline 5–6 levels deep)
    │   ├── Deoptimization checkpoints (guards for every speculation)
    │   └── Backend: instruction selection, register allocation → native x64/ARM64
    │
    ▼ Optimized Native Code (stored in code space, GC-tracked)
    │
    ▼ [If speculation fails: Deoptimization]
    │   - Abandons native frame
    │   - Reconstructs interpreter frame from deopt metadata
    │   - Re-executes in Ignition
    │   - Marks function as "deopt'd" — TurboFan may recompile with new constraints
```

### 5.1 Hidden Classes (Shapes) — V8's Type System

JavaScript has no static types, yet V8 achieves near-static-dispatch speed through **hidden classes** (also called "maps" or "shapes"):

```javascript
// V8 creates a hidden class HC0 for empty object {}
const obj = {};               // HC0: { }

obj.x = 1;                    // Transitions: HC0 → HC1: { x }
obj.y = 2;                    // Transitions: HC1 → HC2: { x, y }

// All objects created with same property-assignment order share the same hidden class.
// V8 can then inline property access as a fixed offset load — no hash lookup!

// ❌ DEOPTIMIZATION TRAP: Adding properties in different orders
function Point(x, y) { this.x = x; this.y = y; }
const p1 = new Point(1, 2);   // HC: { x, y }

function BadPoint(x, y) { this.y = y; this.x = x; }  // reversed!
const p2 = new BadPoint(1, 2); // HC: { y, x } — different hidden class!
// V8 cannot share the optimized IC across p1 and p2 shapes.
```

### 5.2 Inline Caches (ICs)

Every property access (`obj.x`) in Ignition bytecode has an associated **Inline Cache** slot. The IC records:

- **Monomorphic:** One hidden class observed → direct field offset load
- **Polymorphic:** 2–4 hidden classes observed → small dispatch chain
- **Megamorphic:** 5+ hidden classes → falls back to generic hash lookup, **never optimized by TurboFan**

Writing idiomatic, shape-stable JavaScript (consistent property assignment order, avoiding `delete`, not changing object shapes mid-loop) is the primary lever for V8 performance.

### 5.3 Deoptimization: When Speculation Fails

```javascript
function add(a, b) {
    return a + b;  // TurboFan compiles this as int32 addition (observed: always numbers)
}

add(1, 2);      // 10,000 calls: all integers → TurboFan compiles optimized version
add("x", "y"); // DEOPTIMIZATION: string observed — violates int32 assumption
               // Frame is unrolled, control returns to Ignition
               // TurboFan recompiles with number-or-string union type
               // Result: slower than the monomorphic version
```

**The senior engineering lesson:** V8's peak performance requires **type stability**. Functions called with inconsistent argument types, or objects with inconsistently shaped properties, will never reach peak TurboFan optimization. This is why TypeScript, while not affecting V8 at runtime, encourages the type-stable coding patterns that V8 performs best on.

---

## 6. Intermediate Representations

Every compiler has at least one IR — an internal data structure that sits between the source AST and the final machine code, designed for analysis and transformation.

| Language/Compiler | Primary IR | Characteristics |
|---|---|---|
| **GCC (C/C++)** | GIMPLE → RTL | Tree-based, then register-transfer level |
| **Clang/Rust (LLVM)** | LLVM IR | SSA form, typed, platform-independent |
| **Go** | SSA (gc) | Go-specific SSA, simpler than LLVM |
| **Java (HotSpot C2)** | Sea of Nodes | Unified control+data flow graph |
| **V8 (TurboFan)** | Sea of Nodes | Similar to C2; enables global value numbering |
| **CPython** | CPython Bytecode | Stack-based VM, high-level ops |
| **JVM** | JVM Bytecode | Stack-based, typed, verifiable |
| **CLR** | CIL/MSIL | Stack-based, richer value-type semantics than JVM |

**LLVM IR is the most consequential IR in modern systems:** both Clang (C/C++) and rustc use it. Any optimization written for LLVM benefits both languages. This is why Rust gets LTO, PGO, and auto-vectorization essentially for free.

```llvm
; LLVM IR for: int add(int a, int b) { return a + b; }
define i32 @add(i32 %a, i32 %b) {
entry:
  %result = add i32 %a, %b   ; typed SSA form
  ret i32 %result
}
; LLVM can vectorize, inline, constant-fold this across translation units
; with LTO enabled — something CPython/Ruby/PHP eval loops cannot do.
```

---

## 7. Runtime Startup, Warmup, and the Serverless Problem

### 7.1 Cold Start Taxonomy

```
Cold Start Time (approximate, minimal "hello world" app):

  Language/Runtime        Cold Start     Notes
  ──────────────────────────────────────────────────────────────────────
  C/C++ (bare binary)     ~0.5–2ms       OS load time only
  Rust (bare binary)      ~0.5–2ms       Identical to C
  Go (minimal binary)     ~5–15ms        Runtime init: GC, scheduler
  Node.js                 ~80–200ms      V8 init + module loading
  Python (CPython)        ~50–150ms      Import chain + bytecode load
  Ruby (MRI)              ~100–300ms     Gem loading, global state init
  PHP (FPM, OPcache)      ~5–20ms/req    Share-nothing; no warmup concept
  Java (JVM, Spring Boot) ~3,000–15,000ms Class loading + JIT warmup
  C# (ASP.NET Core)       ~500–2,000ms   CoreCLR faster than JVM; better AOT story
  Java (GraalVM Native)   ~20–100ms      AOT: no JVM, no JIT warmup
```

### 7.2 The JVM Warmup Curve

JVM applications do not reach peak throughput immediately. The JIT tier system means performance improves over time:

```
JVM Throughput Over Time:

  Req/s
   ▲
   │                                         ┌──────────── Tier 4 (C2) plateau
   │                              ┌──────────┘  (peak: ~10–30min after start)
   │                    ┌─────────┘  Tier 3 (C1 + profiling)
   │          ┌─────────┘  Tier 1/2 (C1)
   │──────────┘  Tier 0 (Interpreter)
   │
   └──────────────────────────────────────────────────► Time (minutes)
        0    1    5    10   20   60
```

**In a long-running monolith**, this warmup is acceptable — the JVM delivers best-in-class JIT performance for statically-structured, server-side workloads after warmup.

**In a serverless (FaaS) environment**, the Lambda or Cloud Run invocation may complete in milliseconds — the JVM never warms up, and you pay the cold-start penalty on every invocation. This is why Go and Rust (and GraalVM Native) dominate serverless performance benchmarks.

### 7.3 Mitigation Strategies by Runtime

| Runtime | Serverless Mitigation | Long-Running Mitigation |
|---|---|---|
| **JVM** | GraalVM Native Image; SnapStart (AWS Lambda) | None needed; warmup amortized over lifetime |
| **Node.js** | Lightweight; ~150ms cold start acceptable | Worker threads for CPU; clustering |
| **Go** | ~10ms cold start; ideal for FaaS | Standard operation |
| **Python** | Lambda layers for dependency caching; Mangum (ASGI adapter) | Gunicorn workers; avoid re-import |
| **PHP** | Not typically used for FaaS; Bref (Serverless PHP) | PHP-FPM with OPcache |
| **C#/.NET** | NativeAOT; ReadyToRun (partial AOT precompilation) | Standard JIT; excellent throughput |
| **Rust** | ~1ms cold start; ideal for Lambda | Standard operation |

### 7.4 SnapStart and JVM Memory Snapshots

AWS Lambda SnapStart (introduced 2022 for Java 11+) takes a **Firecracker microVM snapshot** of the JVM after initialization:

```
SnapStart Lifecycle:
  1. First deployment → Lambda initializes JVM + application
  2. AWS snapshots memory state (Firecracker CRaC: Coordinated Restore at Checkpoint)
  3. Subsequent invocations → restore from snapshot (50–100ms) vs. full cold start (3–15s)
  4. Caveats: randomness, network connections, and time-sensitive state
              must be re-initialized on restore (onBeforeCheckpoint / onAfterRestore hooks)
```

This technique is a workaround for the fundamental mismatch between JVM's design assumptions (long-running process, amortized warmup) and the FaaS execution model.

---

## 8. Comparative Architecture Summary

### 8.1 Execution Model Matrix

| Language | Compilation | Runtime | Code Cache | Startup | Peak Perf |
|---|---|---|---|---|---|
| **C** | AOT (native) | None (libc) | N/A — static binary | ~1ms | Highest |
| **C++** | AOT (native) | Minimal (libstdc++) | N/A | ~1ms | Highest |
| **Rust** | AOT (LLVM) | Minimal (Rust stdlib) | N/A | ~1ms | Highest |
| **Go** | AOT + runtime | Go runtime (GC, scheduler) | N/A | ~10ms | Very High |
| **Java** | Bytecode + JIT | JVM (HotSpot/OpenJ9) | Code Cache (JIT output) | ~3–15s | Very High (post-warmup) |
| **C#** | IL + JIT | CLR / CoreCLR | JIT cache | ~500ms | Very High |
| **JavaScript** | JIT (V8) | V8 + libuv (Node.js) | Code Space (V8 heap) | ~150ms | High |
| **Python** | Bytecode (interp.) | CPython | .pyc (bytecode only) | ~100ms | Low–Medium |
| **Ruby** | Bytecode (YARV) | MRI + YJIT | YJIT code | ~200ms | Low–Medium |
| **PHP** | Bytecode + OPcache | Zend Engine | OPcache (shared mem) | ~10ms/req | Medium |

### 8.2 ASCII Architecture: Where Code Lives at Runtime

```
Process Memory Layout (schematic — not to scale):

C/C++/Rust Process:
┌─────────────────────┐ High addresses
│ Kernel space        │
├─────────────────────┤
│ Stack               │ ← grows down; function frames, local vars
│        ↓            │
│                     │
│        ↑            │
│ Heap                │ ← malloc/new; manual or RAII-managed
├─────────────────────┤
│ BSS (uninit data)   │ ← zero-initialized globals
│ Data (init data)    │ ← initialized globals, string literals
├─────────────────────┤
│ Text (code)         │ ← read-only; all machine code pre-baked
└─────────────────────┘ Low addresses

JVM Process:
┌─────────────────────┐
│ Kernel space        │
├─────────────────────┤
│ Native Stack        │ ← JVM internals, C2 compiler threads
├─────────────────────┤
│ Java Heap           │ ← ALL Java objects: Young Gen + Old Gen
│  ├── Eden Space     │   (managed by GC — Stop-The-World pauses)
│  ├── Survivor 0/1  │
│  └── Old Gen        │
├─────────────────────┤
│ Metaspace           │ ← Class metadata, constant pools (not GC'd normally)
├─────────────────────┤
│ Code Cache          │ ← JIT-compiled native code (configurable size)
├─────────────────────┤
│ Direct Memory       │ ← NIO ByteBuffer.allocateDirect() — off-heap
└─────────────────────┘

Node.js / V8 Process:
┌─────────────────────┐
│ Kernel space        │
├─────────────────────┤
│ Native Stack        │ ← V8 C++ internals, libuv
├─────────────────────┤
│ V8 Heap             │
│  ├── New Space      │ ← Young gen (2x semi-space, minor GC: Scavenge)
│  ├── Old Space      │ ← Survived objects (major GC: incremental mark-sweep)
│  ├── Code Space     │ ← TurboFan compiled native code (executable pages)
│  ├── Map Space      │ ← Hidden class descriptors
│  └── Large Object   │ ← Objects > 512KB (never moved by GC)
├─────────────────────┤
│ libuv Event Loop    │ ← I/O poll, timer heap (outside V8 heap)
└─────────────────────┘
```

---

## 9. Module Summary & Key Mental Models

### Core Takeaways

1. **AOT gives you maximum optimization time at zero runtime cost.** The trade-off is compile time, loss of runtime adaptability, and (for C/C++) no safety guarantees.

2. **JIT gives you speculative optimization based on observed runtime behavior.** This is uniquely powerful: a JVM program running the same workload for hours can outperform an equivalent C++ program for that specific workload. The cost is warmup time, GC pressure, and nondeterministic pause latency.

3. **Interpreters (CPython, YARV, Zend) trade execution speed for developer ergonomics.** Every primitive operation involves C function dispatch overhead. Python's slowness vs. Go is not a language design choice — it is a direct consequence of `PyObject*` boxing and the interpreter eval loop.

4. **V8 is an interpreter and a JIT compiler simultaneously.** Ignition ensures fast startup; TurboFan ensures peak throughput. The hidden class system is V8's solution to achieving static-dispatch speeds in a dynamically-typed language.

5. **Serverless environments expose the hidden cost of runtime warmup.** Any runtime requiring JIT warmup (JVM, CLR) pays a disproportionate penalty in FaaS. Go and Rust — with their minimal runtimes and AOT binaries — have an architectural advantage in Lambda-style deployments.

### Mental Model: The Optimization Budget

Think of each paradigm as having an **optimization budget** — time available to analyze and transform your code:

```
Optimization Time Budget:

AOT (C++/Rust with -O3/LTO):
  Minutes of compile time → Maximum analysis depth
  Budget: Unlimited (build time)
  └── Full program analysis, PGO, cross-module inlining

JIT (JVM C2 / TurboFan):
  Microseconds per compilation unit → Must balance speed vs. quality
  Budget: ~1–100ms per method compilation (background thread)
  └── Profile-guided, speculative, incremental

Interpreter (CPython):
  No compilation → No budget; execute directly
  Budget: 0
  └── Per-opcode dispatch; no cross-instruction analysis
```

---

## Further Reading

- **"Engineering a Compiler" (Cooper & Torczon):** Canonical reference for IR design and optimization passes.
- **V8 Blog (v8.dev):** Primary source on TurboFan, Ignition, and hidden class mechanics.
- **"JVM Internals" (James D. Bloom):** Deep dive on HotSpot's tier system and GC interaction.
- **Rust Reference — The Borrow Checker:** Formal description of lifetime and ownership rules.
- **"Systems Performance" (Brendan Gregg):** CPU cache behavior, CPU branch prediction, and how they interact with JIT output.

---

*End of Module 01.*

---

> **Ready to proceed?** Module 02 covers **Memory Management, Allocators, and Storage Internals** — tracing how each runtime allocates, tracks, and reclaims memory, including GC algorithm mechanics (Generational ZGC, Go's tri-color mark-and-sweep), and how memory layout choices affect CPU cache locality in high-throughput systems. Shall I generate Module 02?
