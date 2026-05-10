# ES6+ Modern Syntax

> ES6 (ECMAScript 2015) and subsequent versions introduced powerful syntactic features that make JavaScript more expressive and less error-prone. These features build on the foundational concepts we've covered—they're not just "syntactic sugar," but tools that leverage scope, closures, and execution context in meaningful ways.

## Arrow Functions
Arrow functions provide a concise syntax and have different this binding behavior:

<details>
<summary> details </summary>

```js
// Traditional function
function add(a, b) {  
  return a + b;  
}

// Arrow function  
const add = (a, b) => a + b;

// Variations:  
const square = x => x * x; // Single parameter, no parentheses  
const greet = () => 'Hello'; // No parameters  
const makeObject = () => ({ name: 'John' }); // Return object (needs parentheses to mark that {} = obj not a code block)  
const multiLine = (x, y) => {  
  const sum = x + y;  
  return sum * 2;  
}; // Multiple statements need braces and explicit return
```

### Binding Difference
```js
// Traditional function: 'this' depends on how it's called
const obj1 = {  
  name: 'Object 1',  
  greet: function() {  
    console.log(`Hello from ${this.name}`);  
  }  
};

obj1.greet(); // 'Hello from Object 1'  
const greetFn = obj1.greet;  
greetFn(); // 'Hello from undefined' (this is lost!)

// Arrow function: 'this' is lexically bound  
const obj2 = {  
  name: 'Object 2',  
  greet: () => {  
    console.log(`Hello from ${this.name}`);  
  }  
};

obj2.greet(); // 'Hello from undefined' (this is from outer scope!)

// Practical use case: Callbacks  
class Timer {  
  constructor() {  
    this.seconds = 0;  
  }

  startTraditional() {
    setInterval(function() {
      this.seconds++; // Error! 'this' is not Timer instance
      console.log(this.seconds);
    }, 1000);
  }

  startWithArrow() {
    setInterval(() => {
      this.seconds++; // Works! 'this' is lexically bound to Timer
      console.log(this.seconds);
    }, 1000);
  }

  startWithBind() {
    setInterval(function() {
      this.seconds++;
      console.log(this.seconds);
    }.bind(this), 1000); // Old way: explicitly bind 'this'
  }
}

const timer = new Timer();  
timer.startWithArrow(); // Works correctly
```

</details>

## Destructuring
Destructuring allows you to extract values from arrays or properties from objects into distinct variables:

<details>
<summary> details </summary>

### Array Destructuring
```js
// Basic array destructuring
const [first, second, third] = [1, 2, 3];  
console.log(first, second, third); // 1 2 3

// Skipping elements  
const [a, , c] = [1, 2, 3];  
console.log(a, c); // 1 3

// Rest pattern  
const [head, ...tail] = [1, 2, 3, 4, 5];  
console.log(head); // 1  
console.log(tail); // [2, 3, 4, 5]

// Default values  
const [x = 10, y = 20] = [5];  
console.log(x, y); // 5 20

// Swapping variables  
let m = 1, n = 2;  
[m, n] = [n, m];  
console.log(m, n); // 2 1

// Nested destructuring  
const [p, [q, r]] = [1, [2, 3]];  
console.log(p, q, r); // 1 2 3

// Function return values  
function getCoordinates() {  
  return [10, 20];  
}  
const [x, y] = getCoordinates();

// Ignoring return values  
const [, , third] = getCoordinates(); // Only want third value
````

### Object Destructuring
```// Basic object destructuring
const person = { name: 'John', age: 30, city: 'NYC' };  
const { name, age, city } = person;  
console.log(name, age, city); // 'John' 30 'NYC'

// Renaming variables  
const { name: personName, age: personAge } = person;  
console.log(personName, personAge); // 'John' 30

// Default values  
const { name, country = 'USA' } = person;  
console.log(country); // 'USA'

// Rest pattern  
const { name, ...rest } = person;  
console.log(rest); // { age: 30, city: 'NYC' }

// Nested destructuring  
const user = {  
  id: 1,  
  info: {  
    name: 'John',  
    address: {  
      city: 'NYC',  
      zip: '10001'  
    }  
  }  
};

const {  
  info: {  
    name,  
    address: { city, zip }  
  }  
} = user;  
console.log(name, city, zip); // 'John' 'NYC' '10001'

// Function parameters  
function greet({ name, age }) {  
  console.log(`${name} is ${age} years old`);  
}  
greet(person); // 'John is 30 years old'

// With defaults in parameters  
function createUser({ name = 'Anonymous', role = 'user' } = {}) {  
  return { name, role };  
}  
console.log(createUser()); // { name: 'Anonymous', role: 'user' }  
console.log(createUser({ name: 'John' })); // { name: 'John', role: 'user' }
```

### Destructuring Edge Cases
```js
// Destructuring null or undefined throws error
const { x } = null; // TypeError  
const { y } = undefined; // TypeError

// But you can provide defaults  
const { x } = null || {}; // No error, x is undefined

// Computed property names  
const key = 'name';  
const { [key]: value } = { name: 'John' };  
console.log(value); // 'John'

// Destructuring in loops  
const users = [  
  { name: 'John', age: 30 },  
  { name: 'Jane', age: 25 }  
];

for (const { name, age } of users) {  
  console.log(`${name} is ${age}`);  
}

// Mixed destructuring  
const data = {  
  id: 1,  
  items: [10, 20, 30]  
};

const {  
  id,  
  items: [first, ...restItems]  
} = data;

console.log(id, first, restItems); // 1 10 [20, 30]
```

</details>

## Spread & Rest Operator
The `...` operator has two uses: spread (expanding) and rest (collecting).

<details>
<summary> details </summary>

### Spread

```js
// Array spread
const arr1 = [1, 2, 3];  
const arr2 = [4, 5, 6];  
const combined = [...arr1, ...arr2];  
console.log(combined); // [1, 2, 3, 4, 5, 6]

// Copying arrays (shallow copy)  
const original = [1, 2, 3];  
const copy = [...original];  
copy.push(4);  
console.log(original); // [1, 2, 3] (unchanged)  
console.log(copy); // [1, 2, 3, 4]

// Object spread  
const obj1 = { a: 1, b: 2 };  
const obj2 = { c: 3, d: 4 };  
const merged = { ...obj1, ...obj2 };  
console.log(merged); // { a: 1, b: 2, c: 3, d: 4 }

// Overriding properties  
const defaults = { theme: 'light', lang: 'en' };  
const userPrefs = { theme: 'dark' };  
const config = { ...defaults, ...userPrefs };  
console.log(config); // { theme: 'dark', lang: 'en' }

// Function arguments  
function sum(a, b, c) {  
  return a + b + c;  
}  
const numbers = [1, 2, 3];  
console.log(sum(...numbers)); // 6

// String spread  
const str = 'hello';  
const chars = [...str];  
console.log(chars); // ['h', 'e', 'l', 'l', 'o']

// Spread in array literals  
const arr = [1, ...[2, 3], 4, ...[5, 6]];  
console.log(arr); // [1, 2, 3, 4, 5, 6]
```

### Rest Parameters
```js
// Collecting function arguments
function sum(...numbers) {  
  return numbers.reduce((acc, n) => acc + n, 0);  
}  
console.log(sum(1, 2, 3, 4, 5)); // 15

// Rest must be last parameter  
function greet(greeting, ...names) {  
  return `${greeting}, ${names.join(' and ')}!`;  
}  
console.log(greet('Hello', 'John', 'Jane', 'Bob')); // 'Hello, John and Jane and Bob!'

// Rest in destructuring (already covered above)  
const [first, ...rest] = [1, 2, 3, 4];  
const { a, ...others } = { a: 1, b: 2, c: 3 };
```

### Spread vs Rest: The Difference
```js
// Spread: Expands an iterable into individual elements  
const numbers = [1, 2, 3];  
console.log(...numbers); // 1 2 3 (three separate arguments)

// Rest: Collects individual elements into an array  
function collect(...args) {  
  console.log(args); // [1, 2, 3] (one array)  
}  
collect(1, 2, 3);

// Context determines which one:  
// - Right side of assignment = Spread  
// - Left side of assignment = Rest  
// - Function parameters = Rest  
// - Function arguments = Spread

const arr = [...[1, 2, 3]]; // Spread  
const [a, ...b] = [1, 2, 3]; // Rest

function fn(...params) {} // Rest  
fn(...[1, 2, 3]); // Spread
```
### Shallow Copy Behavior (Important!)
```js
// Spread creates SHALLOW copies
const original = {  
  name: 'John',  
  address: {  
    city: 'NYC',  
    zip: '10001'  
  }  
};

const copy = { ...original };

// Top-level properties are copied  
copy.name = 'Jane';  
console.log(original.name); // 'John' (unchanged)

// But nested objects are referenced!  
copy.address.city = 'LA';  
console.log(original.address.city); // 'LA' (changed!)

// Same with arrays  
const arr1 = [[1, 2], [3, 4]];  
const arr2 = [...arr1];

arr2[0][0] = 99;  
console.log(arr1[0][0]); // 99 (changed!)

// For deep copy, you need different approaches (covered later)
```
### Advanced Spread Patterns
```js
// Conditional spreading
const baseConfig = { theme: 'light' };  
const isDarkMode = true;

const config = {  
  ...baseConfig,  
  ...(isDarkMode && { theme: 'dark' })  
};  
console.log(config); // { theme: 'dark' }

// Removing properties  
const user = { id: 1, name: 'John', password: 'secret' };  
const { password, ...publicUser } = user;  
console.log(publicUser); // { id: 1, name: 'John' }

// Merging with priority  
const defaults = { a: 1, b: 2, c: 3 };  
const overrides = { b: 20, c: 30 };  
const final = { ...defaults, ...overrides };  
console.log(final); // { a: 1, b: 20, c: 30 }

// Array deduplication with Set and spread  
const numbers = [1, 2, 2, 3, 3, 3, 4];  
const unique = [...new Set(numbers)];  
console.log(unique); // [1, 2, 3, 4]

// Converting NodeList to Array  
const divs = document.querySelectorAll('div');  
const divArray = [...divs]; // Now you can use array methods

// Combining multiple arrays  
const arr1 = [1, 2];  
const arr2 = [3, 4];  
const arr3 = [5, 6];  
const combined = [...arr1, ...arr2, ...arr3];  
console.log(combined); // [1, 2, 3, 4, 5, 6]
```
</details>

## Optional Chaining

Optional chaining `?.` allows you to safely access nested properties without having to explicitly check if each reference in the chain is valid. This is one of the most practical additions to modern JavaScript.
<details>
<summary> details </summary>

```js
// Without optional chaining (the old way)
const user = {  
  name: 'John',  
  address: {  
    city: 'NYC'  
  }  
};

// Accessing nested properties safely required verbose checks  
let city;  
if (user && user.address && user.address.city) {  
  city = user.address.city;  
}

// Or using ternary operators  
const zip = user && user.address && user.address.zip;

// With optional chaining (the new way)  
const city = user?.address?.city; // 'NYC'  
const zip = user?.address?.zip; // undefined (no error!)  
const country = user?.address?.country?.code; // undefined (no error!)

// If the value before ?. is null or undefined,
// the expression short-circuits and returns undefined

// But it doesn't protect against other falsy values  
const obj3 = 0;  
console.log(obj3?.toString()); // '0' (0 is not null/undefined)
const obj4 = '';  
console.log(obj4?.length); // 0 (empty string is not null/undefined)

// Calling methods that might not exist  
console.log(user.greet?.()); // 'Hello, John'  
console.log(user.farewell?.()); // undefined (no error)
```

### Edge Cases & Gotchas
```js
// 1. Optional chaining only checks null/undefined
const obj = { value: 0 };  
console.log(obj?.value); // 0 (not undefined!)

const obj2 = { value: false };  
console.log(obj2?.value); // false (not undefined!)

const obj3 = { value: '' };  
console.log(obj3?.value); // '' (not undefined!)

// 2. Can't use on the left side of assignment  
const obj = {};  
obj?.property = 'value'; // SyntaxError!

// 3. Short-circuiting behavior  
let count = 0;  
const obj = null;  
obj?.[count++]; // count is still 0 (expression never evaluated)  
console.log(count); // 0

// Compare with:  
const obj2 = {};  
obj2?.[count++]; // count is incremented  
console.log(count); // 1

// 4. Combining with nullish coalescing  
const user = null;  
const name = user?.name ?? 'Anonymous';  
console.log(name); // 'Anonymous'

// 5. Delete with optional chaining  
const obj = { a: 1 };  
delete obj?.a; // Works  
delete obj?.b; // No error (b doesn't exist)

// 6. Parentheses matter  
const obj = { a: { b: 1 } };  
console.log((obj?.a).b); // 1  
console.log(obj?.a.b); // 1 (same result)

const obj2 = null;  
console.log((obj2?.a).b); // TypeError! (undefined).b  
console.log(obj2?.a.b); // undefined (short-circuits)
```
</details>

## Nullish Coalescing
The nullish coalescing operator `??` returns the right-hand operand when the left-hand operand is null or undefined, and otherwise returns the left-hand operand.

> Common Mistake: Don't confuse ?? with ||. Use ?? when you want to preserve falsy values like 0, '', or false. Use || only when you want to treat all falsy values as "missing."

<details>
<summary>details</summary>

```js
--- The Problem with OR (||)
// The old way: using || for defaults
function greet(name) {  
  name = name || 'Guest';  
  return `Hello, ${name}`;  
}

console.log(greet('John')); // 'Hello, John'  
console.log(greet()); // 'Hello, Guest'

// But || has a problem with falsy values:  
console.log(greet('')); // 'Hello, Guest' (empty string is falsy!)  
console.log(greet(0)); // 'Hello, Guest' (0 is falsy!)  
console.log(greet(false)); // 'Hello, Guest' (false is falsy!)

// These might be valid values you want to keep!

--- Nullish Coalescing to the Rescue
// ?? only treats null and undefined as "nullish"
function greet(name) {  
  name = name ?? 'Guest';  
  return `Hello, ${name}`;  
}

console.log(greet('John')); // 'Hello, John'  
console.log(greet()); // 'Hello, Guest'  
console.log(greet(null)); // 'Hello, Guest'  
console.log(greet(undefined)); // 'Hello, Guest'

// But these falsy values are preserved:  
console.log(greet('')); // 'Hello, ' (empty string kept!)  
console.log(greet(0)); // 'Hello, 0' (0 kept!)  
console.log(greet(false)); // 'Hello, false' (false kept!)

--- Comparison: || vs ??
// Falsy values in JavaScript:
// false, 0, -0, 0n, '', null, undefined, NaN

const value1 = 0;  
console.log(value1 || 100); // 100 (0 is falsy)  
console.log(value1 ?? 100); // 0 (0 is not nullish)

const value2 = '';  
console.log(value2 || 'default'); // 'default' ('' is falsy)  
console.log(value2 ?? 'default'); // '' ('' is not nullish)

const value3 = false;  
console.log(value3 || true); // true (false is falsy)  
console.log(value3 ?? true); // false (false is not nullish)

const value4 = null;  
console.log(value4 || 'default'); // 'default' (null is falsy)  
console.log(value4 ?? 'default'); // 'default' (null is nullish)

const value5 = undefined;  
console.log(value5 || 'default'); // 'default' (undefined is falsy)  
console.log(value5 ?? 'default'); // 'default' (undefined is nullish)

--- Chaining and Combining
// Chaining ?? operators
const value = null ?? undefined ?? 'default' ?? 'fallback';  
console.log(value); // 'default' (first non-nullish value)

// Combining with optional chaining  
const user = {  
  preferences: {  
    theme: null  
  }  
};

const theme = user?.preferences?.theme ?? 'light';  
console.log(theme); // 'light'

// Multiple levels  
const config = {  
  user: {  
    settings: {  
      notifications: {  
        email: false // false is a valid setting  
      }  
    }  
  }  
};

const emailNotifs = config?.user?.settings?.notifications?.email ?? true;  
console.log(emailNotifs); // false (preserved!)

// With ||, it would be true (wrong!)
```
</details>

## Template Literals

Template literals (template strings) provide an elegant way to create strings with embedded expressions, multi-line strings, and even tagged templates for advanced string processing.

<details>
<summary> details </summary>

```js
// Old way: String concatenation
const name = 'John';  
const age = 30;  
const message = 'Hello, my name is ' + name + ' and I am ' + age + ' years old.';

// New way: Template literals  
const message = `Hello, my name is ${name} and I am ${age} years old.`;

// Any expression works inside ${}
const a = 10;
const b = 20;
console.log(`The sum is ${a + b}`); // 'The sum is 30'
console.log(`Double: ${a * 2}`); // 'Double: 20'
console.log(`Is adult: ${age >= 18}`); // 'Is adult: true'
```
</details>

# Static/Dynamic

To explain this clearly, we have to look at the **Creation Phase** (static/compile-time) and the **Execution Phase** (dynamic/runtime). 

Here is how the JavaScript engine treats your code in both phases, specifically regarding the `this` keyword and the scope.

<details>
<summary> details </summary>

### 1. Static Phase: Creation & Lexical Scoping
This happens **before** the code actually runs. The engine "scans" the code to map out the relationships.

* **Scoping:** The engine notes that the arrow function is nested inside a parent context. Because it’s an arrow function, it is "marked" to never create its own `this`.
* **Binding:** It establishes the **Lexical Environment**. It determines that if `this` is ever called inside the arrow function, it must point to the `this` of the nearest parent function or the global scope.
* **Memory Allocation:** Space is carved out in the **Heap** for the function definitions.

### 2. Dynamic Phase: Execution & Dispatch
This happens **while** the code is running line-by-line.

* **Call Stack:** When the parent function is called, its **Execution Context** is pushed onto the stack, and its `this` is determined by the caller.
* **Context Inheritance:** When the arrow function starts running, it doesn't do a "lookup" to see how it was called. It simply reaches into the static map created in the first phase and uses the existing `this` value from the parent's context.

---

### Comparison of Phases

| Feature | Static (Creation Phase) | Dynamic (Execution Phase) |
| :--- | :--- | :--- |
| **Normal Function `this`** | Placeholder created. | Bound based on **who called it**. |
| **Arrow Function `this`** | Fixed to the **Parent's Scope**. | Inherited from the **Parent's Context**. |
| **Scope Chain** | Formed based on where code is written. | Traversed to find variables/`this`. |

### When Not to Use Arrow Fns
```js
// 1. Object methods (when you need dynamic 'this')
const person = {  
  name: 'John',  
  greet: () => {  
    console.log(`Hello, ${this.name}`); // 'this' is not person!  
  }  
};

// 2. Prototype methods  
function Person(name) {  
  this.name = name;  
}  
Person.prototype.greet = () => {  
  console.log(this.name); // 'this' is not the instance!  
};

// 3. Event handlers (when you need 'this' to be the element)  
button.addEventListener('click', () => {  
  console.log(this); // 'this' is not the button!  
});

// 4. Functions that need 'arguments' object  
const sum = () => {  
  console.log(arguments); // ReferenceError: arguments is not defined  
};

// Use rest parameters instead:  
const sum = (...numbers) => {  
  return numbers.reduce((acc, n) => acc + n, 0);  
};

//  Assignment with ??=  
let x = null;  
x ??= 10; // Assigns only if x is nullish  
console.log(x); // 10
```

</details>


**Key Takeaway:** * **Statically**, the Arrow function decides "I am my parent's child" regardless of how you use it later.
* **Dynamically**, the Regular function decides "I belong to whoever is calling me right now."