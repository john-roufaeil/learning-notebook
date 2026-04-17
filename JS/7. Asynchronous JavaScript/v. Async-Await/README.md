# Async/Await
Async/await is syntactic sugar over Promises, making asynchronous code look and behave more like synchronous code.

```js
// Promise version
function fetchUserData() {  
  return getUser(1)  
    .then(user => {  
      return getOrders(user.id);  
    })  
    .then(orders => {  
      return getOrderDetails(orders[0].id);  
    })  
    .then(details => {  
      return details;  
    })  
    .catch(error => {  
      console.error(error);  
      throw error;  
    });  
}

// Async/await version (much cleaner!)  
async function fetchUserData() {  
  try {  
    const user = await getUser(1);  
    const orders = await getOrders(user.id);  
    const details = await getOrderDetails(orders[0].id);  
    return details;  
  } catch (error) {  
    console.error(error);  
    throw error;  
  }  
}

// Key points:  
// 1. async function always returns a Promise  
// 2. await pauses execution until Promise resolves  
// 3. Can use try/catch for error handling  
// 4. Code reads top-to-bottom like synchronous code
```

## Async/Await Patterns
```js
// Pattern 1: Parallel execution with Promise.all
async function fetchAllData() {  
  // Sequential (slow - 300ms total)  
  const user = await getUser(1);      // 100ms  
  const posts = await getPosts(1);    // 100ms  
  const comments = await getComments(1); // 100ms

  return { user, posts, comments };
}

async function fetchAllDataParallel() {  
  // Parallel (fast - 100ms total)  
  const [user, posts, comments] = await Promise.all([  
    getUser(1),  
    getPosts(1),  
    getComments(1)  
  ]);

  return { user, posts, comments };
}

// Pattern 2: Sequential with dependency  
async function processOrder(orderId) {  
  const order = await getOrder(orderId);  
  const user = await getUser(order.userId); // Depends on order  
  const payment = await processPayment(order.total); // Depends on order

  return { order, user, payment };
}

// Pattern 3: Conditional await  
async function getData(useCache) {  
  if (useCache) {  
    return getCachedData(); // Returns immediately if cached  
  }

  return await fetchFreshData(); // Only awaits if needed
}

// Pattern 4: Async iteration  
async function processItems(items) {  
  for (const item of items) {  
    await processItem(item); // Sequential  
  }  
}

async function processItemsParallel(items) {  
  await Promise.all(items.map(item => processItem(item))); // Parallel  
}

// Pattern 5: Async map  
async function asyncMap(array, asyncFn) {  
  const promises = array.map(asyncFn);  
  return await Promise.all(promises);  
}

const userIds = [1, 2, 3, 4, 5];  
const users = await asyncMap(userIds, id => getUser(id));

// Pattern 6: Async reduce (sequential)  
async function asyncReduce(array, asyncFn, initialValue) {  
  let accumulator = initialValue;

  for (const item of array) {
    accumulator = await asyncFn(accumulator, item);
  }

  return accumulator;
}

const total = await asyncReduce(  
  [1, 2, 3],  
  async (sum, n) => sum + n,  
  0  
);
```

## Async/Await Edge Cases
```js
// Edge case 1: Top-level await (ES2022)
// In modules, you can use await at the top level  
const data = await fetch('/api/data');  
// No need to wrap in async function

// Edge case 2: Async function always returns a Promise  
async function getValue() {  
  return 42; // Automatically wrapped in Promise.resolve(42)  
}

getValue().then(value => console.log(value)); // 42

async function getPromise() {  
  return Promise.resolve(42);  
}

getPromise().then(value => console.log(value)); // 42 (not nested)

// Edge case 3: Throwing in async function  
async function throwError() {  
  throw new Error('Oops'); // Automatically becomes Promise.reject()  
}

throwError().catch(error => console.error(error));

// Edge case 4: Await non-Promise values  
async function test() {  
  const value = await 42; // Works! Wraps in Promise.resolve()  
  console.log(value); // 42  
}

// Edge case 5: Forgetting await  
async function forgotAwait() {  
  const promise = getUser(1); // Forgot await!  
  console.log(promise); // Promise object, not the value

  const user = await getUser(1); // Correct
  console.log(user); // Actual user object
}

// Edge case 6: Async IIFE  
(async () => {  
  const data = await fetchData();  
  console.log(data);  
})();

// Edge case 7: Async arrow functions  
const fetchData = async () => {  
  return await getData();  
};

const processData = async (data) => {  
  return await process(data);  
};

// Edge case 8: Async methods  
class DataService {  
  async fetchUser(id) {  
    return await getUser(id);  
  }

  async fetchAll() {
    const [users, posts] = await Promise.all([
      this.fetchUsers(),
      this.fetchPosts()
    ]);
    return { users, posts };
  }
}

// Edge case 9: Async constructors (NOT ALLOWED)  
class MyClass {  
  constructor() {  
    // Can't use await here!  
    // this.data = await fetchData(); // SyntaxError  
  }

  // Solution: Use static factory method
  static async create() {
    const instance = new MyClass();
    instance.data = await fetchData();
    return instance;
  }
}

const obj = await MyClass.create();

// Edge case 10: Async event handlers  
button.addEventListener('click', async (event) => {  
  const data = await fetchData();  
  updateUI(data);  
});

// Warning: Event handler doesn't wait for async function to complete  
// If you need to prevent default, do it synchronously:  
button.addEventListener('click', async (event) => {  
  event.preventDefault(); // Do this first!  
  const data = await fetchData();  
  updateUI(data);  
});
```