# Object-Oriented Programming: A Deep Dive

---

## 1. Why Does OOP Exist?

Before OOP, the dominant paradigm was **procedural programming** — code was a sequence of instructions, **data and functions were separate**, and as programs grew larger, managing complexity became a nightmare. If you had 10,000 lines of code where any function could modify any data at any time, debugging and extending that code was chaotic.

OOP was born to solve a fundamental problem: **how do you model complex systems in a way that mirrors how humans think about the real world?**

Humans think in terms of *things* that have *properties* and *behaviors*. A `Car` has a color, a speed, and can `accelerate()` or `brake()`. OOP lets you model software the same way — as a collection of interacting objects.

### The real practical reasons we need OOP:

- **Managing complexity**: Large codebases become manageable when split into self-contained objects.
- **Reusability**: Write a class once, use it everywhere, extend it without rewriting it.
- **Maintainability**: Changing the internals of a class doesn't break code that uses it (if done properly).
- **Collaboration**: Different developers can work on different classes independently.
- **Modeling the real world**: Software systems often mirror real-world domains (users, orders, products), and OOP maps naturally to that.

OOP is not the only way to solve these problems — functional programming, for example, tackles them differently — but OOP became dominant because it matches the way most people reason about systems.

---

## 2. Why Java Doesn't Have Multiple Inheritance (But Python Does)

### The Diamond Problem

Multiple inheritance means a class can inherit from more than one parent class. The problem is: what happens when two parent classes both define the same method?

```
      A
     / \
    B   C
     \ /
      D
```

If `B` and `C` both override a method from `A`, and `D` inherits from both `B` and `C`, which version does `D` use? This is the **diamond problem**.

### Java's Decision: No Multiple Inheritance of Classes

Java's designers decided the risk of ambiguity and complexity was too high. Java prevents the diamond problem by **disallowing multiple inheritance of classes entirely**. A class can only extend one other class.

However, Java compensates for this with **interfaces** — which allow a class to implement multiple contracts. Before Java 8, interfaces had no method bodies (only signatures), so there was no conflict. After Java 8, `default` methods were added to interfaces, which brought the diamond problem back partially — but Java resolves this by requiring the implementing class to explicitly override the conflicting method.

```java
// Java: ONE class parent only
class Dog extends Animal { } // OK
class Dog extends Animal, Pet { } // COMPILE ERROR
```

### Python's Decision: Multiple Inheritance with MRO

Python allows multiple inheritance and resolves the diamond problem using the **Method Resolution Order (MRO)** — specifically, the **C3 Linearization algorithm**. When Python looks up a method, it follows a deterministic, predictable order across the class hierarchy.

```python
class A:
    def greet(self): print("A")

class B(A):
    def greet(self): print("B")

class C(A):
    def greet(self): print("C")

class D(B, C):
    pass

D().greet()  # Prints "B" — Python uses MRO (left to right, depth first)
print(D.__mro__)  # (D, B, C, A, object)
```

### The Real Distinction Made Here

| | Java | Python |
|---|---|---|
| Multiple inheritance of classes | ❌ Not allowed | ✅ Allowed |
| Resolution mechanism | Enforced at compile time | MRO (C3 linearization) at runtime |
| Compensation mechanism | Interfaces (multiple allowed) | `super()` + MRO |
| Philosophy | "Prevent errors by design" | "We trust the programmer" |

Java prioritizes **safety and simplicity** — it would rather restrict you than let you shoot yourself in the foot. Python prioritizes **expressiveness and developer freedom** — it gives you the tool and trusts you to use it correctly.

Neither approach is universally superior. They reflect different philosophies about the relationship between the language and the programmer.

---

## 3. The Real Distinction Between Abstraction and Encapsulation

These two are the most commonly confused OOP concepts because they are deeply related. Here is the clearest way to distinguish them:

### Encapsulation — *Hiding HOW something works*

Encapsulation is about **bundling data and the methods that operate on that data together**, and **restricting direct access to the internal state**. It's a protective mechanism.

Think of it like a capsule (hence the name): the internal state is sealed inside. The outside world can only interact through a controlled interface.

```python
class BankAccount:
    def __init__(self, balance):
        self.__balance = balance  # Private — hidden from outside

    def deposit(self, amount):
        if amount > 0:
            self.__balance += amount

    def get_balance(self):
        return self.__balance

account = BankAccount(1000)
account.deposit(500)
print(account.get_balance())  # 1500
print(account.__balance)      # AttributeError — you can't touch it directly
```

The `__balance` is encapsulated. You can't mess with it directly — you must go through `deposit()` and `get_balance()`. This protects the integrity of the data.

**Encapsulation answers**: *Who can touch this data, and how?*

### Abstraction — *Hiding WHAT is irrelevant*

Abstraction is about **exposing only what is necessary** and **hiding complexity that the user of the code doesn't need to care about**. It's a design concept.

When you call `account.deposit(500)`, you don't care about the validation logic, the internal variable name, or how the balance is stored. That complexity is abstracted away — you just see a clean interface.

```python
from abc import ABC, abstractmethod

class Shape(ABC):
    @abstractmethod
    def area(self) -> float:
        pass  # We don't care HOW it's calculated yet

class Circle(Shape):
    def __init__(self, radius):
        self.radius = radius

    def area(self) -> float:
        return 3.14159 * self.radius ** 2

class Rectangle(Shape):
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def area(self) -> float:
        return self.width * self.height
```

The `Shape` abstraction says "every shape has an area" — but says nothing about how. That's abstraction. Each subclass fills in the concrete details.

**Abstraction answers**: *What should the outside world know and see?*

### The Crisp Distinction

| | Encapsulation | Abstraction |
|---|---|---|
| Focus | Protecting internal state | Simplifying the interface |
| Mechanism | Access modifiers (`private`, `__`) | Abstract classes, interfaces |
| Question answered | "Can the outside world change this?" | "Does the outside world need to know this?" |
| Example | `self.__balance` hidden behind methods | `Shape.area()` defined without implementation |
| Goal | Data integrity and security | Reduced complexity, cleaner design |

**One-liner to remember**: Encapsulation is about *protection*. Abstraction is about *simplification*.

They work together: you abstract the *what* (every shape has an area), and you encapsulate the *how* (the Circle's radius is private implementation detail).

---

## 4. Overriding vs. Overwriting vs. Overloading

### Overriding (Runtime Polymorphism)

**Overriding** happens when a subclass provides a new implementation of a method that already exists in its parent class. The method signature (name, parameters) stays the same.

```python
class Animal:
    def speak(self):
        return "..."

class Dog(Animal):
    def speak(self):       # Overrides Animal.speak
        return "Woof!"

class Cat(Animal):
    def speak(self):       # Overrides Animal.speak
        return "Meow!"

animals = [Dog(), Cat(), Animal()]
for a in animals:
    print(a.speak())      # Woof! / Meow! / ...
```

The correct `speak()` is chosen at **runtime** based on the actual type of the object. This is **runtime polymorphism**.

### Overwriting (Not a Formal OOP Term)

**Overwriting** is not a formal OOP concept. It generally refers to simply replacing a value or function in the simplest sense — reassigning a variable or redefining something in the same scope. In Python:

```python
def greet():
    print("Hello")

def greet():           # Overwrites the previous greet function
    print("Hi there")

greet()  # "Hi there" — the first definition is simply gone
```

This is not a class-level concept. It's just redefinition. It doesn't involve inheritance, polymorphism, or class hierarchies. Some people confuse it with overriding, but they are different: overriding is intentional, structured, and preserves the parent's version (via `super()`). Overwriting just replaces the thing entirely.

### Overloading (Compile-time Polymorphism)

**Overloading** means defining multiple methods with the **same name but different parameters** (different number or types). The correct version is chosen at **compile time** (or based on argument types).

Java supports this natively:

```java
// Java
class Calculator {
    int add(int a, int b) { return a + b; }
    double add(double a, double b) { return a + b; }
    int add(int a, int b, int c) { return a + b + c; }
}
```

**Python does NOT support traditional overloading.** If you define the same method name twice, the second definition simply overwrites the first (see above). Python compensates with:

```python
# Python — using default arguments or *args
def add(a, b, c=0):
    return a + b + c

add(1, 2)      # 3
add(1, 2, 3)   # 6

# Or using functools singledispatch for type-based dispatch
from functools import singledispatch

@singledispatch
def process(value):
    raise NotImplementedError

@process.register(int)
def _(value):
    return f"Integer: {value}"

@process.register(str)
def _(value):
    return f"String: {value}"

process(42)     # "Integer: 42"
process("hi")   # "String: hi"
```

### Which One Is Polymorphism?

**Both overriding and overloading are forms of polymorphism.**

- **Overriding** → **Runtime polymorphism** (dynamic dispatch): the method called is determined at runtime.
- **Overloading** → **Compile-time polymorphism** (static dispatch): the method version is determined at compile time based on the argument types/count.

**Overwriting is NOT polymorphism.** It's just replacement.

In Python specifically, the dominant form of polymorphism is **overriding** (runtime polymorphism), since Python is dynamically typed and doesn't have classical compile-time overloading.

---

## 5. Abstract Class vs. Interface

### Interface

An interface is a **pure contract**. It defines *what* a class must do, but provides zero implementation. Every method is abstract. A class can implement multiple interfaces.

In Java, the `interface` keyword explicitly defines this:

```java
interface Flyable {
    void fly();       // No body — just a signature
    void land();
}

interface Swimmable {
    void swim();
}

class Duck implements Flyable, Swimmable {
    public void fly() { System.out.println("Duck flying"); }
    public void land() { System.out.println("Duck landing"); }
    public void swim() { System.out.println("Duck swimming"); }
}
```

Python doesn't have a native `interface` keyword, but you can simulate one using `ABC` with all abstract methods and no concrete implementation.

### Abstract Class

An abstract class is **a class that cannot be instantiated** and may contain a mix of abstract methods (no implementation) and concrete methods (with implementation). It serves as a base template.

```python
from abc import ABC, abstractmethod

class Vehicle(ABC):
    def __init__(self, brand):
        self.brand = brand         # Concrete attribute

    @abstractmethod
    def fuel_type(self) -> str:   # Must be implemented by subclasses
        pass

    def describe(self):            # Concrete method — shared by all
        return f"{self.brand} runs on {self.fuel_type()}"

class ElectricCar(Vehicle):
    def fuel_type(self) -> str:
        return "electricity"

class GasCar(Vehicle):
    def fuel_type(self) -> str:
        return "gasoline"

v = Vehicle("Toyota")   # TypeError — cannot instantiate abstract class
e = ElectricCar("Tesla")
print(e.describe())      # "Tesla runs on electricity"
```

### The Key Differences

| | Abstract Class | Interface |
|---|---|---|
| Can have concrete methods | ✅ Yes | ❌ No (pure contract) |
| Can have state (attributes) | ✅ Yes | ❌ No |
| Multiple inheritance | ❌ Typically one (in Java) | ✅ A class can implement many |
| Purpose | Shared base implementation + template | Define a capability contract |
| Python keyword | `ABC` with `@abstractmethod` | `ABC` with all `@abstractmethod` |
| When to use | "Is-a" relationship with shared code | "Can-do" capability contract |

### When to Use Which

- Use an **abstract class** when subclasses share a significant amount of common code and have a clear "is-a" relationship (a `Dog` is an `Animal`).
- Use an **interface** when you want to define a capability that unrelated classes can share (a `Duck` and an `Airplane` can both be `Flyable`, even though they're unrelated).

---

## 6. The Four Pillars of OOP — Deep Dive in Python

---

### Pillar 1: Encapsulation

**Core idea**: Bundle related data and methods together, and control access to the internal state.

Python uses naming conventions (not hard enforcement) for access control:

- `name` — Public: accessible anywhere
- `_name` — Protected (convention): "don't touch from outside unless you know what you're doing"
- `__name` — Private (name mangling): Python renames it to `_ClassName__name`, making accidental access very hard

```python
class Employee:
    def __init__(self, name, salary):
        self.name = name           # Public
        self._department = "HR"    # Protected (convention)
        self.__salary = salary     # Private (name-mangled)

    def get_salary(self):
        return self.__salary

    def give_raise(self, amount):
        if amount > 0:
            self.__salary += amount
        else:
            raise ValueError("Raise must be positive")

emp = Employee("Alice", 50000)
print(emp.name)              # "Alice" — works fine
print(emp._department)       # "HR" — works, but flagged by convention
print(emp.__salary)          # AttributeError!
print(emp._Employee__salary) # 50000 — name mangling bypassed, but don't do this
print(emp.get_salary())      # 50000 — proper access
```

**Python's approach**: Python doesn't enforce private access at the language level — it relies on the developer's discipline. The mangling is a deterrent, not a wall.

**Properties** are Python's elegant way to add getters/setters:

```python
class Temperature:
    def __init__(self, celsius):
        self.__celsius = celsius

    @property
    def celsius(self):
        return self.__celsius

    @celsius.setter
    def celsius(self, value):
        if value < -273.15:
            raise ValueError("Temperature below absolute zero!")
        self.__celsius = value

    @property
    def fahrenheit(self):
        return self.__celsius * 9/5 + 32

t = Temperature(25)
print(t.celsius)     # 25
print(t.fahrenheit)  # 77.0
t.celsius = 30       # Uses setter
t.celsius = -300     # ValueError!
```

---

### Pillar 2: Abstraction

**Core idea**: Expose only what's necessary. Hide complexity behind clean interfaces.

Python implements abstraction through the `abc` module (Abstract Base Classes).

```python
from abc import ABC, abstractmethod

class DatabaseConnector(ABC):
    """Abstract base — defines the contract for all DB connectors."""

    @abstractmethod
    def connect(self, host: str, port: int) -> bool:
        pass

    @abstractmethod
    def query(self, sql: str) -> list:
        pass

    @abstractmethod
    def disconnect(self) -> None:
        pass

    def execute_with_retry(self, sql: str, retries: int = 3) -> list:
        """Concrete method — shared logic using the abstract interface."""
        for attempt in range(retries):
            try:
                return self.query(sql)
            except Exception as e:
                if attempt == retries - 1:
                    raise e
        return []

class PostgresConnector(DatabaseConnector):
    def connect(self, host, port):
        print(f"Connecting to Postgres at {host}:{port}")
        return True

    def query(self, sql):
        print(f"Executing: {sql}")
        return [{"id": 1, "name": "Alice"}]

    def disconnect(self):
        print("Disconnecting from Postgres")

class MongoConnector(DatabaseConnector):
    def connect(self, host, port):
        print(f"Connecting to MongoDB at {host}:{port}")
        return True

    def query(self, sql):
        print(f"Running MongoDB query: {sql}")
        return [{"_id": "abc", "name": "Bob"}]

    def disconnect(self):
        print("Disconnecting from MongoDB")

# The caller doesn't care which database — they just use the abstract interface
def run_report(db: DatabaseConnector, sql: str):
    db.connect("localhost", 5432)
    results = db.execute_with_retry(sql)
    db.disconnect()
    return results
```

The power here: `run_report` works with ANY `DatabaseConnector` implementation, now or in the future. The abstraction decouples the caller from the implementation.

---

### Pillar 3: Inheritance

**Core idea**: A class can acquire the properties and behaviors of another class, enabling code reuse and hierarchical relationships.

```python
class Animal:
    def __init__(self, name, age):
        self.name = name
        self.age = age

    def breathe(self):
        return f"{self.name} breathes"

    def speak(self):
        return "..."

    def __repr__(self):
        return f"{self.__class__.__name__}(name={self.name}, age={self.age})"

class Dog(Animal):
    def __init__(self, name, age, breed):
        super().__init__(name, age)     # Call parent's __init__
        self.breed = breed

    def speak(self):                    # Override
        return "Woof!"

    def fetch(self):                    # New behavior
        return f"{self.name} fetches the ball!"

class GuideDog(Dog):
    def __init__(self, name, age, breed, owner):
        super().__init__(name, age, breed)
        self.owner = owner

    def guide(self):
        return f"{self.name} guides {self.owner}"

rex = GuideDog("Rex", 4, "Labrador", "John")
print(rex.breathe())   # Inherited from Animal
print(rex.speak())     # "Woof!" — inherited from Dog (which overrides Animal)
print(rex.fetch())     # Inherited from Dog
print(rex.guide())     # GuideDog's own method
```

### Multiple Inheritance in Python

```python
class Flyable:
    def fly(self):
        return "Flying!"

class Swimmable:
    def swim(self):
        return "Swimming!"

class Duck(Animal, Flyable, Swimmable):
    def __init__(self, name, age):
        super().__init__(name, age)

    def speak(self):
        return "Quack!"

donald = Duck("Donald", 5)
print(donald.fly())    # Flying!
print(donald.swim())   # Swimming!
print(donald.speak())  # Quack!
print(Duck.__mro__)    # (Duck, Animal, Flyable, Swimmable, object)
```

**`super()` in Python** is not just "call the parent class" — it follows the MRO, which is critical in multiple inheritance:

```python
class A:
    def hello(self):
        print("A")

class B(A):
    def hello(self):
        super().hello()
        print("B")

class C(A):
    def hello(self):
        super().hello()
        print("C")

class D(B, C):
    def hello(self):
        super().hello()
        print("D")

D().hello()
# Output:
# A
# C
# B
# D
# MRO: D -> B -> C -> A -> object
```

Each `super()` call follows the MRO, not just "the immediate parent". This is how Python ensures every class in the hierarchy gets called exactly once.

---

### Pillar 4: Polymorphism

**Core idea**: Different objects can be treated through the same interface, and the correct behavior is chosen at runtime based on the object's actual type.

Python's dynamic typing makes polymorphism extremely natural — Python uses **duck typing**: "if it walks like a duck and quacks like a duck, it's a duck." You don't need a common base class to achieve polymorphism.

#### Runtime Polymorphism (Overriding)

```python
class Shape:
    def area(self):
        raise NotImplementedError

class Circle(Shape):
    def __init__(self, r): self.r = r
    def area(self): return 3.14159 * self.r ** 2

class Rectangle(Shape):
    def __init__(self, w, h): self.w, self.h = w, h
    def area(self): return self.w * self.h

class Triangle(Shape):
    def __init__(self, b, h): self.b, self.h = b, h
    def area(self): return 0.5 * self.b * self.h

shapes = [Circle(5), Rectangle(4, 6), Triangle(3, 8)]
for shape in shapes:
    print(f"{type(shape).__name__}: area = {shape.area():.2f}")
# Circle: area = 78.54
# Rectangle: area = 24.00
# Triangle: area = 12.00
```

#### Duck Typing — Polymorphism Without Inheritance

```python
class Dog:
    def speak(self):
        return "Woof!"

class Cat:
    def speak(self):
        return "Meow!"

class Robot:
    def speak(self):
        return "Beep boop."

# None of these share a base class — but they're polymorphic
def make_it_speak(thing):
    print(thing.speak())

for creature in [Dog(), Cat(), Robot()]:
    make_it_speak(creature)
# Woof! / Meow! / Beep boop.
```

#### Operator Overloading — Polymorphism on Operators

Python allows you to define how operators behave for your custom classes using **dunder methods** (double underscore):

```python
class Vector:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __add__(self, other):      # Polymorphism on +
        return Vector(self.x + other.x, self.y + other.y)

    def __mul__(self, scalar):     # Polymorphism on *
        return Vector(self.x * scalar, self.y * scalar)

    def __repr__(self):
        return f"Vector({self.x}, {self.y})"

v1 = Vector(1, 2)
v2 = Vector(3, 4)
print(v1 + v2)    # Vector(4, 6)
print(v1 * 3)     # Vector(3, 6)
```

The `+` operator is polymorphic: `1 + 2` (integers), `"a" + "b"` (strings), and `v1 + v2` (vectors) all use `+` but invoke entirely different logic depending on the type.

---

## 7. Summary Table: Everything in One Place

| Concept | What It Is | Python Mechanism |
|---|---|---|
| **Encapsulation** | Bundle data + methods, restrict access | `__private`, `_protected`, `@property` |
| **Abstraction** | Expose only what's needed, hide complexity | `ABC`, `@abstractmethod` |
| **Inheritance** | Reuse and extend behavior from parent classes | `class Child(Parent):`, `super()`, MRO |
| **Polymorphism** | Same interface, different behavior per type | Method overriding, duck typing, dunder methods |
| **Overriding** | Redefining a parent method in a subclass | `def same_method_name(self):` in subclass |
| **Overloading** | Same method name, different parameters | Not native; use default args or `singledispatch` |
| **Overwriting** | Simple redefinition (not an OOP concept) | Redefining a function/variable in same scope |
| **Abstract Class** | Partial template — some abstract, some concrete | `class MyABC(ABC):` with some `@abstractmethod` |
| **Interface** | Pure contract — all abstract, no implementation | `class MyInterface(ABC):` all `@abstractmethod` |
| **Multiple Inheritance** | Inherit from multiple parent classes | `class D(B, C):` — Python allows, Java doesn't |
| **MRO** | Order Python searches for methods | C3 linearization; inspect via `Class.__mro__` |
| **Duck Typing** | Polymorphism without shared base class | Python's dynamic typing — "if it has the method, use it" |


You can have:
- Abstraction without polymorphism (useless)
- Polymorphism without abstraction (duck typing)