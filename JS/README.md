class = function constructor

---

Object categories:
- Host objects:
  - BOM
  - DOM
- Built-in objects:
  - Object
  - Math
  - String
  - Boolean
  - Array
  - Date
  - Number
  - ...
- Author Objects
  - All other objects

---

Variables defined in function without keyword is defined globally
NOT Hoisted
```js
function fn() {
  x = 20
  console.log(x); // 20
}
fn();
console.log(x); // 20
```

---

```js
// Function declaration (hoisted)
function greet(name) {
  return "Hello, " + name;
}

// Function expression (not hoisted)
const greet2 = function(name) {
  return "Hello, " + name;
};

// All functions return undefined by default if no return statement
function doNothing() {}
console.log(doNothing()); // undefined
```

---

### The `arguments` Object

> **Gap filled:** Every regular function (not arrow functions) automatically has access to a special object called `arguments`. It holds all values passed in during the call — even if you didn't declare parameters.

```js
function showArgs() {
  console.log(arguments);       // Arguments [1, 2, 3]
  console.log(arguments.length); // 3
  console.log(arguments[0]);    // 1
}
showArgs(1, 2, 3);
```

It's array-like but not a real array — it has `length` and index access, but no `.forEach()` or `.map()`. It comes from the function's execution context automatically.


### Shadowing

```js
var name = "global";

function greet() {
  var name = "local"; // shadows the global name
  console.log(name);  // "local"
}

greet();
console.log(name);    // "global" — unaffected
```

### Hoisting

The JS engine scans the file first and moves `var` declarations and function definitions to the top of their scope **before** any code runs.

```js
console.log(x);    // undefined — not an error, because var x is hoisted
var x = 5;
console.log(x);    // 5

greet();           // works! function declarations are fully hoisted
function greet() {
  console.log("hi");
}

// But expressions are NOT fully hoisted:
sayBye();          // TypeError: sayBye is not a function
var sayBye = function() { console.log("bye"); };
// var sayBye is hoisted (value = undefined), but the function isn't
```

### `throw` and Error Handling

```js
function divide(a, b) {
  if (b === 0) throw new Error("Can't divide by zero");
  return a / b;
}

try {
  divide(10, 0);
} catch (e) {
  console.log(e.message); // "Can't divide by zero"
}
```

Uncaught `throw` stops the program immediately.

---

### Closure

A **closure** is when an inner function "remembers" the variables of its outer function even after the outer function has finished executing.

```js
function makeCounter() {
  let count = 0;           // lives in makeCounter's scope
  return function() {
    count++;               // inner function closes over count
    return count;
  };
}

const counter = makeCounter(); // outer function done, but count is kept alive
console.log(counter()); // 1
console.log(counter()); // 2
console.log(counter()); // 3
```

**Why does `count` survive?** The inner function holds a reference to the lexical environment where it was created. The garbage collector won't delete `count` because something still references it.

### IIFE — Immediately Invoked Function Expression

An anonymous function that runs the moment it's defined. Used to avoid polluting the global scope — variables declared inside stay inside.

```js
(function() {
  let secret = "only I know this";
  console.log("I ran immediately");
})();

console.log(secret); // ReferenceError — not accessible outside
```

### Callback Functions

A **callback** is a function passed as an argument to another function, which then calls it.

```js
function myForEach(arr, callback) {
  for (var i = 0; i < arr.length; i++) {
    callback(arr[i], i, arr); // passes element, index, array
  }
}

myForEach([10, 20, 30], function(item, index) {
  console.log(index + ": " + item);
});
// 0: 10
// 1: 20
// 2: 30
```

This is how the built-in `Array.prototype.forEach()` works under the hood.
