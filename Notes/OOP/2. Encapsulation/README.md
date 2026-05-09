# OOP Lesson 2: Encapsulation - The Protective Shell

## The Core Concept

**Encapsulation** = **Bundling data with methods** + **Restricting direct access to data**

Think of a capsule medicine pill: the medicine (data) is wrapped in a protective coating (class), and you can't directly touch the medicine inside.

## The Two Parts of Encapsulation

### Part 1: Data + Methods Together

```cpp
// WITHOUT Encapsulation (procedural)
struct Point {
    int x;
    int y;
};

void movePoint(Point* p, int dx, int dy) {
    p->x += dx;
    p->y += dy;
}

double distanceFromOrigin(const Point* p) {
    return sqrt(p->x * p->x + p->y * p->y);
}

// Data and functions are separate!
```

```cpp
// WITH Encapsulation
class Point {
private:
    int x;
    int y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
    
    double distanceFromOrigin() const {
        return sqrt(x * x + y * y);
    }
    
    // Getters provide controlled access
    int getX() const { return x; }
    int getY() const { return y; }
};

// Data and methods are bundled in one cohesive unit!
```

### Part 2: Access Control (Information Hiding)

This is the **critical** part most people miss. Encapsulation isn't just about grouping - it's about **hiding implementation details**.

```cpp
class BankAccount {
private:  // Implementation details - HIDDEN
    double balance;
    string accountNumber;
    vector<string> transactionLog;
    
    // Private helper method
    void logTransaction(const string& details) {
        transactionLog.push_back(details);
    }
    
public:   // Public interface - EXPOSED
    BankAccount(string accNum, double initial) 
        : accountNumber(accNum), balance(initial) {}
    
    // Controlled access with validation
    bool deposit(double amount) {
        if (amount <= 0) return false;
        
        balance += amount;
        logTransaction("Deposit: $" + to_string(amount));
        return true;
    }
    
    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance) return false;
        
        balance -= amount;
        logTransaction("Withdrawal: $" + to_string(amount));
        return true;
    }
    
    double getBalance() const { return balance; }
};
```

## Why Access Control Matters

### Example: Preventing Invalid States

```cpp
class Rectangle {
private:
    double width;
    double height;
    
public:
    // Without encapsulation, this could happen:
    // rect.width = -10;  // Invalid rectangle!
    
    // With encapsulation, we control it:
    void setWidth(double w) {
        if (w > 0) {
            width = w;
        } else {
            throw invalid_argument("Width must be positive");
        }
    }
    
    void setHeight(double h) {
        if (h > 0) {
            height = h;
        } else {
            throw invalid_argument("Height must be positive");
        }
    }
    
    double getWidth() const { return width; }
    double getHeight() const { return height; }
    double getArea() const { return width * height; }
};
```

### Real-World Example: Email Validator

```cpp
class User {
private:
    string email;
    bool emailVerified;
    
    // Private validation method
    bool isValidEmail(const string& email) {
        // Simplified check
        return email.find('@') != string::npos && 
               email.find('.') != string::npos;
    }
    
public:
    User(const string& email) : emailVerified(false) {
        setEmail(email);
    }
    
    void setEmail(const string& newEmail) {
        if (!isValidEmail(newEmail)) {
            throw invalid_argument("Invalid email format");
        }
        email = newEmail;
        emailVerified = false;  // Reset verification
    }
    
    string getEmail() const { return email; }
    bool isVerified() const { return emailVerified; }
    
    void verifyEmail() { emailVerified = true; }
};

// Usage
User user("john@example.com");  // OK
// User badUser("notanemail");   // Throws exception
```

## The Benefits of Encapsulation

### 1. Data Integrity

```cpp
class Temperature {
private:
    double kelvin;  // Store internally in Kelvin
    
public:
    // Can't set temperature below absolute zero
    void setKelvin(double k) {
        if (k < 0) {
            throw invalid_argument("Temperature below absolute zero!");
        }
        kelvin = k;
    }
    
    void setCelsius(double c) {
        setKelvin(c + 273.15);
    }
    
    void setFahrenheit(double f) {
        setCelsius((f - 32) * 5.0 / 9.0);
    }
    
    double getKelvin() const { return kelvin; }
    double getCelsius() const { return kelvin - 273.15; }
    double getFahrenheit() const { return getCelsius() * 9.0 / 5.0 + 32; }
};

// The user doesn't know (or care) that we store in Kelvin internally!
```

### 2. Flexibility to Change Implementation

```cpp
class Stack {
private:
    // Version 1: Array implementation
    // int data[100];
    // int top;
    
    // Version 2: Vector implementation (better!)
    vector<int> data;
    
public:
    void push(int value) {
        data.push_back(value);  // Changed internally
    }
    
    int pop() {
        if (data.empty()) throw runtime_error("Stack empty");
        int value = data.back();
        data.pop_back();
        return value;
    }
    
    int size() const {
        return data.size();
    }
    
    bool isEmpty() const {
        return data.empty();
    }
};

// Users of Stack class don't need to change their code!
// The public interface stayed the same
```

### 3. Easier Debugging and Maintenance

```cpp
class Counter {
private:
    int count;
    
public:
    Counter() : count(0) {}
    
    void increment() {
        count++;
        // Easy to add debugging here
        cout << "Counter incremented to: " << count << endl;
    }
    
    void decrement() {
        if (count > 0) {
            count--;
            cout << "Counter decremented to: " << count << endl;
        }
    }
    
    int getCount() const { return count; }
};

// If count could be modified directly, we'd never know where bugs came from!
```

## Access Specifiers in Detail

### Private

```cpp
class MyClass {
private:
    int secretData;  // Only accessible inside MyClass
    
    void secretMethod() {
        secretData = 42;  // OK - inside the class
    }
};

// Outside the class:
// myObj.secretData = 10;   // ERROR!
// myObj.secretMethod();    // ERROR!
```

### Protected (for inheritance - we'll cover this later)

```cpp
class Base {
protected:
    int protectedData;  // Accessible in Base and derived classes
};

class Derived : public Base {
    void method() {
        protectedData = 10;  // OK - in derived class
    }
};
```

### Public

```cpp
class MyClass {
public:
    int publicData;  // Accessible everywhere
    
    void publicMethod() {
        // Anyone can call this
    }
};

MyClass obj;
obj.publicData = 10;    // OK
obj.publicMethod();     // OK
```

## Best Practices for Encapsulation

### 1. Make Data Members Private

```cpp
// BAD
class Person {
public:
    string name;
    int age;
};

// GOOD
class Person {
private:
    string name;
    int age;
    
public:
    void setAge(int a) {
        if (a >= 0 && a <= 150) {
            age = a;
        }
    }
    
    int getAge() const { return age; }
};
```

### 2. Provide Getters and Setters Only When Needed

```cpp
class Password {
private:
    string hashedPassword;
    
public:
    // NO getter for hashedPassword - security!
    
    void setPassword(const string& plaintext) {
        hashedPassword = hash(plaintext);
    }
    
    bool verify(const string& plaintext) const {
        return hash(plaintext) == hashedPassword;
    }
};
```

### 3. Use Const Correctness

```cpp
class Book {
private:
    string title;
    string author;
    int pages;
    
public:
    // Getters should be const - they don't modify the object
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    int getPages() const { return pages; }
    
    // Setters are not const - they modify the object
    void setTitle(const string& t) { title = t; }
};
```

## Common Mistakes

### Mistake 1: Exposing Internal Data Structures

```cpp
// BAD - exposes internal implementation
class Library {
private:
    vector<Book> books;
    
public:
    vector<Book>& getBooks() {  // Returns reference to internal data!
        return books;
    }
};

// Someone can now do:
// library.getBooks().clear();  // DISASTER!

// GOOD - return a copy or const reference
class Library {
private:
    vector<Book> books;
    
public:
    vector<Book> getBooks() const {  // Returns copy
        return books;
    }
    
    // Or provide controlled access
    const Book& getBook(int index) const {
        return books.at(index);
    }
    
    void addBook(const Book& book) {
        books.push_back(book);
    }
};
```

### Mistake 2: Getters/Setters for Everything

```cpp
// BAD - defeats the purpose of encapsulation
class Point {
private:
    int x, y;
    
public:
    int getX() const { return x; }
    void setX(int x) { this->x = x; }
    int getY() const { return y; }
    void setY(int y) { this->y = y; }
};

// Essentially the same as public data!

// GOOD - provide meaningful operations
class Point {
private:
    int x, y;
    
public:
    Point(int x, int y) : x(x), y(y) {}
    
    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
    
    double distanceTo(const Point& other) const {
        int dx = x - other.x;
        int dy = y - other.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    // Only provide getters if really needed
    int getX() const { return x; }
    int getY() const { return y; }
};
```

## Summary

**Encapsulation is**:
- Bundling data and methods that operate on that data into a single unit (class)
- Hiding implementation details and exposing only what's necessary
- Protecting data integrity through controlled access
- The foundation that makes other OOP principles possible

**Key takeaway**: Encapsulation is about building a protective barrier around your data, exposing only a clean, safe interface to the outside world.

---

**Next Lesson**: We'll explore **Abstraction** and finally clear up the confusion between abstraction and encapsulation!