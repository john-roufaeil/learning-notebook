# OOP Lesson 5: Polymorphism - One Interface, Many Forms

## The Core Concept

**Polymorphism** (Greek: "many forms") is the ability to treat objects of different types through a common interface, where each type can respond to the same message in its own way.

**The Magic**: Call the same method on different objects and get different, appropriate behavior.

## Real-World Analogy

Think of a universal remote control:
- You press "PLAY"
- DVD player starts playing the disc
- Music player starts playing a song
- Game console starts playing a game

Same button (interface), different behaviors (polymorphism).

## Types of Polymorphism

### 1. Compile-Time Polymorphism (Static)

Resolved during compilation.

#### Function Overloading

```cpp
class Calculator {
public:
    // Same function name, different parameters
    int add(int a, int b) {
        return a + b;
    }
    
    double add(double a, double b) {
        return a + b;
    }
    
    int add(int a, int b, int c) {
        return a + b + c;
    }
    
    string add(string a, string b) {
        return a + b;
    }
};

Calculator calc;
cout << calc.add(5, 3) << endl;           // Calls int version
cout << calc.add(5.5, 3.2) << endl;       // Calls double version
cout << calc.add(1, 2, 3) << endl;        // Calls three-int version
cout << calc.add("Hello ", "World") << endl; // Calls string version
```

#### Operator Overloading

```cpp
class Complex {
private:
    double real;
    double imag;
    
public:
    Complex(double r, double i) : real(r), imag(i) {}
    
    // Overload + operator
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }
    
    // Overload * operator
    Complex operator*(const Complex& other) const {
        return Complex(
            real * other.real - imag * other.imag,
            real * other.imag + imag * other.real
        );
    }
    
    // Overload << operator for printing
    friend ostream& operator<<(ostream& os, const Complex& c) {
        os << c.real << " + " << c.imag << "i";
        return os;
    }
};

Complex c1(3, 4);
Complex c2(1, 2);

Complex sum = c1 + c2;      // Uses overloaded +
Complex product = c1 * c2;  // Uses overloaded *

cout << sum << endl;        // Uses overloaded <<
```

### 2. Runtime Polymorphism (Dynamic)

Resolved during program execution - this is the **TRUE POWER** of polymorphism!

## Runtime Polymorphism: Virtual Functions

### The Problem Without Virtual Functions

```cpp
class Animal {
public:
    void makeSound() {
        cout << "Some generic animal sound" << endl;
    }
};

class Dog : public Animal {
public:
    void makeSound() {
        cout << "Woof!" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() {
        cout << "Meow!" << endl;
    }
};

// Without virtual functions
Animal* animal = new Dog();
animal->makeSound();  // Prints "Some generic animal sound" - WRONG!
// It calls Animal's version, not Dog's!
```

### The Solution: Virtual Functions

```cpp
class Animal {
public:
    virtual void makeSound() {  // Mark as virtual
        cout << "Some generic animal sound" << endl;
    }
    
    virtual ~Animal() = default;  // Virtual destructor!
};

class Dog : public Animal {
public:
    void makeSound() override {  // Override base version
        cout << "Woof!" << endl;
    }
};

class Cat : public Animal {
public:
    void makeSound() override {
        cout << "Meow!" << endl;
    }
};

class Cow : public Animal {
public:
    void makeSound() override {
        cout << "Moo!" << endl;
    }
};

// Now with virtual functions
Animal* animal1 = new Dog();
Animal* animal2 = new Cat();
Animal* animal3 = new Cow();

animal1->makeSound();  // Prints "Woof!" - CORRECT!
animal2->makeSound();  // Prints "Meow!" - CORRECT!
animal3->makeSound();  // Prints "Moo!" - CORRECT!
```

## The Power of Polymorphism

### Example 1: Animal Orchestra

```cpp
void animalConcert(Animal* animals[], int size) {
    cout << "=== Animal Concert ===" << endl;
    for (int i = 0; i < size; i++) {
        animals[i]->makeSound();  // Each animal makes its own sound!
    }
}

Animal* orchestra[5];
orchestra[0] = new Dog();
orchestra[1] = new Cat();
orchestra[2] = new Dog();
orchestra[3] = new Cow();
orchestra[4] = new Cat();

animalConcert(orchestra, 5);
// Output:
// Woof!
// Meow!
// Woof!
// Moo!
// Meow!
```

### Example 2: Drawing Shapes

```cpp
class Shape {
public:
    virtual void draw() const = 0;  // Pure virtual function
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    void draw() const override {
        cout << "Drawing Circle with radius " << radius << endl;
    }
    
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    void draw() const override {
        cout << "Drawing Rectangle " << width << "x" << height << endl;
    }
    
    double area() const override {
        return width * height;
    }
};

class Triangle : public Shape {
private:
    double base, height;
    
public:
    Triangle(double b, double h) : base(b), height(h) {}
    
    void draw() const override {
        cout << "Drawing Triangle with base " << base << endl;
    }
    
    double area() const override {
        return 0.5 * base * height;
    }
};

// Polymorphic function - works with ANY shape!
void renderShape(const Shape& shape) {
    shape.draw();
    cout << "Area: " << shape.area() << " square units" << endl;
    cout << "---" << endl;
}

void renderAllShapes(const vector<Shape*>& shapes) {
    double totalArea = 0;
    
    for (const Shape* shape : shapes) {
        shape->draw();
        totalArea += shape->area();
    }
    
    cout << "Total area of all shapes: " << totalArea << endl;
}

// Usage
vector<Shape*> shapes;
shapes.push_back(new Circle(5.0));
shapes.push_back(new Rectangle(4.0, 6.0));
shapes.push_back(new Triangle(3.0, 4.0));
shapes.push_back(new Circle(3.0));

renderAllShapes(shapes);
```

## Pure Virtual Functions and Abstract Classes

### Abstract Classes Cannot Be Instantiated

```cpp
class Shape {
public:
    virtual void draw() const = 0;  // Pure virtual = must be overridden
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

// Shape shape;  // ERROR: Cannot instantiate abstract class!

class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    // MUST implement all pure virtual functions
    void draw() const override {
        cout << "Drawing circle" << endl;
    }
    
    double area() const override {
        return 3.14159 * radius * radius;
    }
};

Circle circle(5.0);  // OK - Circle is concrete
```

### When to Use Abstract Classes

```cpp
// Define interface (contract) that all derived classes must follow
class Database {
public:
    virtual bool connect(const string& connectionString) = 0;
    virtual void disconnect() = 0;
    virtual ResultSet query(const string& sql) = 0;
    virtual bool execute(const string& sql) = 0;
    virtual ~Database() = default;
};

// Any database implementation MUST provide these methods
class MySQLDatabase : public Database {
    // Must implement all pure virtual functions
};

class PostgreSQLDatabase : public Database {
    // Must implement all pure virtual functions
};

class SQLiteDatabase : public Database {
    // Must implement all pure virtual functions
};
```

## Real-World Example: Payment Processing System

```cpp
class PaymentMethod {
protected:
    string accountInfo;
    
public:
    PaymentMethod(const string& info) : accountInfo(info) {}
    
    // Pure virtual functions - interface
    virtual bool authorize(double amount) = 0;
    virtual bool processPayment(double amount) = 0;
    virtual void generateReceipt() const = 0;
    virtual string getPaymentType() const = 0;
    
    virtual ~PaymentMethod() = default;
};

class CreditCard : public PaymentMethod {
private:
    string cardNumber;
    string expiryDate;
    int cvv;
    
public:
    CreditCard(string num, string exp, int cvv)
        : PaymentMethod("Credit Card"), cardNumber(num), 
          expiryDate(exp), cvv(cvv) {}
    
    bool authorize(double amount) override {
        cout << "Authorizing credit card payment of $" << amount << endl;
        // Check with bank, verify CVV, etc.
        return true;
    }
    
    bool processPayment(double amount) override {
        if (authorize(amount)) {
            cout << "Processing credit card payment..." << endl;
            return true;
        }
        return false;
    }
    
    void generateReceipt() const override {
        cout << "Credit Card Receipt" << endl;
        cout << "Card ending in: " << cardNumber.substr(cardNumber.length() - 4) << endl;
    }
    
    string getPaymentType() const override {
        return "Credit Card";
    }
};

class PayPal : public PaymentMethod {
private:
    string email;
    
public:
    PayPal(string email)
        : PaymentMethod("PayPal"), email(email) {}
    
    bool authorize(double amount) override {
        cout << "Authorizing PayPal payment of $" << amount << endl;
        // Redirect to PayPal, verify credentials
        return true;
    }
    
    bool processPayment(double amount) override {
        if (authorize(amount)) {
            cout << "Processing PayPal payment..." << endl;
            return true;
        }
        return false;
    }
    
    void generateReceipt() const override {
        cout << "PayPal Receipt" << endl;
        cout << "Account: " << email << endl;
    }
    
    string getPaymentType() const override {
        return "PayPal";
    }
};

class Bitcoin : public PaymentMethod {
private:
    string walletAddress;
    
public:
    Bitcoin(string address)
        : PaymentMethod("Bitcoin"), walletAddress(address) {}
    
    bool authorize(double amount) override {
        cout << "Verifying Bitcoin transaction of $" << amount << endl;
        // Check blockchain, verify wallet balance
        return true;
    }
    
    bool processPayment(double amount) override {
        if (authorize(amount)) {
            cout << "Broadcasting Bitcoin transaction..." << endl;
            return true;
        }
        return false;
    }
    
    void generateReceipt() const override {
        cout << "Bitcoin Receipt" << endl;
        cout << "Wallet: " << walletAddress << endl;
    }
    
    string getPaymentType() const override {
        return "Bitcoin";
    }
};

// Shopping cart works with ANY payment method!
class ShoppingCart {
private:
    vector<double> itemPrices;
    
public:
    void addItem(double price) {
        itemPrices.push_back(price);
    }
    
    double getTotal() const {
        double total = 0;
        for (double price : itemPrices) {
            total += price;
        }
        return total;
    }
    
    // Polymorphic payment processing!
    bool checkout(PaymentMethod* payment) {
        double total = getTotal();
        cout << "Total: $" << total << endl;
        cout << "Payment method: " << payment->getPaymentType() << endl;
        
        if (payment->processPayment(total)) {
            payment->generateReceipt();
            cout << "Payment successful!" << endl;
            return true;
        }
        
        cout << "Payment failed!" << endl;
        return false;
    }
};

// Usage
ShoppingCart cart;
cart.addItem(29.99);
cart.addItem(49.99);
cart.addItem(15.50);

// Choose payment method at runtime!
PaymentMethod* payment1 = new CreditCard("1234-5678-9012-3456", "12/25", 123);
cart.checkout(payment1);

cout << endl;

PaymentMethod* payment2 = new PayPal("user@example.com");
cart.checkout(payment2);

cout << endl;

PaymentMethod* payment3 = new Bitcoin("1A1zP1eP5QGefi2DMPTfTL5SLmv7DivfNa");
cart.checkout(payment3);
```

## Virtual Function Table (VTable)

How does C++ achieve runtime polymorphism?

```cpp
class Animal {
public:
    virtual void makeSound() { cout << "Animal sound" << endl; }
    virtual void move() { cout << "Animal moving" << endl; }
};

class Dog : public Animal {
public:
    void makeSound() override { cout << "Woof!" << endl; }
    void move() override { cout << "Dog running" << endl; }
};
```

**Under the hood**:

1. Each class with virtual functions gets a **VTable** (virtual function table)
2. Each object gets a **VPtr** (virtual pointer) pointing to its class's VTable
3. When you call a virtual function, C++ looks up the correct function in the VTable

```
Animal VTable:           Dog VTable:
+------------------+     +------------------+
| makeSound() -----|--> Animal::makeSound  | makeSound() -----|--> Dog::makeSound
| move() ----------|--> Animal::move       | move() ----------|--> Dog::move
+------------------+                        +------------------+

Animal* ptr = new Dog();
ptr->makeSound();  // Follows VPtr -> Dog VTable -> Dog::makeSound
```

## Interface Segregation

Don't force classes to implement methods they don't need.

```cpp
// BAD - One fat interface
class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
};

class Robot : public Worker {
    void work() override { /* OK */ }
    void eat() override { /* Robots don't eat! */ }
    void sleep() override { /* Robots don't sleep! */ }
};

// GOOD - Separate interfaces
class Workable {
public:
    virtual void work() = 0;
    virtual ~Workable() = default;
};

class Eatable {
public:
    virtual void eat() = 0;
    virtual ~Eatable() = default;
};

class Sleepable {
public:
    virtual void sleep() = 0;
    virtual ~Sleepable() = default;
};

class Human : public Workable, public Eatable, public Sleepable {
    void work() override { /* ... */ }
    void eat() override { /* ... */ }
    void sleep() override { /* ... */ }
};

class Robot : public Workable {
    void work() override { /* ... */ }
    // Only implements what it needs!
};
```

## Design Patterns Using Polymorphism

### Strategy Pattern

```cpp
class SortingStrategy {
public:
    virtual void sort(vector<int>& data) = 0;
    virtual ~SortingStrategy() = default;
};

class BubbleSort : public SortingStrategy {
public:
    void sort(vector<int>& data) override {
        // Bubble sort implementation
        cout << "Sorting with Bubble Sort" << endl;
    }
};

class QuickSort : public SortingStrategy {
public:
    void sort(vector<int>& data) override {
        // Quick sort implementation
        cout << "Sorting with Quick Sort" << endl;
    }
};

class MergeSort : public SortingStrategy {
public:
    void sort(vector<int>& data) override {
        // Merge sort implementation
        cout << "Sorting with Merge Sort" << endl;
    }
};

class Sorter {
private:
    SortingStrategy* strategy;
    
public:
    Sorter(SortingStrategy* s) : strategy(s) {}
    
    void setStrategy(SortingStrategy* s) {
        strategy = s;
    }
    
    void performSort(vector<int>& data) {
        strategy->sort(data);  // Polymorphic call!
    }
};

// Usage
vector<int> data = {5, 2, 8, 1, 9};

Sorter sorter(new BubbleSort());
sorter.performSort(data);  // Uses Bubble Sort

sorter.setStrategy(new QuickSort());
sorter.performSort(data);  // Uses Quick Sort

sorter.setStrategy(new MergeSort());
sorter.performSort(data);  // Uses Merge Sort
```

## Best Practices

1. **Always use `virtual` keyword for base class methods you want to override**
2. **Always use `override` keyword in derived classes** (catches typos)
3. **Always make base class destructors virtual**
```cpp
class Base {
public:
    virtual ~Base() = default;  // ALWAYS!
};
```

4. **Use abstract classes to define interfaces**
5. **Prefer pure virtual functions for true interfaces**
6. **Don't call virtual functions in constructors/destructors**

## Summary

**Polymorphism**:
- **Compile-time**: Function overloading, operator overloading
- **Runtime**: Virtual functions, abstract classes
- Enables writing generic code that works with many types
- Foundation of flexible, extensible designs
- Requires virtual functions for dynamic dispatch
- Uses VTable and VPtr under the hood

**Key Insight**: Write code that works with base class pointers/references, and it automatically works with all derived classes!

---

**Next Lesson**: We'll explore **Advanced OOP Concepts** including SOLID principles, design patterns, and composition vs inheritance!