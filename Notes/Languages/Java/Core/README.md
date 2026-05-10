# Java Core Concepts: Complete Comparison Guide

## 1. Polymorphism: Compile-time vs Runtime

### **Compile-time Polymorphism (Method Overloading)**
```java
class Calculator {
    // Same method name, different parameters
    public int add(int a, int b) {
        return a + b;
    }
    
    public int add(int a, int b, int c) {
        return a + b + c;
    }
    
    public double add(double a, double b) {
        return a + b;
    }
}
```

**Characteristics:**
- Also called **Static/Early Binding**
- Determined at **compile time**
- Achieved through **method overloading**
- Based on **method signature** (name + parameter list)

**Rules:**
- Methods must have same name but different parameters
- Can differ in: number/type/order of parameters
- Cannot differ only by return type
- Can have different access modifiers
- Can throw different exceptions

**Example:**
```java
Calculator calc = new Calculator();
calc.add(5, 10);       // Calls add(int, int)
calc.add(5, 10, 15);   // Calls add(int, int, int)
calc.add(5.5, 10.5);   // Calls add(double, double)
```

---

### **Runtime Polymorphism (Method Overriding)**
```java
class Animal {
    public void makeSound() {
        System.out.println("Animal makes sound");
    }
}

class Dog extends Animal {
    @Override
    public void makeSound() {
        System.out.println("Dog barks");
    }
}

class Cat extends Animal {
    @Override
    public void makeSound() {
        System.out.println("Cat meows");
    }
}
```

**Characteristics:**
- Also called **Dynamic/Late Binding**
- Determined at **runtime**
- Achieved through **method overriding**
- Based on **object type** (not reference type)

**Rules:**
- Method must have same signature as parent
- Cannot reduce visibility (private < default < protected < public)
- Cannot override:
  - Static methods
  - Final methods
  - Private methods
- Can use `@Override` annotation (recommended)

**Example:**
```java
Animal myAnimal = new Animal();
myAnimal.makeSound();  // "Animal makes sound"

myAnimal = new Dog();  // Upcasting
myAnimal.makeSound();  // "Dog barks" (Runtime decision)

myAnimal = new Cat();
myAnimal.makeSound();  // "Cat meows"
```

---

## 2. Static vs Final Keywords

### **Static Keyword**
```java
class Company {
    // Static variable (class-level)
    static String companyName = "TechCorp";
    static int employeeCount = 0;
    
    // Instance variable
    String employeeName;
    
    // Static block (runs when class loads)
    static {
        System.out.println("Company class loaded");
    }
    
    // Static method
    static void displayCompanyInfo() {
        System.out.println("Company: " + companyName);
        // Cannot access: this.employeeName (non-static)
    }
    
    // Constructor
    public Company(String name) {
        this.employeeName = name;
        employeeCount++;  // Increment shared counter
    }
}
```

**Uses:**
- **Variables**: Shared across all instances
- **Methods**: Can be called without creating object
- **Blocks**: Executed when class loads
- **Nested Classes**: Can exist without outer class instance

**Example:**
```java
// Access static members without instance
System.out.println(Company.companyName);
Company.displayCompanyInfo();

// Instance creation affects static variable
Company emp1 = new Company("Alice");
Company emp2 = new Company("Bob");
System.out.println(Company.employeeCount);  // 2
```

---

### **Final Keyword**
```java
final class ImmutableClass {  // Cannot be extended
    final int MAX_VALUE = 100;  // Constant variable
    
    final void display() {  // Cannot be overridden
        System.out.println("Final method");
    }
}

class Parent {
    final int VALUE;  // Blank final variable
    
    // Initialize in constructor
    public Parent(int value) {
        this.VALUE = value;  // Can set once
    }
    
    final void finalMethod() {
        // Cannot be overridden by child classes
    }
}
```

**Uses:**
- **Variables**: Constant values (must be initialized)
- **Methods**: Cannot be overridden
- **Classes**: Cannot be extended
- **Parameters**: Cannot be modified in method

**Types of Final Variables:**
```java
class Example {
    final int a = 10;              // Initialized final
    final int b;                   // Blank final
    static final int MAX = 100;    // Static + final = constant
    final List<String> list;       // Reference final
    
    public Example() {
        b = 20;                    // Must initialize in constructor
        list = new ArrayList<>();  // Reference is final, not object
        list.add("Hello");         // Allowed - object can be modified
        list = new ArrayList<>();  // ERROR - cannot reassign reference
    }
}
```

---

## 3. Interface vs Abstract Class

### **Abstract Class**
```java
// Cannot be instantiated directly
abstract class Vehicle {
    // Can have instance variables
    protected String brand;
    private int speed;
    
    // Can have constructor
    public Vehicle(String brand) {
        this.brand = brand;
    }
    
    // Abstract method (no implementation)
    public abstract void start();
    
    // Concrete method (with implementation)
    public void stop() {
        System.out.println("Vehicle stopping");
        this.speed = 0;
    }
    
    // Can have static methods
    public static void vehicleInfo() {
        System.out.println("This is a vehicle");
    }
    
    // Can have final methods
    public final void honk() {
        System.out.println("Beep beep!");
    }
}

class Car extends Vehicle {
    public Car(String brand) {
        super(brand);
    }
    
    @Override
    public void start() {
        System.out.println(brand + " car starting with key");
    }
}
```

**Characteristics:**
- Can have **abstract and concrete methods**
- Can have **constructors**
- Can have **instance variables** with any access modifier
- Can have **static, final, private** methods
- **Single inheritance** only (extends one class)
- Represents **"is-a"** relationship
- Use when sharing **common base implementation**

---

### **Interface (Pre-Java 8)**
```java
// Pure abstraction before Java 8
interface Drivable {
    // Constant (implicitly public static final)
    int MAX_SPEED = 120;
    
    // Abstract method (implicitly public abstract)
    void accelerate();
    void brake();
}

interface FuelEfficient {
    double getMileage();
}

// Multiple interfaces can be implemented
class Sedan implements Drivable, FuelEfficient {
    @Override
    public void accelerate() {
        System.out.println("Sedan accelerating");
    }
    
    @Override
    public void brake() {
        System.out.println("Sedan braking");
    }
    
    @Override
    public double getMileage() {
        return 25.5;
    }
}
```

---

### **Interface (Java 8+)**
```java
interface ModernInterface {
    // Abstract method
    void abstractMethod();
    
    // Default method (with implementation)
    default void defaultMethod() {
        System.out.println("Default implementation");
        privateMethod();  // Can call private method
    }
    
    // Static method
    static void staticMethod() {
        System.out.println("Static method in interface");
    }
    
    // Private method (Java 9+)
    private void privateMethod() {
        System.out.println("Private helper method");
    }
}

class Implementation implements ModernInterface {
    @Override
    public void abstractMethod() {
        System.out.println("Implemented abstract method");
    }
    
    // Optional: Override default method
    @Override
    public void defaultMethod() {
        System.out.println("Overridden default method");
        ModernInterface.super.defaultMethod();  // Call parent default
    }
}
```

---

## Comparison Table: Interface vs Abstract Class

| Feature | Abstract Class | Interface |
|---------|----------------|-----------|
| **Keyword** | `abstract class` | `interface` |
| **Inheritance** | Single (extends one class) | Multiple (implements many interfaces) |
| **Methods** | Abstract + Concrete | All abstract (pre-Java 8), Default/Static/Private (Java 8+) |
| **Variables** | Any type, any modifier | Only `public static final` (constants) |
| **Constructors** | Yes | No |
| **Access Modifiers** | Any for methods/variables | Public (implicitly) |
| **Object State** | Can maintain state | Stateless (no instance variables) |
| **When to Use** | Sharing code, template method pattern | Defining contract, multiple inheritance, API design |

---

## Complete Example: Real-world Usage

```java
// Abstract Class Example
abstract class PaymentProcessor {
    protected double amount;
    
    public PaymentProcessor(double amount) {
        this.amount = amount;
    }
    
    // Template method pattern
    public final void processPayment() {
        validate();
        process();
        sendReceipt();
    }
    
    protected abstract void process();
    
    protected void validate() {
        System.out.println("Validating payment: $" + amount);
    }
    
    protected void sendReceipt() {
        System.out.println("Receipt sent for: $" + amount);
    }
}

// Interface Example
interface Refundable {
    void refund(double amount);
    default boolean isRefundAllowed() {
        return true;
    }
}

interface FraudCheckable {
    boolean checkForFraud();
    static String getFraudCheckVersion() {
        return "1.2.0";
    }
}

// Concrete Implementation
class CreditCardProcessor extends PaymentProcessor 
                          implements Refundable, FraudCheckable {
    
    public CreditCardProcessor(double amount) {
        super(amount);
    }
    
    @Override
    protected void process() {
        System.out.println("Processing credit card payment");
    }
    
    @Override
    public void refund(double amount) {
        System.out.println("Refunding $" + amount + " to credit card");
    }
    
    @Override
    public boolean checkForFraud() {
        System.out.println("Performing fraud check");
        return false; // No fraud detected
    }
}

// Usage
public class Main {
    public static void main(String[] args) {
        PaymentProcessor processor = new CreditCardProcessor(100.50);
        processor.processPayment();
        
        // Accessing interface methods
        if (processor instanceof Refundable) {
            ((Refundable) processor).refund(50.25);
        }
        
        // Static interface method
        String version = FraudCheckable.getFraudCheckVersion();
    }
}
```

---

## Best Practices & When to Use

### **Use Abstract Class When:**
1. Sharing common code among related classes
2. Need to declare non-public members
3. Need to define constructors
4. Implementing template method pattern
5. Expecting future evolution with common base code

### **Use Interface When:**
1. Defining a contract for unrelated classes
2. Multiple inheritance of type is needed
3. Creating APIs for other developers
4. Need to add functionality without breaking existing code (default methods)
5. Working with lambda expressions and functional programming

### **Use Both Together:**
```java
// Common pattern
abstract class BaseClass {
    // Shared implementation
}

interface Capability1 {
    void doSomething();
}

interface Capability2 {
    void doSomethingElse();
}

class ConcreteClass extends BaseClass implements Capability1, Capability2 {
    // Get benefits of both
}
```

---

## Key Takeaways

1. **Polymorphism:**
   - Overloading: Same class, compile-time, different signatures
   - Overriding: Inheritance, runtime, same signatures

2. **Static vs Final:**
   - Static: Class-level, shared memory
   - Final: Immutability, prevents modification/inheritance

3. **Abstract vs Interface:**
   - Abstract: "Is-a", code reuse, single inheritance
   - Interface: "Can-do", contract, multiple inheritance

4. **Modern Java:**
   - Interfaces now have default, static, private methods
   - Abstract classes still needed for constructors and state
   - Choose based on "state vs behavior" and inheritance needs

This comprehensive guide covers the essential differences and use cases for these fundamental Java concepts, helping you make informed design decisions in your applications.