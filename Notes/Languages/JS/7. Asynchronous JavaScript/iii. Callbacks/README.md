# Callbacks
Callbacks are the foundation of asynchronous JavaScript.

> A callback is a **function** **passed** to another function to be executed *later*.

```js
// Simple callback example
function fetchData(callback) {  
  setTimeout(() => {  
    const data = { id: 1, name: 'John' };  
    callback(data);  
  }, 1000);  
}

fetchData((data) => {  
  console.log('Received:', data);  
});

// Error-first callback pattern (Node.js convention)  
function readFile(filename, callback) {  
  setTimeout(() => {  
    const error = null; // or new Error('File not found')  
    const data = 'file contents';  
    callback(error, data);  
  }, 1000);  
}

readFile('test.txt', (error, data) => {  
  if (error) {  
    console.error('Error:', error);  
    return;  
  }  
  console.log('Data:', data);  
});
```

## Callback Hell (Pyramid of Doom)
```js
// The problem with callbacks: nested async operations
getUser(userId, (error, user) => {  
  if (error) {  
    handleError(error);  
    return;  
  }

  getOrders(user.id, (error, orders) => {
    if (error) {
      handleError(error);
      return;
    }

    getOrderDetails(orders[0].id, (error, details) => {
      if (error) {
        handleError(error);
        return;
      }

      processPayment(details.total, (error, receipt) => {
        if (error) {
          handleError(error);
          return;
        }

        sendConfirmation(receipt, (error, result) => {
          if (error) {
            handleError(error);
            return;
          }

          console.log('Success!', result);
        });
      });
    });
  });
});

// Problems:  
// 1. Hard to read (deeply nested)  
// 2. Error handling is repetitive  
// 3. Hard to reason about flow  
// 4. Difficult to add error recovery  
// 5. Can't use try/catch
```

## Callback Patterns
```js
// Pattern 1: Named functions (reduces nesting)
function handleUser(error, user) {  
  if (error) return handleError(error);  
  getOrders(user.id, handleOrders);  
}

function handleOrders(error, orders) {  
  if (error) return handleError(error);  
  getOrderDetails(orders[0].id, handleDetails);  
}

function handleDetails(error, details) {  
  if (error) return handleError(error);  
  processPayment(details.total, handlePayment);  
}

getUser(userId, handleUser);

// Pattern 2: Callback wrapper for error handling  
function asyncWrapper(fn) {  
  return function(error, data) {  
    if (error) {  
      handleError(error);  
      return;  
    }  
    fn(data);  
  };  
}

getUser(userId, asyncWrapper((user) => {  
  getOrders(user.id, asyncWrapper((orders) => {  
    // Still nested, but cleaner error handling  
  }));  
}));

// Pattern 3: Parallel callbacks  
function parallel(tasks, callback) {  
  const results = [];  
  let completed = 0;

  tasks.forEach((task, index) => {
    task((error, result) => {
      if (error) {
        callback(error);
        return;
      }

      results[index] = result;
      completed++;

      if (completed === tasks.length) {
        callback(null, results);
      }
    });
  });
}

parallel([  
  (cb) => setTimeout(() => cb(null, 'Task 1'), 100),  
  (cb) => setTimeout(() => cb(null, 'Task 2'), 200),  
  (cb) => setTimeout(() => cb(null, 'Task 3'), 50)  
], (error, results) => {  
  console.log(results); // ['Task 1', 'Task 2', 'Task 3']  
});

// Pattern 4: Sequential callbacks  
function series(tasks, callback) {  
  let index = 0;  
  const results = [];

  function next() {
    if (index >= tasks.length) {
      callback(null, results);
      return;
    }

    tasks[index]((error, result) => {
      if (error) {
        callback(error);
        return;
      }

      results.push(result);
      index++;
      next();
    });
  }

  next();
}

series([  
  (cb) => setTimeout(() => cb(null, 'First'), 100),  
  (cb) => setTimeout(() => cb(null, 'Second'), 50),  
  (cb) => setTimeout(() => cb(null, 'Third'), 75)  
], (error, results) => {  
  console.log(results); // ['First', 'Second', 'Third'] (in order)  
});
```