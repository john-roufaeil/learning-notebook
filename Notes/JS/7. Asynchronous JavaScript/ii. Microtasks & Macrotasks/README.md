# Microtasks & Macrotasks
Understanding the difference between microtasks and macrotasks is essential for predicting async execution order.

**Macrotasks (Tasks)** include:
- setTimeout
- setInterval
- setImmediate (Node.js)
- I/O operations
- UI rendering
- requestAnimationFrame

**Microtasks include**:
- Promise callbacks (.then, .catch, .finally)
- queueMicrotask()
- MutationObserver
- process.nextTick (Node.js - even higher priority than promises)

```js
// Demonstrating the difference
console.log('Script start');

setTimeout(() => {  
  console.log('setTimeout');  
}, 0);

Promise.resolve()  
  .then(() => {  
    console.log('Promise 1');  
  })  
  .then(() => {  
    console.log('Promise 2');  
  });

queueMicrotask(() => {  
  console.log('queueMicrotask');  
});

console.log('Script end');

// Output:  
// Script start  
// Script end  
// Promise 1  
// queueMicrotask  
// Promise 2  
// setTimeout

// Why?  
// 1. Synchronous code runs: 'Script start', 'Script end'  
// 2. All microtasks run: 'Promise 1', 'queueMicrotask', 'Promise 2'  
// 3. Then macrotask: 'setTimeout'
```

## Event Loop Algorithm
```js
Simplified event loop algorithm:

while (true) {  
    1. Execute all synchronous code  
    executeCallStack();  
 
    2. Process ALL microtasks  
    while (microtaskQueue.length > 0) {  
        task = microtaskQueue.shift();  
        execute(task);  
    }  
 
    3. Render UI (if needed)  
    if (needsRendering) {  
        renderUI();  
    }  
 
    4. Process ONE macrotask  
    if (macrotaskQueue.length > 0) {  
        task = macrotaskQueue.shift();  
        execute(task);  
    }  
 
    Loop continues...  
}

This explains why microtasks can "starve" macrotasks:  
function recursiveMicrotask() {  
  Promise.resolve().then(() => {  
    console.log('Microtask');  
    recursiveMicrotask(); // Creates another microtask  
  });  
}

recursiveMicrotask();

setTimeout(() => {  
  console.log('This will never run!');  
}, 0);

// The microtask keeps creating more microtasks,  
// so the event loop never gets to the setTimeout!
```

## Practical Implications
```js
// Example 1: Breaking up long tasks
function processLargeArray(array) {  
  // Bad: Blocks the UI  
  array.forEach(item => {  
    expensiveOperation(item);  
  });  
}

// Better: Use setTimeout to yield to the event loop  
function processLargeArrayAsync(array, index = 0) {  
  const batchSize = 100;  
  const batch = array.slice(index, index + batchSize);

  batch.forEach(item => {
    expensiveOperation(item);
  });

  if (index + batchSize < array.length) {
    setTimeout(() => {
      processLargeArrayAsync(array, index + batchSize);
    }, 0);
  }
}

// Example 2: Ensuring DOM updates  
button.addEventListener('click', () => {  
  // Update DOM  
  element.textContent = 'Loading...';

  // This runs before the DOM update is visible:
  expensiveOperation();

  // Solution: Use setTimeout to let the browser render
  setTimeout(() => {
    expensiveOperation();
  }, 0);

  // Or use requestAnimationFrame for visual updates
  requestAnimationFrame(() => {
    expensiveOperation();
  });
});

// Example 3: Promise vs setTimeout timing  
async function test() {  
  console.log('1');

  setTimeout(() => console.log('2'), 0);

  await Promise.resolve();
  console.log('3');

  setTimeout(() => console.log('4'), 0);
}

test();  
console.log('5');

// Output: 1, 5, 3, 2, 4  
// Why?  
// - '1' and '5' are synchronous  
// - await creates a microtask, so '3' runs before any setTimeout  
// - '2' and '4' are macrotasks, run in order
```

> Connecting to Call Stack: Remember the call stack from Section 1? The event loop only moves tasks from queues to the call stack when the call stack is empty. This is why synchronous code always completes before any async callbacks run.