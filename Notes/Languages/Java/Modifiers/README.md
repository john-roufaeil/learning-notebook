# All Java Modifiers - Complete Reference

## What Are Modifiers?

**Modifiers** are keywords that you add to variables, methods, classes, and other elements to change their behavior or properties.

## Two Categories of Modifiers

1. **Access Modifiers**: Control access level (visibility)
2. **Non-Access Modifiers**: Provide other functionality

---

# Part 1: Access Modifiers

Control **who can access** the class, method, or variable.

## The Four Access Modifiers

| Modifier | Class | Package | Subclass | World |
|----------|-------|---------|----------|-------|
| **public** | ✅ | ✅ | ✅ | ✅ |
| **protected** | ✅ | ✅ | ✅ | ❌ |
| **default** (no modifier) | ✅ | ✅ | ❌ | ❌ |
| **private** | ✅ | ❌ | ❌ | ❌ |

### 1. public - Accessible Everywhere

```java
// File: Person.java
public class Person {
    public String name;
    public int age;
    
    public void introduce() {
        System.out.println("Hi, I'm " + name);
    }
    
    public static void greet() {
        System.out.println("Hello!");
    }
}

// File: Main.java (different package)
import com.example.Person;

public class Main {
    public static void main(String[] args) {
        Person person = new Person();  // ✅ Can access - public class
        person.name = "Alice";         // ✅ Can access - public field
        person.introduce();            // ✅ Can access - public method
        Person.greet();                // ✅ Can access - public static method
    }
}
```

### 2. protected - Package + Subclasses

```java
// File: Animal.java (package: com.example.animals)
package com.example.animals;

public class Animal {
    protected String species;
    protected int age;
    
    protected void eat() {
        System.out.println("Eating...");
    }
}

// File: Dog.java (same package)
package com.example.animals;

public class Dog extends Animal {
    public void display() {
        species = "Canine";  // ✅ Can access - same package
        eat();               // ✅ Can access - same package
    }
}

// File: Cat.java (different package but subclass)
package com.example.pets;

import com.example.animals.Animal;

public class Cat extends Animal {
    public void display() {
        species = "Feline";  // ✅ Can access - subclass
        eat();               // ✅ Can access - subclass
    }
}

// File: Main.java (different package, not subclass)
package com.example.main;

import com.example.animals.Animal;

public class Main {
    public static void main(String[] args) {
        Animal animal = new Animal();
        // animal.species = "Test";  // ❌ CANNOT access - not subclass
        // animal.eat();             // ❌ CANNOT access - not subclass
    }
}
```

### 3. Default (Package-Private) - No Modifier

```java
// File: Helper.java (package: com.example.utils)
package com.example.utils;

class Helper {  // No access modifier = default
    String data;
    
    void process() {
        System.out.println("Processing...");
    }
}

// File: Utility.java (same package)
package com.example.utils;

public class Utility {
    public void use() {
        Helper helper = new Helper();  // ✅ Can access - same package
        helper.data = "test";          // ✅ Can access - same package
        helper.process();              // ✅ Can access - same package
    }
}

// File: Main.java (different package)
package com.example.main;

import com.example.utils.Helper;  // ❌ CANNOT import - not visible

public class Main {
    public static void main(String[] args) {
        // Helper helper = new Helper();  // ❌ CANNOT access
    }
}
```

### 4. private - Only Within Class

```java
public class BankAccount {
    private double balance;        // Only accessible within this class
    private String accountNumber;
    
    private void validateTransaction(double amount) {
        if (amount <= 0) {
            throw new IllegalArgumentException("Invalid amount");
        }
    }
    
    public void deposit(double amount) {
        validateTransaction(amount);  // ✅ Can call - same class
        balance += amount;            // ✅ Can access - same class
    }
    
    public double getBalance() {
        return balance;  // ✅ Can access - same class
    }
}

public class Main {
    public static void main(String[] args) {
        BankAccount account = new BankAccount();
        // account.balance = 1000;           // ❌ CANNOT access - private
        // account.validateTransaction(100); // ❌ CANNOT access - private
        account.deposit(100);                // ✅ Can access - public method
    }
}
```

## Access Modifiers - Where They Can Be Used

### For Top-Level Classes

```java
// ✅ Can use: public, default (no modifier)
public class PublicClass { }

class DefaultClass { }  // package-private

// ❌ CANNOT use: private, protected (compile error)
// private class PrivateClass { }    // ❌ ERROR
// protected class ProtectedClass { } // ❌ ERROR
```

### For Nested Classes

```java
public class Outer {
    // ✅ Can use: ALL four access modifiers
    public class PublicInner { }
    protected class ProtectedInner { }
    class DefaultInner { }
    private class PrivateInner { }
}
```

### For Class Members (Fields and Methods)

```java
public class Example {
    // ✅ Can use: ALL four access modifiers
    public int publicField;
    protected int protectedField;
    int defaultField;
    private int privateField;
    
    public void publicMethod() { }
    protected void protectedMethod() { }
    void defaultMethod() { }
    private void privateMethod() { }
}
```

### For Constructors

```java
public class Example {
    // ✅ Can use: ALL four access modifiers
    public Example() { }           // Anyone can create
    protected Example(int x) { }   // Package + subclasses
    Example(String s) { }          // Package only
    private Example(double d) { }  // Only within class (Singleton pattern)
}
```

---

# Part 2: Non-Access Modifiers

Provide additional functionality beyond access control.

## 1. static - Belongs to Class, Not Instance

### Static Variables (Class Variables)

```java
public class Counter {
    private static int count = 0;  // Shared by all instances
    private int instanceId;
    
    public Counter() {
        count++;                  // Increment shared counter
        instanceId = count;
    }
    
    public static int getCount() {
        return count;  // Static method can access static variable
    }
    
    public int getInstanceId() {
        return instanceId;
    }
}

// Usage
Counter c1 = new Counter();
Counter c2 = new Counter();
Counter c3 = new Counter();

System.out.println(Counter.getCount());  // 3
System.out.println(c1.getInstanceId());  // 1
System.out.println(c2.getInstanceId());  // 2
System.out.println(c3.getInstanceId());  // 3
```

### Static Methods

```java
public class MathUtils {
    // Static method - can be called without creating instance
    public static int add(int a, int b) {
        return a + b;
    }
    
    public static double calculateArea(double radius) {
        return Math.PI * radius * radius;
    }
    
    // Static method can only access static members
    private static final double TAX_RATE = 0.1;
    
    public static double calculateTax(double amount) {
        return amount * TAX_RATE;  // ✅ Can access static field
        // return this.amount;     // ❌ Cannot use 'this' in static context
    }
}

// Usage - No need to create instance!
int sum = MathUtils.add(5, 3);
double area = MathUtils.calculateArea(10);
double tax = MathUtils.calculateTax(100);
```

### Static Blocks (Initialization)

```java
public class DatabaseConnection {
    private static Connection connection;
    
    // Static block - runs once when class is loaded
    static {
        System.out.println("Initializing database connection...");
        try {
            connection = DriverManager.getConnection("jdbc:...");
        } catch (SQLException e) {
            throw new RuntimeException("Failed to connect", e);
        }
    }
    
    public static Connection getConnection() {
        return connection;
    }
}
```

### Static Nested Classes

```java
public class Outer {
    private static int outerStatic = 100;
    private int outerInstance = 200;
    
    // Static nested class
    public static class StaticNested {
        public void display() {
            System.out.println(outerStatic);    // ✅ Can access
            // System.out.println(outerInstance); // ❌ Cannot access
        }
    }
}

// Usage - No outer instance needed
Outer.StaticNested nested = new Outer.StaticNested();
```

## 2. final - Cannot Be Changed

### Final Variables (Constants)

```java
public class Constants {
    // Must be initialized
    public static final double PI = 3.14159;
    public static final int MAX_SIZE = 100;
    
    // Can initialize in constructor
    private final String id;
    
    public Constants(String id) {
        this.id = id;  // ✅ Can assign once in constructor
        // this.id = "new";  // ❌ Cannot reassign
    }
    
    public void method() {
        final int localConstant = 10;
        // localConstant = 20;  // ❌ Cannot change final variable
    }
}
```

### Final Methods (Cannot Be Overridden)

```java
public class Parent {
    public final void importantMethod() {
        System.out.println("This behavior cannot be changed");
    }
    
    public void normalMethod() {
        System.out.println("This can be overridden");
    }
}

public class Child extends Parent {
    // ❌ Cannot override final method
    // public void importantMethod() { }  // COMPILE ERROR
    
    @Override
    public void normalMethod() {  // ✅ Can override
        System.out.println("Overridden behavior");
    }
}
```

### Final Classes (Cannot Be Extended)

```java
// Cannot be subclassed
public final class ImmutableClass {
    private final int value;
    
    public ImmutableClass(int value) {
        this.value = value;
    }
    
    public int getValue() {
        return value;
    }
}

// ❌ Cannot extend final class
// public class SubClass extends ImmutableClass { }  // COMPILE ERROR

// Real-world examples: String, Integer, Double are all final
// public final class String { ... }
```

### Final with Reference Types

```java
public class Example {
    // Final reference - cannot point to different object
    private final List<String> list = new ArrayList<>();
    
    public void demonstrate() {
        list.add("Item 1");  // ✅ Can modify the object
        list.add("Item 2");  // ✅ Can modify the object
        
        // list = new ArrayList<>();  // ❌ Cannot reassign reference
    }
}
```

## 3. abstract - Incomplete, Must Be Extended

### Abstract Classes

```java
// Cannot instantiate abstract class
public abstract class Animal {
    private String name;
    
    // Can have constructor
    public Animal(String name) {
        this.name = name;
    }
    
    // Can have regular methods
    public void sleep() {
        System.out.println(name + " is sleeping");
    }
    
    // Abstract method - no implementation
    public abstract void makeSound();
    
    // Can have abstract methods
    public abstract void move();
}

// ❌ Cannot instantiate
// Animal animal = new Animal("Test");  // COMPILE ERROR

// ✅ Must extend and implement abstract methods
public class Dog extends Animal {
    public Dog(String name) {
        super(name);
    }
    
    @Override
    public void makeSound() {
        System.out.println("Woof!");
    }
    
    @Override
    public void move() {
        System.out.println("Running on four legs");
    }
}
```

### Abstract Methods

```java
public abstract class Shape {
    // Abstract methods have no body
    public abstract double calculateArea();
    public abstract double calculatePerimeter();
    
    // Can have concrete methods too
    public void display() {
        System.out.println("Area: " + calculateArea());
        System.out.println("Perimeter: " + calculatePerimeter());
    }
}

public class Circle extends Shape {
    private double radius;
    
    public Circle(double radius) {
        this.radius = radius;
    }
    
    @Override
    public double calculateArea() {
        return Math.PI * radius * radius;
    }
    
    @Override
    public double calculatePerimeter() {
        return 2 * Math.PI * radius;
    }
}
```

## 4. synchronized - Thread-Safe

```java
public class Counter {
    private int count = 0;
    
    // Synchronized method - only one thread at a time
    public synchronized void increment() {
        count++;
    }
    
    // Synchronized block
    public void incrementBlock() {
        synchronized(this) {
            count++;
        }
    }
    
    // Static synchronized method
    private static int staticCount = 0;
    
    public static synchronized void staticIncrement() {
        staticCount++;
    }
}

// Multiple threads calling increment() will be thread-safe
```

## 5. volatile - Visibility Across Threads

```java
public class TaskManager {
    // Volatile ensures all threads see the latest value
    private volatile boolean running = true;
    
    public void startTask() {
        new Thread(() -> {
            while (running) {
                // Do work
                System.out.println("Working...");
            }
            System.out.println("Stopped");
        }).start();
    }
    
    public void stopTask() {
        running = false;  // All threads will see this change immediately
    }
}
```

## 6. transient - Skip Serialization

```java
import java.io.Serializable;

public class User implements Serializable {
    private String username;
    private transient String password;  // Won't be serialized
    private int age;
    
    public User(String username, String password, int age) {
        this.username = username;
        this.password = password;
        this.age = age;
    }
}

// When serialized:
// username: saved ✅
// password: not saved ❌ (security!)
// age: saved ✅
```

## 7. native - Implemented in Native Code

```java
public class NativeExample {
    // Implemented in C/C++ or other native language
    public native void nativeMethod();
    
    static {
        System.loadLibrary("native-lib");
    }
}
```

## 8. strictfp - Strict Floating-Point

```java
// Ensures floating-point calculations are consistent across platforms
public strictfp class MathCalculations {
    public double calculate() {
        return 1.0 / 3.0;  // Will be exactly the same on all platforms
    }
}
```

---

# Complete Modifier Compatibility Chart

## For Classes

| Modifier | Top-Level Class | Nested Class |
|----------|-----------------|--------------|
| public | ✅ | ✅ |
| protected | ❌ | ✅ |
| private | ❌ | ✅ |
| default | ✅ | ✅ |
| abstract | ✅ | ✅ |
| final | ✅ | ✅ |
| static | ❌ | ✅ (only nested) |
| strictfp | ✅ | ✅ |

## For Methods

| Modifier | Instance Method | Static Method | Abstract Method |
|----------|----------------|---------------|-----------------|
| public | ✅ | ✅ | ✅ |
| protected | ✅ | ✅ | ✅ |
| private | ✅ | ✅ | ❌ |
| default | ✅ | ✅ | ✅ |
| static | ❌ | ✅ | ❌ |
| final | ✅ | ✅ | ❌ |
| abstract | ✅ | ❌ | ✅ |
| synchronized | ✅ | ✅ | ❌ |
| native | ✅ | ✅ | ❌ |
| strictfp | ✅ | ✅ | ❌ |

## For Variables

| Modifier | Instance Variable | Static Variable | Local Variable | Parameter |
|----------|------------------|-----------------|----------------|-----------|
| public | ✅ | ✅ | ❌ | ❌ |
| protected | ✅ | ✅ | ❌ | ❌ |
| private | ✅ | ✅ | ❌ | ❌ |
| default | ✅ | ✅ | ❌ | ❌ |
| static | ❌ | ✅ | ❌ | ❌ |
| final | ✅ | ✅ | ✅ | ✅ |
| transient | ✅ | ❌ | ❌ | ❌ |
| volatile | ✅ | ✅ | ❌ | ❌ |

---

# Common Combinations

```java
// Class modifiers
public class RegularClass { }
public final class FinalClass { }
public abstract class AbstractClass { }
public strictfp class StrictClass { }

// Method modifiers
public static void staticMethod() { }
public final void finalMethod() { }
public abstract void abstractMethod();
public synchronized void syncMethod() { }
public static synchronized void staticSyncMethod() { }
private final void privateFinalMethod() { }

// Variable modifiers
public static final int CONSTANT = 100;
private final String id;
protected static int counter;
private transient String tempData;
private volatile boolean flag;
```

## Illegal Combinations

```java
// ❌ Cannot combine abstract with final
// public abstract final class Bad { }  // ERROR

// ❌ Cannot combine abstract with private
// private abstract void bad();  // ERROR

// ❌ Cannot have abstract method in non-abstract class
/*
public class Bad {
    public abstract void method();  // ERROR
}
*/

// ❌ Cannot combine final with abstract
// public abstract final void bad();  // ERROR

// ❌ Cannot combine static with abstract (for methods)
// public static abstract void bad();  // ERROR
```

## Summary

**Access Modifiers** (visibility):
- `public`: Everywhere
- `protected`: Package + subclasses
- default: Package only
- `private`: Class only

**Non-Access Modifiers** (behavior):
- `static`: Class-level
- `final`: Immutable/non-overridable
- `abstract`: Incomplete
- `synchronized`: Thread-safe
- `volatile`: Thread-visible
- `transient`: Non-serializable
- `native`: Platform-specific
- `strictfp`: Consistent floating-point

**Key Rules**:
- Top-level classes: `public` or default only
- Abstract + final = illegal
- Abstract + private = illegal
- Static methods cannot be abstract

---

**Congratulations!** You've mastered inner classes, lambdas, method references, and all Java modifiers!