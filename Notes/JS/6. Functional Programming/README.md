# Functional Programming

Functional programming (FP) is a programming paradigm that treats computation as the evaluation of mathematical functions and avoids changing state and mutable data. JavaScript is a multi-paradigm language.

## First-Class Functions

In JavaScript, functions are first-class citizens, meaning they can be:
- Assigned to variables
- Passed as arguments to other functions
- Returned from functions
- Stored in data structures


```js
// 1. Assigned to variables
const greet = function(name) {  
  return `Hello, ${name}`;  
};

const sayHi = greet; // Functions are values  
console.log(sayHi('John')); // 'Hello, John'

// 2. Passed as arguments  
function executeFunction(fn, value) {  
  return fn(value);  
}

console.log(executeFunction(greet, 'Jane')); // 'Hello, Jane'

// 3. Returned from functions  
function createMultiplier(factor) {  
  return function(number) {  
    return number * factor;  
  };  
}

const double = createMultiplier(2);  
const triple = createMultiplier(3);

console.log(double(5)); // 10  
console.log(triple(5)); // 15

// 4. Stored in data structures  
const operations = {  
  add: (a, b) => a + b,  
  subtract: (a, b) => a - b,  
  multiply: (a, b) => a * b,  
  divide: (a, b) => a / b  
};

console.log(operations.add(10, 5)); // 15  
console.log(operations.multiply(10, 5)); // 50

const mathFunctions = [  
  Math.abs,  
  Math.floor,  
  Math.ceil,  
  Math.round  
];

console.log(mathFunctions[0](-5)); // 5  
console.log(mathFunctions[1](4.7)); // 4
```

> 🔗 Connecting to Closures: First-class functions combined with closures (Section 4) enable powerful patterns like the factory functions we just saw. The returned function "remembers" the factor parameter through closure.

---

## Pure vs Impure Functions
Understanding the difference between pure and impure functions is fundamental to functional programming.

### Pure Functions
A pure function is a function that:

1. Given the same inputs, always returns the same output (deterministic)
2. Has no side effects (doesn't modify external state)

```js
// Pure function examples
function add(a, b) {  
  return a + b;  
}
console.log(add(2, 3)); // Always 5
function getFullName(firstName, lastName) {  
  return `${firstName} ${lastName}`;  
}

// Pure function with objects (doesn't mutate input)  
function addProperty(obj, key, value) {  
  return {  
    ...obj,  
    [key]: value  
  };  
}

const original = { name: 'John' };  
const updated = addProperty(original, 'age', 30);  
console.log(original); // { name: 'John' } (unchanged)  
console.log(updated); // { name: 'John', age: 30 }

// Pure function with arrays  
function addItem(array, item) {  
  return [...array, item];  
}

const arr1 = [1, 2, 3];  
const arr2 = addItem(arr1, 4);  
console.log(arr1); // [1, 2, 3] (unchanged)  
console.log(arr2); // [1, 2, 3, 4]
```

### Impure Functions
An impure function is a function that:
1. May return different outputs for the same inputs
2. Has side effects (modifies external state, I/O, etc.)

```js
// Impure: Depends on external state
let counter = 0;

function increment() {  
  counter++; // Modifies external state  
  return counter;  
}

console.log(increment()); // 1  
console.log(increment()); // 2 (different output, same input!)

// Impure: Mutates input  
function addPropertyImpure(obj, key, value) {  
  obj[key] = value; // Mutates the input  
  return obj;  
}

const obj = { name: 'John' };  
addPropertyImpure(obj, 'age', 30);  
console.log(obj); // { name: 'John', age: 30 } (mutated!)

// Impure: Non-deterministic  
function getCurrentTime() {  
  return new Date(); // Different output each call  
}

function getRandomNumber() {  
  return Math.random(); // Different output each call  
}

// Impure: I/O operations  
function logMessage(message) {  
  console.log(message); // Side effect: console output  
}

function fetchData(url) {  
  return fetch(url); // Side effect: network request  
}

// Impure: DOM manipulation  
function updateDOM(elementId, text) {  
  document.getElementById(elementId).textContent = text; // Side effect  
}
```

### Why Pure Functions Matter
```js
// Benefits of pure functions:

// 1. Testability - Easy to test, no setup needed  
function add(a, b) {  
  return a + b;  
}

// Test is simple:  
console.assert(add(2, 3) === 5);  
console.assert(add(-1, 1) === 0);

// Compare with impure function:  
let total = 0;  
function addToTotal(value) {  
  total += value;  
  return total;  
}

// Test requires setup and cleanup:  
total = 0; // Setup  
console.assert(addToTotal(5) === 5);  
total = 0; // Reset for next test  
console.assert(addToTotal(10) === 10);

// 2. Predictability - No surprises  
const result1 = add(5, 10);  
const result2 = add(5, 10);  
// result1 === result2 (always!)

// 3. Cacheable (Memoization)  
const cache = {};

function expensiveCalculation(n) {  
  if (cache[n]) {  
    return cache[n];  
  }

  // Expensive operation
  const result = n * n;
  cache[n] = result;
  return result;
}

// 4. Parallelizable - Safe to run concurrently  
const numbers = [1, 2, 3, 4, 5];

// Pure function can be safely parallelized  
const doubled = numbers.map(n => n * 2);

// 5. Easier to reason about  
function calculateTotal(items) {  
  return items.reduce((sum, item) => sum + item.price, 0);  
}
// You know exactly what this does without checking external state
```

### Making Impure Functions Pure
```js
// Impure: Modifies global state
let count = 0;  
function incrementImpure() {  
  count++;  
  return count;  
}

// Pure: Returns new state  
function incrementPure(count) {  
  return count + 1;  
}

// Usage:  
let state = 0;  
state = incrementPure(state); // 1  
state = incrementPure(state); // 2

// Impure: Mutates array  
function addItemImpure(array, item) {  
  array.push(item);  
  return array;  
}

// Pure: Returns new array  
function addItemPure(array, item) {  
  return [...array, item];  
}

// Impure: Mutates object  
function updateUserImpure(user, updates) {  
  Object.assign(user, updates);  
  return user;  
}

// Pure: Returns new object  
function updateUserPure(user, updates) {  
  return { ...user, ...updates };  
}

// Impure: Depends on external data  
const taxRate = 0.08;  
function calculateTotalImpure(price) {  
  return price * (1 + taxRate);  
}

// Pure: Takes all dependencies as parameters  
function calculateTotalPure(price, taxRate) {  
  return price * (1 + taxRate);  
}

console.log(calculateTotalPure(100, 0.08)); // 108
```

> Reality Check: Not all functions can or should be pure. I/O operations, API calls, DOM manipulation, and logging are inherently impure. The goal is to isolate impure code and keep as much of your logic pure as possible.

## Higher-Order Functions
A higher-order function is a function that either:

1. Takes one or more functions as arguments, OR
2. Returns a function as its result

### Built-in higher order functions
```js
// Array.prototype.map - transforms each element
const numbers = [1, 2, 3, 4, 5];  
const doubled = numbers.map(n => n * 2);  
console.log(doubled); // [2, 4, 6, 8, 10]

const users = [  
  { name: 'John', age: 30 },  
  { name: 'Jane', age: 25 }  
];  
const names = users.map(user => user.name);  
console.log(names); // ['John', 'Jane']

// Array.prototype.filter - selects elements  
const evens = numbers.filter(n => n % 2 === 0);  
console.log(evens); // [2, 4]

const adults = users.filter(user => user.age >= 18);  
console.log(adults); // Both users

// Array.prototype.reduce - accumulates a value  
const sum = numbers.reduce((acc, n) => acc + n, 0);  
console.log(sum); // 15

const totalAge = users.reduce((acc, user) => acc + user.age, 0);  
console.log(totalAge); // 55

// Array.prototype.forEach - executes for each element  
numbers.forEach(n => console.log(n * 2));

// Array.prototype.find - finds first matching element  
const firstEven = numbers.find(n => n % 2 === 0);  
console.log(firstEven); // 2

// Array.prototype.some - tests if any element matches  
const hasEven = numbers.some(n => n % 2 === 0);  
console.log(hasEven); // true

// Array.prototype.every - tests if all elements match  
const allPositive = numbers.every(n => n > 0);  
console.log(allPositive); // true

// Array.prototype.sort - sorts with comparator  
const sorted = [...numbers].sort((a, b) => b - a);  
console.log(sorted); // [5, 4, 3, 2, 1]
```

## Currying
Currying is the technique of converting a function that takes multiple arguments into a sequence of functions that each take a single argument.

> In JavaScript, this is primarily achieved using closures to "remember" previous arguments as you provide new ones.

```js
// Regular function
function add(a, b, c) {  
  return a + b + c;  
}

console.log(add(1, 2, 3)); // 6

// Curried version  
function addCurried(a) {  
  return function(b) {  
    return function(c) {  
      return a + b + c;  
    };  
  };  
}

console.log(addCurried(1)(2)(3)); // 6

// With arrow functions (cleaner)  
const addCurriedArrow = a => b => c => a + b + c;

console.log(addCurriedArrow(1)(2)(3)); // 6

// Partial application  
const add1 = addCurriedArrow(1);  
const add1and2 = add1(2);  
console.log(add1and2(3)); // 6  
console.log(add1and2(10)); // 13
```

## Function Composition
Function composition is the process of combining two or more functions to produce a new function. It's like a pipeline where the output of one function becomes the input of the next.

```js
// Manual composition
const add1 = x => x + 1;  
const double = x => x * 2;  
const square = x => x * x;

// Nested calls (hard to read)  
const result = square(double(add1(3)));  
console.log(result); // ((3 + 1) * 2)² = 64

// Compose function (right to left)  
function compose(...fns) {  
  return function(value) {  
    return fns.reduceRight((acc, fn) => fn(acc), value);  
  };  
}

const compute = compose(square, double, add1);  
console.log(compute(3)); // 64

// Pipe function (left to right - more intuitive)  
function pipe(...fns) {  
  return function(value) {  
    return fns.reduce((acc, fn) => fn(acc), value);  
  };  
}

const process = pipe(add1, double, square);  
console.log(process(3)); // 64

// Step by step:  
// 3 → add1 → 4  
// 4 → double → 8  
// 8 → square → 64
```

Examples
```js
// Example 1: Data transformation pipeline
const users = [  
  { name: 'john doe', age: 30, active: true },  
  { name: 'jane smith', age: 25, active: false },  
  { name: 'bob jones', age: 35, active: true }  
];

const capitalize = str => str.charAt(0).toUpperCase() + str.slice(1);  
const getWords = str => str.split(' ');  
const capitalizeWords = words => words.map(capitalize);  
const joinWords = words => words.join(' ');

const titleCase = pipe(getWords, capitalizeWords, joinWords);

const formattedUsers = users.map(user => ({  
  ...user,  
  name: titleCase(user.name)  
}));

console.log(formattedUsers);  
// [{ name: 'John Doe', ... }, { name: 'Jane Smith', ... }, ...]

// Example 2: Validation pipeline  
const trim = str => str.trim();  
const toLowerCase = str => str.toLowerCase();  
const removeSpaces = str => str.replace(/\s/g, '');

const normalizeEmail = pipe(trim, toLowerCase, removeSpaces);

console.log(normalizeEmail('  John@Example.COM  ')); // 'john@example.com'

// Example 3: Number processing  
const parseNumber = str => parseFloat(str);  
const roundToTwo = num => Math.round(num * 100) / 100;  
const formatCurrency = num => `$${num.toFixed(2)}`;

const processPrice = pipe(parseNumber, roundToTwo, formatCurrency);

console.log(processPrice('19.999')); // '$20.00'

// Example 4: Array operations  
const filterActive = users => users.filter(u => u.active);  
const sortByAge = users => [...users].sort((a, b) => a.age - b.age);  
const getNames = users => users.map(u => u.name);

const getActiveUserNamesSorted = pipe(filterActive, sortByAge, getNames);

console.log(getActiveUserNamesSorted(users));  
// ['john doe', 'bob jones']
```

> Connecting Concepts: Function composition leverages:
> - First-class functions - Functions as values
> - Pure functions - Predictable transformations
> - Higher-order functions - Functions that return functions
> - Currying - Single-argument functions for easy composition
> - These concepts work together to create powerful, reusable code.

## Memoization
Memoization is an optimization technique that caches the results of expensive function calls and returns the cached result when the same inputs occur again.

```js
// Without memoization - slow for large numbers
function fibonacci(n) {  
  if (n <= 1) return n;  
  return fibonacci(n - 1) + fibonacci(n - 2);  
}
fibonacci(40); // Takes several seconds! 

// With memoization - much faster  
function memoize(fn) {  
  const cache = {};

  return function(...args) {
    const key = JSON.stringify(args);

    if (key in cache) {
      console.log('Cache hit for:', args);
      return cache[key];
    }

    console.log('Computing for:', args);
    const result = fn(...args);
    cache[key] = result;
    return result;
  };
}

const fibonacciMemo = memoize(function fib(n) {  
  if (n <= 1) return n;  
  return fibonacciMemo(n - 1) + fibonacciMemo(n - 2);  
});

fibonacciMemo(40); // Much faster!
```

### When to Use
```js
// Good candidates for memoization:

// 1. Expensive computations  
const factorial = memoize(function fact(n) {  
  if (n <= 1) return 1;  
  return n * factorial(n - 1);  
});

// 2. Recursive functions  
const fibonacci = memoize(function fib(n) {  
  if (n <= 1) return n;  
  return fibonacci(n - 1) + fibonacci(n - 2);  
});

// 3. API calls with same parameters  
const getUserData = memoize(async (userId) => {  
  const response = await fetch(`/api/users/${userId}`);

  return response.json();
});

// 4. Pure functions with deterministic output  
const complexCalculation = memoize((x, y, z) => {  
  // Expensive operation  
  return Math.pow(x, y) + Math.sqrt(z);  
});

// 5. Frequently called functions with repeated inputs  
const getFullName = memoize((firstName, lastName) => {  
  return `${firstName} ${lastName}`.trim();  
});

// Bad candidates for memoization:

// 1. Functions with side effects  
const logAndReturn = (x) => {  
  console.log(x); // Side effect!  
  return x * 2;  
};  
// Don't memoize - you want the side effect every time

// 2. Functions that return different values for same input  
const getRandomNumber = () => Math.random();  
const getCurrentTime = () => new Date();  
// Don't memoize - defeats the purpose

// 3. Functions called with unique inputs every time  
const processUniqueId = (id) => {  
  return `processed-${id}`;  
};  
// If every ID is unique, cache never hits

// 4. Functions with large or complex return values  
const generateLargeDataset = (size) => {  
  return new Array(size).fill(0).map((_, i) => ({ id: i, data: '...' }));  
};  
// Memory cost may outweigh computation savings
```

> Connection to Next Topic: Memoization is particularly powerful with asynchronous operations. As we move into async JavaScript, you'll see how memoizing API calls and async functions can dramatically improve performance. The event loop and promise mechanics we'll cover next determine when memoized async functions actually execute.