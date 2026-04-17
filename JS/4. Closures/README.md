# Closures

> A **closure** is a function that accesses variables in its outer (enclosing) lexical scope, **after** the outer function has returned.

> Every function in JS is a closure. Functions always have access to their lexical scope, regardless of where or when they're executed.

```js
function outer() {
  const message = 'Hello from outer';

  function inner() {
    console.log(message); // Accesses outer's variable
  }

  return inner;
}

const myFunction = outer(); // outer() has finished executing  
myFunction(); // 'Hello from outer' (still works!)

// How? inner() "closes over" the variable 'message'  
// The scope chain is preserved even after outer() returns
```


```html
Step 1: outer() is called
- Creates execution context  
- Declares 'message' in its scope  
- Declares 'inner' function  
- inner's [[Scope]] property points to outer's scope

Step 2: outer() returns inner  
- outer's execution context is popped from call stack  
- BUT: outer's scope is NOT garbage collected  
- Why? Because inner still references it

Step 3: myFunction() is called (which is inner)  
- Creates new execution context for inner  
- Looks for 'message' in scope chain  
- Finds it in outer's preserved scope  
- Logs 'Hello from outer'
```

Memory Leak Warning: Closures can cause memory leaks if you're not careful. Common scenarios:
- Event listeners that aren't removed
- Timers (setTimeout/setInterval) that aren't cleared
- Circular references in closures