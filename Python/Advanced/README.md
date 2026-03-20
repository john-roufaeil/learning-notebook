# Python Advanced Topics — Interview Study Guide

> **How to use this guide**: Every topic starts with a plain-English explanation (what it is, why it exists), then builds to real code examples, then closes with the key interview talking points. Assume zero prior knowledge of each topic.

---

# PART 1: METAPROGRAMMING

Metaprogramming means **writing code that manipulates other code**. Instead of writing logic that processes data, you write logic that processes functions, classes, or attributes themselves. Python makes this unusually accessible.

---

## 1.1 Decorators — Functions That Modify Other Functions

### What is it?

A decorator is a function that **takes another function as input and returns a modified version of it**. It's a way to add behavior to a function without changing the function's own code.

Think of it like a gift wrapper: the gift inside doesn't change, but it now comes with a bow on top.

### Why does it exist?

Without decorators, if you wanted to add logging to 10 different functions, you'd have to manually add log lines inside every single one. With a decorator, you write the logging logic once and apply it anywhere with a single `@` symbol.

### The mechanics

```python
# Step 1: A basic decorator
def my_decorator(func):
    def wrapper(*args, **kwargs):
        print("Before the function runs")
        result = func(*args, **kwargs)   # Call the original function
        print("After the function runs")
        return result
    return wrapper

# Step 2: Apply it with @ syntax
@my_decorator
def say_hello(name):
    print(f"Hello, {name}!")

say_hello("Alice")
# Output:
# Before the function runs
# Hello, Alice!
# After the function runs
```

The `@my_decorator` line is just shorthand for `say_hello = my_decorator(say_hello)`.

### Real-world example: Timing decorator

```python
import time
import functools

def timer(func):
    @functools.wraps(func)    # Preserves the original function's name/docstring
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        result = func(*args, **kwargs)
        end = time.perf_counter()
        print(f"{func.__name__} took {end - start:.4f} seconds")
        return result
    return wrapper

@timer
def slow_function():
    time.sleep(1)
    return "done"

slow_function()
# slow_function took 1.0012 seconds
```

### Decorator with arguments (decorator factory)

```python
def repeat(n):
    """Decorator factory — returns a decorator"""
    def decorator(func):
        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            for _ in range(n):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return decorator

@repeat(3)
def greet():
    print("Hi!")

greet()
# Hi!
# Hi!
# Hi!
```

### Stacking decorators

```python
@timer
@repeat(2)
def work():
    time.sleep(0.1)

work()
# work took 0.2003 seconds  (applied bottom-up: repeat first, then timer wraps both)
```

### Class-based decorators

```python
class CountCalls:
    def __init__(self, func):
        functools.update_wrapper(self, func)
        self.func = func
        self.count = 0

    def __call__(self, *args, **kwargs):
        self.count += 1
        print(f"Call #{self.count} to {self.func.__name__}")
        return self.func(*args, **kwargs)

@CountCalls
def add(a, b):
    return a + b

add(1, 2)   # Call #1 to add
add(3, 4)   # Call #2 to add
print(add.count)  # 2
```

### Interview talking points
- Always use `@functools.wraps(func)` inside your wrapper — it preserves `__name__`, `__doc__`, etc.
- Decorators are applied at **definition time**, not at call time.
- The order of stacked decorators matters: they apply bottom-up but wrap outside-in.
- Common built-in decorators: `@property`, `@staticmethod`, `@classmethod`, `@functools.lru_cache`.

---

## 1.2 Metaclasses — Classes That Create Other Classes

### What is it?

In Python, **everything is an object** — including classes. A metaclass is the class that creates class objects. Just as a class is a blueprint for instances, a metaclass is a blueprint for classes.

By default, every class in Python is created by the built-in metaclass `type`.

### The mental model

```
type  →  creates →  MyClass
MyClass  →  creates →  my_instance
```

### Why does it exist?

Metaclasses let you intercept and modify class creation. This is used for:
- Enforcing coding rules across all classes in a hierarchy (e.g., all subclasses must implement certain methods)
- Automatically registering classes (plugin systems)
- Adding attributes or methods to all classes automatically (ORMs like Django models)

### `type` — the default metaclass

```python
# These two are equivalent:
class Dog:
    species = "Canis familiaris"

Dog2 = type("Dog2", (), {"species": "Canis familiaris"})

# type(name, bases, dict)
print(type(Dog))   # <class 'type'>
```

### Creating a custom metaclass

```python
class EnforceMethods(type):
    """Metaclass that ensures all subclasses implement 'validate'"""

    def __new__(mcs, name, bases, namespace):
        # This runs when a new class is being CREATED (not instantiated)
        cls = super().__new__(mcs, name, bases, namespace)

        # Skip enforcement for the base class itself
        if bases:  # If this class has parents (i.e., it's a subclass)
            if "validate" not in namespace:
                raise TypeError(f"Class '{name}' must implement a 'validate' method")

        return cls

class BaseModel(metaclass=EnforceMethods):
    pass

class UserModel(BaseModel):
    def validate(self):
        return True
# Works fine

class BrokenModel(BaseModel):
    pass
# TypeError: Class 'BrokenModel' must implement a 'validate' method
```

### Auto-registering classes (plugin pattern)

```python
class PluginRegistry(type):
    registry = {}

    def __new__(mcs, name, bases, namespace):
        cls = super().__new__(mcs, name, bases, namespace)
        if bases:  # Don't register the base class
            mcs.registry[name] = cls
        return cls

class Plugin(metaclass=PluginRegistry):
    pass

class PDFExporter(Plugin):
    def export(self): return "Exporting PDF"

class CSVExporter(Plugin):
    def export(self): return "Exporting CSV"

print(PluginRegistry.registry)
# {'PDFExporter': <class 'PDFExporter'>, 'CSVExporter': <class 'CSVExporter'>}

# Now you can load any plugin by name:
exporter = PluginRegistry.registry["PDFExporter"]()
print(exporter.export())  # "Exporting PDF"
```

### Interview talking points
- Metaclasses are advanced — in an interview, acknowledge that and explain when they're appropriate.
- The common alternatives to metaclasses are class decorators and `__init_subclass__` (simpler and preferred for most cases).
- Django uses a metaclass in its ORM so that `class User(models.Model)` automatically creates database table mappings.
- `__new__` on a metaclass runs at **class creation time**; `__init__` on a metaclass runs just after.

---

## 1.3 Descriptors — Custom Attribute Access

### What is it?

A descriptor is an object that defines how attribute **access, assignment, and deletion** works on another class. It gives you complete control over what happens when you do `obj.attribute`.

### Why does it exist?

`@property` is a built-in descriptor. But when you need the same logic applied to many attributes across many classes, writing a descriptor is cleaner than writing many properties.

### The protocol

An object becomes a descriptor by implementing one or more of:
- `__get__(self, obj, objtype)` — called on attribute read
- `__set__(self, obj, value)` — called on attribute write
- `__delete__(self, obj)` — called on attribute deletion

```python
class Validated:
    """Descriptor that enforces a value is a positive number"""

    def __set_name__(self, owner, name):
        # Python calls this automatically when the descriptor is assigned to a class attribute
        self.name = name
        self.private_name = f"_{name}"

    def __get__(self, obj, objtype=None):
        if obj is None:
            return self   # Class-level access returns the descriptor itself
        return getattr(obj, self.private_name, 0)

    def __set__(self, obj, value):
        if not isinstance(value, (int, float)):
            raise TypeError(f"{self.name} must be a number, got {type(value)}")
        if value < 0:
            raise ValueError(f"{self.name} must be positive, got {value}")
        setattr(obj, self.private_name, value)


class Product:
    price = Validated()
    stock = Validated()

    def __init__(self, name, price, stock):
        self.name = name
        self.price = price    # Goes through Validated.__set__
        self.stock = stock    # Goes through Validated.__set__


p = Product("Widget", 9.99, 100)
print(p.price)       # 9.99
p.price = 12.50      # Fine
p.price = -5         # ValueError: price must be positive, got -5
p.stock = "lots"     # TypeError: stock must be a number, got <class 'str'>
```

### Interview talking points
- `@property` is actually a descriptor under the hood.
- Data descriptors (define `__set__`) take priority over instance `__dict__`.
- Non-data descriptors (only `__get__`) are overridden by instance `__dict__`.
- Django model fields use descriptors to intercept attribute access on model instances.

---

## 1.4 Dynamic Attribute Access — `__getattr__` and `__setattr__`

### What is it?

These dunder methods let you intercept attribute access and assignment at the instance level.

- `__getattr__(self, name)` — called only when the attribute is **not found** normally
- `__getattribute__(self, name)` — called on **every** attribute access (use carefully)
- `__setattr__(self, name, value)` — called on **every** attribute assignment

### Why does it exist?

Dynamic attribute access is used to build proxy objects, lazy-loading systems, or objects with flexible schemas where you don't know the attribute names ahead of time.

```python
class DynamicConfig:
    """Object where any attribute set is stored in an internal dict"""

    def __init__(self):
        # Use object.__setattr__ to avoid recursion!
        object.__setattr__(self, "_data", {})

    def __setattr__(self, name, value):
        # Intercepts ALL attribute assignments
        self._data[name] = value

    def __getattr__(self, name):
        # Only called when normal lookup fails
        if name in self._data:
            return self._data[name]
        raise AttributeError(f"No config key: '{name}'")

    def __repr__(self):
        return f"DynamicConfig({self._data})"


config = DynamicConfig()
config.host = "localhost"
config.port = 5432
config.debug = True

print(config.host)    # "localhost"
print(config.port)    # 5432
print(config.timeout) # AttributeError: No config key: 'timeout'
```

### Proxy object example

```python
class ReadOnlyProxy:
    """Wraps an object and makes it read-only"""

    def __init__(self, obj):
        object.__setattr__(self, "_obj", obj)

    def __getattr__(self, name):
        return getattr(self._obj, name)

    def __setattr__(self, name, value):
        raise AttributeError("This object is read-only")


class Config:
    host = "localhost"
    port = 8080

c = ReadOnlyProxy(Config())
print(c.host)     # "localhost"
c.host = "prod"   # AttributeError: This object is read-only
```

### Interview talking points
- Never call `self.name` inside `__setattr__` — it causes infinite recursion. Use `object.__setattr__(self, name, value)` instead.
- `__getattr__` is a fallback; `__getattribute__` intercepts everything. Prefer `__getattr__`.
- Used heavily in ORM query builders (e.g., `User.objects.filter(name="Alice")` dynamically building queries).

---

# PART 2: CONCURRENCY, PARALLELISM & ASYNC

These three topics solve the same problem — doing multiple things — but they solve it differently. Understanding *when to use which* is the most important interview skill here.

### The fundamental distinction

| | Threading | Multiprocessing | Asyncio |
|---|---|---|---|
| Best for | I/O-bound tasks | CPU-bound tasks | I/O-bound tasks (many concurrent) |
| Actual parallelism | No (GIL) | Yes | No (single thread) |
| Overhead | Medium | High | Low |
| Complexity | Medium | Medium | High |

### The GIL (Global Interpreter Lock) — why it matters

Python has a GIL — a lock that ensures only one thread executes Python bytecode at a time. This means:
- **Threads** don't achieve true parallelism for CPU-heavy tasks.
- **Threads** are still useful for I/O because threads can release the GIL while waiting for I/O (file reads, network calls).
- **Multiprocessing** bypasses the GIL by using separate processes with separate memory spaces.

---

## 2.1 Threading — I/O-bound Tasks

### What is it?

Threading lets you run multiple functions concurrently within the same process. While one thread waits for a network response, another thread can run.

```python
import threading
import time

def fetch_url(url, results, index):
    """Simulates fetching a URL"""
    print(f"Fetching {url}...")
    time.sleep(1)  # Simulate network delay
    results[index] = f"Response from {url}"
    print(f"Done: {url}")

urls = [
    "https://api.example.com/users",
    "https://api.example.com/orders",
    "https://api.example.com/products",
]

results = [None] * len(urls)
threads = []

start = time.perf_counter()

for i, url in enumerate(urls):
    t = threading.Thread(target=fetch_url, args=(url, results, i))
    threads.append(t)
    t.start()

for t in threads:
    t.join()  # Wait for all threads to complete

end = time.perf_counter()
print(f"Total time: {end - start:.2f}s")  # ~1 second, not 3
print(results)
```

### Thread safety — the problem with shared state

```python
import threading

counter = 0
lock = threading.Lock()

def increment():
    global counter
    for _ in range(100_000):
        with lock:      # Ensures only one thread modifies counter at a time
            counter += 1

threads = [threading.Thread(target=increment) for _ in range(5)]
for t in threads: t.start()
for t in threads: t.join()

print(counter)  # Always 500000 with lock; unpredictable without it
```

### Thread-local storage

```python
import threading

local_data = threading.local()

def process(name):
    local_data.user = name  # Each thread has its own 'user'
    time.sleep(0.1)
    print(f"Thread {threading.current_thread().name}: user = {local_data.user}")

threads = [threading.Thread(target=process, args=(name,), name=name)
           for name in ["Alice", "Bob", "Charlie"]]
for t in threads: t.start()
for t in threads: t.join()
# Each thread prints its own name — no cross-contamination
```

### Interview talking points
- Threads share memory — this is powerful but dangerous (race conditions).
- Always use `Lock`, `RLock`, `Semaphore`, or `Queue` to coordinate shared state.
- The GIL means threads don't speed up CPU-bound code — use `multiprocessing` for that.
- `threading.Event` and `threading.Condition` are for thread signaling.

---

## 2.2 Multiprocessing — CPU-bound Tasks

### What is it?

`multiprocessing` spawns separate Python interpreter processes, each with its own GIL. They run truly in parallel on multiple CPU cores. The tradeoff is higher overhead (process creation, IPC — inter-process communication).

```python
import multiprocessing
import time

def cpu_heavy(n):
    """A CPU-intensive calculation"""
    total = sum(i * i for i in range(n))
    return total

if __name__ == "__main__":  # CRITICAL: required on Windows/macOS
    numbers = [10_000_000, 10_000_000, 10_000_000, 10_000_000]

    # Sequential
    start = time.perf_counter()
    results = [cpu_heavy(n) for n in numbers]
    print(f"Sequential: {time.perf_counter() - start:.2f}s")

    # Parallel
    start = time.perf_counter()
    with multiprocessing.Pool(processes=4) as pool:
        results = pool.map(cpu_heavy, numbers)
    print(f"Parallel: {time.perf_counter() - start:.2f}s")
    # Sequential: ~4s, Parallel: ~1s on a 4-core machine
```

### Sharing state between processes

```python
from multiprocessing import Process, Queue

def producer(q):
    for i in range(5):
        q.put(i)
        print(f"Produced: {i}")
    q.put(None)  # Sentinel to signal done

def consumer(q):
    while True:
        item = q.get()
        if item is None:
            break
        print(f"Consumed: {item}")

if __name__ == "__main__":
    q = Queue()
    p1 = Process(target=producer, args=(q,))
    p2 = Process(target=consumer, args=(q,))
    p1.start(); p2.start()
    p1.join(); p2.join()
```

### Interview talking points
- Always guard `multiprocessing` code with `if __name__ == "__main__":` to prevent recursive spawning.
- `Pool.map` is the most common pattern — distributes a list of inputs across worker processes.
- `Pool.starmap` is for functions with multiple arguments.
- Use `multiprocessing.Manager` for shared dicts/lists (with performance cost).
- Processes don't share memory — inter-process communication (IPC) is the bottleneck.

---

## 2.3 Asyncio — Asynchronous I/O

### What is it?

Asyncio uses a **single thread** with an **event loop** to handle many I/O operations concurrently. Instead of blocking while waiting for I/O, a coroutine "yields" control back to the event loop, which runs other coroutines in the meantime.

Think of it like a waiter in a restaurant: instead of standing at one table waiting for a customer to decide, the waiter takes the order, goes to another table, comes back, etc. One waiter (thread) serves many tables (tasks) concurrently.

### Core concepts

- `async def` — defines a coroutine function
- `await` — pauses the coroutine until the awaited thing completes, yielding control to the event loop
- `asyncio.gather()` — runs multiple coroutines concurrently

```python
import asyncio
import time

async def fetch_data(url, delay):
    """Simulates an async HTTP request"""
    print(f"Starting: {url}")
    await asyncio.sleep(delay)  # Non-blocking sleep — other tasks run here
    print(f"Finished: {url}")
    return f"Data from {url}"

async def main():
    start = time.perf_counter()

    # Run all three concurrently
    results = await asyncio.gather(
        fetch_data("api/users", 1),
        fetch_data("api/orders", 2),
        fetch_data("api/products", 1.5),
    )

    elapsed = time.perf_counter() - start
    print(f"\nAll done in {elapsed:.2f}s")  # ~2 seconds, not 4.5
    print(results)

asyncio.run(main())
```

### Tasks and cancellation

```python
async def slow_task(name, delay):
    try:
        await asyncio.sleep(delay)
        return f"{name} complete"
    except asyncio.CancelledError:
        print(f"{name} was cancelled!")
        raise

async def main():
    task1 = asyncio.create_task(slow_task("Task A", 1))
    task2 = asyncio.create_task(slow_task("Task B", 10))

    result = await task1
    print(result)

    task2.cancel()
    try:
        await task2
    except asyncio.CancelledError:
        print("Task B cancelled successfully")

asyncio.run(main())
```

### Async context managers and iterators

```python
class AsyncDatabaseConn:
    async def __aenter__(self):
        print("Opening async DB connection")
        await asyncio.sleep(0.1)  # Simulate async connection
        return self

    async def __aexit__(self, *args):
        print("Closing async DB connection")
        await asyncio.sleep(0.1)

    async def query(self, sql):
        await asyncio.sleep(0.1)
        return [{"id": 1, "name": "Alice"}]

async def main():
    async with AsyncDatabaseConn() as db:
        rows = await db.query("SELECT * FROM users")
        print(rows)

asyncio.run(main())
```

### Interview talking points
- `async def` makes a function a coroutine — calling it doesn't run it, it returns a coroutine object.
- `await` can only be used inside `async def` functions.
- Asyncio is NOT multi-threaded — it's cooperative multitasking on a single thread.
- The event loop runs one coroutine at a time; `await` is the only yield point.
- `asyncio.gather` runs coroutines concurrently; `asyncio.wait` gives more control over completion.
- Don't mix blocking calls (like `time.sleep`, regular file I/O) in async code — use `asyncio.sleep`, `aiofiles`, `httpx`, etc.
- `asyncio.run()` is the entry point (Python 3.7+).

---

## 2.4 concurrent.futures — High-Level Interface

### What is it?

`concurrent.futures` is Python's high-level, unified API for both threading and multiprocessing. It abstracts away most of the complexity into a simple `Executor` interface.

```python
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, as_completed
import time

def fetch(url):
    time.sleep(1)
    return f"Response from {url}"

def process_heavy(n):
    return sum(i * i for i in range(n))

urls = ["api/users", "api/orders", "api/products"]

# --- ThreadPoolExecutor for I/O-bound ---
with ThreadPoolExecutor(max_workers=3) as executor:
    futures = {executor.submit(fetch, url): url for url in urls}

    for future in as_completed(futures):
        url = futures[future]
        result = future.result()
        print(f"{url}: {result}")

# --- ProcessPoolExecutor for CPU-bound ---
numbers = [5_000_000, 8_000_000, 10_000_000]

if __name__ == "__main__":
    with ProcessPoolExecutor() as executor:
        results = list(executor.map(process_heavy, numbers))
    print(results)
```

### Future object — checking status

```python
from concurrent.futures import ThreadPoolExecutor
import time

def task(n):
    time.sleep(n)
    return n * 2

with ThreadPoolExecutor(max_workers=2) as executor:
    f1 = executor.submit(task, 1)
    f2 = executor.submit(task, 3)

    print(f1.done())   # False — still running
    time.sleep(1.5)
    print(f1.done())   # True — completed
    print(f1.result()) # 2

    # Add callback
    f2.add_done_callback(lambda f: print(f"Done! Result: {f.result()}"))
```

### Interview talking points
- `executor.map(fn, iterable)` is like `map()` but concurrent — returns results in order.
- `as_completed(futures)` yields futures as they finish (order not guaranteed).
- Use `ThreadPoolExecutor` for I/O, `ProcessPoolExecutor` for CPU — same API, different backends.
- A `Future` is a handle to a computation that may not have finished yet.
- Always use as a context manager (`with`) to ensure proper cleanup.

---

# PART 3: ADVANCED OOP PATTERNS

---

## 3.1 Abstract Base Classes (abc module)

### What is it?

An Abstract Base Class (ABC) is a class that **cannot be instantiated** and defines a contract: any subclass must implement the marked abstract methods, or Python raises a `TypeError`.

```python
from abc import ABC, abstractmethod

class PaymentProcessor(ABC):

    @abstractmethod
    def charge(self, amount: float) -> bool:
        """Charge the customer. Must return True on success."""
        pass

    @abstractmethod
    def refund(self, transaction_id: str) -> bool:
        pass

    def process(self, amount: float) -> str:
        """Concrete method — shared by all processors"""
        if self.charge(amount):
            return f"Charged ${amount:.2f} successfully"
        return "Charge failed"


class StripeProcessor(PaymentProcessor):
    def charge(self, amount: float) -> bool:
        print(f"Stripe: charging ${amount:.2f}")
        return True

    def refund(self, transaction_id: str) -> bool:
        print(f"Stripe: refunding {transaction_id}")
        return True


class PayPalProcessor(PaymentProcessor):
    def charge(self, amount: float) -> bool:
        print(f"PayPal: charging ${amount:.2f}")
        return True

    def refund(self, transaction_id: str) -> bool:
        print(f"PayPal: refunding {transaction_id}")
        return True


# Cannot instantiate the ABC:
# p = PaymentProcessor()  # TypeError: Can't instantiate abstract class

# Can instantiate concrete subclasses:
stripe = StripeProcessor()
print(stripe.process(49.99))   # Stripe: charging $49.99 / Charged $49.99 successfully
```

### Abstract properties

```python
class Shape(ABC):
    @property
    @abstractmethod
    def area(self) -> float:
        pass

    @property
    @abstractmethod
    def perimeter(self) -> float:
        pass

class Circle(Shape):
    def __init__(self, radius):
        self.radius = radius

    @property
    def area(self) -> float:
        return 3.14159 * self.radius ** 2

    @property
    def perimeter(self) -> float:
        return 2 * 3.14159 * self.radius

c = Circle(5)
print(c.area)       # 78.53975
print(c.perimeter)  # 31.4159
```

### Interview talking points
- ABCs enforce a contract at **class instantiation time**, not at definition time.
- `@abstractmethod` + `@property` must be ordered as shown (property second in Python 3.3+).
- ABCs can register virtual subclasses: `MyABC.register(SomeClass)` — `isinstance(obj, MyABC)` returns True without inheritance.
- Use ABCs when you want to define a shared interface that all subclasses must fulfill.

---

## 3.2 Data Classes

### What is it?

Data classes (introduced in Python 3.7 via `@dataclass`) automatically generate boilerplate methods like `__init__`, `__repr__`, and `__eq__` based on class-level field declarations.

```python
from dataclasses import dataclass, field
from typing import List

# Without dataclass — painful boilerplate:
class OldPoint:
    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __repr__(self):
        return f"Point(x={self.x}, y={self.y})"
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

# With dataclass — clean:
@dataclass
class Point:
    x: float
    y: float

p1 = Point(1.0, 2.0)
p2 = Point(1.0, 2.0)
print(p1)           # Point(x=1.0, y=2.0)
print(p1 == p2)     # True (auto-generated __eq__)
print(repr(p1))     # Point(x=1.0, y=2.0)
```

### Default values and field()

```python
from dataclasses import dataclass, field
from datetime import datetime

@dataclass
class User:
    name: str
    email: str
    age: int = 0
    tags: List[str] = field(default_factory=list)   # NEVER use [] as default directly
    created_at: datetime = field(default_factory=datetime.now)
    _id: int = field(default=0, repr=False)         # Hidden from repr

u = User("Alice", "alice@example.com", age=30, tags=["admin"])
print(u)
# User(name='Alice', email='alice@example.com', age=30, tags=['admin'], created_at=...)
```

### Frozen (immutable) data classes

```python
@dataclass(frozen=True)
class Config:
    host: str
    port: int
    debug: bool = False

c = Config("localhost", 5432)
# c.host = "prod"   # FrozenInstanceError: cannot assign to field 'host'
hash(c)           # Works because it's immutable — can be used as dict key
```

### Post-init processing

```python
@dataclass
class Circle:
    radius: float

    def __post_init__(self):
        if self.radius <= 0:
            raise ValueError(f"Radius must be positive, got {self.radius}")
        self.area = 3.14159 * self.radius ** 2   # Derived field

c = Circle(5.0)
print(c.area)   # 78.53975
# Circle(-1)    # ValueError: Radius must be positive
```

### Interview talking points
- `@dataclass` is not just a shortcut — it makes the data-holding intent explicit.
- Never use mutable default values (like `[]`) directly — use `field(default_factory=list)`.
- `frozen=True` makes the dataclass immutable and hashable (usable as a dict key or set member).
- `order=True` auto-generates `<`, `>`, `<=`, `>=` based on field order.
- For more validation power, use `pydantic`'s `BaseModel` (covered in Security section).

---

## 3.3 Practical Use Cases for `__new__`

### What is it?

`__new__` is the method responsible for **creating** a new instance. It runs before `__init__`. Normally you never need it — but it's essential in some patterns.

`__init__` initializes an existing object. `__new__` creates the object itself.

### Singleton pattern

```python
class Singleton:
    _instance = None

    def __new__(cls, *args, **kwargs):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance   # Always return the same instance

    def __init__(self, value):
        self.value = value

s1 = Singleton(10)
s2 = Singleton(20)
print(s1 is s2)      # True — same object!
print(s1.value)      # 20 (overwritten by second init)
```

### Immutable subclassing (e.g., subclassing int, str)

`__new__` is necessary when subclassing immutable types because their value is set at creation:

```python
class PositiveInt(int):
    def __new__(cls, value):
        if value <= 0:
            raise ValueError(f"Must be positive, got {value}")
        return super().__new__(cls, value)

x = PositiveInt(5)
print(x + 3)    # 8 — works like a normal int
# PositiveInt(-1) # ValueError: Must be positive, got -1
```

### Object caching/pooling

```python
class CachedConnection:
    _cache = {}

    def __new__(cls, host):
        if host not in cls._cache:
            instance = super().__new__(cls)
            cls._cache[host] = instance
        return cls._cache[host]

    def __init__(self, host):
        if not hasattr(self, "initialized"):   # Avoid re-initializing
            self.host = host
            self.initialized = True
            print(f"New connection to {host}")

c1 = CachedConnection("db.prod.com")   # New connection to db.prod.com
c2 = CachedConnection("db.prod.com")   # No output — reused
print(c1 is c2)  # True
```

### Interview talking points
- `__new__` receives the class as its first argument (`cls`); `__init__` receives the instance (`self`).
- Return value of `__new__` is the new object. If you return something that isn't an instance of `cls`, `__init__` won't be called.
- Most common uses: Singleton, object caching, subclassing immutables, custom object factories.

---

## 3.4 Multiple Inheritance and MRO

### What is it?

Python allows a class to inherit from multiple parent classes. The **Method Resolution Order (MRO)** is the algorithm Python uses to determine which class's method to call when there's ambiguity.

Python uses the **C3 linearization algorithm**, which guarantees a consistent, predictable order.

```python
class A:
    def method(self): print("A")

class B(A):
    def method(self): print("B")

class C(A):
    def method(self): print("C")

class D(B, C):
    pass

D().method()     # "B" — follows MRO
print(D.__mro__) # (D, B, C, A, object)
```

### The diamond problem, solved

```python
class Vehicle:
    def start(self):
        print("Vehicle starting")

class Car(Vehicle):
    def start(self):
        super().start()
        print("Car starting")

class Electric(Vehicle):
    def start(self):
        super().start()
        print("Electric motor starting")

class ElectricCar(Car, Electric):
    def start(self):
        super().start()
        print("ElectricCar starting")

ec = ElectricCar()
ec.start()
# Vehicle starting
# Electric motor starting
# Car starting
# ElectricCar starting

print(ElectricCar.__mro__)
# (ElectricCar, Car, Electric, Vehicle, object)
```

`super()` follows the MRO, so every class in the chain gets called exactly once — that's the key insight.

### Mixin pattern — the most common use of multiple inheritance

```python
class LogMixin:
    def log(self, message):
        print(f"[{self.__class__.__name__}] {message}")

class ValidateMixin:
    def validate(self):
        for field, value in self.__dict__.items():
            if value is None:
                raise ValueError(f"Field '{field}' cannot be None")
        return True

class JSONMixin:
    def to_json(self):
        import json
        return json.dumps(self.__dict__)

class User(LogMixin, ValidateMixin, JSONMixin):
    def __init__(self, name, email):
        self.name = name
        self.email = email

    def save(self):
        self.validate()
        self.log(f"Saving user {self.name}")
        return True

u = User("Alice", "alice@example.com")
u.save()           # [User] Saving user Alice
print(u.to_json()) # {"name": "Alice", "email": "alice@example.com"}
```

### Interview talking points
- MRO is computed at class definition time. You can inspect it with `ClassName.__mro__` or `ClassName.mro()`.
- `super()` is cooperative — it doesn't just call the immediate parent, it follows the MRO.
- Mixins are "capability classes" — they add behavior without being a conceptual base class.
- Python raises `TypeError: Cannot create a consistent MRO` if the MRO would be ambiguous.

---

## 3.5 Context Managers — `__enter__` and `__exit__`

### What is it?

A context manager is an object that defines what happens **before** and **after** a block of code using the `with` statement. It guarantees cleanup even if an exception occurs.

```python
with open("file.txt") as f:
    data = f.read()
# File is automatically closed here, even if an exception occurred
```

### Implementing `__enter__` and `__exit__`

```python
class DatabaseTransaction:
    def __init__(self, db_name):
        self.db_name = db_name
        self.connection = None

    def __enter__(self):
        print(f"Opening connection to {self.db_name}")
        self.connection = {"db": self.db_name, "active": True}
        return self.connection   # This is bound to the 'as' variable

    def __exit__(self, exc_type, exc_val, exc_tb):
        # exc_type: type of exception (or None)
        # exc_val: exception instance (or None)
        # exc_tb: traceback (or None)
        if exc_type:
            print(f"Exception {exc_type.__name__}: rolling back transaction")
        else:
            print("Committing transaction")
        print("Closing connection")
        self.connection["active"] = False
        return False   # False = don't suppress the exception

with DatabaseTransaction("users_db") as conn:
    print(f"Using connection: {conn}")
```

### Using contextlib.contextmanager (simpler approach)

```python
from contextlib import contextmanager

@contextmanager
def managed_resource(name):
    print(f"Acquiring {name}")
    resource = {"name": name, "active": True}
    try:
        yield resource    # Everything before yield is __enter__
    except Exception as e:
        print(f"Error: {e}, cleaning up {name}")
        raise
    finally:
        resource["active"] = False
        print(f"Releasing {name}")  # Always runs — like __exit__

with managed_resource("GPU") as r:
    print(f"Using {r['name']}")
```

### Suppressing exceptions

```python
from contextlib import suppress

with suppress(FileNotFoundError):
    open("nonexistent.txt")  # Exception is silently suppressed
```

### Interview talking points
- `__exit__` returning `True` suppresses the exception; returning `False` (or `None`) re-raises it.
- `__exit__` always runs — even if an exception is raised inside the `with` block.
- Use `contextlib.contextmanager` for simple cases; implement `__enter__`/`__exit__` for classes with state.
- Common uses: file handles, DB connections, locks, timers, mock patching in tests.

---

## 3.6 Iterator Protocol — `__iter__` and `__next__`

### What is it?

An iterator is an object that produces values one at a time on demand. The iterator protocol consists of two methods:
- `__iter__(self)` — returns the iterator object itself
- `__next__(self)` — returns the next value, or raises `StopIteration` when done

This is what powers `for` loops, `list()`, `sum()`, and all Python iteration.

```python
class Countdown:
    def __init__(self, start):
        self.current = start

    def __iter__(self):
        return self   # The object is its own iterator

    def __next__(self):
        if self.current <= 0:
            raise StopIteration
        self.current -= 1
        return self.current + 1

for n in Countdown(5):
    print(n)
# 5 4 3 2 1

# Under the hood, a for loop does this:
it = iter(Countdown(3))   # calls __iter__
while True:
    try:
        print(next(it))   # calls __next__
    except StopIteration:
        break
```

### Iterable vs Iterator

- **Iterable**: has `__iter__` — can be looped over. Lists, dicts, strings are iterables.
- **Iterator**: has both `__iter__` and `__next__` — produces values one at a time.

```python
class NumberRange:
    """Iterable — creates a fresh iterator each time"""

    def __init__(self, start, end):
        self.start = start
        self.end = end

    def __iter__(self):
        return NumberRangeIterator(self.start, self.end)

class NumberRangeIterator:
    """Iterator — stateful, single-use"""

    def __init__(self, start, end):
        self.current = start
        self.end = end

    def __iter__(self):
        return self

    def __next__(self):
        if self.current >= self.end:
            raise StopIteration
        val = self.current
        self.current += 1
        return val

r = NumberRange(1, 5)
for x in r: print(x)   # 1 2 3 4
for x in r: print(x)   # 1 2 3 4 — reusable!
```

### Generator — the pythonic iterator

```python
def fibonacci():
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a + b

fib = fibonacci()
print([next(fib) for _ in range(10)])
# [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
```

### Interview talking points
- Every `for x in obj` calls `iter(obj)` then repeatedly `next()` until `StopIteration`.
- An iterator is always an iterable (it has `__iter__`), but an iterable is not necessarily an iterator.
- Generators (`yield`) are syntactic sugar for iterators and are always preferred over manual classes.
- Infinite iterators are safe as long as you consume them lazily (with `next()` or `itertools.islice`).

---

# PART 4: ADVANCED FUNCTIONAL PROGRAMMING

---

## 4.1 Monads — Maybe/Either Patterns

### What is it?

A monad is a design pattern from functional programming that lets you **chain operations on values that might be absent, failed, or in some other special state** — without writing explicit `if/else` checks everywhere.

The two most practical monads in Python are:
- **Maybe (Option)**: a value that might be `None`
- **Either (Result)**: a value that is either a success or an error

### Why does it exist?

Without monads, code handling optional values or errors gets deeply nested:

```python
# Without Maybe monad — defensive pyramid
def get_user_city(user_id):
    user = get_user(user_id)
    if user is None:
        return None
    address = user.get("address")
    if address is None:
        return None
    city = address.get("city")
    return city
```

### Maybe monad

```python
class Maybe:
    def __init__(self, value):
        self._value = value

    @classmethod
    def of(cls, value):
        return cls(value)

    def is_nothing(self):
        return self._value is None

    def map(self, func):
        """Apply func if value exists, propagate Nothing if not"""
        if self.is_nothing():
            return self
        try:
            return Maybe(func(self._value))
        except Exception:
            return Maybe(None)

    def get_or_else(self, default):
        return default if self.is_nothing() else self._value

    def __repr__(self):
        return f"Nothing" if self.is_nothing() else f"Just({self._value})"


# Usage — clean chaining without if/else
user_data = {
    "name": "Alice",
    "address": {"city": "Cairo", "zip": "11511"}
}

city = (Maybe.of(user_data)
        .map(lambda u: u.get("address"))
        .map(lambda a: a.get("city"))
        .map(str.upper)
        .get_or_else("Unknown City"))

print(city)  # "CAIRO"

# With a missing field:
user_no_address = {"name": "Bob"}

city2 = (Maybe.of(user_no_address)
         .map(lambda u: u.get("address"))    # Returns Nothing
         .map(lambda a: a.get("city"))        # Skipped
         .get_or_else("Unknown City"))

print(city2)  # "Unknown City"
```

### Either/Result monad — for error handling

```python
class Result:
    def __init__(self, value=None, error=None):
        self._value = value
        self._error = error
        self._is_ok = error is None

    @classmethod
    def ok(cls, value):
        return cls(value=value)

    @classmethod
    def err(cls, error):
        return cls(error=error)

    def map(self, func):
        if not self._is_ok:
            return self   # Propagate error
        try:
            return Result.ok(func(self._value))
        except Exception as e:
            return Result.err(str(e))

    def get(self):
        if not self._is_ok:
            raise RuntimeError(f"Called get() on error: {self._error}")
        return self._value

    def __repr__(self):
        return f"Ok({self._value})" if self._is_ok else f"Err({self._error})"


def parse_age(value):
    age = int(value)
    if age < 0 or age > 150:
        raise ValueError("Age out of range")
    return age

result = (Result.ok("25")
          .map(parse_age)
          .map(lambda age: f"You are {age} years old"))

print(result)  # Ok(You are 25 years old)

result2 = Result.ok("abc").map(parse_age)
print(result2)  # Err(invalid literal for int() with base 10: 'abc')
```

### Interview talking points
- Python doesn't have built-in monads, but libraries like `returns` provide production-ready implementations.
- The point of monads is eliminating defensive `if/else` chains through composable transformations.
- `Optional` in type hints is conceptually related but is just a type annotation — it doesn't add behavior.
- Either/Result monad is a clean alternative to try/except for pipelines of operations.

---

## 4.2 Memoization — Beyond lru_cache

### What is it?

Memoization is the technique of caching the result of a function call so that repeated calls with the same arguments return the cached result instantly rather than recomputing.

```python
from functools import lru_cache

@lru_cache(maxsize=128)
def fibonacci(n):
    if n < 2:
        return n
    return fibonacci(n - 1) + fibonacci(n - 2)

print(fibonacci(50))  # Instant, even for large n
print(fibonacci.cache_info())  # CacheInfo(hits=48, misses=51, maxsize=128, currsize=51)
```

### Custom memoization with TTL (time-to-live)

```python
import time
from functools import wraps

def memoize_with_ttl(ttl_seconds):
    def decorator(func):
        cache = {}

        @wraps(func)
        def wrapper(*args):
            now = time.monotonic()
            if args in cache:
                result, timestamp = cache[args]
                if now - timestamp < ttl_seconds:
                    print(f"Cache hit for {args}")
                    return result
                else:
                    print(f"Cache expired for {args}")

            result = func(*args)
            cache[args] = (result, now)
            return result

        wrapper.clear_cache = lambda: cache.clear()
        return wrapper
    return decorator


@memoize_with_ttl(ttl_seconds=5)
def get_exchange_rate(currency):
    print(f"Fetching rate for {currency}...")
    return {"USD": 1.0, "EUR": 0.92, "EGP": 30.8}.get(currency, 0)


print(get_exchange_rate("EUR"))   # Fetching... 0.92
print(get_exchange_rate("EUR"))   # Cache hit: 0.92
```

### Memoized property (lazy, once-computed)

```python
class memoized_property:
    """Descriptor for lazy, once-computed properties"""

    def __init__(self, func):
        self.func = func
        self.attr = f"_cached_{func.__name__}"

    def __get__(self, obj, cls):
        if obj is None:
            return self
        if not hasattr(obj, self.attr):
            setattr(obj, self.attr, self.func(obj))
        return getattr(obj, self.attr)


class DataProcessor:
    def __init__(self, data):
        self.data = data

    @memoized_property
    def processed(self):
        print("Processing (expensive)...")
        return [x * 2 for x in self.data]

    @memoized_property
    def total(self):
        print("Computing total...")
        return sum(self.processed)


dp = DataProcessor([1, 2, 3, 4, 5])
print(dp.processed)  # Processing (expensive)... [2, 4, 6, 8, 10]
print(dp.processed)  # (no print — cached)
print(dp.total)      # Computing total... 30
```

### Interview talking points
- `lru_cache` uses a Least Recently Used eviction strategy — `maxsize=None` means unlimited.
- `cache` (Python 3.9+) is `lru_cache(maxsize=None)` with a simpler interface.
- `lru_cache` only works with hashable arguments (no lists, dicts as args).
- TTL memoization is essential for data that changes over time (API responses, exchange rates).
- `functools.cached_property` (Python 3.8+) is the built-in version of `memoized_property`.

---

## 4.3 Trampolining — For Recursive Functions

### What is it?

Trampolining is a technique to run deeply recursive functions **without hitting Python's recursion limit** (`RecursionError`). Instead of calling functions recursively (growing the call stack), you return a *thunk* (a callable representing the next step) and let a loop invoke it.

### The problem

```python
# Python's default limit is 1000 recursive calls
def factorial(n):
    if n <= 1:
        return 1
    return n * factorial(n - 1)

# factorial(5000)  # RecursionError: maximum recursion depth exceeded
```

### Trampoline solution

```python
def trampoline(f):
    """Keep calling f() as long as it returns a callable"""
    while callable(f):
        f = f()
    return f

def factorial_trampoline(n, accumulator=1):
    if n <= 1:
        return accumulator
    return lambda: factorial_trampoline(n - 1, n * accumulator)

result = trampoline(factorial_trampoline(10000))
print(result)  # Huge number, no RecursionError!
```

### Generalized trampoline decorator

```python
from functools import wraps

def trampolined(func):
    @wraps(func)
    def wrapper(*args, **kwargs):
        result = func(*args, **kwargs)
        while callable(result):
            result = result()
        return result
    return wrapper

@trampolined
def sum_to(n, acc=0):
    if n == 0:
        return acc
    return lambda: sum_to(n - 1, acc + n)

print(sum_to(100_000))  # 5000050000 — no stack overflow
```

### Interview talking points
- Python's default recursion limit is 1000 (can be changed with `sys.setrecursionlimit`).
- Trampolining converts recursion into iteration at the cost of lambda overhead.
- An alternative is converting the recursive algorithm to iterative manually.
- Trampolining is rarely needed in practice but shows deep understanding of the call stack.

---

## 4.4 Lazy Evaluation — Generators and itertools

### What is it?

Lazy evaluation means **computing values only when they are needed**, rather than computing everything upfront. In Python, generators are the primary mechanism for this.

### Generators

```python
# Eager (loads everything into memory)
squares_list = [x**2 for x in range(1_000_000)]   # 8 MB in memory

# Lazy (computes one value at a time)
squares_gen = (x**2 for x in range(1_000_000))    # Almost no memory

# Generator function
def infinite_counter(start=0):
    while True:
        yield start
        start += 1

counter = infinite_counter(10)
print(next(counter))  # 10
print(next(counter))  # 11
print(next(counter))  # 12
```

### Generator pipelines

```python
def read_large_file(filepath):
    with open(filepath) as f:
        for line in f:
            yield line.strip()

def filter_errors(lines):
    for line in lines:
        if "ERROR" in line:
            yield line

def extract_timestamps(lines):
    for line in lines:
        yield line.split("]")[0].lstrip("[")

# Process a 10GB log file with constant memory:
# lines = read_large_file("app.log")
# errors = filter_errors(lines)
# timestamps = extract_timestamps(errors)
# for ts in timestamps:
#     print(ts)
```

### itertools — the lazy toolkit

```python
import itertools

# Infinite iterators
counter = itertools.count(1, 2)              # 1, 3, 5, 7, ...
cycler = itertools.cycle(["red", "blue"])    # red, blue, red, blue, ...

# Slicing infinite sequences
print(list(itertools.islice(counter, 5)))   # [1, 3, 5, 7, 9]

# Combinations and permutations
print(list(itertools.combinations("ABC", 2)))     # [('A','B'), ('A','C'), ('B','C')]
print(list(itertools.permutations("AB", 2)))      # [('A','B'), ('B','A')]

# Chaining iterables
a = [1, 2, 3]
b = [4, 5, 6]
print(list(itertools.chain(a, b)))           # [1, 2, 3, 4, 5, 6]

# Grouping (data must be sorted by key first)
data = [("Alice", "HR"), ("Bob", "HR"), ("Charlie", "Eng")]
data.sort(key=lambda x: x[1])
for dept, members in itertools.groupby(data, key=lambda x: x[1]):
    print(dept, list(members))

# Accumulate (running totals)
sales = [100, 200, 150, 300]
print(list(itertools.accumulate(sales)))   # [100, 300, 450, 750]
```

### Interview talking points
- Generators use `yield` — they pause execution and resume from where they left off.
- Generator expressions `(x for x in ...)` are lazy; list comprehensions `[x for x in ...]` are eager.
- `send()` on a generator allows two-way communication (advanced coroutine pattern).
- `yield from` delegates to another generator (sub-generator protocol).
- `itertools.islice` is essential for safely consuming infinite generators.

---

# PART 5: MEMORY MANAGEMENT

---

## 5.1 Garbage Collection — Reference Counting + Generational GC

### What is it?

Python manages memory automatically using two mechanisms:
1. **Reference counting**: every object tracks how many variables point to it. When count hits 0, the object is immediately freed.
2. **Generational garbage collector**: handles circular references that reference counting misses.

### Reference counting

```python
import sys

a = [1, 2, 3]
print(sys.getrefcount(a))   # 2 (one for 'a', one for the getrefcount argument)

b = a              # b also points to the same list
print(sys.getrefcount(a))   # 3

del b
print(sys.getrefcount(a))   # 2 — back to normal

a = None           # The list's refcount drops to 0 — immediately freed
```

### Circular references — reference counting's weakness

```python
import gc

class Node:
    def __init__(self, value):
        self.value = value
        self.next = None
        self.prev = None

n1 = Node(1)
n2 = Node(2)
n1.next = n2    # n1 -> n2
n2.prev = n1    # n2 -> n1 — circular!

del n1, n2      # Reference counts drop but not to 0 (they reference each other)
# Memory is NOT freed yet — requires the cyclic GC

gc.collect()    # Force cyclic garbage collection
```

### Generational GC

Python's cyclic GC tracks objects in three generations based on age:
- **Generation 0**: newly created objects (collected most frequently)
- **Generation 1**: objects that survived one GC cycle
- **Generation 2**: long-lived objects (collected least frequently)

```python
import gc

print(gc.get_threshold())   # (700, 10, 10) — default thresholds
print(gc.get_count())       # (objects in gen0, gen1, gen2)
print(gc.collect(0))        # Collect only generation 0
```

### Interview talking points
- Reference counting is deterministic (objects freed immediately) but misses circular references.
- The cyclic GC is non-deterministic (runs periodically) and handles circular references.
- `__del__` (destructor) is called when an object's refcount hits 0, but don't rely on it for cleanup — use context managers instead.
- CPython (the standard Python) uses reference counting; PyPy and Jython use other GC strategies.

---

## 5.2 Memory Views — Zero-Copy Operations

### What is it?

A `memoryview` exposes the internal buffer of an object (like `bytes` or `bytearray`) directly, allowing slicing and manipulation **without copying data**. This is critical for performance when working with large binary data.

```python
data = bytearray(b"Hello, World!")

# Regular slicing — creates a copy (expensive for large data)
copy = data[0:5]           # b"Hello" — new bytearray allocated

# memoryview — no copy
mv = memoryview(data)
view = mv[0:5]             # A view into the same memory
print(bytes(view))         # b"Hello"

# Modify through memoryview — modifies original
mv[0] = ord("J")
print(data)                # bytearray(b"Jello, World!")
```

### memoryview for large binary processing

```python
import struct

def process_binary_stream(data: bytes):
    mv = memoryview(data)
    chunk_size = 4

    results = []
    for offset in range(0, len(mv), chunk_size):
        chunk = mv[offset:offset + chunk_size]
        value = struct.unpack_from("I", chunk)[0]  # Unpack uint32 — no copy
        results.append(value)
    return results

data = struct.pack("IIII", 100, 200, 300, 400)
print(process_binary_stream(data))   # [100, 200, 300, 400]
```

### Interview talking points
- `memoryview` is most useful for large buffers (images, video frames, network packets).
- Supports slicing without allocating new memory — the slice is another view.
- Works on objects that implement the buffer protocol: `bytes`, `bytearray`, `array.array`, numpy arrays.
- Use `memoryview.cast()` to reinterpret the buffer as a different type.

---

## 5.3 Weak References — weakref module

### What is it?

A weak reference is a reference to an object that **does not increase its reference count**. If the only remaining references to an object are weak references, the object can be garbage-collected.

### Why does it exist?

The main use case is **caches**: you want to cache objects, but not prevent them from being freed when they're no longer used elsewhere.

```python
import weakref
import gc

class ExpensiveObject:
    def __init__(self, name):
        self.name = name

    def __del__(self):
        print(f"{self.name} is being deleted")

# Weak reference
obj = ExpensiveObject("Resource B")
weak = weakref.ref(obj)

print(weak())    # <ExpensiveObject: Resource B> — still alive
del obj          # "Resource B is being deleted"
print(weak())    # None — the object is gone, reference is dead
```

### WeakValueDictionary — cache that auto-cleans

```python
import weakref
import gc

class ImageCache:
    def __init__(self):
        self._cache = weakref.WeakValueDictionary()

    def load(self, path):
        if path in self._cache:
            print(f"Cache hit: {path}")
            return self._cache[path]
        print(f"Loading: {path}")
        img = {"path": path, "data": "binary_data"}
        self._cache[path] = img
        return img

cache = ImageCache()
img1 = cache.load("photo.jpg")   # Loading: photo.jpg
img2 = cache.load("photo.jpg")   # Cache hit: photo.jpg

del img1, img2
gc.collect()
img3 = cache.load("photo.jpg")   # Loading again — cache was cleared
```

### Interview talking points
- Weak references prevent memory leaks in caches and event systems.
- `weakref.WeakValueDictionary` and `weakref.WeakKeyDictionary` are the most common utilities.
- `weakref.WeakSet` is a set of objects that don't prevent GC.
- Calling a dead `weakref.ref()` returns `None` — always check before using.
- Weak references are also used in observer/event patterns to avoid keeping listeners alive.

---

## 5.4 `__slots__` — Custom Memory Allocation

### What is it?

By default, Python stores instance attributes in a `__dict__` — a dictionary per object. `__slots__` replaces this with a fixed-size array, reducing memory usage and slightly improving attribute access speed.

```python
import sys

class RegularPoint:
    def __init__(self, x, y):
        self.x = x
        self.y = y

class SlottedPoint:
    __slots__ = ("x", "y")

    def __init__(self, x, y):
        self.x = x
        self.y = y

r = RegularPoint(1, 2)
s = SlottedPoint(1, 2)

print(hasattr(r, "__dict__"))       # True
print(hasattr(s, "__dict__"))       # False
print(r.__dict__)                   # {'x': 1, 'y': 2}

# Cannot add arbitrary attributes to slotted objects:
r.z = 3   # Fine
# s.z = 3   # AttributeError: 'SlottedPoint' object has no attribute 'z'
```

### Interview talking points
- `__slots__` is most beneficial for classes with many instances (data models, game entities, point clouds).
- With `__slots__`, you cannot add new attributes at runtime.
- Inheritance with `__slots__` requires each class in the hierarchy to define `__slots__`.
- If any parent class doesn't define `__slots__`, the subclass gets a `__dict__` anyway.
- Python `dataclass` + `slots=True` (Python 3.10+): `@dataclass(slots=True)`.

---

# PART 6: SECURITY CONSIDERATIONS

---

## 6.1 Input Validation — Pydantic and Marshmallow

### What is it?

Input validation ensures that data coming into your system (from users, APIs, files) matches the expected shape, types, and constraints before you process it.

### Pydantic — type-driven validation

```python
from pydantic import BaseModel, Field, validator
from typing import Optional, List

class Address(BaseModel):
    street: str
    city: str
    country: str = "Egypt"
    zip_code: str

class UserCreate(BaseModel):
    name: str = Field(min_length=2, max_length=50)
    email: str
    age: int = Field(ge=0, le=150)   # ge=greater-or-equal, le=less-or-equal
    tags: List[str] = []
    address: Optional[Address] = None

    @validator("name")
    def name_must_be_alphabetic(cls, v):
        if not v.replace(" ", "").isalpha():
            raise ValueError("Name must contain only letters")
        return v.title()

    @validator("email")
    def email_must_be_valid(cls, v):
        if "@" not in v or "." not in v.split("@")[-1]:
            raise ValueError("Invalid email format")
        return v.lower()


# Valid input
user = UserCreate(
    name="alice smith",
    email="Alice@Example.COM",
    age=30,
    tags=["admin"],
    address={"street": "123 Main St", "city": "Cairo", "zip_code": "11511"}
)
print(user.name)    # "Alice Smith" (auto-transformed)
print(user.email)   # "alice@example.com"

# Invalid input — Pydantic raises ValidationError
try:
    from pydantic import ValidationError
    bad_user = UserCreate(name="A", email="not-an-email", age=200)
except ValidationError as e:
    print(e)
```

### Marshmallow — schema-based serialization/deserialization

```python
from marshmallow import Schema, fields, validate, validates, ValidationError

class ProductSchema(Schema):
    id = fields.Int(dump_only=True)
    name = fields.Str(required=True, validate=validate.Length(min=1, max=100))
    price = fields.Float(required=True, validate=validate.Range(min=0))
    category = fields.Str(validate=validate.OneOf(["electronics", "food", "clothing"]))
    in_stock = fields.Bool(load_default=True)

    @validates("name")
    def validate_name(self, value):
        if value.lower() == "unknown":
            raise ValidationError("Product name cannot be 'unknown'")

schema = ProductSchema()

try:
    product = schema.load({
        "name": "Laptop",
        "price": 999.99,
        "category": "electronics"
    })
    print(product)  # {'name': 'Laptop', 'price': 999.99, ...}
except ValidationError as e:
    print(e.messages)
```

### Interview talking points
- Pydantic v2 is significantly faster (Rust-based core). It's the standard in FastAPI.
- Marshmallow is older and more flexible for complex serialization scenarios.
- Never trust user input — always validate at the boundary of your system.
- Pydantic models can be exported to JSON Schema automatically — great for API documentation.

---

## 6.2 Secure String Handling — secrets module

### What is it?

The `secrets` module generates cryptographically strong random values suitable for passwords, tokens, and URLs. The standard `random` module is NOT suitable for security use — it's predictable.

```python
import secrets
import string

# Generating tokens
token = secrets.token_hex(32)          # 64 hex chars — for API keys
url_safe = secrets.token_urlsafe(32)   # Base64 URL-safe — for reset links

print(token)       # e.g., "a3f8d2c1..."
print(url_safe)    # e.g., "X8kLm2Nv..."

# Generating a secure password
alphabet = string.ascii_letters + string.digits + "!@#$%"
password = "".join(secrets.choice(alphabet) for _ in range(16))
print(password)

# Constant-time comparison (prevents timing attacks)
stored_token = "abc123"
provided_token = "abc123"

# WRONG — vulnerable to timing attacks:
# if stored_token == provided_token: ...

# CORRECT — always takes the same time:
if secrets.compare_digest(stored_token, provided_token):
    print("Valid")
```

### Interview talking points
- `random.random()` is seeded deterministically — never use it for security.
- `secrets.compare_digest` prevents timing attacks on token comparison.
- Always use `bcrypt`, `argon2`, or `scrypt` for password hashing — never SHA256 alone.
- Store tokens as hashes, not plaintext.

---

## 6.3 Code Injection Prevention

```python
# DANGEROUS — eval() executes arbitrary code
# user_input = "__import__('os').system('rm -rf /')"
# result = eval(user_input)   # Don't do this!

# SAFER — use ast.literal_eval() for parsing literal Python data
import ast
safe_input = "[1, 2, 3]"
result = ast.literal_eval(safe_input)   # Only evaluates literals
print(result)  # [1, 2, 3]

# SQL injection prevention
import sqlite3

conn = sqlite3.connect(":memory:")
conn.execute("CREATE TABLE users (id INTEGER, name TEXT)")
conn.execute("INSERT INTO users VALUES (1, 'Alice')")

# VULNERABLE:
# username = "' OR '1'='1"
# query = f"SELECT * FROM users WHERE name = '{username}'"

# SAFE — parameterized queries:
username = "' OR '1'='1"
cursor = conn.execute("SELECT * FROM users WHERE name = ?", (username,))
print(cursor.fetchall())   # [] — no injection possible
```

### Interview talking points
- Never use `eval()` on user input — it's almost always replaceable with safer alternatives.
- `ast.literal_eval()` for parsing data structures; JSON parsing for API input.
- Always use parameterized queries for SQL — never string formatting.
- ORMs (SQLAlchemy, Django ORM) use parameterized queries by default.

---

## 6.4 SSL/TLS Configuration

```python
import ssl
import requests

# Correct — verify=True is the default (don't change it):
# response = requests.get("https://api.example.com", verify=True)

# DO NOT DO THIS in production — disables all verification:
# response = requests.get("https://api.example.com", verify=False)

# Creating a secure context for manual socket use:
context = ssl.create_default_context()
context.minimum_version = ssl.TLSVersion.TLSv1_2   # Enforce minimum version
```

### Interview talking points
- Never set `verify=False` in production — it makes MITM attacks trivial.
- Use `ssl.create_default_context()` for client connections — it handles certificate validation.
- Minimum recommended TLS version: TLS 1.2; prefer TLS 1.3.
- `certifi` package provides an up-to-date CA bundle for Python.

---

# PART 7: TESTING AND DEBUGGING

---

## 7.1 Pytest

### What is it?

Pytest is Python's most popular testing framework. It auto-discovers tests, provides concise assertions, and has a rich plugin ecosystem.

```python
# math_utils.py
def add(a, b): return a + b
def divide(a, b):
    if b == 0:
        raise ZeroDivisionError("Cannot divide by zero")
    return a / b

# test_math_utils.py
import pytest
from math_utils import add, divide

def test_add():
    assert add(1, 2) == 3
    assert add(-1, 1) == 0
    assert add(0, 0) == 0

def test_divide_by_zero():
    with pytest.raises(ZeroDivisionError, match="Cannot divide by zero"):
        divide(10, 0)

@pytest.mark.parametrize("a, b, expected", [
    (2, 3, 5),
    (0, 0, 0),
    (-1, 1, 0),
    (100, -50, 50),
])
def test_add_parametrized(a, b, expected):
    assert add(a, b) == expected

@pytest.fixture
def sample_data():
    return {"users": ["Alice", "Bob"], "count": 2}

def test_user_count(sample_data):
    assert sample_data["count"] == len(sample_data["users"])

@pytest.fixture(scope="session")
def db_connection():
    conn = {"connected": True}
    yield conn
    conn["connected"] = False   # Teardown

def test_db(db_connection):
    assert db_connection["connected"]
```

### Markers for organizing tests

```python
@pytest.mark.slow
def test_heavy_computation():
    pass

@pytest.mark.skip(reason="Not implemented yet")
def test_future_feature():
    pass

@pytest.mark.xfail(reason="Known bug #123")
def test_known_broken():
    assert False

# Run: pytest -m "not slow" to skip slow tests
# Run: pytest -v --tb=short for verbose output
```

### Interview talking points
- Pytest uses plain `assert` statements — no `assertEqual`, `assertTrue`, etc.
- Fixtures replace `setUp`/`tearDown` from `unittest` and are more composable.
- `conftest.py` is a special file where shared fixtures live.
- `scope` options: `function` (default), `class`, `module`, `session`.
- Run with `pytest --cov` (with pytest-cov) for coverage reports.

---

## 7.2 Property-Based Testing — hypothesis

### What is it?

In standard testing, you write specific input/output pairs. In property-based testing, you define **properties** (invariants) your code must satisfy, and Hypothesis automatically generates hundreds of test cases — including edge cases you'd never think of.

```python
from hypothesis import given, assume
from hypothesis import strategies as st

# Property: sorting a list and then taking min/max should equal
# directly taking min/max from the original
@given(st.lists(st.integers(), min_size=1))
def test_sort_preserves_min_max(lst):
    sorted_lst = sorted(lst)
    assert sorted_lst[0] == min(lst)
    assert sorted_lst[-1] == max(lst)

# Property: encoding and decoding should be inverse operations
@given(st.text())
def test_encode_decode_roundtrip(text):
    encoded = text.encode("utf-8")
    decoded = encoded.decode("utf-8")
    assert decoded == text

# Property: addition is commutative
@given(st.integers(), st.integers())
def test_addition_commutative(a, b):
    assert a + b == b + a

# With assume() — filter invalid inputs
@given(st.floats(), st.floats())
def test_division_result(a, b):
    assume(b != 0 and abs(b) > 1e-10)
    result = a / b
    assert abs(result * b - a) < abs(a) * 1e-9
```

### Interview talking points
- Hypothesis finds edge cases automatically: empty lists, `None`, overflow values, unicode edge cases.
- When Hypothesis finds a failing case, it **shrinks** it to the simplest failing example.
- It saves failing examples in a database — re-runs them first on subsequent test runs.
- Property-based testing complements, not replaces, example-based tests.

---

## 7.3 Mocking — unittest.mock

### What is it?

Mocking replaces real dependencies (databases, APIs, file systems) with controlled fake versions during testing. This makes tests fast, isolated, and deterministic.

```python
from unittest.mock import Mock, MagicMock, patch

# Basic Mock
mock = Mock()
mock.some_method(1, 2, 3)
mock.some_method.assert_called_once_with(1, 2, 3)
print(mock.some_method.call_count)  # 1

# Configuring return values
mock_db = Mock()
mock_db.get_user.return_value = {"id": 1, "name": "Alice"}
user = mock_db.get_user(1)
print(user["name"])   # "Alice"

# Configuring side effects
mock_api = Mock()
mock_api.fetch.side_effect = [
    {"data": "first call"},
    {"data": "second call"},
    ConnectionError("API is down"),
]
print(mock_api.fetch())   # {'data': 'first call'}
print(mock_api.fetch())   # {'data': 'second call'}
# mock_api.fetch()        # ConnectionError: API is down
```

### patch() — replacing real objects in tests

```python
# email_service.py
import smtplib

def send_welcome_email(user_email):
    with smtplib.SMTP("smtp.gmail.com", 587) as server:
        server.sendmail("admin@app.com", user_email, "Welcome!")
    return True

# test_email_service.py
from unittest.mock import patch, MagicMock
from email_service import send_welcome_email

@patch("email_service.smtplib.SMTP")  # Replace the class in the module being tested
def test_send_welcome_email(mock_smtp_class):
    mock_server = MagicMock()
    mock_smtp_class.return_value.__enter__.return_value = mock_server

    result = send_welcome_email("user@example.com")

    assert result is True
    mock_server.sendmail.assert_called_once_with(
        "admin@app.com", "user@example.com", "Welcome!"
    )
```

### Interview talking points
- `Mock` vs `MagicMock`: `MagicMock` pre-configures magic methods (`__len__`, `__enter__`, etc.).
- Always use `spec=` when mocking — it prevents tests from passing when the real interface changes.
- `patch` target is the location where the thing is **used**, not where it's defined.
- `assert_called_once_with` is strict about arguments; `assert_called_with` only checks the last call.

---

## 7.4 Debugging — pdb and ipdb

### What is it?

`pdb` (Python Debugger) lets you pause execution and interactively inspect state. `ipdb` is an enhanced version with syntax highlighting and tab completion.

```python
def buggy_function(data):
    result = []
    for item in data:
        breakpoint()   # Execution pauses here (Python 3.7+)
        processed = item * 2
        result.append(processed)
    return result
```

### pdb commands

```
n         -> next line (step over)
s         -> step into function
c         -> continue (until next breakpoint)
q         -> quit debugger
p expr    -> print expression
pp expr   -> pretty-print expression
l         -> list source code around current line
w         -> show call stack (where am I?)
u / d     -> up/down the call stack
b 42      -> set breakpoint at line 42
```

### Post-mortem debugging

```python
import pdb

try:
    1 / 0
except:
    pdb.post_mortem()   # Opens debugger at the point of exception
```

### Interview talking points
- `pdb.set_trace()` pauses execution and opens an interactive REPL.
- `breakpoint()` (Python 3.7+) is the modern standard — respects `PYTHONBREAKPOINT` env var.
- `ipdb` provides a much better experience — syntax highlighting, tab completion, same commands.
- For production debugging, use logging instead of pdb.
- `pdb.pm()` (post-mortem) is for inspecting state after an unhandled exception.

---

## 7.5 Static Analysis — mypy and pylint

### What is it?

Static analysis tools check your code for errors **without running it**:
- **mypy**: checks type annotations for consistency
- **pylint**: checks code style, potential errors, and complexity

### mypy — type checking

```python
# With type hints — mypy catches type errors before runtime
def greet(name: str) -> str:
    return "Hello, " + name

# greet(123)  # mypy: Argument 1 to "greet" has incompatible type "int"; expected "str"

from typing import List, Optional, Dict

def process_users(users: List[Dict[str, str]]) -> Optional[str]:
    if not users:
        return None
    return users[0].get("name")

# Generics
from typing import TypeVar, Generic

T = TypeVar("T")

class Stack(Generic[T]):
    def __init__(self) -> None:
        self._items: List[T] = []

    def push(self, item: T) -> None:
        self._items.append(item)

    def pop(self) -> T:
        return self._items.pop()

s: Stack[int] = Stack()
s.push(1)
# s.push("hello")  # mypy error: incompatible type "str"; expected "int"
```

### Interview talking points
- Type hints are optional but mypy enforces them — they're documentation that can be verified.
- mypy uses `--strict` for maximum checking; `--ignore-missing-imports` for third-party libraries without stubs.
- pylint scores your code 0-10 and provides actionable feedback.
- `# type: ignore` suppresses a mypy error on a line (use sparingly).
- `Protocol` in typing allows structural subtyping — like interfaces without inheritance.

---

# PART 8: PERFORMANCE OPTIMIZATION

---

## 8.1 Caching — functools.lru_cache

```python
from functools import lru_cache, cache
import time

@lru_cache(maxsize=128)
def fibonacci(n: int) -> int:
    if n < 2:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

@cache  # Python 3.9+ — unlimited cache
def expensive_computation(x: int) -> int:
    time.sleep(0.1)  # Simulate expensive work
    return x ** 2

print(fibonacci(50))
print(fibonacci.cache_info())
# CacheInfo(hits=48, misses=51, maxsize=128, currsize=51)

fibonacci.cache_clear()  # Clear the cache
```

### Interview talking points
- `lru_cache` uses Least Recently Used eviction — oldest unused entries are discarded first.
- `maxsize=None` means unlimited cache.
- Only works with hashable arguments — no lists or dicts.
- `functools.cached_property` caches a property value after first access.

---

## 8.2 Profiling — cProfile and timeit

### cProfile — where is the time going?

```python
import cProfile
import pstats
import io

def slow_function():
    total = 0
    for i in range(100_000):
        total += i ** 2
    return total

profiler = cProfile.Profile()
profiler.enable()
slow_function()
profiler.disable()

stream = io.StringIO()
stats = pstats.Stats(profiler, stream=stream)
stats.sort_stats("cumulative")
stats.print_stats(10)  # Top 10 functions by cumulative time
print(stream.getvalue())
```

### timeit — measuring small snippets

```python
import timeit

list_comp = timeit.timeit("[x**2 for x in range(1000)]", number=10_000)
map_func = timeit.timeit("list(map(lambda x: x**2, range(1000)))", number=10_000)

print(f"List comprehension: {list_comp:.3f}s")
print(f"map/lambda: {map_func:.3f}s")
```

### Interview talking points
- Profile first, optimize second — never guess where the bottleneck is.
- `cProfile` shows cumulative and per-call times for every function.
- Sort by "cumulative" to find the function at the root of slowness.
- `timeit` is for micro-benchmarks; `cProfile` is for whole-program profiling.

---

## 8.3 Cython — Writing C Extensions

### What is it?

Cython compiles Python-like code to C, giving near-C performance for CPU-bound code while keeping a Python-like syntax.

```cython
# mandelbrot.pyx (Cython file)
# cython: boundscheck=False, wraparound=False
import numpy as np
cimport numpy as np

def mandelbrot_cy(int width, int height, int max_iter):
    cdef int[:,:] result = np.zeros((height, width), dtype=np.int32)
    cdef double x, y, x2, y2, cx, cy
    cdef int i, xi, yi

    for yi in range(height):
        for xi in range(width):
            cx = -2.5 + xi * 3.5 / width
            cy = -1.25 + yi * 2.5 / height
            x = y = 0.0
            for i in range(max_iter):
                x2, y2 = x*x, y*y
                if x2 + y2 > 4:
                    break
                x, y = x2 - y2 + cx, 2*x*y + cy
            result[yi, xi] = i
    return result
```

### Interview talking points
- Cython is most useful for tight numerical loops that NumPy can't vectorize.
- Adding `cdef` type declarations is what provides the speedup.
- Alternatives: NumPy vectorization, Numba (JIT), PyPy.

---

## 8.4 Just-In-Time Compilation — Numba

### What is it?

Numba JIT-compiles Python functions to machine code at runtime using LLVM. It's especially powerful for numerical code and works seamlessly with NumPy.

```python
from numba import njit
import numpy as np
import time

@njit   # "no Python" mode — strict, fastest
def sum_squares(n):
    total = 0.0
    for i in range(n):
        total += i ** 2
    return total

# First call: compiles (slow), subsequent calls: very fast
sum_squares(1_000_000)   # Compilation happens here

start = time.perf_counter()
result = sum_squares(100_000_000)
print(f"Numba: {time.perf_counter() - start:.3f}s")  # ~0.1s vs ~10s in pure Python
print(result)
```

### Interview talking points
- First call to a `@jit` function is slow (compilation time); subsequent calls are fast.
- `@njit` (nopython mode) gives the best performance but restricts which Python features you can use.
- Numba supports parallel execution with `@njit(parallel=True)` and `prange()`.
- For GPU acceleration: `@cuda.jit` from Numba.
- Best for numerical loops with NumPy arrays.

---

# PART 9: ADVANCED DATA STRUCTURES

---

## 9.1 Heap Queues — heapq module

### What is it?

A heap is a tree-based data structure where the parent is always smaller (min-heap) or larger (max-heap) than its children. Python's `heapq` provides a min-heap. The key property: getting the minimum element is always O(1); inserting/removing is O(log n).

```python
import heapq

nums = [3, 1, 4, 1, 5, 9, 2, 6]
heapq.heapify(nums)      # Converts list to heap in-place: O(n)
print(nums)              # Heap-ordered, not fully sorted

heapq.heappush(nums, 0)  # O(log n)
print(heapq.heappop(nums))  # 0 — always the minimum: O(log n)

# Get N smallest/largest without full sort
data = [5, 2, 8, 1, 9, 3]
print(heapq.nsmallest(3, data))  # [1, 2, 3]
print(heapq.nlargest(3, data))   # [9, 8, 5]

# Max-heap: negate values
max_heap = [-x for x in [3, 1, 4, 1, 5]]
heapq.heapify(max_heap)
print(-heapq.heappop(max_heap))  # 5 — the maximum

# Priority queue with (priority, item) tuples
tasks = []
heapq.heappush(tasks, (3, "low priority task"))
heapq.heappush(tasks, (1, "urgent task"))
heapq.heappush(tasks, (2, "normal task"))

while tasks:
    priority, task = heapq.heappop(tasks)
    print(f"[{priority}] {task}")
# [1] urgent task
# [2] normal task
# [3] low priority task
```

### Interview talking points
- `heapq` is a min-heap — smallest element is always at index 0.
- For a max-heap, negate values or use a wrapper class.
- `heapq.merge()` merges sorted iterables lazily.
- Common interview use: "find the k largest/smallest elements" — use `heapq.nlargest(k, data)`.
- Priority queues for Dijkstra's algorithm, A*, task scheduling.

---

## 9.2 Tries — Prefix Trees

### What is it?

A trie (pronounced "try") is a tree where each node represents a character, and paths from root to leaf represent strings. It's extremely efficient for prefix-based operations (autocomplete, spell check, IP routing).

```python
class TrieNode:
    def __init__(self):
        self.children = {}    # char -> TrieNode
        self.is_end = False   # True if this node ends a word

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word: str) -> None:
        node = self.root
        for char in word:
            if char not in node.children:
                node.children[char] = TrieNode()
            node = node.children[char]
        node.is_end = True

    def search(self, word: str) -> bool:
        node = self.root
        for char in word:
            if char not in node.children:
                return False
            node = node.children[char]
        return node.is_end

    def starts_with(self, prefix: str) -> bool:
        node = self.root
        for char in prefix:
            if char not in node.children:
                return False
            node = node.children[char]
        return True

    def autocomplete(self, prefix: str) -> list:
        node = self.root
        for char in prefix:
            if char not in node.children:
                return []
            node = node.children[char]
        results = []
        self._dfs(node, prefix, results)
        return results

    def _dfs(self, node, current, results):
        if node.is_end:
            results.append(current)
        for char, child in node.children.items():
            self._dfs(child, current + char, results)


trie = Trie()
words = ["apple", "app", "application", "apply", "apt", "banana"]
for word in words:
    trie.insert(word)

print(trie.search("app"))           # True
print(trie.search("ap"))            # False (not a complete word)
print(trie.starts_with("ap"))       # True
print(trie.autocomplete("app"))     # ['app', 'apple', 'application', 'apply']
```

### Interview talking points
- Insert, search, and prefix search are all O(m) where m is the string length — independent of dictionary size.
- Space complexity is O(alphabet_size * max_word_length * number_of_words) in the worst case.
- Common uses: autocomplete, spell checker, IP address lookup, prefix-based search.
- Compressed tries (Patricia trees) save space by merging single-child nodes.

---

## 9.3 Bloom Filters — Probabilistic Data Structure

### What is it?

A bloom filter is a space-efficient data structure that answers "is this element in the set?" with two possible answers: **definitely not** or **probably yes**. It can have false positives (saying something is in the set when it isn't) but never false negatives.

### Why does it exist?

When you have billions of items and can't store them all, a bloom filter uses a tiny bit array to check membership extremely cheaply.

```python
import hashlib
import math

class BloomFilter:
    def __init__(self, capacity: int, error_rate: float = 0.01):
        self.size = math.ceil(-(capacity * math.log(error_rate)) / (math.log(2) ** 2))
        self.hash_count = math.ceil((self.size / capacity) * math.log(2))
        self.bit_array = bytearray(math.ceil(self.size / 8))

    def _hash(self, item: str, seed: int) -> int:
        h = hashlib.md5(f"{seed}{item}".encode()).hexdigest()
        return int(h, 16) % self.size

    def _set_bit(self, pos: int) -> None:
        byte_idx, bit_idx = pos // 8, pos % 8
        self.bit_array[byte_idx] |= (1 << bit_idx)

    def _get_bit(self, pos: int) -> bool:
        byte_idx, bit_idx = pos // 8, pos % 8
        return bool(self.bit_array[byte_idx] & (1 << bit_idx))

    def add(self, item: str) -> None:
        for seed in range(self.hash_count):
            self._set_bit(self._hash(item, seed))

    def __contains__(self, item: str) -> bool:
        return all(self._get_bit(self._hash(item, seed))
                   for seed in range(self.hash_count))


bf = BloomFilter(capacity=1000, error_rate=0.01)
bf.add("apple")
bf.add("banana")
bf.add("cherry")

print("apple" in bf)    # True
print("banana" in bf)   # True
print("grape" in bf)    # False (or very rarely, a false positive)
```

### Interview talking points
- Never false negatives — if the filter says "not in set," it's definitely not there.
- Possible false positives — if it says "in set," it might be wrong.
- Once an element is added, it cannot be removed (use counting bloom filters for removal).
- 1% error rate with 1M elements requires only ~1.2 MB vs ~40 MB for a hash set.
- Used in: Google Chrome's safe browsing, Cassandra/HBase/LevelDB, web crawlers.

---

## 9.4 Persistent Data Structures — pyrsistent

### What is it?

Persistent data structures preserve their previous versions when modified. Instead of changing the original, every modification returns a new structure that shares as much of the old structure as possible (structural sharing).

```python
# pip install pyrsistent
from pyrsistent import pvector, pmap, freeze, thaw

# Persistent vector (like a list)
v1 = pvector([1, 2, 3])
v2 = v1.append(4)
v3 = v2.set(0, 99)

print(v1)   # pvector([1, 2, 3])      — unchanged
print(v2)   # pvector([1, 2, 3, 4])   — new version
print(v3)   # pvector([99, 2, 3, 4])  — another new version

# Persistent map (like a dict)
m1 = pmap({"a": 1, "b": 2})
m2 = m1.set("c", 3)
m3 = m2.remove("a")

print(m1)   # pmap({'a': 1, 'b': 2})            — unchanged
print(m2)   # pmap({'a': 1, 'b': 2, 'c': 3})
print(m3)   # pmap({'b': 2, 'c': 3})

# Freeze/thaw for converting between mutable and immutable
nested = {"user": {"name": "Alice", "scores": [1, 2, 3]}}
frozen = freeze(nested)          # Deep-freezes to pmap/pvector
mutable = thaw(frozen)           # Converts back to regular dicts/lists
```

### Undo/redo with persistent structures

```python
history = [pmap({"balance": 1000})]

def deposit(state, amount):
    return state.set("balance", state["balance"] + amount)

def withdraw(state, amount):
    return state.set("balance", state["balance"] - amount)

state = history[-1]
state = deposit(state, 500); history.append(state)    # balance: 1500
state = withdraw(state, 200); history.append(state)   # balance: 1300
state = deposit(state, 100); history.append(state)    # balance: 1400

# Undo last 2 operations:
state = history[-3]
print(state["balance"])   # 1500 — rolled back
```

### Interview talking points
- Persistent = immutable + structural sharing (not just copying everything).
- Structural sharing means `v2 = v1.append(x)` shares most of `v1`'s memory.
- Perfect for undo/redo, event sourcing, concurrent programming, and functional patterns.
- Python's built-in `frozenset` and `tuple` are immutable but not structurally sharing.
- Redux (JavaScript) uses the same concept for state management.

---

# QUICK REFERENCE: Interview Cheat Sheet

| Topic | One-liner | Key interview point |
|---|---|---|
| Decorators | Wrap functions to add behavior | Always use `@functools.wraps` |
| Metaclasses | Classes that create classes | Use `__init_subclass__` instead when possible |
| Descriptors | Control attribute access | `__get__`/`__set__`/`__delete__` protocol |
| `__getattr__` | Fallback for missing attributes | Never recurse — use `object.__setattr__` |
| Threading | Concurrent I/O, shared memory | GIL limits CPU parallelism |
| Multiprocessing | True CPU parallelism | No shared memory — use Queue/Pipe |
| Asyncio | Many I/O tasks, single thread | Don't mix blocking and async |
| concurrent.futures | High-level thread/process pool | `as_completed` for first-done patterns |
| ABC | Enforced interface contracts | Can't instantiate if abstract methods missing |
| Dataclass | Auto-generate boilerplate | `field(default_factory=...)` for mutables |
| `__new__` | Object creation (before `__init__`) | Singleton, immutable subclasses, caching |
| MRO | Method lookup order | C3 linearization, `super()` follows MRO |
| Context managers | Guaranteed cleanup | `__exit__` always runs, `True` suppresses exc |
| Iterators | Produce values on demand | `__iter__` returns self, `__next__` or StopIteration |
| Maybe monad | Handle None without if/else | Chain `.map()` calls |
| Memoization | Cache function results | `lru_cache`, TTL for time-sensitive data |
| Trampolining | Deep recursion without stack overflow | Return lambdas instead of recursive calls |
| Generators | Lazy sequences | `yield` suspends, resumes on `next()` |
| Reference counting | Immediate memory reclamation | Circular refs need cyclic GC |
| `__slots__` | Fixed attributes, less memory | 3-5x memory reduction for many instances |
| weakref | References that don't block GC | Use in caches to avoid memory leaks |
| Pydantic | Type-validated data models | Core to FastAPI, v2 is Rust-powered |
| secrets | Cryptographic randomness | Never use `random` for security |
| SQL injection | Parameterized queries | Never concatenate user input into SQL |
| pytest | Auto-discovery, fixtures, markers | `conftest.py` for shared fixtures |
| hypothesis | Property-based test generation | Finds edge cases automatically, shrinks examples |
| mock.patch | Replace dependencies in tests | Target where it's USED, not defined |
| pdb | Interactive debugger | `breakpoint()` is modern standard |
| mypy | Static type checking | `--strict` for maximum enforcement |
| lru_cache | Memoize function results | Only works with hashable arguments |
| cProfile | Find performance bottlenecks | Sort by `cumulative` to find the root cause |
| Numba | JIT compile numeric code | First call is slow (compilation), then fast |
| heapq | Priority queue / min-heap | O(1) min, O(log n) push/pop |
| Trie | Prefix-based lookups | O(m) search where m = string length |
| Bloom filter | Probabilistic membership test | No false negatives, possible false positives |
| Persistent DS | Immutable + structural sharing | Perfect for undo/redo, concurrent state |

---

*All examples are Python 3.8+ compatible. Run tests with `python -m pytest`.*