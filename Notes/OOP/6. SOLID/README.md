# OOP Lesson 6: SOLID Principles - Writing Better OOP Code

## What is SOLID?

SOLID is an acronym for five design principles that make software designs more understandable, flexible, and maintainable.

- **S** - Single Responsibility Principle
- **O** - Open/Closed Principle
- **L** - Liskov Substitution Principle
- **I** - Interface Segregation Principle
- **D** - Dependency Inversion Principle

These principles were introduced by Robert C. Martin (Uncle Bob) and are fundamental to writing clean, professional OOP code.

## S - Single Responsibility Principle (SRP)

**"A class should have one, and only one, reason to change."**

Each class should do one thing and do it well.

### ❌ Bad Example - Multiple Responsibilities

```cpp
class Employee {
private:
    string name;
    int id;
    double salary;
    
public:
    Employee(string n, int i, double s) : name(n), id(i), salary(s) {}
    
    // Responsibility 1: Employee data management
    void setName(string n) { name = n; }
    string getName() const { return name; }
    
    // Responsibility 2: Salary calculation
    double calculatePay() {
        // Complex payroll logic
        return salary * 1.1;
    }
    
    // Responsibility 3: Database operations
    void save() {
        // Database code
        cout << "Saving to database..." << endl;
    }
    
    // Responsibility 4: Reporting
    void generateReport() {
        cout << "Employee Report:" << endl;
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
        cout << "Salary: " << salary << endl;
    }
};

// Problem: If database changes, or report format changes, or payroll rules change,
// we have to modify this class!
```

### ✅ Good Example - Single Responsibility

```cpp
// Responsibility 1: Employee data
class Employee {
private:
    string name;
    int id;
    double baseSalary;
    
public:
    Employee(string n, int i, double s) : name(n), id(i), baseSalary(s) {}
    
    string getName() const { return name; }
    int getId() const { return id; }
    double getBaseSalary() const { return baseSalary; }
    void setName(string n) { name = n; }
};

// Responsibility 2: Salary calculation
class PayrollCalculator {
public:
    double calculatePay(const Employee& emp) {
        // Complex payroll logic isolated here
        double bonus = emp.getBaseSalary() * 0.1;
        return emp.getBaseSalary() + bonus;
    }
};

// Responsibility 3: Database operations
class EmployeeRepository {
private:
    // Database connection details
    
public:
    void save(const Employee& emp) {
        cout << "Saving employee " << emp.getName() << " to database..." << endl;
        // Database code here
    }
    
    Employee* findById(int id) {
        // Query database
        return nullptr;
    }
};

// Responsibility 4: Reporting
class EmployeeReportGenerator {
public:
    void generateReport(const Employee& emp, double calculatedPay) {
        cout << "=== Employee Report ===" << endl;
        cout << "Name: " << emp.getName() << endl;
        cout << "ID: " << emp.getId() << endl;
        cout << "Base Salary: $" << emp.getBaseSalary() << endl;
        cout << "Total Pay: $" << calculatedPay << endl;
    }
};

// Now each class has ONE reason to change!
```

### Why SRP Matters

- **Easier to understand**: Each class has a clear purpose
- **Easier to maintain**: Changes are localized
- **Easier to test**: Test one responsibility at a time
- **More reusable**: Small, focused classes can be reused

## O - Open/Closed Principle (OCP)

**"Software entities should be open for extension, but closed for modification."**

You should be able to add new functionality without changing existing code.

### ❌ Bad Example - Modifying Existing Code

```cpp
class AreaCalculator {
public:
    double calculateArea(const string& shapeType, double... dimensions) {
        if (shapeType == "circle") {
            double radius = dimensions;
            return 3.14159 * radius * radius;
        }
        else if (shapeType == "rectangle") {
            double width = dimensions;
            double height = ...; // How to handle multiple parameters?
            return width * height;
        }
        else if (shapeType == "triangle") {
            // Every time we add a shape, we modify this method!
            double base = dimensions;
            double height = ...;
            return 0.5 * base * height;
        }
        return 0;
    }
};

// Adding a new shape? MODIFY the class!
```

### ✅ Good Example - Open for Extension

```cpp
// Abstract base class - interface
class Shape {
public:
    virtual double calculateArea() const = 0;
    virtual ~Shape() = default;
};

// Each shape is a separate class
class Circle : public Shape {
private:
    double radius;
    
public:
    Circle(double r) : radius(r) {}
    
    double calculateArea() const override {
        return 3.14159 * radius * radius;
    }
};

class Rectangle : public Shape {
private:
    double width, height;
    
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    double calculateArea() const override {
        return width * height;
    }
};

class Triangle : public Shape {
private:
    double base, height;
    
public:
    Triangle(double b, double h) : base(b), height(h) {}
    
    double calculateArea() const override {
        return 0.5 * base * height;
    }
};

// Calculator works with ANY shape!
class AreaCalculator {
public:
    double calculateTotalArea(const vector<Shape*>& shapes) {
        double total = 0;
        for (const Shape* shape : shapes) {
            total += shape->calculateArea();
        }
        return total;
    }
};

// Adding a new shape? Just create a new class - NO modification needed!
class Pentagon : public Shape {
private:
    double sideLength;
    
public:
    Pentagon(double s) : sideLength(s) {}
    
    double calculateArea() const override {
        return 1.72048 * sideLength * sideLength;
    }
};
```

### Why OCP Matters

- **Less risk**: Don't touch working code
- **Easier to add features**: Just add new classes
- **Better for teams**: Multiple developers can work independently
- **Regression prevention**: Existing tests still pass

## L - Liskov Substitution Principle (LSP)

**"Objects of a superclass should be replaceable with objects of a subclass without breaking the application."**

Derived classes must be substitutable for their base classes.

### ❌ Bad Example - Violating LSP

```cpp
class Rectangle {
protected:
    int width, height;
    
public:
    virtual void setWidth(int w) { width = w; }
    virtual void setHeight(int h) { height = h; }
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    int getArea() const { return width * height; }
};

class Square : public Rectangle {
public:
    // Square must have equal sides - violates LSP!
    void setWidth(int w) override {
        width = w;
        height = w;  // Force height to equal width
    }
    
    void setHeight(int h) override {
        height = h;
        width = h;  // Force width to equal height
    }
};

// This breaks!
void testRectangle(Rectangle& rect) {
    rect.setWidth(5);
    rect.setHeight(4);
    
    // Expected: 20
    // With Square: 16 (both become 4!)
    cout << "Area: " << rect.getArea() << endl;
}

Rectangle rectangle;
testRectangle(rectangle);  // Works: 20

Square square;
testRectangle(square);  // Broken: 16 (expected 20!)
```

### ✅ Good Example - Following LSP

```cpp
// Common interface for all shapes
class Shape {
public:
    virtual int getArea() const = 0;
    virtual ~Shape() = default;
};

class Rectangle : public Shape {
private:
    int width, height;
    
public:
    Rectangle(int w, int h) : width(w), height(h) {}
    
    void setWidth(int w) { width = w; }
    void setHeight(int h) { height = h; }
    
    int getArea() const override {
        return width * height;
    }
};

class Square : public Shape {
private:
    int side;
    
public:
    Square(int s) : side(s) {}
    
    void setSide(int s) { side = s; }
    
    int getArea() const override {
        return side * side;
    }
};

// Now both can be used through Shape interface correctly
void printArea(const Shape& shape) {
    cout << "Area: " << shape.getArea() << endl;
}

Rectangle rect(5, 4);
printArea(rect);  // 20

Square square(5);
printArea(square);  // 25
```

### LSP Rules

1. **Preconditions cannot be strengthened** in derived class
2. **Postconditions cannot be weakened** in derived class
3. **Invariants must be preserved**
4. **No new exceptions** unless they're subtypes of base exceptions

### Another Example: Bird Hierarchy

```cpp
// BAD - Violates LSP
class Bird {
public:
    virtual void fly() {
        cout << "Flying..." << endl;
    }
};

class Penguin : public Bird {
public:
    void fly() override {
        // Penguins can't fly!
        throw runtime_error("Penguins can't fly!");
    }
};

void makeBirdFly(Bird& bird) {
    bird.fly();  // Expects all birds to fly - BREAKS with Penguin!
}

// GOOD - Respects LSP
class Bird {
public:
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual ~Bird() = default;
};

class FlyingBird : public Bird {
public:
    virtual void fly() = 0;
};

class Sparrow : public FlyingBird {
public:
    void eat() override { cout << "Sparrow eating" << endl; }
    void sleep() override { cout << "Sparrow sleeping" << endl; }
    void fly() override { cout << "Sparrow flying" << endl; }
};

class Penguin : public Bird {
public:
    void eat() override { cout << "Penguin eating" << endl; }
    void sleep() override { cout << "Penguin sleeping" << endl; }
    void swim() { cout << "Penguin swimming" << endl; }
};
```

## I - Interface Segregation Principle (ISP)

**"No client should be forced to depend on methods it does not use."**

Make fine-grained interfaces that are client-specific.

### ❌ Bad Example - Fat Interface

```cpp
class Worker {
public:
    virtual void work() = 0;
    virtual void eat() = 0;
    virtual void sleep() = 0;
    virtual void getPaid() = 0;
    virtual ~Worker() = default;
};

class Human : public Worker {
public:
    void work() override { cout << "Working..." << endl; }
    void eat() override { cout << "Eating..." << endl; }
    void sleep() override { cout << "Sleeping..." << endl; }
    void getPaid() override { cout << "Getting paid..." << endl; }
};

class Robot : public Worker {
public:
    void work() override { cout << "Robot working..." << endl; }
    
    // Forced to implement things robots don't do!
    void eat() override { /* Robots don't eat */ }
    void sleep() override { /* Robots don't sleep */ }
    void getPaid() override { /* Robots don't get paid */ }
};
```

### ✅ Good Example - Segregated Interfaces

```cpp
// Split into focused interfaces
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

class Payable {
public:
    virtual void getPaid() = 0;
    virtual ~Payable() = default;
};

// Human implements all interfaces
class Human : public Workable, public Eatable, public Sleepable, public Payable {
public:
    void work() override { cout << "Human working..." << endl; }
    void eat() override { cout << "Human eating..." << endl; }
    void sleep() override { cout << "Human sleeping..." << endl; }
    void getPaid() override { cout << "Human getting paid..." << endl; }
};

// Robot only implements what it needs
class Robot : public Workable {
public:
    void work() override { cout << "Robot working..." << endl; }
};

// Intern implements subset
class Intern : public Workable, public Eatable, public Sleepable {
public:
    void work() override { cout << "Intern working..." << endl; }
    void eat() override { cout << "Intern eating..." << endl; }
    void sleep() override { cout << "Intern sleeping..." << endl; }
    // Not paid! (sad but realistic)
};
```

## D - Dependency Inversion Principle (DIP)

**"High-level modules should not depend on low-level modules. Both should depend on abstractions."**

Depend on abstractions (interfaces), not concrete classes.

### ❌ Bad Example - Direct Dependency

```cpp
// Low-level module
class MySQLDatabase {
public:
    void connect() {
        cout << "Connecting to MySQL..." << endl;
    }
    
    void query(const string& sql) {
        cout << "Querying MySQL: " << sql << endl;
    }
};

// High-level module depends on concrete MySQL class
class UserService {
private:
    MySQLDatabase database;  // Direct dependency!
    
public:
    void getUser(int id) {
        database.connect();
        database.query("SELECT * FROM users WHERE id = " + to_string(id));
    }
};

// Problem: Can't switch to PostgreSQL without changing UserService!
```

### ✅ Good Example - Depend on Abstraction

```cpp
// Abstraction (interface)
class Database {
public:
    virtual void connect() = 0;
    virtual void query(const string& sql) = 0;
    virtual ~Database() = default;
};

// Low-level modules implement abstraction
class MySQLDatabase : public Database {
public:
    void connect() override {
        cout << "Connecting to MySQL..." << endl;
    }
    
    void query(const string& sql) override {
        cout << "Querying MySQL: " << sql << endl;
    }
};

class PostgreSQLDatabase : public Database {
public:
    void connect() override {
        cout << "Connecting to PostgreSQL..." << endl;
    }
    
    void query(const string& sql) override {
        cout << "Querying PostgreSQL: " << sql << endl;
    }
};

// High-level module depends on abstraction
class UserService {
private:
    Database* database;  // Depends on interface!
    
public:
    UserService(Database* db) : database(db) {}
    
    void getUser(int id) {
        database->connect();
        database->query("SELECT * FROM users WHERE id = " + to_string(id));
    }
};

// Can easily switch databases!
MySQLDatabase mysql;
UserService service1(&mysql);

PostgreSQLDatabase postgres;
UserService service2(&postgres);  // Works seamlessly!
```

### Dependency Injection

DIP is often implemented through **Dependency Injection**:

```cpp
class EmailService {
public:
    virtual void sendEmail(const string& to, const string& message) = 0;
    virtual ~EmailService() = default;
};

class SMTPEmailService : public EmailService {
public:
    void sendEmail(const string& to, const string& message) override {
        cout << "Sending via SMTP to " << to << ": " << message << endl;
    }
};

class SendGridEmailService : public EmailService {
public:
    void sendEmail(const string& to, const string& message) override {
        cout << "Sending via SendGrid to " << to << ": " << message << endl;
    }
};

class UserRegistration {
private:
    EmailService* emailService;  // Injected dependency
    
public:
    // Constructor injection
    UserRegistration(EmailService* service) : emailService(service) {}
    
    void registerUser(const string& email, const string& password) {
        // Registration logic...
        
        // Send welcome email
        emailService->sendEmail(email, "Welcome to our service!");
    }
};

// Easy to test - inject mock email service
// Easy to switch - inject different implementations
SMTPEmailService smtp;
UserRegistration registration1(&smtp);

SendGridEmailService sendgrid;
UserRegistration registration2(&sendgrid);
```

## Putting It All Together: E-commerce Example

```cpp
// S - Single Responsibility: Each class has one job

// Product class - manages product data only
class Product {
private:
    int id;
    string name;
    double price;
    
public:
    Product(int i, string n, double p) : id(i), name(n), price(p) {}
    
    int getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
};

// O - Open/Closed: Payment processing open for extension
class PaymentProcessor {
public:
    virtual bool processPayment(double amount) = 0;
    virtual ~PaymentProcessor() = default;
};

class CreditCardProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        cout << "Processing credit card payment: $" << amount << endl;
        return true;
    }
};

class PayPalProcessor : public PaymentProcessor {
public:
    bool processPayment(double amount) override {
        cout << "Processing PayPal payment: $" << amount << endl;
        return true;
    }
};

// L - Liskov Substitution: All processors can substitute base
void makePayment(PaymentProcessor* processor, double amount) {
    processor->processPayment(amount);  // Works with any processor
}

// I - Interface Segregation: Separate concerns
class ProductRepository {
public:
    virtual Product* findById(int id) = 0;
    virtual void save(const Product& product) = 0;
    virtual ~ProductRepository() = default;
};

class OrderRepository {
public:
    virtual void saveOrder(int orderId, double total) = 0;
    virtual ~OrderRepository() = default;
};

// D - Dependency Inversion: Depend on abstractions
class OrderService {
private:
    ProductRepository* productRepo;
    PaymentProcessor* paymentProcessor;
    OrderRepository* orderRepo;
    
public:
    // Dependencies injected
    OrderService(ProductRepository* pr, PaymentProcessor* pp, OrderRepository* or)
        : productRepo(pr), paymentProcessor(pp), orderRepo(or) {}
    
    bool placeOrder(int productId) {
        Product* product = productRepo->findById(productId);
        
        if (product && paymentProcessor->processPayment(product->getPrice())) {
            orderRepo->saveOrder(rand(), product->getPrice());
            cout << "Order placed successfully!" << endl;
            return true;
        }
        
        return false;
    }
};
```

## Summary

**SOLID Principles**:

1. **SRP**: One class, one responsibility
2. **OCP**: Open for extension, closed for modification
3. **LSP**: Subtypes must be substitutable for base types
4. **ISP**: Many specific interfaces better than one general interface
5. **DIP**: Depend on abstractions, not concretions

**Benefits of SOLID**:
- More maintainable code
- Easier to test
- More flexible and extensible
- Better code organization
- Reduced coupling
- Increased cohesion

---

**Next Lesson**: We'll explore **Composition vs Inheritance** and **Design Patterns**!