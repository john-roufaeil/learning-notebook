# Event Loop

The event loop is the mechanism that allows JavaScript to perform **non-blocking** operations despite being **single-threaded**. It's the heart of JavaScript's asynchronous behavior.

## The Components
JavaScript's runtime consists of several key components:

- **Call Stack:** Where function execution contexts are pushed and popped (we covered this in Section 1)
- **Web APIs:** Browser-provided APIs (setTimeout, fetch, DOM events, etc.)
- **Callback Queue (Task Queue):** Where callbacks from async operations wait
- **Microtask Queue:** Priority queue for promises and mutation observers
- **Event Loop:** Monitors the call stack and queues, moving tasks when appropriate

---

## How It Works
```js
console.log('Start');

setTimeout(() => {  
  console.log('Timeout');  
}, 0);

Promise.resolve().then(() => {  
  console.log('Promise');  
});

console.log('End');

// Output:  
// Start  
// End  
// Promise  
// Timeout

// Why this order? Let's trace through the event loop:

// 1. 'Start' is logged (synchronous)  
// 2. setTimeout is called:  
//    - Timer is set in Web API  
//    - Callback is registered  
//    - Execution continues (non-blocking)  
// 3. Promise.resolve() is called:  
//    - Promise resolves immediately  
//    - .then() callback goes to microtask queue  
// 4. 'End' is logged (synchronous)  
// 5. Call stack is now empty  
// 6. Event loop checks microtask queue first  
//    - Executes Promise callback: logs 'Promise'  
// 7. Event loop checks callback queue  
//    - Executes setTimeout callback: logs 'Timeout'
```

## Detailed Event Loop Visualization
```js
// Complex example showing event loop behavior
console.log('1: Start');

setTimeout(() => {  
  console.log('2: setTimeout 1');  
  Promise.resolve().then(() => {  
    console.log('3: Promise inside setTimeout');  
  });  
}, 0);

Promise.resolve()  
  .then(() => {  
    console.log('4: Promise 1');  
    setTimeout(() => {  
      console.log('5: setTimeout inside Promise');  
    }, 0);  
  })  
  .then(() => {  
    console.log('6: Promise 2');  
  });

setTimeout(() => {  
  console.log('7: setTimeout 2');  
}, 0);

console.log('8: End');

// Output:  
// 1: Start  
// 8: End  
// 4: Promise 1  
// 6: Promise 2  
// 2: setTimeout 1  
// 3: Promise inside setTimeout  
// 7: setTimeout 2  
// 5: setTimeout inside Promise

// Step-by-step execution:  
//  
// Synchronous code runs first:  
// - Log '1: Start'  
// - Register setTimeout 1 (goes to Web API)  
// - Register Promise 1 (microtask queue)  
// - Register setTimeout 2 (goes to Web API)  
// - Log '8: End'  
//  
// Call stack empty, check microtask queue:  
// - Execute Promise 1: log '4: Promise 1', register setTimeout 3  
// - Execute Promise 2: log '6: Promise 2'  
//  
// Microtask queue empty, check callback queue:  
// - Execute setTimeout 1: log '2: setTimeout 1', register Promise 3 (microtask)  
//  
// Check microtask queue again (before next callback):  
// - Execute Promise 3: log '3: Promise inside setTimeout'  
//  
// Continue with callback queue:  
// - Execute setTimeout 2: log '7: setTimeout 2'  
// - Execute setTimeout 3: log '5: setTimeout inside Promise'
```

> Key Rule: The event loop always processes ALL microtasks before moving to the next callback (macrotask). This is why promises always execute before setTimeout, even with setTimeout(..., 0).