# Execution Context & Call Stack

> Understand how JS executes code.

## What is an Execution Context?
- An abstract idea of the enviornment where JS code is evaluated (craetion) & executed.

- Think: a container holding all info needed to execute a piece of code.

- Has three types:
  - **Global Execution Context (GEC):** 
   Created when your JS file first runs. 
    There's only one GEC per program. 
    Creates the global object (```window``` in browsers, ```global``` in Node.js) and sets ```this``` to reference it.
  - **Function Execution Context (FEC):**
    Created when a function is invoked.
    Each fn gets its own FEC.
  - **Eval Execution Context (EEC):**
    Code executed inside eval() - rarely used

- Each EC has three important components:
  1. **Variable Environemnt:** stores variable & fn declarations.
  2. **Lexical Environment:** similar to VE & outer env reference (scope chain)
  3. **this Binding:** determines what ``this`` refers to

## Call Stack

> LIFO DS tracks execution contexts.

```js
function first() {second()}
function second() {third()}
function third() {}
```
```html
1: Global Execution Context created
[Global EC]

2: first() is called
[first() EC]
[Global EC]

3: second() is called from within first()  
[second() EC]
[first() EC]
[Global EC]

4: third() is called from within second()  
[third() EC]
[second() EC]
[first() EC]
[Global EC]

5: third() completes and is popped off  
[second() EC]
[first() EC]
[Global EC]

6: second() completes and is popped off 
[first() EC]
[Global EC]

7: first() completes and is popped off 
[Global EC]

```

> Call Stack Size Exceeded = you've created too many ECs.

- Each browser/engine has a limit (10k-15k calls)
- Call stack is single-threaded
---

## Execution Phases

Each **Execution Context** is *created* in two phases:

### 1. Creation Phase (Memory Allocation)
- JS creates Variable Env & Lexical Env
- JS sets up scope chain
- JS allocates memory for vars and fns
- JS initializes any `var` to `undefined`
- JS stores function declarations in memory
- JS determines the value of `this`

### 2. Execution Phase
- JS executes code line by line
- JS assigns values to variables
- JS executes function calls

```js
EXAMPLE

console.log(x); // undefined
console.log(y); // Reference Error: Cannot access 'y' before initialization
console.log(greet); // [Function: greet]

var x = 10;
let y = 20;
function greet() {return 'Hello';}
```
```html
During Creation Phase:
- x is allocated and initalized to undefined
- y is allocated but NOT initialized (TDZ)
- greet is fully stored in memory

During Execution Phase:
- x gets assigned to 10
- y gets assigned to 20
```

> The creation phase behavior causes "hoisting" - variables and functions appear to be moved to the top of their scope. It's NOT about moving code; it's about HOW the *creation phase* **allocates memory**.