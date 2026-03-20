# JavaScript Advanced Topics — Interview Study Guide

> **Assumption**: You know basic JS (variables, loops, functions, DOM). This guide covers what separates junior devs from mid/senior devs in interviews.

---

# PART 1: THE CORE ENGINE — HOW JS ACTUALLY WORKS

Understanding this section alone puts you ahead of 80% of candidates.

---

## 1.1 The Event Loop, Call Stack, and Task Queues

### What is it?

JavaScript is **single-threaded** — it can only do one thing at a time. The Event Loop is the mechanism that lets JS appear to do many things concurrently (timers, network calls, UI events) despite being single-threaded.

### The components

- **Call Stack**: where your code runs. Functions push onto it, return pops them off.
- **Web APIs / Node APIs**: browser/Node handles async operations (setTimeout, fetch, fs.readFile) outside the JS thread.
- **Callback Queue (Macrotask Queue)**: completed async callbacks wait here.
- **Microtask Queue**: Promise `.then()` handlers wait here — runs before the Callback Queue.
- **Event Loop**: constantly checks "is the call stack empty? if yes, move something from a queue."

### The order of execution

```
1. Run all synchronous code (call stack)
2. Run ALL microtasks (Promise .then, queueMicrotask, MutationObserver)
3. Run ONE macrotask (setTimeout, setInterval, setImmediate, I/O)
4. Run ALL microtasks again
5. Render (browser only)
6. Repeat
```

```javascript
console.log("1 - sync");

setTimeout(() => console.log("2 - macrotask"), 0);

Promise.resolve().then(() => console.log("3 - microtask"));

queueMicrotask(() => console.log("4 - microtask 2"));

console.log("5 - sync");

// Output:
// 1 - sync
// 5 - sync
// 3 - microtask      <- microtasks before macrotasks!
// 4 - microtask 2
// 2 - macrotask
```

### Starvation — when microtasks go wrong

```javascript
// This will FREEZE the browser/Node forever!
function infiniteMicrotasks() {
    Promise.resolve().then(infiniteMicrotasks);
}
infiniteMicrotasks();
// Microtasks keep scheduling themselves — macrotasks never get a chance
```

### Interview talking points
- `setTimeout(fn, 0)` doesn't mean "run immediately" — it means "run after the current task and all microtasks."
- Promise `.then()` is ALWAYS asynchronous, even for already-resolved promises.
- `async/await` is syntactic sugar for Promises — `await` suspends the async function and queues the rest as a microtask.
- Node.js adds: `process.nextTick` (runs before other microtasks), `setImmediate` (macrotask after I/O).

---

## 1.2 Closures

### What is it?

A closure is a function that **remembers the variables from its outer scope even after that outer function has returned**. The inner function "closes over" the variables it needs.

### Why does it exist?

Closures are the foundation of data privacy, factory functions, memoization, and callback-based patterns in JavaScript.

```javascript
function makeCounter() {
    let count = 0;    // This variable is "closed over"

    return {
        increment() { count++; },
        decrement() { count--; },
        value()     { return count; }
    };
}

const counter = makeCounter();
counter.increment();
counter.increment();
counter.increment();
console.log(counter.value());  // 3
console.log(count);            // ReferenceError — count is private!
```

### Classic interview trap — closures in loops

```javascript
// WRONG — all callbacks share the same 'i' (var is function-scoped)
for (var i = 0; i < 3; i++) {
    setTimeout(() => console.log(i), 100);
}
// Output: 3, 3, 3  (not 0, 1, 2!)

// FIX 1 — use let (block-scoped, new binding each iteration)
for (let i = 0; i < 3; i++) {
    setTimeout(() => console.log(i), 100);
}
// Output: 0, 1, 2

// FIX 2 — use an IIFE to capture the value
for (var i = 0; i < 3; i++) {
    ((j) => setTimeout(() => console.log(j), 100))(i);
}
// Output: 0, 1, 2
```

### Real-world closure: function factory

```javascript
function multiplier(factor) {
    return (number) => number * factor;  // Closes over 'factor'
}

const double = multiplier(2);
const triple = multiplier(3);

console.log(double(5));   // 10
console.log(triple(5));   // 15
```

### Interview talking points
- A closure is created every time a function is defined inside another function.
- Closures retain a **reference** to the outer variable, not a copy (that's why the loop trap works).
- Every function in JS is a closure — it always has access to its outer scope chain.
- Be able to explain memory implications: closed-over variables live as long as the inner function lives.

---

## 1.3 Prototypes and Prototype Chain

### What is it?

Every JavaScript object has a hidden `[[Prototype]]` property that points to another object. When you access a property that doesn't exist on an object, JS looks up the prototype chain until it finds it or reaches `null`.

This is JavaScript's inheritance model — it's prototype-based, not class-based (even though ES6 `class` syntax looks like classes).

```javascript
const animal = {
    breathe() {
        return `${this.name} breathes`;
    }
};

const dog = {
    name: "Rex",
    bark() { return "Woof!"; }
};

// Set dog's prototype to animal
Object.setPrototypeOf(dog, animal);

console.log(dog.bark());     // "Woof!" — own method
console.log(dog.breathe());  // "Rex breathes" — found on prototype!
console.log(dog.toString()); // "[object Object]" — found on Object.prototype

// Chain: dog -> animal -> Object.prototype -> null
```

### Constructor functions and prototype

```javascript
function Person(name, age) {
    this.name = name;
    this.age = age;
    // DON'T put methods here — each instance gets its own copy (memory waste)
}

// Put shared methods on the prototype — all instances share ONE copy
Person.prototype.greet = function() {
    return `Hi, I'm ${this.name}, ${this.age} years old`;
};

const alice = new Person("Alice", 30);
const bob = new Person("Bob", 25);

console.log(alice.greet());   // "Hi, I'm Alice, 30 years old"
console.log(bob.greet());     // "Hi, I'm Bob, 25 years old"
console.log(alice.greet === bob.greet);  // true — same function!
```

### ES6 class is prototype inheritance under the hood

```javascript
class Animal {
    constructor(name) {
        this.name = name;
    }

    speak() {
        return `${this.name} makes a noise`;
    }
}

class Dog extends Animal {
    speak() {
        return `${this.name} barks`;
    }
}

const d = new Dog("Rex");
console.log(d.speak());   // "Rex barks"

// Under the hood:
// d.__proto__ === Dog.prototype
// Dog.prototype.__proto__ === Animal.prototype
// Animal.prototype.__proto__ === Object.prototype
```

### Interview talking points
- `__proto__` is the accessible property; `[[Prototype]]` is the internal slot. Use `Object.getPrototypeOf()`.
- `instanceof` checks the prototype chain: `d instanceof Animal` walks up the chain.
- `hasOwnProperty()` checks if a property is on the object itself, not the chain.
- ES6 `class` is syntactic sugar — there are no "real" classes in JS, only prototypes.
- `Object.create(proto)` creates an object with `proto` as its prototype.

---

## 1.4 `this` — The Most Confusing Keyword

### What is it?

`this` refers to the **execution context** — the object that "owns" the current function call. The value of `this` is determined by **how** a function is called, not where it's defined.

```javascript
const person = {
    name: "Alice",
    greet() {
        return `Hello from ${this.name}`;
    }
};

// 1. Method call — this = the object before the dot
console.log(person.greet());   // "Hello from Alice"

// 2. Regular function call — this = undefined (strict) or window (sloppy)
const fn = person.greet;
console.log(fn());   // "Hello from undefined" or TypeError in strict mode

// 3. Arrow functions — this = lexical (inherited from surrounding scope)
const obj = {
    name: "Bob",
    greetArrow: () => `Hello from ${this.name}`,  // 'this' is outer scope (window/undefined)
    greetRegular() {
        const inner = () => `Hello from ${this.name}`;  // 'this' is obj
        return inner();
    }
};

console.log(obj.greetArrow());   // "Hello from undefined"
console.log(obj.greetRegular()); // "Hello from Bob"
```

### The four rules of `this`

```javascript
// 1. Default binding — standalone function call
function show() { console.log(this); }
show();  // window (browser) or undefined (strict mode)

// 2. Implicit binding — method call
const obj = { name: "A", show() { console.log(this.name); } };
obj.show();  // "A"

// 3. Explicit binding — call, apply, bind
function greet(greeting) { return `${greeting}, ${this.name}`; }
const user = { name: "Alice" };
console.log(greet.call(user, "Hello"));         // "Hello, Alice"
console.log(greet.apply(user, ["Hi"]));         // "Hi, Alice"
const boundGreet = greet.bind(user);
console.log(boundGreet("Hey"));                 // "Hey, Alice"

// 4. new binding — constructor call
function Person(name) { this.name = name; }
const p = new Person("Bob");
console.log(p.name);  // "Bob"
```

### Arrow functions and this

```javascript
class Timer {
    constructor() {
        this.seconds = 0;
    }

    start() {
        // WRONG: regular function — 'this' is undefined or window inside setTimeout
        // setInterval(function() { this.seconds++; }, 1000);

        // RIGHT: arrow function — 'this' lexically inherits from start()
        setInterval(() => { this.seconds++; }, 1000);
    }
}
```

### Interview talking points
- Arrow functions don't have their own `this` — they inherit from the surrounding lexical scope.
- `call(thisArg, arg1, arg2)` — individual args. `apply(thisArg, [args])` — array of args.
- `bind()` returns a new function with `this` permanently bound.
- `new` creates a new object, sets `this` to it, and returns it.
- The rule: `this` is set at call time, not at definition time (except for arrow functions).

---

## 1.5 Scope, Hoisting, and the Temporal Dead Zone

### Scope

```javascript
// var — function scoped, hoisted, no block scope
function example() {
    if (true) {
        var x = 10;  // Hoisted to function scope
    }
    console.log(x);  // 10 — var leaks out of block!
}

// let/const — block scoped, not accessible before declaration
{
    let y = 20;
    const z = 30;
}
// console.log(y);  // ReferenceError
// console.log(z);  // ReferenceError
```

### Hoisting

```javascript
// var declarations are hoisted (but NOT initializations)
console.log(name);   // undefined (not ReferenceError — hoisted!)
var name = "Alice";
console.log(name);   // "Alice"

// Function declarations are fully hoisted
console.log(add(2, 3));  // 5 — works before the declaration!
function add(a, b) { return a + b; }

// Function expressions are NOT hoisted
// console.log(subtract(5, 2));  // TypeError: subtract is not a function
var subtract = function(a, b) { return a - b; };
```

### Temporal Dead Zone (TDZ)

```javascript
// let and const are hoisted BUT not initialized — accessing them before
// declaration throws a ReferenceError (the "Temporal Dead Zone")
console.log(myVar);   // undefined (var — hoisted and initialized)
// console.log(myLet);   // ReferenceError: Cannot access 'myLet' before initialization (TDZ!)
var myVar = 1;
let myLet = 2;
```

### Interview talking points
- `var` is function-scoped, `let`/`const` are block-scoped.
- All declarations (`var`, `let`, `const`, `function`, `class`) are hoisted — but `let`/`const`/`class` are in the TDZ.
- Function declarations are fully hoisted (body included); function expressions are not.
- The TDZ exists from the start of the block until the declaration is reached.

---

# PART 2: ASYNCHRONOUS JAVASCRIPT

---

## 2.1 Promises

### What is it?

A Promise is an object representing the eventual completion (or failure) of an asynchronous operation. It's a cleaner alternative to callbacks for handling async code.

A Promise is always in one of three states: **pending**, **fulfilled**, or **rejected**.

```javascript
// Creating a Promise
function fetchUser(id) {
    return new Promise((resolve, reject) => {
        setTimeout(() => {
            if (id > 0) {
                resolve({ id, name: "Alice" });  // Success
            } else {
                reject(new Error("Invalid ID"));  // Failure
            }
        }, 1000);
    });
}

// Consuming a Promise
fetchUser(1)
    .then(user => {
        console.log(user.name);  // "Alice"
        return fetchUser(2);     // Chain another async call
    })
    .then(user2 => console.log(user2.name))
    .catch(err => console.error(err.message))
    .finally(() => console.log("Done"));
```

### Promise combinators

```javascript
const p1 = Promise.resolve(1);
const p2 = Promise.resolve(2);
const p3 = Promise.reject(new Error("fail"));

// all — waits for ALL to resolve, rejects if ANY rejects
Promise.all([p1, p2])
    .then(([r1, r2]) => console.log(r1, r2));  // 1 2

// allSettled — waits for ALL to settle (never rejects)
Promise.allSettled([p1, p3])
    .then(results => results.forEach(r => console.log(r.status)));
// "fulfilled", "rejected"

// race — resolves/rejects with the FIRST settled promise
Promise.race([
    new Promise(r => setTimeout(() => r("slow"), 1000)),
    new Promise(r => setTimeout(() => r("fast"), 100)),
]).then(winner => console.log(winner));  // "fast"

// any — resolves with FIRST fulfilled, rejects only if ALL reject
Promise.any([p3, p1, p2])
    .then(first => console.log(first));  // 1
```

### Interview talking points
- Once a Promise is settled (fulfilled or rejected), it cannot change state.
- `.catch(fn)` is equivalent to `.then(undefined, fn)`.
- `.finally()` runs regardless of outcome and doesn't receive a value.
- Promise chains return a new Promise at each `.then()` — that's what enables chaining.
- `Promise.all` short-circuits on the first rejection.

---

## 2.2 async/await

### What is it?

`async/await` is syntactic sugar over Promises that lets you write asynchronous code that reads like synchronous code.

```javascript
// Promise chain version
function loadUserData(userId) {
    return fetchUser(userId)
        .then(user => fetchOrders(user.id))
        .then(orders => fetchDetails(orders[0].id))
        .then(details => ({ details }))
        .catch(err => console.error(err));
}

// async/await version — much cleaner
async function loadUserData(userId) {
    try {
        const user = await fetchUser(userId);
        const orders = await fetchOrders(user.id);
        const details = await fetchDetails(orders[0].id);
        return { details };
    } catch (err) {
        console.error(err);
    }
}
```

### Parallel vs sequential execution

```javascript
// SEQUENTIAL — total time = 1s + 1s + 1s = 3 seconds (usually wrong)
async function sequential() {
    const a = await delay(1000, "A");
    const b = await delay(1000, "B");
    const c = await delay(1000, "C");
    return [a, b, c];
}

// PARALLEL — total time = 1 second (usually what you want)
async function parallel() {
    const [a, b, c] = await Promise.all([
        delay(1000, "A"),
        delay(1000, "B"),
        delay(1000, "C"),
    ]);
    return [a, b, c];
}

// Helper
function delay(ms, value) {
    return new Promise(resolve => setTimeout(() => resolve(value), ms));
}
```

### Error handling patterns

```javascript
// Pattern 1: try/catch
async function getData() {
    try {
        const data = await riskyOperation();
        return data;
    } catch (err) {
        console.error("Failed:", err);
        return null;
    }
}

// Pattern 2: .catch() on the await expression
async function getData2() {
    const data = await riskyOperation().catch(err => null);  // null on failure
    if (!data) return;
    return data;
}

// Pattern 3: helper that returns [error, result]
async function safeAwait(promise) {
    try {
        return [null, await promise];
    } catch (err) {
        return [err, null];
    }
}

async function getData3() {
    const [err, data] = await safeAwait(riskyOperation());
    if (err) { console.error(err); return; }
    return data;
}
```

### Interview talking points
- `async` functions always return a Promise, even if you `return 42` (it wraps to `Promise.resolve(42)`).
- `await` can only be used inside `async` functions (or top-level in modules).
- Don't `await` in a loop when the operations are independent — use `Promise.all`.
- Forgetting `await` is a common bug — the function returns a Promise, not the resolved value.

---

## 2.3 Generators and Iterators

### What is it?

A generator is a function that can be **paused and resumed**. It produces values lazily using `yield`. This is the foundation of custom iteration in JS.

```javascript
function* counter(start = 0) {
    while (true) {
        yield start++;   // Pauses here, returns start, then resumes
    }
}

const gen = counter(10);
console.log(gen.next());  // { value: 10, done: false }
console.log(gen.next());  // { value: 11, done: false }
console.log(gen.next());  // { value: 12, done: false }

// Generators are iterables
function* range(start, end) {
    for (let i = start; i < end; i++) {
        yield i;
    }
}

for (const n of range(1, 5)) {
    console.log(n);  // 1 2 3 4
}

console.log([...range(1, 6)]);  // [1, 2, 3, 4, 5]
```

### Two-way communication with generators

```javascript
function* calculator() {
    let result = 0;
    while (true) {
        const input = yield result;  // yield sends result out, receives input
        if (input === null) break;
        result += input;
    }
    return result;
}

const calc = calculator();
calc.next();      // Start it — { value: 0, done: false }
calc.next(5);     // Send 5 in — { value: 5, done: false }
calc.next(3);     // Send 3 in — { value: 8, done: false }
calc.next(null);  // Signal done — { value: 8, done: true }
```

### Interview talking points
- `gen.next(value)` resumes the generator and sends `value` as the result of the `yield` expression.
- `gen.return(value)` terminates the generator.
- `gen.throw(error)` throws an error at the current yield point.
- Generators are the foundation of async/await in older transpiled code (Babel).
- `yield*` delegates to another iterable/generator.

---

# PART 3: FUNCTIONAL PROGRAMMING IN JS

---

## 3.1 Pure Functions, Immutability, and Side Effects

### What is it?

A **pure function** always returns the same output for the same input and has **no side effects** (doesn't modify anything outside itself). Immutability means never modifying data in place — always creating new data.

```javascript
// IMPURE — modifies external state and has unpredictable output
let total = 0;
function addToTotal(n) {
    total += n;  // Side effect: modifies outer variable
    console.log(Date.now());  // Different result each call!
    return total;
}

// PURE — same input = same output, no side effects
function add(a, b) {
    return a + b;
}

// Immutability with objects
const user = { name: "Alice", age: 30, tags: ["admin"] };

// MUTABLE (bad):
user.age = 31;  // Modifies the original

// IMMUTABLE (good) — create a new object:
const updatedUser = { ...user, age: 31 };
const addedTag = { ...user, tags: [...user.tags, "editor"] };

// Immutability with arrays
const arr = [1, 2, 3];
const newArr = [...arr, 4];        // Add
const removed = arr.filter(n => n !== 2);  // Remove
const mapped = arr.map(n => n * 2);        // Transform
```

### Interview talking points
- Pure functions are easily testable — no setup/teardown needed.
- They're referentially transparent — you can replace a call with its result.
- Side effects aren't always avoidable (I/O, logging, network) — the goal is to isolate them.
- `const` prevents reassignment but NOT mutation — `const arr = []; arr.push(1)` still works.

---

## 3.2 Higher-Order Functions — map, filter, reduce

### What is it?

A higher-order function either takes a function as an argument or returns a function. `map`, `filter`, and `reduce` are the core trio.

```javascript
const orders = [
    { id: 1, product: "Laptop",  price: 999,  category: "tech",  qty: 1 },
    { id: 2, product: "Phone",   price: 499,  category: "tech",  qty: 2 },
    { id: 3, product: "Desk",    price: 299,  category: "home",  qty: 1 },
    { id: 4, product: "Monitor", price: 349,  category: "tech",  qty: 1 },
];

// map — transform each element
const productNames = orders.map(order => order.product);
// ["Laptop", "Phone", "Desk", "Monitor"]

// filter — keep elements that pass a test
const techOrders = orders.filter(order => order.category === "tech");
// [Laptop, Phone, Monitor orders]

// reduce — accumulate into a single value
const totalRevenue = orders.reduce((acc, order) => acc + order.price * order.qty, 0);
// 999 + 998 + 299 + 349 = 2645

// Chaining — get total revenue from tech orders only
const techRevenue = orders
    .filter(o => o.category === "tech")
    .reduce((acc, o) => acc + o.price * o.qty, 0);
// 999 + 998 + 349 = 2346

// reduce can do anything — group by category:
const grouped = orders.reduce((acc, order) => {
    const key = order.category;
    if (!acc[key]) acc[key] = [];
    acc[key].push(order);
    return acc;
}, {});
// { tech: [...], home: [...] }
```

### Interview talking points
- `map` always returns an array of the same length. `filter` returns a subset. `reduce` returns anything.
- These methods don't mutate the original array.
- `reduce` is the most powerful — you can implement `map` and `filter` using `reduce`.
- `flatMap` is `map` followed by flattening one level.

---

## 3.3 Currying and Function Composition

### What is it?

**Currying** transforms a function with multiple arguments into a sequence of functions each taking one argument. **Composition** combines functions so the output of one becomes the input of the next.

```javascript
// Regular function
const add = (a, b) => a + b;

// Curried version
const curriedAdd = a => b => a + b;
const add5 = curriedAdd(5);
console.log(add5(3));   // 8
console.log(add5(10));  // 15

// Practical currying — reusable filters
const filterBy = key => value => arr => arr.filter(item => item[key] === value);
const filterByCategory = filterBy("category");
const techOnly = filterByCategory("tech");

const products = [
    { name: "Laptop", category: "tech" },
    { name: "Chair", category: "home" },
    { name: "Phone", category: "tech" },
];

console.log(techOnly(products));  // [Laptop, Phone]

// Function composition
const compose = (...fns) => x => fns.reduceRight((acc, fn) => fn(acc), x);
const pipe = (...fns) => x => fns.reduce((acc, fn) => fn(acc), x);

const trim = str => str.trim();
const toLowerCase = str => str.toLowerCase();
const replace = (from, to) => str => str.replace(from, to);

const sanitize = pipe(
    trim,
    toLowerCase,
    replace(/\s+/g, "-")
);

console.log(sanitize("  Hello World  "));  // "hello-world"
```

### Interview talking points
- Currying enables partial application — pre-configuring functions with some arguments.
- `compose` applies right-to-left (mathematical), `pipe` applies left-to-right (more readable).
- Libraries like Ramda and Lodash/fp provide curried versions of all utility functions.

---

# PART 4: MODERN JAVASCRIPT (ES6+)

---

## 4.1 Destructuring, Spread, and Rest

```javascript
// Object destructuring
const user = { name: "Alice", age: 30, address: { city: "Cairo" } };
const { name, age, address: { city } } = user;
const { name: userName, role = "user" } = user;  // rename + default

// Array destructuring
const [first, second, ...rest] = [1, 2, 3, 4, 5];
console.log(first, second, rest);  // 1, 2, [3, 4, 5]

// Swap variables
let a = 1, b = 2;
[a, b] = [b, a];

// Function parameters
function display({ name, age = 0, skills = [] }) {
    console.log(`${name}, ${age}, ${skills.join(", ")}`);
}
display({ name: "Bob", skills: ["JS", "Python"] });

// Spread
const arr1 = [1, 2, 3];
const arr2 = [...arr1, 4, 5];         // [1, 2, 3, 4, 5]
const obj1 = { a: 1 };
const obj2 = { ...obj1, b: 2 };       // { a: 1, b: 2 }
const merged = { ...obj1, ...{ a: 99, c: 3 } };  // { a: 99, c: 3 } — last wins

// Rest parameters
function sum(...numbers) {
    return numbers.reduce((acc, n) => acc + n, 0);
}
console.log(sum(1, 2, 3, 4, 5));  // 15
```

---

## 4.2 Modules (ES Modules)

### What is it?

ES Modules provide a native module system with static imports/exports, enabling code splitting, tree shaking, and clear dependency graphs.

```javascript
// math.js — named exports
export const PI = 3.14159;
export function add(a, b) { return a + b; }
export function multiply(a, b) { return a * b; }

// utils.js — default export
export default function formatNumber(n) {
    return n.toLocaleString();
}

// main.js — importing
import { PI, add } from "./math.js";           // Named imports
import formatNumber from "./utils.js";           // Default import
import { add as sum } from "./math.js";          // Rename
import * as MathUtils from "./math.js";          // Namespace import

console.log(MathUtils.PI);       // 3.14159
console.log(sum(2, 3));          // 5

// Dynamic import — lazy loading
async function loadChart() {
    const { Chart } = await import("./chart.js");
    new Chart();
}
```

### Interview talking points
- ES Modules are static — imports/exports are analyzed at parse time (enables tree shaking).
- `import` is hoisted to the top of the file.
- Dynamic `import()` returns a Promise — useful for code splitting.
- Modules are singletons — importing the same module twice gives the same instance.
- CommonJS (`require`) is synchronous and dynamic; ES Modules are asynchronous and static.

---

## 4.3 Symbols, WeakMap, WeakSet, WeakRef

### Symbol — unique identifiers

```javascript
const id = Symbol("id");
const id2 = Symbol("id");
console.log(id === id2);  // false — every Symbol is unique

const user = {
    name: "Alice",
    [id]: 42   // Symbol as a property key — won't show in for...in or Object.keys
};

console.log(user[id]);          // 42
console.log(Object.keys(user)); // ["name"] — Symbol not included
```

### WeakMap and WeakSet — memory-safe collections

```javascript
// WeakMap — keys must be objects, doesn't prevent GC
const cache = new WeakMap();

function processUser(user) {
    if (cache.has(user)) return cache.get(user);
    const result = expensiveCalculation(user);
    cache.set(user, result);  // When 'user' is GC'd, cache entry is too
    return result;
}

// WeakSet — a set of objects, doesn't prevent GC
const visited = new WeakSet();
function trackVisit(element) {
    if (visited.has(element)) return;
    visited.add(element);
    // When element is removed from DOM and GC'd, it's removed from WeakSet too
}
```

### Interview talking points
- Symbols are useful for defining "private-ish" object properties and protocol methods.
- `Symbol.iterator`, `Symbol.toPrimitive`, `Symbol.hasInstance` are built-in well-known symbols.
- `WeakMap`/`WeakSet` are not iterable — you can't loop over them or get their size.
- They're primarily used for caches and metadata storage where you don't want to prevent GC.

---

## 4.4 Proxy and Reflect

### What is it?

`Proxy` wraps an object and intercepts operations on it (get, set, delete, function calls). `Reflect` provides methods that mirror the Proxy traps with default behavior.

```javascript
// Validation proxy
function createValidatedUser(data) {
    return new Proxy(data, {
        set(target, property, value) {
            if (property === "age" && (typeof value !== "number" || value < 0)) {
                throw new TypeError("Age must be a positive number");
            }
            if (property === "name" && typeof value !== "string") {
                throw new TypeError("Name must be a string");
            }
            return Reflect.set(target, property, value);  // Default behavior
        },
        get(target, property) {
            if (!(property in target)) {
                throw new ReferenceError(`Property "${property}" doesn't exist`);
            }
            return Reflect.get(target, property);
        }
    });
}

const user = createValidatedUser({ name: "Alice", age: 30 });
user.name = "Bob";   // OK
user.age = 25;       // OK
user.age = -5;       // TypeError: Age must be a positive number
user.foo;            // ReferenceError: Property "foo" doesn't exist
```

### Observable pattern with Proxy

```javascript
function observable(target, onChange) {
    return new Proxy(target, {
        set(obj, key, value) {
            const oldValue = obj[key];
            const result = Reflect.set(obj, key, value);
            if (oldValue !== value) onChange(key, value, oldValue);
            return result;
        }
    });
}

const state = observable({ count: 0, name: "App" }, (key, newVal, oldVal) => {
    console.log(`${key}: ${oldVal} -> ${newVal}`);
});

state.count = 1;   // "count: 0 -> 1"
state.name = "MyApp";  // "name: App -> MyApp"
```

### Interview talking points
- Proxy is how Vue 3's reactivity system works under the hood.
- `Reflect` methods mirror Proxy traps and provide the default behavior.
- Available traps: `get`, `set`, `has`, `deleteProperty`, `apply`, `construct`, `ownKeys`, etc.
- Cannot proxy primitive values.

---

# PART 5: DESIGN PATTERNS IN JAVASCRIPT

---

## 5.1 Module Pattern and IIFE

```javascript
// IIFE (Immediately Invoked Function Expression) — creates private scope
const Counter = (() => {
    let count = 0;  // Private

    return {
        increment: () => ++count,
        decrement: () => --count,
        value: () => count,
        reset: () => { count = 0; }
    };
})();

Counter.increment();
Counter.increment();
console.log(Counter.value());  // 2
console.log(Counter.count);    // undefined — private!
```

---

## 5.2 Observer / EventEmitter Pattern

```javascript
class EventEmitter {
    #listeners = new Map();

    on(event, listener) {
        if (!this.#listeners.has(event)) {
            this.#listeners.set(event, new Set());
        }
        this.#listeners.get(event).add(listener);
        return () => this.off(event, listener);  // Returns unsubscribe function
    }

    off(event, listener) {
        this.#listeners.get(event)?.delete(listener);
    }

    emit(event, ...args) {
        this.#listeners.get(event)?.forEach(listener => listener(...args));
    }

    once(event, listener) {
        const wrapper = (...args) => {
            listener(...args);
            this.off(event, wrapper);
        };
        return this.on(event, wrapper);
    }
}

const emitter = new EventEmitter();
const unsubscribe = emitter.on("data", (data) => console.log("Received:", data));
emitter.once("error", (err) => console.error("Error:", err));

emitter.emit("data", { id: 1 });   // "Received: { id: 1 }"
emitter.emit("error", "timeout");  // "Error: timeout"
emitter.emit("error", "timeout");  // (nothing — once removes itself)
unsubscribe();                     // Remove the data listener
```

---

## 5.3 Factory and Builder Patterns

```javascript
// Factory Pattern — creates objects without exposing construction logic
function createAnimal(type, name) {
    const types = {
        dog: { sound: "Woof", legs: 4 },
        cat: { sound: "Meow", legs: 4 },
        bird: { sound: "Tweet", legs: 2 },
    };
    const config = types[type];
    if (!config) throw new Error(`Unknown animal type: ${type}`);

    return {
        name,
        type,
        ...config,
        speak() { return `${this.name} says ${this.sound}`; }
    };
}

const dog = createAnimal("dog", "Rex");
console.log(dog.speak());  // "Rex says Woof"

// Builder Pattern — constructs complex objects step by step
class QueryBuilder {
    #table = "";
    #conditions = [];
    #columns = ["*"];
    #limit = null;

    from(table) { this.#table = table; return this; }
    select(...cols) { this.#columns = cols; return this; }
    where(condition) { this.#conditions.push(condition); return this; }
    limitTo(n) { this.#limit = n; return this; }

    build() {
        let query = `SELECT ${this.#columns.join(", ")} FROM ${this.#table}`;
        if (this.#conditions.length) query += ` WHERE ${this.#conditions.join(" AND ")}`;
        if (this.#limit) query += ` LIMIT ${this.#limit}`;
        return query;
    }
}

const query = new QueryBuilder()
    .from("users")
    .select("id", "name", "email")
    .where("age > 18")
    .where("active = true")
    .limitTo(10)
    .build();

console.log(query);
// SELECT id, name, email FROM users WHERE age > 18 AND active = true LIMIT 10
```

---

## 5.4 Memoization and Debounce/Throttle

```javascript
// Memoization
function memoize(fn) {
    const cache = new Map();
    return function(...args) {
        const key = JSON.stringify(args);
        if (cache.has(key)) return cache.get(key);
        const result = fn.apply(this, args);
        cache.set(key, result);
        return result;
    };
}

const expensiveFn = memoize((n) => {
    console.log(`Computing for ${n}...`);
    return n * n;
});

console.log(expensiveFn(5));  // "Computing for 5..." → 25
console.log(expensiveFn(5));  // 25 (no computation!)

// Debounce — wait until user STOPS doing something (e.g., stop typing)
function debounce(fn, delay) {
    let timer;
    return function(...args) {
        clearTimeout(timer);
        timer = setTimeout(() => fn.apply(this, args), delay);
    };
}

const searchInput = debounce((value) => {
    console.log(`Searching for: ${value}`);
}, 300);

// Throttle — execute at most once per interval (e.g., scroll handler)
function throttle(fn, limit) {
    let lastCall = 0;
    return function(...args) {
        const now = Date.now();
        if (now - lastCall >= limit) {
            lastCall = now;
            return fn.apply(this, args);
        }
    };
}

const handleScroll = throttle(() => console.log("Scroll event"), 100);
```

### Interview talking points
- Debounce: last call wins — use for search input, window resize, form validation.
- Throttle: first call wins, then one per interval — use for scroll, mouse move, API rate limiting.
- The difference: debounce resets the timer on each call, throttle ignores calls during the interval.

---

# PART 6: ADVANCED CONCEPTS

---

## 6.1 Memory Management and Leaks

```javascript
// Common memory leaks:

// 1. Forgotten event listeners
const button = document.getElementById("btn");
function handleClick() { console.log("clicked"); }
button.addEventListener("click", handleClick);
// FIX: remove when no longer needed
button.removeEventListener("click", handleClick);

// 2. Closures holding large data
function createLeak() {
    const largeData = new Array(1_000_000).fill("data");
    return function() {
        // This closure captures largeData even if it doesn't use it
        console.log("hello");
    };
}

// 3. Detached DOM nodes
let detached;
const div = document.createElement("div");
document.body.appendChild(div);
detached = div;  // Holds reference
document.body.removeChild(div);  // Removed from DOM but not from memory!
detached = null;  // FIX: release the reference

// 4. Global variables
function badFunction() {
    leakedVar = "I'm global!";  // Forgot 'let/const/var' — becomes global
}
```

---

## 6.2 TypeScript Essentials for JS Interviews

### What is it?

TypeScript is a typed superset of JavaScript that compiles to JS. Most modern JS interviews now assume TypeScript knowledge.

```typescript
// Basic types
let name: string = "Alice";
let age: number = 30;
let active: boolean = true;
let anything: unknown = "could be anything";  // safer than 'any'

// Interfaces — define object shapes
interface User {
    id: number;
    name: string;
    email: string;
    role?: "admin" | "user";  // Optional + union type
}

// Generics — reusable type-safe functions
function identity<T>(value: T): T {
    return value;
}

function head<T>(arr: T[]): T | undefined {
    return arr[0];
}

// Generic constraint
function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
    return obj[key];
}

const user: User = { id: 1, name: "Alice", email: "alice@example.com" };
const name2 = getProperty(user, "name");  // string — type inferred!

// Type guards
function isString(value: unknown): value is string {
    return typeof value === "string";
}

// Utility types
type ReadonlyUser = Readonly<User>;         // All fields readonly
type PartialUser = Partial<User>;           // All fields optional
type UserPreview = Pick<User, "id" | "name">;  // Only id and name
type WithoutEmail = Omit<User, "email">;    // All except email
```

### Interview talking points
- `unknown` is safer than `any` — you must narrow the type before using it.
- `never` is the type of things that never happen (unreachable code, empty unions).
- Generics let you write type-safe code without sacrificing flexibility.
- Utility types (`Partial`, `Required`, `Pick`, `Omit`, `Record`) are must-knows.
- `interface` vs `type`: `interface` can be extended and merged; `type` can represent unions/intersections.

---

## 6.3 Error Handling Best Practices

```javascript
// Custom error classes
class AppError extends Error {
    constructor(message, statusCode = 500, code = "INTERNAL_ERROR") {
        super(message);
        this.name = this.constructor.name;
        this.statusCode = statusCode;
        this.code = code;
        Error.captureStackTrace(this, this.constructor);
    }
}

class ValidationError extends AppError {
    constructor(message, field) {
        super(message, 400, "VALIDATION_ERROR");
        this.field = field;
    }
}

class NotFoundError extends AppError {
    constructor(resource, id) {
        super(`${resource} with id ${id} not found`, 404, "NOT_FOUND");
    }
}

// Usage
try {
    throw new ValidationError("Invalid email", "email");
} catch (err) {
    if (err instanceof ValidationError) {
        console.log(`Validation failed on field: ${err.field}`);
    } else if (err instanceof AppError) {
        console.log(`App error: ${err.code}`);
    } else {
        throw err;  // Re-throw unknown errors
    }
}

// Async error handling
async function fetchData(url) {
    const response = await fetch(url);
    if (!response.ok) {
        throw new AppError(`HTTP ${response.status}`, response.status);
    }
    return response.json();
}
```

---

# INTERVIEW QUESTIONS AND ANSWERS

## JavaScript Interview Q&A

**Q1: What is the output of this code? Explain why.**
```javascript
console.log(typeof null);
console.log(null instanceof Object);
```
**A**: `typeof null` outputs `"object"` — this is a historical bug in JavaScript that cannot be fixed for backward compatibility. `null instanceof Object` outputs `false` because `instanceof` checks the prototype chain, and `null` has no prototype chain.

---

**Q2: Explain the difference between `==` and `===`.**
**A**: `===` (strict equality) compares both value AND type with no coercion. `==` (loose equality) coerces types before comparing. Example: `0 == "0"` is `true` but `0 === "0"` is `false`. Always prefer `===`.

---

**Q3: What will this output? Why?**
```javascript
const arr = [1, 2, 3];
const result = arr.map(parseInt);
console.log(result);
```
**A**: `[1, NaN, NaN]`. Because `map` passes three arguments to its callback: `(element, index, array)`. So `parseInt(1, 0, arr)` → `1`, `parseInt(2, 1, arr)` → `NaN` (base 1 is invalid), `parseInt(3, 2, arr)` → `NaN` (3 is not a valid base-2 digit).

---

**Q4: What is event delegation and why is it useful?**
**A**: Event delegation is attaching a single event listener to a parent element instead of many listeners to child elements. It works because events bubble up the DOM. It's useful for: performance (one listener vs hundreds), and dynamic elements that don't exist yet when the listener is attached.
```javascript
document.getElementById("list").addEventListener("click", (e) => {
    if (e.target.tagName === "LI") console.log(e.target.textContent);
});
```

---

**Q5: Explain the difference between `call`, `apply`, and `bind`.**
**A**: All three set `this` explicitly. `call(thisArg, arg1, arg2)` invokes the function immediately with individual args. `apply(thisArg, [arg1, arg2])` invokes immediately with an array of args. `bind(thisArg, arg1)` returns a new function with `this` permanently bound — doesn't invoke immediately.

---

**Q6: What is a Promise and how does it differ from a callback?**
**A**: A Promise is an object representing a future value. It avoids callback hell through chaining (`.then().then()`), provides centralized error handling (`.catch()`), and is composable (`Promise.all`). Callbacks are simpler but lead to deeply nested code, error handling must be repeated everywhere, and they can't be easily composed.

---

**Q7: What is the difference between `undefined` and `null`?**
**A**: `undefined` means a variable has been declared but not assigned a value — it's the engine's default. `null` is an intentional absence of value — it's set explicitly by developers. `typeof undefined` is `"undefined"`, `typeof null` is `"object"` (historical bug). `undefined == null` is `true` but `undefined === null` is `false`.

---

**Q8: What does `Object.freeze()` do and is it deep?**
**A**: `Object.freeze()` prevents adding, deleting, or modifying properties of an object. It is **shallow** — nested objects are not frozen. For deep immutability, you need a recursive freeze function or a library like Immer.

---

**Q9: Explain closures and give a practical use case.**
**A**: A closure is a function that retains access to its outer scope's variables even after the outer function has returned. Practical use: creating private state (counter with private count), factory functions (multiplier(2) returns a doubler), and memoization (caching results in a closure-based cache).

---

**Q10: What is the difference between `var`, `let`, and `const`?**
**A**: `var` is function-scoped, hoisted (initialized to `undefined`), and can be re-declared. `let` is block-scoped, in the TDZ until declaration, can be reassigned but not re-declared. `const` is block-scoped, in the TDZ, cannot be reassigned or re-declared (but the value itself can be mutated if it's an object/array). Best practice: use `const` by default, `let` when reassignment is needed, avoid `var`.