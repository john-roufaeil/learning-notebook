# L — Liskov Substitution Principle (LSP)
### *"If S is a subtype of T, then objects of type T may be replaced with objects of type S without altering any of the desirable properties of the program."* — Barbara Liskov, 1987

---

## Table of Contents
1. [The Pain — When Inheritance Goes Wrong](#the-pain--when-inheritance-goes-wrong)
2. [What LSP Actually Means in Plain English](#what-lsp-actually-means-in-plain-english)
3. [The "Is-A" vs "Behaves-Like-A" Trap](#the-is-a-vs-behaves-like-a-trap)
4. [The Classic Violation: Square inheriting Rectangle](#the-classic-violation-square-inheriting-rectangle)
5. [Example 2 — The Bird that Cannot Fly](#example-2--the-bird-that-cannot-fly)
6. [Example 3 — Read-Only File in a File System](#example-3--read-only-file-in-a-file-system)
7. [Example 4 — Premium User That Breaks User Contracts](#example-4--premium-user-that-breaks-user-contracts)
8. [Real World Scenario — E-Commerce Product Catalog](#real-world-scenario--e-commerce-product-catalog)
9. [The 5 Rules of LSP](#the-5-rules-of-lsp)
10. [How to Fix LSP Violations](#how-to-fix-lsp-violations)
11. [LSP and Interfaces](#lsp-and-interfaces)
12. [Common Misunderstandings](#common-misunderstandings)
13. [Summary](#summary)

---

## The Pain — When Inheritance Goes Wrong

You're a developer. You see a `Bird` class with a `fly()` method. You need to add an `Ostrich`. Ostrich is a bird. So naturally:

```python
class Ostrich(Bird):
    def fly(self):
        raise Exception("Ostriches can't fly!")
```

Seems reasonable, right? Until your colleague writes this:

```python
def make_birds_fly(birds: list[Bird]):
    for bird in birds:
        bird.fly()  # 💥 CRASHES on Ostrich
```

The function accepted `list[Bird]`. It followed the rules. It called `fly()` — which every `Bird` is supposed to support. And it blew up.

This is an LSP violation. The `Ostrich` subclass **broke the expectations** of any code that used the `Bird` base class. Code that worked perfectly with `Bird` now crashes when given an `Ostrich`.

That's the pain: **you thought inheritance was helping you, but it was secretly setting traps.**

The deeper, scarier problem: these bugs don't show up until runtime. You can't easily spot them by reading the code. You might ship a working system, and only discover the crash six months later when an edge case hits production.

---

## What LSP Actually Means in Plain English

Barbara Liskov's formal definition is dense, but here's the plain English version:

> **If you have code that works with a base class, it must work correctly with any subclass — without knowing which subclass it's dealing with, and without any special handling.**

Think of it as the **substitution test**:
> Can I replace every occurrence of the parent class with a child class, and have the program behave exactly as expected?

If YES → LSP is satisfied.
If NO → LSP is violated.

The key phrase: **"without altering any desirable properties."** This means:
- No exceptions where there weren't before
- No returning null where a value was expected
- No doing nothing when something was expected
- No violating preconditions or postconditions

The simplest mental model: **a subclass should be a fully functional replacement for its parent, not a trap waiting to explode.**

---

## The "Is-A" vs "Behaves-Like-A" Trap

Here's the most common source of LSP violations: confusing **real-world "is-a"** with **behavioral "is-a"**.

In the real world:
- A Square IS-A Rectangle ✅ (mathematically true)
- An Ostrich IS-A Bird ✅ (biologically true)
- A ReadOnlyList IS-A List ✅ (conceptually true)

But in code, "is-a" must mean **"behaves like a"** — it must fulfill every behavioral contract the parent establishes.

- A Square does NOT behave like a Rectangle (you can't set width/height independently)
- An Ostrich does NOT behave like a fly-capable Bird
- A ReadOnlyList does NOT behave like a full List (you can't add/remove elements)

**Real-world taxonomy ≠ code hierarchy.** This is the trap. You use inheritance because something "is-a" kind of thing in English, but the behavioral contracts don't match.

---

## The Classic Violation: Square inheriting Rectangle

This is the most famous LSP example because it's so counterintuitive. A square IS mathematically a rectangle. But inheriting from Rectangle in code breaks LSP.

### ❌ BAD — Square inherits Rectangle

```python
class Rectangle:
    def __init__(self, width: float, height: float):
        self._width = width
        self._height = height

    def set_width(self, width: float):
        self._width = width

    def set_height(self, height: float):
        self._height = height

    def area(self) -> float:
        return self._width * self._height


class Square(Rectangle):
    # A square must always have equal sides
    # So we "override" both setters to enforce this
    def set_width(self, width: float):
        self._width = width
        self._height = width  # Force height to match

    def set_height(self, height: float):
        self._height = height
        self._width = height  # Force width to match
```

Now look at this perfectly reasonable function:

```python
def test_rectangle_area(rect: Rectangle):
    rect.set_width(5)
    rect.set_height(10)
    expected_area = 50  # 5 * 10
    actual_area = rect.area()
    assert actual_area == expected_area, f"Expected 50, got {actual_area}"

# Works fine with a Rectangle
test_rectangle_area(Rectangle(1, 1))  # ✅ Passes

# FAILS with a Square!
test_rectangle_area(Square(1, 1))
# set_width(5) → both sides become 5
# set_height(10) → both sides become 10 (overrides the width!)
# area() = 10 * 10 = 100 ≠ 50
# 💥 AssertionError: Expected 50, got 100
```

The function did nothing wrong. It received a `Rectangle`. It called standard `Rectangle` methods. But `Square` broke its expectations. This is a textbook LSP violation.

### ✅ GOOD — Separate classes, shared interface for what they have in common

```python
from abc import ABC, abstractmethod

# Abstract base: only describes what ALL shapes share
class Shape(ABC):
    @abstractmethod
    def area(self) -> float:
        pass

# Rectangle has independent width and height
class Rectangle(Shape):
    def __init__(self, width: float, height: float):
        self.width = width
        self.height = height

    def area(self) -> float:
        return self.width * self.height

# Square has a single side — completely independent class
class Square(Shape):
    def __init__(self, side: float):
        self.side = side

    def area(self) -> float:
        return self.side ** 2

# Now this function works perfectly with BOTH — no surprises
def print_area(shape: Shape):
    print(f"Area: {shape.area()}")

print_area(Rectangle(5, 10))  # Area: 50
print_area(Square(5))          # Area: 25
```

Both `Rectangle` and `Square` are `Shape`s. But they're not in a parent-child relationship — they're siblings. No traps. No surprises.

---

## Example 2 — The Bird that Cannot Fly

Back to our ostrich problem. Let's design this properly.

### ❌ BAD

```python
class Bird:
    def fly(self):
        print(f"{self.__class__.__name__} is flying")

class Eagle(Bird):
    pass  # Eagles fly — fine

class Penguin(Bird):
    def fly(self):
        raise NotImplementedError("Penguins can't fly!")  # 💥 LSP violation

class Ostrich(Bird):
    def fly(self):
        raise NotImplementedError("Ostriches can't fly!")  # 💥 LSP violation

def make_all_birds_migrate(birds: list[Bird]):
    for bird in birds:
        bird.fly()  # Crashes on Penguin and Ostrich

make_all_birds_migrate([Eagle(), Penguin()])  # 💥 Runtime error
```

### ✅ GOOD — Separate the "can fly" behavior into an interface

```python
from abc import ABC, abstractmethod

# All birds share these traits
class Bird(ABC):
    @abstractmethod
    def eat(self):
        pass

    @abstractmethod
    def sleep(self):
        pass

# Separate interface for flying — not all birds implement this
class Flyable(ABC):
    @abstractmethod
    def fly(self):
        pass

# Flying birds implement BOTH Bird and Flyable
class Eagle(Bird, Flyable):
    def eat(self):
        print("Eagle hunts fish")

    def sleep(self):
        print("Eagle sleeps in nest")

    def fly(self):
        print("Eagle soars at high altitude")

class Sparrow(Bird, Flyable):
    def eat(self):
        print("Sparrow eats seeds")

    def sleep(self):
        print("Sparrow sleeps in bush")

    def fly(self):
        print("Sparrow flutters quickly")

# Non-flying birds only implement Bird
class Penguin(Bird):
    def eat(self):
        print("Penguin eats krill")

    def sleep(self):
        print("Penguin huddles with colony")

class Ostrich(Bird):
    def eat(self):
        print("Ostrich eats plants")

    def sleep(self):
        print("Ostrich rests on ground")

# ✅ These functions only accept what makes sense
def feed_all_birds(birds: list[Bird]):
    for bird in birds:
        bird.eat()  # All birds can eat — safe

def coordinate_air_migration(flyers: list[Flyable]):
    for flyer in flyers:
        flyer.fly()  # Only called on birds that CAN fly — safe

# Usage
all_birds = [Eagle(), Sparrow(), Penguin(), Ostrich()]
flying_birds = [Eagle(), Sparrow()]

feed_all_birds(all_birds)           # ✅ Works for all
coordinate_air_migration(flying_birds)  # ✅ Only the ones that fly
```

Now there's no way to accidentally call `fly()` on a Penguin. The type system prevents it. LSP is satisfied because every `Bird` truly behaves like a `Bird`, and every `Flyable` truly behaves like a `Flyable`.

---

## Example 3 — Read-Only File in a File System

This is a very real scenario in system programming.

### ❌ BAD

```javascript
class File {
  read() { /* read file content */ }
  write(data) { /* write data to file */ }
  delete() { /* delete the file */ }
}

class ReadOnlyFile extends File {
  write(data) {
    throw new Error("This file is read-only!"); // 💥 LSP violation
  }

  delete() {
    throw new Error("This file is read-only!"); // 💥 LSP violation
  }
}

// This code expects a File — a contract that includes write()
function backupFile(file: File, newContent: string) {
  file.write(newContent);  // 💥 Explodes on ReadOnlyFile
}

// The caller had no idea — they received a "File"!
const configFile = new ReadOnlyFile("config.json");
backupFile(configFile, "backup data");  // 💥 CRASH
```

### ✅ GOOD — Model the hierarchy correctly

```javascript
// Base: only what ALL files can do
class File {
  constructor(path) {
    this.path = path;
  }

  read() {
    return fs.readFileSync(this.path, "utf8");
  }

  getMetadata() {
    return fs.statSync(this.path);
  }
}

// Mutable files extend File with write capabilities
class MutableFile extends File {
  write(data) {
    fs.writeFileSync(this.path, data);
  }

  delete() {
    fs.unlinkSync(this.path);
  }
}

// ReadOnlyFile IS a File — it fulfills ALL File contracts
class ReadOnlyFile extends File {
  // read() is inherited — works perfectly
  // getMetadata() is inherited — works perfectly
  // No write(), no delete() — because ReadOnlyFile doesn't promise those
}

// Functions only ask for what they actually need
function displayFileContent(file: File) {
  console.log(file.read());  // ✅ Works for ALL File types
}

function saveUserData(file: MutableFile, data: string) {
  file.write(data);  // ✅ Only called on files that support writing
}

// Usage
const configFile = new ReadOnlyFile("config.json");
const dataFile = new MutableFile("data.json");

displayFileContent(configFile);  // ✅
displayFileContent(dataFile);    // ✅
saveUserData(dataFile, "{}");    // ✅
// saveUserData(configFile, "{}"); // ✅ Won't even compile in TypeScript!
```

---

## Example 4 — Premium User That Breaks User Contracts

This one is very common in web applications.

### ❌ BAD

```python
class User:
    def __init__(self, name, email):
        self.name = name
        self.email = email

    def get_monthly_download_limit(self) -> int:
        return 100  # Regular users get 100 downloads/month

    def can_access_feature(self, feature: str) -> bool:
        return feature in ["basic_search", "profile_edit"]

class PremiumUser(User):
    def get_monthly_download_limit(self) -> int:
        return float("inf")  # ♾️ Unlimited — but returns a float, not int!

    def can_access_feature(self, feature: str) -> bool:
        return True  # Premium users access everything

# The problem:
def enforce_download_limit(user: User, current_downloads: int) -> bool:
    limit = user.get_monthly_download_limit()
    remaining = limit - current_downloads  # Works for int User
    if remaining < 0:  # 💥 float("inf") - 50 = inf, never < 0, fine actually
        return False
    # But this breaks:
    formatted = f"You have {remaining} downloads remaining"  # inf displays weirdly
    # And division?
    daily_budget = remaining // 30  # 💥 inf // 30 = inf — may break downstream code
    return True
```

The return type contract was `int`. `PremiumUser` returns `float("inf")`. Any code that does integer operations on that return value is now broken.

### ✅ GOOD

```python
class User:
    def __init__(self, name, email):
        self.name = name
        self.email = email

    def get_monthly_download_limit(self) -> int:
        return 100

    def has_unlimited_downloads(self) -> bool:
        return False

    def can_access_feature(self, feature: str) -> bool:
        return feature in ["basic_search", "profile_edit"]

class PremiumUser(User):
    def get_monthly_download_limit(self) -> int:
        return 999_999  # A large practical number, still an int

    def has_unlimited_downloads(self) -> bool:
        return True  # NEW method — extends behavior, doesn't break existing contract

    def can_access_feature(self, feature: str) -> bool:
        return True

# Now the check is clean and correct
def enforce_download_limit(user: User, current_downloads: int) -> bool:
    if user.has_unlimited_downloads():
        return True
    limit = user.get_monthly_download_limit()
    remaining = limit - current_downloads  # Always an int — safe
    daily_budget = remaining // 30          # Always works
    print(f"You have {remaining} downloads remaining")
    return remaining > 0
```

---

## Real World Scenario — E-Commerce Product Catalog

```python
class Product:
    def __init__(self, name: str, price: float, stock: int):
        self.name = name
        self.price = price
        self.stock = stock

    def get_price(self) -> float:
        return self.price

    def is_in_stock(self) -> bool:
        return self.stock > 0

    def reserve(self, quantity: int):
        if quantity > self.stock:
            raise ValueError("Not enough stock")
        self.stock -= quantity

class DigitalProduct(Product):
    """Digital products: no physical stock, always available"""

    def __init__(self, name: str, price: float):
        super().__init__(name, price, stock=999_999)

    def is_in_stock(self) -> bool:
        return True  # Always in stock — correct overriding

    def reserve(self, quantity: int):
        pass  # No physical stock to reduce — but doesn't crash!
        # This is key: it does nothing rather than raising an error
        # The contract says "reserve some" — digital products fulfill this
        # by simply acknowledging the reservation

class SaleProduct(Product):
    """Product on sale — overrides price, respects all other contracts"""

    def __init__(self, product: Product, discount_pct: float):
        super().__init__(product.name, product.price, product.stock)
        self.discount_pct = discount_pct

    def get_price(self) -> float:
        return self.price * (1 - self.discount_pct)  # Returns float — LSP satisfied

# This function works correctly with ALL product types
def add_to_cart(product: Product, quantity: int, cart: list):
    if not product.is_in_stock():
        print(f"Sorry, {product.name} is out of stock")
        return

    product.reserve(quantity)
    cart.append({
        "product": product.name,
        "quantity": quantity,
        "price": product.get_price() * quantity
    })
    print(f"Added {quantity}x {product.name} to cart")

# All substitutable without any special handling
cart = []
add_to_cart(Product("Book", 29.99, 10), 1, cart)           # ✅
add_to_cart(DigitalProduct("eBook", 9.99), 1, cart)         # ✅
add_to_cart(SaleProduct(Product("Shirt", 49.99, 5), 0.2), 2, cart)  # ✅
```

---

## The 5 Rules of LSP

These are the formal behavioral conditions a subclass must meet to satisfy LSP:

### Rule 1: Contravariance of Method Arguments (Preconditions cannot be strengthened)
A subclass method must accept at least as broad a range of inputs as the parent.
- Parent accepts `number` → child must also accept `number` (not just `positive_number`)
- You can't restrict what a method accepts in a subclass.

### Rule 2: Covariance of Return Types (Postconditions cannot be weakened)
A subclass method must return a type that is at least as specific as the parent.
- Parent returns `Animal` → child can return `Dog` (more specific) ✅
- Parent returns `Dog` → child cannot return `Animal` (less specific) ❌

### Rule 3: Exception Rule (No new exceptions)
A subclass must not throw exceptions that the parent doesn't throw (unless they're subtypes of the parent's exceptions).
- Parent's `save()` throws `DatabaseException` → child can throw `SqlException extends DatabaseException` ✅
- Parent's `save()` throws nothing → child cannot throw `IOException` ❌

### Rule 4: History Rule (Don't break invariants)
A subclass must not modify state in a way the parent would not allow.
- If the parent says "balance can never go negative", the child must maintain this.
- If the parent says "list is always sorted after insertion", the child must maintain this.

### Rule 5: Behavioral Compatibility (Don't throw where parent wouldn't)
Never throw `NotImplementedException` or similar from an override. If you can't implement it, the design is wrong — not the code.

---

## How to Fix LSP Violations

When you discover an LSP violation, you have three options:

### Fix 1: Flatten the hierarchy — remove the inheritance
If the subclass can't truly substitute for the parent, maybe they shouldn't be related by inheritance.

### Fix 2: Split the interface
Use multiple smaller interfaces. Only have classes implement the interfaces they can truly fulfill (see the Flyable example above — also the core of Interface Segregation Principle).

### Fix 3: Invert the relationship
Instead of `Square extends Rectangle`, have `Rectangle extends Shape` AND `Square extends Shape` — siblings, not parent-child.

### Fix 4: Use composition instead of inheritance
Instead of inheriting, have the class *contain* the other as a field and delegate only the methods that make sense.

```python
# Instead of ReadOnlyFile extends File (and breaking write())
class ReadOnlyFile:
    def __init__(self, path):
        self._file = File(path)  # Contains a File

    def read(self):
        return self._file.read()  # Delegates only what makes sense

    # No write(), no delete() — not even inherited, not even broken
```

---

## LSP and Interfaces

LSP applies equally to interfaces. If a class implements an interface, it must fully and correctly implement every method in that interface. If it can't, it shouldn't implement that interface.

```typescript
interface Saveable {
  save(): Promise<void>;
  load(): Promise<Data>;
  delete(): Promise<void>;
}

// ❌ BAD: ReadOnlyStorage "implements" Saveable but can't fulfill it
class ReadOnlyStorage implements Saveable {
  async save() { throw new Error("Read-only!"); }    // 💥 LSP violation
  async load() { return await db.load(); }
  async delete() { throw new Error("Read-only!"); }  // 💥 LSP violation
}

// ✅ GOOD: Split the interface
interface Loadable {
  load(): Promise<Data>;
}

interface Persistable extends Loadable {
  save(): Promise<void>;
  delete(): Promise<void>;
}

class ReadOnlyStorage implements Loadable {
  async load() { return await db.load(); }  // Fulfills the full contract
}

class ReadWriteStorage implements Persistable {
  async load() { return await db.load(); }
  async save() { await db.save(); }
  async delete() { await db.delete(); }
}
```

---

## Common Misunderstandings

### ❌ Myth 1: "LSP just means 'don't throw NotImplementedException'"
**Deeper than that.** You can break LSP without throwing exceptions — by returning wrong types, breaking invariants, or doing nothing when something was expected.

### ❌ Myth 2: "If the class inherits the method, it satisfies LSP"
**Wrong.** Inheriting a method and properly implementing the behavioral contract are different things. Doing nothing in an override (silently ignoring a `write()` call, for example) may also be an LSP violation if callers expect the write to actually happen.

### ❌ Myth 3: "LSP is only about runtime exceptions"
**Wrong.** LSP violations can be silent — returning wrong values, corrupting state, or producing incorrect results that only manifest much later.

### ❌ Myth 4: "Override = LSP violation"
**Wrong.** Overriding methods is completely fine — that's polymorphism. The key is that the override must still satisfy the behavioral contract of the parent.

---

## Summary

| Concept | Description |
|---|---|
| **The Principle** | Subtypes must be substitutable for their parent types |
| **The Problem It Solves** | Inheritance hierarchies that explode at runtime in unexpected ways |
| **The Test** | Can I replace every Parent reference with a Subclass reference and still have things work correctly? |
| **The Root Cause** | Confusing real-world "is-a" with behavioral "behaves-like-a" |
| **Classic Violation** | Square extends Rectangle, Ostrich extends Flyable Bird |
| **The Fix** | Flatten hierarchy, split interfaces, or use composition |
| **The Benefit** | Predictable, trustworthy inheritance — polymorphism that actually works |

> **Practical rule of thumb**: Before writing `class Child extends Parent`, ask: "Can I pass a `Child` anywhere a `Parent` is expected and have everything work correctly?" If you can't answer "yes" without caveats, rethink the inheritance.