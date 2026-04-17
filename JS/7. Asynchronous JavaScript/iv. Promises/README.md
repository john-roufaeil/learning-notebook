# Promises
Promises were introduced to solve callback hell. 

> A Promise represents a value that may be available now, later, or never.

## Promise States
A Promise is always in one of three states:
- **Pending**: Initial state, neither fulfilled nor rejected
- **Fulfilled**: Operation completed successfully
- **Rejected**: Operation failed

```js
// Creating a Promise
const promise = new Promise((resolve, reject) => {  
  // Async operation  
  setTimeout(() => {  
    const success = true;

    if (success) {
      resolve('Operation succeeded!');
    } else {
      reject(new Error('Operation failed!'));
    }
  }, 1000);
});

// Consuming a Promise  
promise  
  .then(result => {  
    console.log(result); // 'Operation succeeded!'  
    return result.toUpperCase();  
  })  
  .then(upperResult => {  
    console.log(upperResult); // 'OPERATION SUCCEEDED!'  
  })  
  .catch(error => {  
    console.error('Error:', error);  
  })  
  .finally(() => {  
    console.log('Cleanup code runs regardless of outcome');  
  });
```

## Promise Chaining
```js
// Solving callback hell with promises
function getUser(userId) {  
  return new Promise((resolve, reject) => {  
    setTimeout(() => {  
      resolve({ id: userId, name: 'John' });  
    }, 100);  
  });  
}

function getOrders(userId) {  
  return new Promise((resolve, reject) => {  
    setTimeout(() => {  
      resolve([{ id: 1, total: 100 }]);  
    }, 100);  
  });  
}

function getOrderDetails(orderId) {  
  return new Promise((resolve, reject) => {  
    setTimeout(() => {  
      resolve({ id: orderId, items: ['item1', 'item2'] });  
    }, 100);  
  });  
}

// Clean, linear flow  
getUser(1)  
  .then(user => {  
    console.log('User:', user);  
    return getOrders(user.id);  
  })  
  .then(orders => {  
    console.log('Orders:', orders);  
    return getOrderDetails(orders[0].id);  
  })  
  .then(details => {  
    console.log('Details:', details);  
  })  
  .catch(error => {  
    console.error('Error at any step:', error);  
  });

// Key insight: Each .then() returns a new Promise  
// This allows chaining
```

## Promise Methods
```js
// Promise.all - Wait for all promises (parallel)
const promise1 = Promise.resolve(3);  
const promise2 = new Promise(resolve => setTimeout(() => resolve(42), 100));  
const promise3 = Promise.resolve('foo');

Promise.all([promise1, promise2, promise3])  
  .then(values => {  
    console.log(values); // [3, 42, 'foo']  
  })  
  .catch(error => {  
    // If ANY promise rejects, catch is called  
    console.error(error);  
  });

// Promise.allSettled - Wait for all, regardless of outcome  
Promise.allSettled([  
  Promise.resolve(1),  
  Promise.reject('error'),  
  Promise.resolve(3)  
]).then(results => {  
  console.log(results);  
  // [  
  //   { status: 'fulfilled', value: 1 },  
  //   { status: 'rejected', reason: 'error' },  
  //   { status: 'fulfilled', value: 3 }  
  // ]  
});

// Promise.race - First to settle wins  
Promise.race([  
  new Promise(resolve => setTimeout(() => resolve('slow'), 500)),  
  new Promise(resolve => setTimeout(() => resolve('fast'), 100))  
]).then(result => {  
  console.log(result); // 'fast'  
});

// Promise.any - First to fulfill wins (ignores rejections)  
Promise.any([  
  Promise.reject('error1'),  
  new Promise(resolve => setTimeout(() => resolve('success'), 100)),  
  Promise.reject('error2')  
]).then(result => {  
  console.log(result); // 'success'  
}).catch(error => {  
  // Only if ALL promises reject  
  console.error(error); // AggregateError  
});

// Practical example: Timeout pattern  
function withTimeout(promise, ms) {  
  const timeout = new Promise((_, reject) => {  
    setTimeout(() => reject(new Error('Timeout')), ms);  
  });

  return Promise.race([promise, timeout]);
}

withTimeout(fetch('/api/data'), 5000)  
  .then(response => response.json())  
  .catch(error => {  
    if (error.message === 'Timeout') {  
      console.log('Request timed out');  
    } else {  
      console.log('Request failed:', error);  
    }  
  });
```

## Promise Patterns and Edge Cases
```js
// Pattern 1: Promisifying callbacks
function promisify(fn) {  
  return function(...args) {  
    return new Promise((resolve, reject) => {  
      fn(...args, (error, result) => {  
        if (error) {  
          reject(error);  
        } else {  
          resolve(result);  
        }  
      });  
    });  
  };  
}

// Usage  
const readFileAsync = promisify(readFile);  
readFileAsync('test.txt')  
  .then(data => console.log(data))  
  .catch(error => console.error(error));

// Pattern 2: Retry logic  
function retry(fn, maxAttempts = 3, delay = 1000) {  
  return new Promise((resolve, reject) => {  
    let attempts = 0;

    function attempt() {
      attempts++;

      fn()
        .then(resolve)
        .catch(error => {
          if (attempts >= maxAttempts) {
            reject(error);
          } else {
            setTimeout(attempt, delay);
          }
        });
    }

    attempt();
  });
}

retry(() => fetch('/api/data'))  
  .then(response => response.json())  
  .catch(error => console.error('Failed after retries:', error));

// Pattern 3: Sequential execution  
function sequential(promises) {  
  return promises.reduce((chain, promise) => {  
    return chain.then(results => {  
      return promise().then(result => [...results, result]);  
    });  
  }, Promise.resolve([]));  
}

sequential([  
  () => Promise.resolve(1),  
  () => Promise.resolve(2),  
  () => Promise.resolve(3)  
]).then(results => {  
  console.log(results); // [1, 2, 3]  
});

// Edge case 1: Returning a value vs returning a Promise  
Promise.resolve(1)  
  .then(x => x + 1) // Returns value  
  .then(x => Promise.resolve(x + 1)) // Returns Promise  
  .then(x => x + 1) // Both work the same!  
  .then(console.log); // 4

// Edge case 2: Forgetting to return  
Promise.resolve(1)  
  .then(x => {  
    x + 1; // Forgot to return!  
  })  
  .then(x => {  
    console.log(x); // undefined  
  });

// Edge case 3: Promise constructor executes immediately  
console.log('Before');  
const p = new Promise((resolve) => {  
  console.log('Inside Promise'); // Runs immediately!  
  resolve();  
});  
console.log('After');  
// Output: Before, Inside Promise, After

// Edge case 4: Unhandled rejections  
const p = Promise.reject('error');  
// If no .catch(), this creates an unhandled rejection warning

// Always handle rejections:  
p.catch(error => console.error(error));

// Or use global handler:  
window.addEventListener('unhandledrejection', event => {  
  console.error('Unhandled rejection:', event.reason);  
});

// Edge case 5: Promise.resolve with a Promise  
const p1 = Promise.resolve(42);  
const p2 = Promise.resolve(p1);  
console.log(p1 === p2); // true (returns same promise!)

// Edge case 6: Thenable objects  
const thenable = {  
  then(resolve, reject) {  
    resolve(42);  
  }  
};

Promise.resolve(thenable).then(value => {  
  console.log(value); // 42 (thenable is treated like a Promise)  
});
```