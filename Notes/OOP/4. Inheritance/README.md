# OOP Lesson 4: Inheritance - Building Class Hierarchies

## The Core Concept

**Inheritance** allows you to create new classes based on existing classes, inheriting their properties and behaviors while adding new ones or modifying existing ones.

Think of it like biological inheritance: you inherit traits from your parents but also have your own unique characteristics.

## Why Inheritance?

### The Problem It Solves

Without inheritance:

```cpp
class Dog {
    string name;
    int age;
    double weight;
    
public:
    void eat() { cout << "Dog eating" << endl; }
    void sleep() { cout << "Dog sleeping" << endl; }
    void bark() { cout << "Woof!" << endl; }
};

class Cat {
    string name;  // DUPLICATE
    int age;      // DUPLICATE
    double weight; // DUPLICATE
    
public:
    void eat() { cout << "Cat eating" << endl; }    // DUPLICATE
    void sleep() { cout << "Cat sleeping" << endl; } // DUPLICATE
    void meow() { cout << "Meow!" << endl; }
};

class Bird {
    string name;   // DUPLICATE AGAIN
    int age;       // DUPLICATE AGAIN
    double weight; // DUPLICATE AGAIN
    
public:
    void eat() { cout << "Bird eating" << endl; }    // DUPLICATE
    void sleep() { cout << "Bird sleeping" << endl; } // DUPLICATE
    void chirp() { cout << "Chirp!" << endl; }
};
```

### With Inheritance

```cpp
// Base class (Parent/Superclass)
class Animal {
protected:  // Accessible to derived classes
    string name;
    int age;
    double weight;
    
public:
    Animal(string n, int a, double w) : name(n), age(a), weight(w) {}
    
    void eat() { 
        cout << name << " is eating" << endl; 
    }
    
    void sleep() { 
        cout << name << " is sleeping" << endl; 
    }
    
    string getName() const { return name; }
};

// Derived classes (Child/Subclass)
class Dog : public Animal {
public:
    Dog(string n, int a, double w) : Animal(n, a, w) {}
    
    void bark() { 
        cout << name << " says: Woof!" << endl; 
    }
};

class Cat : public Animal {
public:
    Cat(string n, int a, double w) : Animal(n, a, w) {}
    
    void meow() { 
        cout << name << " says: Meow!" << endl; 
    }
};

class Bird : public Animal {
public:
    Bird(string n, int a, double w) : Animal(n, a, w) {}
    
    void chirp() { 
        cout << name << " says: Chirp!" << endl; 
    }
};

// Usage
Dog dog("Buddy", 5, 30.5);
dog.eat();    // Inherited from Animal
dog.sleep();  // Inherited from Animal
dog.bark();   // Dog-specific behavior

Cat cat("Whiskers", 3, 4.2);
cat.eat();    // Same inherited method
cat.meow();   // Cat-specific behavior
```

## The IS-A Relationship

Inheritance represents an **"IS-A"** relationship:

- A Dog **IS AN** Animal
- A Cat **IS AN** Animal
- A Bird **IS AN** Animal

```cpp
Dog myDog("Max", 4, 25.0);
Animal* animalPtr = &myDog;  // This works! Dog IS-A Animal
animalPtr->eat();  // Calls Animal's eat() method
```

## Types of Inheritance

### 1. Single Inheritance

One class inherits from one base class.

```cpp
class Vehicle {
protected:
    string brand;
    int year;
    
public:
    Vehicle(string b, int y) : brand(b), year(y) {}
    void start() { cout << "Vehicle starting..." << endl; }
};

class Car : public Vehicle {
private:
    int numDoors;
    
public:
    Car(string b, int y, int doors) 
        : Vehicle(b, y), numDoors(doors) {}
    
    void drive() { cout << "Car driving..." << endl; }
};
```

### 2. Multi-level Inheritance

Chain of inheritance: A → B → C

```cpp
class Vehicle {
public:
    void start() { cout << "Starting vehicle" << endl; }
};

class Car : public Vehicle {
public:
    void drive() { cout << "Driving car" << endl; }
};

class SportsCar : public Car {
public:
    void turboBoost() { cout << "TURBO BOOST!" << endl; }
};

SportsCar myCar;
myCar.start();      // From Vehicle
myCar.drive();      // From Car
myCar.turboBoost(); // From SportsCar
```

### 3. Hierarchical Inheritance

Multiple classes inherit from one base class.

```cpp
class Employee {
protected:
    string name;
    int id;
    double baseSalary;
    
public:
    Employee(string n, int i, double s) 
        : name(n), id(i), baseSalary(s) {}
};

class Manager : public Employee {
private:
    int teamSize;
    
public:
    Manager(string n, int i, double s, int team)
        : Employee(n, i, s), teamSize(team) {}
    
    double getSalary() {
        return baseSalary * 1.5;  // Managers get 50% more
    }
};

class Developer : public Employee {
private:
    string programmingLanguage;
    
public:
    Developer(string n, int i, double s, string lang)
        : Employee(n, i, s), programmingLanguage(lang) {}
    
    double getSalary() {
        return baseSalary * 1.3;  // Developers get 30% more
    }
};

class Intern : public Employee {
public:
    Intern(string n, int i, double s)
        : Employee(n, i, s) {}
    
    double getSalary() {
        return baseSalary * 0.5;  // Interns get half
    }
};
```

### 4. Multiple Inheritance (Use with Caution!)

A class inherits from multiple base classes.

```cpp
class Swimmer {
public:
    void swim() { cout << "Swimming..." << endl; }
};

class Flyer {
public:
    void fly() { cout << "Flying..." << endl; }
};

// Duck can both swim and fly
class Duck : public Swimmer, public Flyer {
public:
    void quack() { cout << "Quack!" << endl; }
};

Duck donald;
donald.swim();  // From Swimmer
donald.fly();   // From Flyer
donald.quack(); // Own method
```

## Access Specifiers in Inheritance

### Public Inheritance (Most Common)

```cpp
class Base {
public:
    int publicVar;
protected:
    int protectedVar;
private:
    int privateVar;
};

class Derived : public Base {
    // publicVar remains public
    // protectedVar remains protected
    // privateVar is not accessible
};
```

### Protected Inheritance

```cpp
class Derived : protected Base {
    // publicVar becomes protected
    // protectedVar remains protected
    // privateVar is not accessible
};
```

### Private Inheritance

```cpp
class Derived : private Base {
    // publicVar becomes private
    // protectedVar becomes private
    // privateVar is not accessible
};
```

## Method Overriding

Derived classes can provide their own implementation of base class methods.

```cpp
class Shape {
public:
    virtual void draw() {
        cout << "Drawing a generic shape" << endl;
    }
    
    virtual double getArea() {
        return 0.0;
    }
};

class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    // Override the draw method
    void draw() override {
        cout << "Drawing a circle with radius " << radius << endl;
    }
    
    // Override the getArea method
    double getArea() override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    void draw() override {
        cout << "Drawing a rectangle " << width << "x" << height << endl;
    }
    
    double getArea() override {
        return width * height;
    }
};

// Usage
Shape* shape1 = new Circle(5.0);
Shape* shape2 = new Rectangle(4.0, 6.0);

shape1->draw();  // Calls Circle's draw()
shape2->draw();  // Calls Rectangle's draw()

cout << shape1->getArea() << endl;  // Calls Circle's getArea()
cout << shape2->getArea() << endl;  // Calls Rectangle's getArea()
```

## Constructors and Destructors in Inheritance

### Constructor Call Order

Base class constructor is called **BEFORE** derived class constructor.

```cpp
class Base {
public:
    Base() {
        cout << "Base constructor called" << endl;
    }
};

class Derived : public Base {
public:
    Derived() {
        cout << "Derived constructor called" << endl;
    }
};

Derived obj;
// Output:
// Base constructor called
// Derived constructor called
```

### Passing Arguments to Base Constructor

```cpp
class Person {
protected:
    string name;
    int age;
    
public:
    Person(string n, int a) : name(n), age(a) {
        cout << "Person created: " << name << endl;
    }
};

class Student : public Person {
private:
    int studentId;
    double gpa;
    
public:
    Student(string n, int a, int id, double g) 
        : Person(n, a),  // Call base constructor FIRST
          studentId(id), 
          gpa(g) {
        cout << "Student created with ID: " << studentId << endl;
    }
};

Student student("Alice", 20, 12345, 3.8);
// Output:
// Person created: Alice
// Student created with ID: 12345
```

### Destructor Call Order

Destructors are called in **REVERSE** order (derived first, then base).

```cpp
class Base {
public:
    ~Base() {
        cout << "Base destructor called" << endl;
    }
};

class Derived : public Base {
public:
    ~Derived() {
        cout << "Derived destructor called" << endl;
    }
};

{
    Derived obj;
}
// Output:
// Derived destructor called
// Base destructor called
```

## Real-World Example: E-commerce System

```cpp
class Product {
protected:
    string name;
    double price;
    int stockQuantity;
    
public:
    Product(string n, double p, int stock) 
        : name(n), price(p), stockQuantity(stock) {}
    
    virtual void displayInfo() {
        cout << "Product: " << name << endl;
        cout << "Price: $" << price << endl;
        cout << "Stock: " << stockQuantity << endl;
    }
    
    virtual double calculateShipping() {
        return 5.0;  // Default shipping
    }
    
    bool isInStock() { return stockQuantity > 0; }
    
    virtual ~Product() = default;
};

class Electronics : public Product {
private:
    int warrantyMonths;
    string brand;
    
public:
    Electronics(string n, double p, int stock, string b, int warranty)
        : Product(n, p, stock), brand(b), warrantyMonths(warranty) {}
    
    void displayInfo() override {
        Product::displayInfo();  // Call base method
        cout << "Brand: " << brand << endl;
        cout << "Warranty: " << warrantyMonths << " months" << endl;
    }
    
    double calculateShipping() override {
        return 10.0;  // Electronics need careful shipping
    }
};

class Clothing : public Product {
private:
    string size;
    string color;
    
public:
    Clothing(string n, double p, int stock, string s, string c)
        : Product(n, p, stock), size(s), color(c) {}
    
    void displayInfo() override {
        Product::displayInfo();
        cout << "Size: " << size << endl;
        cout << "Color: " << color << endl;
    }
    
    double calculateShipping() override {
        return 3.0;  // Clothing is lightweight
    }
};

class Book : public Product {
private:
    string author;
    int pages;
    
public:
    Book(string n, double p, int stock, string auth, int pg)
        : Product(n, p, stock), author(auth), pages(pg) {}
    
    void displayInfo() override {
        Product::displayInfo();
        cout << "Author: " << author << endl;
        cout << "Pages: " << pages << endl;
    }
    
    double calculateShipping() override {
        return pages > 500 ? 5.0 : 2.0;  // Heavy books cost more
    }
};

// Shopping cart can work with any Product type!
class ShoppingCart {
private:
    vector<Product*> items;
    
public:
    void addItem(Product* product) {
        items.push_back(product);
    }
    
    double getTotalCost() {
        double total = 0;
        for (Product* item : items) {
            total += item->price + item->calculateShipping();
        }
        return total;
    }
    
    void displayCart() {
        cout << "=== Shopping Cart ===" << endl;
        for (Product* item : items) {
            item->displayInfo();
            cout << "Shipping: $" << item->calculateShipping() << endl;
            cout << "---" << endl;
        }
        cout << "Total: $" << getTotalCost() << endl;
    }
};
```

## The Diamond Problem (Multiple Inheritance)

```cpp
class Animal {
public:
    void eat() { cout << "Animal eating" << endl; }
};

class Mammal : public Animal {
public:
    void breathe() { cout << "Mammal breathing" << endl; }
};

class WingedAnimal : public Animal {
public:
    void fly() { cout << "Flying" << endl; }
};

// Problem: Bat inherits eat() twice!
class Bat : public Mammal, public WingedAnimal {
    // Which Animal::eat() should it use?
};

// Solution: Virtual Inheritance
class Mammal : virtual public Animal { };
class WingedAnimal : virtual public Animal { };

class Bat : public Mammal, public WingedAnimal {
    // Now only one Animal subobject exists
};
```

## When to Use Inheritance

### ✅ Good Use Cases

1. **Clear IS-A Relationship**
```cpp
class Animal { };
class Dog : public Animal { };  // Dog IS-A Animal ✓
```

2. **Shared Behavior**
```cpp
class Vehicle { void start() { } };
class Car : public Vehicle { };     // Shares start() ✓
```

3. **Polymorphism Needed**
```cpp
void feedAnimal(Animal* animal) {
    animal->eat();  // Works with any Animal type ✓
}
```

### ❌ Bad Use Cases

1. **HAS-A Relationship** (use composition instead)
```cpp
// BAD
class Car : public Engine { };  // Car IS-A Engine? No!

// GOOD
class Car {
    Engine engine;  // Car HAS-A Engine ✓
};
```

2. **Code Reuse Only** (use composition instead)
```cpp
// BAD - just want to reuse ArrayList
class Stack : public ArrayList { };

// GOOD - use composition
class Stack {
private:
    ArrayList data;  // Use it internally
};
```

## Best Practices

1. **Use `virtual` for base class destructors**
```cpp
class Base {
public:
    virtual ~Base() { }  // Always virtual!
};
```

2. **Use `override` keyword**
```cpp
class Derived : public Base {
    void method() override { }  // Catches typos!
};
```

3. **Prefer composition over inheritance**
```cpp
// Instead of inheriting for reuse, compose!
class Car {
    Engine engine;
    Wheels wheels;
    Transmission transmission;
};
```

4. **Keep inheritance hierarchies shallow**
```cpp
// Prefer: 2-3 levels maximum
Animal → Mammal → Dog  ✓

// Avoid: Deep hierarchies
Living → Animal → Vertebrate → Mammal → Carnivore → Canine → Dog  ✗
```

## Summary

**Inheritance**:
- Allows code reuse through class hierarchies
- Represents IS-A relationships
- Enables polymorphism (next lesson!)
- Derived classes inherit members from base classes
- Can override base class methods
- Constructors called base-to-derived, destructors derived-to-base

**Key Rule**: Only use inheritance when there's a genuine IS-A relationship and you need polymorphic behavior. Otherwise, use composition.

---

**Next Lesson**: We'll explore **Polymorphism** - the power to work with objects through their base class interface!