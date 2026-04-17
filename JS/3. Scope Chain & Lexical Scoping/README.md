# Scope Chain & Lexical Scoping
How JS finds variables when you refeence them

## Types of Scope
Scope determines the accessibility of variables

### 1. Global Scope
```js
const globalVar = 'I am global';
function test() {
  console.log(globalVar); // Accessible
}
test(); // 'I am global'
console.log(globalVar); // 'I am global'
```

### 2. Function Scope
```js
function outer() {
  const functionScoped = 'Only in function';
  console.log(functionScoped); // Accessible
}
outer(); // 'Only in function'
console.log(functionScoped); // ReferenceError: functionScoped is not defined
```

### 3. **Block Scope (ES6+)**
```js 
if (true) {
  let blockScoped = 'Only in block';
  const alsoBlockScoped = 'Also only in block';
  var notBlockScoped = 'I escape!';
  console.log(blockScoped); // Accessible
}
console.log(blockScoped); // ReferenceError
console.log(alsoBlockScoped); // ReferenceError
console.log(notBlockScoped); // 'I escape!' (var ignores block scope)
```
```html
Block scope works with:
- let and const
- for loops
- while loops
- if/else blocks
- try/catch blocks
- switch statements
- standalone blocks { }
```

## Lexical Scoping (static scoping)
> Scope of a variable is determined by its position in the source code. JS uses lexical scoping, ie, inner functions have access to variables in outer functions.

```js
const global = 'global';

function outer() {
  const outerVar = 'outer';
  function inner() {
    const innerVar = 'inner';
    console.log(innerVar); // 'inner' (own scope)
    console.log(outerVar); // 'outer' (parent scope)
    console.log(global); // 'global' (globl scrope)
  }
  console.log(innerVar); // Reference Error (can't access child scope)
}
```

> The key word is **lexical** - scope is determined by where functions are written, not where they're called.

## Scope Chain Resolution
> When JS encounters a variable, it follows the **scope chain** to resolve it:
1. Look in the current scope
2. If not found, look in parent scope
3. Continue up the chain until reaching global scope
4. If still not found, throw a ReferenceError

### Shadowing
> When an **inner scope** declares a variable with the **same name** as an *outer scope vairable*, it **shadows** the outer variable

```js
const x = 'global';
function outer() {
  const x = 'outer';
  function inner() {
    const x = 'inner';
    console.log(x); // 'inner' (shadows both outer & global)
  }
  console.log(x); // 'outer' (shadows global)
}
console.log(x); // 'global'
```
```js
"var allows redeclaration (another reason to avoid it)"
let x = 'outer';
function test() {let x = 'inner';} // OK (different scope)
let y = 10;
let y = 20; // SyntaxError: 'y' already declared
var z = 10;
var z = 20; // No error!
```

---

> **Connecting the Dots:** Each execution context (global scope/function call) has a Lexical Environmnet that contains: 
> - **Environment Record:** stores variables and functions in current scope
> - **Outer Environment Reference:** points to parent scope (creates the scope chain)

> This is how the scope chain is physically implemented in JS!

```js
const global = 'global';
function outer() {
  const outerVar = 'outer';
  function inner() {
    const innerVar = 'inner';
    console.log(innerVar, outerVar, global);
  }
}
```
```html
Scope Chain for inner():
inner's Lexical Environemnt {
  Environment Record: { innerVar: 'inner' }
  Outer Reference: -> outer's Lexical Enviornment
}

outer's Lexical Environment {
  Environment Record: { outerVar: 'outer' }
  Outer Reference: -> Global Lexical Environment
}

Global Lexical Environment {
  Environment Record: { global: 'global, outer: [Function]}
  Outer Reference: null
}
```

---

### Edge Cases & Gotchas

```js
// Case 1: loop variable scope with var
for (var i = 0; i < 3; i++) {
  setTimeout(() => console.log(i), 100);
}
// Output: 3, 3, 3 (al closures reference same 'i')
// They are scheduled to log, and they all reference the same thing

// Case 2: Function parameters create their own scope
function test(x = 10) {
  let x = 20 // SyntaxError: Identified 'x' has already been declared
}

// Case 3: Catch block scope
try {
  throw new Error('oops');
} catch (error) {
  console.log(error); // Accessible
}
console.log(error); // ReferenceError (block-scoped to catch)

// Case 4: Global object pollution with var
var globalVar = 'I pollute';
console.log(window.globalVar); // 'I pollute' (in browser)
let globalLet = 'I do not';
console.log(window.globalLet); // undefined (let doesn't pollute)
```

| Keyword | Stored in JS scope | Attached to `window` |
| ------- | ------------------ | -------------------- |
| `var`   | ✅                  | ✅                    |
| `let`   | ✅                  | ❌                    |
| `const` | ✅                  | ❌                    |

---

> The scope chain is established when functions are **created**, not when they're called.
> This means functions "remember" their scope chain even after their parent function has finished executing.
> This phenomenon is called a **closure**, one of JS's most powerful features.