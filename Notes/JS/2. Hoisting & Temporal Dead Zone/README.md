# Hoisting & Temporal Dead Zone

> JS's behavior of moving declarations to the top of their scope during the creation phase. But here's the key insight: **nothing actually moves**. Instead, during the creation phase of an execution context, JS **allocates** memory for declarations before executing any code.

> Hoisting is a consequence of the two-phase execution model (creation + execution), not a separate mechanism.

### var Hoisting
Variables declared with `var` are hoisted and initialized to `undefined`:

```js
console.log(name); // undefined
var name = 'John';
console.log(name); // 'John'
```
```html
What JS actually does:
Creation Phase:
- Allocate memory for 'name'
- Initialize 'name' to undefined
Execution Phase:
- Assigns the value 'John' to 'name' in line 2
```

`var` declarations are **function-scoped** (or **globally-scoped** if not in a function)

```js
function testVar() {
  console.log(x); // undefined
  if (true) {
    var x = 10;
    console.log(x); // 10
  }
  console.log(x); // 10 (still accessible!)
}
// var ignores block scope - x is hoisted to function scope
```

---

### let, const, and TDZ
Variables declared with `let` and `const` are also **hoisted** but **not initialized**. They remain in the **Temporal Dead Zone (TDZ)** until their declaration is executed.

```js
console.log(name); // ReferenceError: cannot access 'name' before initialization
let name = 'John';
```
```html
Creation Phase:
- Allocate memory for 'name'
- DO NOT initialize (enters TDZ)
Execution Phase:
- Try to access 'name' -> still in TDZ
- Initialize 'name' to 'John' -> exits TDZ
```

> TDZ exists from the start of the scope until the declaration is encountered

```js
function testTDZ() {
  // TDZ starts here for 'x'
  console.log(typeof x);         // ReferenceError!
  let x = 10;                    // TDZ ends here
  console.log(typeof x):         // number
}

function testVar() {
  console.log(typeof y); // 'undefined' (no error)
  var y = 10;
  console.log(typeof y); // number
}
```

> Even `typeof` throws an error in TDZ;`typeof` undeclared variables = `undefined`

```js
console.log(typeof x); // 'undefined'
var x = 10;
console.log(typeof y); // ReferenceError
let y = 10;
```

#### const Specifics
`const` behaves like `let` regarding hoisiting & TDZ, but with additional restrictions:

```js
1. Must be initialized at declaration
const x; // SyntaxError: missing initializer
const y = 10; // Correct

2. Cannot be reassigned
const z = 20;
z = 30; // TypeError: Assignment to constant variable

3. But objects/arrays can be mutated
const obj = {name: 'John'};
obj.name = 'Jane'; // Allowed!
obj.age = 24; // Allowed!
obj = {}; // TypeError: Assignment to constant variable

const arr = [1, 2, 3];
arr.push(4); // Allowed!
arr = []; // TypeError: Assignment to constant variable
```

---

## Function Hoisting
Function declaration are fully hoisted, both name and implementation

```js
greet(); // 'Hello!' (works)
function greet() { return 'Hello!'; }
```
```html
Creation Phase:
- Store entire function in memory
Execution Phase:
- Function is already available
```

Function expressions behave differently

```js
greet(); // TypeError: greet is not a function
var greet = function() { return 'Hi!' }
```
```html
Creation Phase:
- greet is hoisted and initialzied to undefined
- trying to call undefined() causes TypeError
```
```js
greet(); // ReferenceError: cannot access 'greet' before initialization
const greet = function() { return 'Hi!' }
OR
let greet = function() { return 'Hi!' }
```
```html
greet is in TDZ until this line executes
```

---

> **Hoisting** determins *when* variables are accessible.

> **Scope** determines *where* variables are accessible.

> **Scope** is established during the creation phase & determines how JS resolves variable references.