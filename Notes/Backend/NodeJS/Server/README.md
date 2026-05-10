# NodeJS Foundations – Lesson 2

This lesson goes deeper into **how Node executes your code and manages data**. This is where many bugs come from if you don’t understand the *why*.

---

## 1. Reference vs Value (Mutation)

### Value types

* number
* string
* boolean
* null
* undefined

Copied by **value**.

```js
let a = 5;
let b = a;
b = 10; // a is still 5
```

### Reference types

* objects
* arrays
* functions

Copied by **reference**.

```js
const user = { name: "Ali" };
const ref = user;
ref.name = "Omar"; // user.name changed
```

### Why this matters in Node

* Shared module state
* Unexpected side effects
* Bugs across requests

### Rule

> If multiple requests touch the same object → mutation is dangerous.

Search deeper:

* "JavaScript reference vs value"
* "Shared state bugs NodeJS"

---

## 2. Impure vs Pure Functions

### Pure function

* Same input → same output
* No side effects

```js
function add(a, b) {
  return a + b;
}
```

### Impure function

* Depends on external state
* Modifies something outside

```js
let total = 0;
function addToTotal(x) {
  total += x;
}
```

### Why purity matters in servers

* Easier testing
* Predictable behavior
* Safer concurrency

Rule:

> Business logic should be pure. I/O should be isolated.

Search:

* "Pure functions backend"

---

## 3. Building Custom Modules

### What a module is

* One file
* Own scope
* Explicit exports

```js
// math.js
function sum(a, b) {
  return a + b;
}
module.exports = sum;
```

### Why modules exist

* Encapsulation
* Reuse
* Dependency boundaries

---

## 4. How `require` Works (Critical)

### Resolution order

When you do:

```js
require("x")
```

Node checks **in order**:

1. Native modules (`fs`, `http`)
2. `node_modules`
3. Relative paths (`./x`, `../x`)

---

### Require a folder

```js
require("./utils")
```

Node looks for:

1. `utils/package.json` → `main`
2. `utils/index.js`

---

### Require `.js` vs `.json`

* `.js` → executed as code
* `.json` → parsed once into object

JSON is **cached too**.

---

## 5. The 5 Globals in Every Module

Each file gets these **automatically**:

1. `require`
2. `module`
3. `exports`
4. `__filename`
5. `__dirname`

### Why this matters

* They explain how modules work
* They are NOT global across files

Search:

* "Node module wrapper function"

---

## 6. Native vs Custom Modules

### Native modules

* Built into Node
* Written in C++/JS
* Example: `fs`, `path`

### Custom modules

* Your own files
* Or npm packages

Both follow **the same system**.

---

## 7. Module Caching (Very Important)

### Behavior

* A module executes **once**
* Future `require` calls return cached exports

### Why this exists

* Performance
* Shared state

### Danger

* Mutating exported objects

Rule:

> Export functions, not mutable objects.

---

## 8. ESM (ECMAScript Modules)

### What it is

* Standard JS module system
* `import / export`

### Why ESM exists

* Browser compatibility
* Static analysis
* Better tooling

### Why Node moved toward it

* Common standard
* Tree shaking
* Clear dependency graph

### Trade-offs vs CJS

| CJS                | ESM            |
| ------------------ | -------------- |
| Dynamic            | Static         |
| require() anywhere | imports at top |
| Simple             | Stricter       |

Lab requirement = you must understand both.

Search:

* "CommonJS vs ESM Node"

---

## 9. Circular Dependencies

### What it is

* A requires B
* B requires A

### What happens

* One module gets **partial exports**

### Why it’s dangerous

* Silent bugs
* Undefined behavior

Rule:

> Circular deps mean bad architecture.

Search:

* "Node circular dependency explained"

---

## 10. What Is a Web Application (Real Definition)

A web app is a **long-running process** that:

1. Listens
2. Receives request
3. Routes it
4. Executes logic
5. Sends response

Everything else is abstraction.

---

## 11. Streams (Extremely Important)

### What is a stream (general)

> Data flowing in chunks over time

Instead of:

* load everything
* then process

You:

* process piece by piece

---

### Streams in Node

Types:

1. Readable
2. Writable
3. Duplex
4. Transform

### Why streams matter

* Low memory
* High performance
* Large files

### Example idea

Reading a 2GB file:

* without streams → crash
* with streams → fine

### Key concept

> Backpressure: don’t overwhelm the consumer.

Search:

* "Node streams backpressure"

---

## What You Must Understand After This Lesson

* Why mutation causes cross-request bugs
* Why purity matters in servers
* How require really loads code
* Why module caching exists
* Why ESM is stricter than CJS
* Why streams are foundational

---

## Thinking Questions (Mandatory)

1. Why is mutating an exported object dangerous?
2. Why does require cache instead of reloading?
3. Why are circular deps a design smell?
4. Why do streams scale better than buffers?

If you struggle here, stop and experiment before moving on.
