# OOP Lesson 8: Advanced C++ OOP Features

## Modern C++ OOP (C++11 and Beyond)

Let's explore advanced features that make C++ OOP powerful and efficient.

## Smart Pointers: Automatic Memory Management

### The Problem with Raw Pointers

```cpp
class Resource {
public:
    Resource() { cout << "Resource acquired" << endl; }
    ~Resource() { cout << "Resource released" << endl; }
};

void problematicFunction() {
    Resource* res = new Resource();
    
    // If exception thrown here, memory leaks!
    if (someCondition) {
        return;  // Memory leak - forgot to delete!
    }
    
    delete res;  // Only reaches here sometimes
}
```

### 1. unique_ptr - Exclusive Ownership

```cpp
#include <memory>

class Resource {
public:
    Resource(string n) : name(n) {
        cout << "Resource " << name << " acquired" << endl;
    }
    
    ~Resource() {
        cout << "Resource " << name << " released" << endl;
    }
    
    void use() {
        cout << "Using resource " << name << endl;
    }
    
private:
    string name;
};

void uniquePtrExample() {
    // Automatic memory management!
    unique_ptr<Resource> res1 = make_unique<Resource>("A");
    res1->use();
    
    // unique_ptr<Resource> res2 = res1;  // ERROR: Can't copy!
    unique_ptr<Resource> res2 = move(res1);  // Transfer ownership
    
    // res1 is now null
    if (res1 == nullptr) {
        cout << "res1 is null" << endl;
    }
    
    res2->use();
    
    // Automatically deleted when going out of scope!
}

// Perfect for class members
class GameWorld {
private:
    unique_ptr<Player> player;
    vector<unique_ptr<Enemy>> enemies;
    
public:
    GameWorld() {
        player = make_unique<Player>("Hero");
        enemies.push_back(make_unique<Enemy>("Goblin"));
        enemies.push_back(make_unique<Enemy>("Orc"));
    }
    
    // No need for destructor - smart pointers handle cleanup!
};
```

### 2. shared_ptr - Shared Ownership

```cpp
class Texture {
public:
    Texture(string n) : name(n) {
        cout << "Loading texture: " << name << endl;
    }
    
    ~Texture() {
        cout << "Unloading texture: " << name << endl;
    }
    
    string name;
};

class Sprite {
private:
    shared_ptr<Texture> texture;
    
public:
    Sprite(shared_ptr<Texture> tex) : texture(tex) {
        cout << "Sprite using " << texture->name << endl;
    }
};

void sharedPtrExample() {
    // Multiple owners can share the same resource
    shared_ptr<Texture> tex = make_shared<Texture>("stone.png");
    
    cout << "Reference count: " << tex.use_count() << endl;  // 1
    
    {
        Sprite sprite1(tex);
        cout << "Reference count: " << tex.use_count() << endl;  // 2
        
        Sprite sprite2(tex);
        cout << "Reference count: " << tex.use_count() << endl;  // 3
        
        // sprites destroyed here
    }
    
    cout << "Reference count: " << tex.use_count() << endl;  // 1
    
    // Texture destroyed when last shared_ptr goes out of scope
}
```

### 3. weak_ptr - Breaking Circular References

```cpp
class Person;

class Person {
public:
    string name;
    shared_ptr<Person> partner;  // Strong reference
    
    Person(string n) : name(n) {
        cout << name << " created" << endl;
    }
    
    ~Person() {
        cout << name << " destroyed" << endl;
    }
    
    void setPartner(shared_ptr<Person> p) {
        partner = p;
    }
};

void circularReferenceProblem() {
    shared_ptr<Person> alice = make_shared<Person>("Alice");
    shared_ptr<Person> bob = make_shared<Person>("Bob");
    
    alice->setPartner(bob);
    bob->setPartner(alice);  // Circular reference!
    
    // Memory leak! Neither gets destroyed because they reference each other
}

// Solution: Use weak_ptr
class PersonFixed {
public:
    string name;
    weak_ptr<PersonFixed> partner;  // Weak reference - doesn't increase count
    
    PersonFixed(string n) : name(n) {
        cout << name << " created" << endl;
    }
    
    ~PersonFixed() {
        cout << name << " destroyed" << endl;
    }
    
    void setPartner(shared_ptr<PersonFixed> p) {
        partner = p;
    }
    
    void greetPartner() {
        // Convert weak_ptr to shared_ptr to use it
        if (auto p = partner.lock()) {  // lock() returns shared_ptr
            cout << name << " says hi to " << p->name << endl;
        } else {
            cout << name << " has no partner" << endl;
        }
    }
};
```

## Move Semantics - Efficient Resource Transfer

### The Problem: Unnecessary Copying

```cpp
class BigData {
private:
    int* data;
    size_t size;
    
public:
    BigData(size_t s) : size(s) {
        data = new int[size];
        cout << "Allocating " << size << " ints" << endl;
    }
    
    // Copy constructor - expensive!
    BigData(const BigData& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
        cout << "Copying " << size << " ints" << endl;
    }
    
    ~BigData() {
        delete[] data;
        cout << "Deallocating" << endl;
    }
};

BigData createData() {
    BigData temp(1000000);  // Allocate
    return temp;            // Copy (expensive!)
}

BigData myData = createData();  // Another copy!
```

### The Solution: Move Constructor and Move Assignment

```cpp
class BigData {
private:
    int* data;
    size_t size;
    
public:
    BigData(size_t s) : size(s) {
        data = new int[size];
        cout << "Allocating " << size << " ints" << endl;
    }
    
    // Copy constructor
    BigData(const BigData& other) : size(other.size) {
        data = new int[size];
        copy(other.data, other.data + size, data);
        cout << "Copying " << size << " ints" << endl;
    }
    
    // Move constructor - steal resources!
    BigData(BigData&& other) noexcept : data(other.data), size(other.size) {
        other.data = nullptr;  // Leave other in valid state
        other.size = 0;
        cout << "Moving " << size << " ints (cheap!)" << endl;
    }
    
    // Copy assignment
    BigData& operator=(const BigData& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new int[size];
            copy(other.data, other.data + size, data);
            cout << "Copy assigning" << endl;
        }
        return *this;
    }
    
    // Move assignment
    BigData& operator=(BigData&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
            cout << "Move assigning (cheap!)" << endl;
        }
        return *this;
    }
    
    ~BigData() {
        delete[] data;
    }
};

// Usage
BigData data1(1000000);           // Allocating
BigData data2 = move(data1);      // Moving (cheap!)
BigData data3(500000);            // Allocating
data3 = move(data2);              // Move assigning (cheap!)
```

### Perfect Forwarding

```cpp
template<typename T>
class Factory {
public:
    // Perfect forwarding - preserves value category
    template<typename... Args>
    static unique_ptr<T> create(Args&&... args) {
        return make_unique<T>(forward<Args>(args)...);
    }
};

class Product {
public:
    Product(int x, string s) {
        cout << "Product(" << x << ", " << s << ")" << endl;
    }
};

auto product = Factory<Product>::create(42, "Hello");
```

## Rule of Five/Zero

### Rule of Five

If you define one of these, you should define all:
1. Destructor
2. Copy constructor
3. Copy assignment operator
4. Move constructor
5. Move assignment operator

```cpp
class Resource {
private:
    int* data;
    
public:
    // Constructor
    Resource(int value) : data(new int(value)) {}
    
    // 1. Destructor
    ~Resource() {
        delete data;
    }
    
    // 2. Copy constructor
    Resource(const Resource& other) : data(new int(*other.data)) {}
    
    // 3. Copy assignment
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            delete data;
            data = new int(*other.data);
        }
        return *this;
    }
    
    // 4. Move constructor
    Resource(Resource&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }
    
    // 5. Move assignment
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};
```

### Rule of Zero

**Better approach**: Don't manage resources manually!

```cpp
class Resource {
private:
    unique_ptr<int> data;  // Smart pointer manages memory
    
public:
    Resource(int value) : data(make_unique<int>(value)) {}
    
    // No need to define destructor, copy/move operations!
    // Compiler-generated versions work correctly
};
```

## Virtual Inheritance

Solves the diamond problem in multiple inheritance.

```cpp
// Diamond problem
class Animal {
public:
    int age;
};

class Mammal : public Animal { };
class WingedAnimal : public Animal { };

class Bat : public Mammal, public WingedAnimal {
    // Has TWO copies of Animal::age - ambiguous!
    // Which age? mammal.age or wingedAnimal.age?
};

// Solution: Virtual inheritance
class Animal {
public:
    int age;
    Animal() : age(0) {}
};

class Mammal : virtual public Animal { };
class WingedAnimal : virtual public Animal { };

class Bat : public Mammal, public WingedAnimal {
    // Only ONE copy of Animal - no ambiguity!
};

Bat bat;
bat.age = 5;  // OK - only one age variable
```

## Final and Override Keywords

### final - Prevent Overriding

```cpp
class Base {
public:
    virtual void method() final {  // Can't be overridden
        cout << "Base method" << endl;
    }
};

class Derived : public Base {
    // void method() override { }  // ERROR: Can't override final method
};

// final class - can't be inherited
class FinalClass final {
public:
    void method() { }
};

// class Derived : public FinalClass { };  // ERROR: Can't inherit from final class
```

### override - Catch Mistakes

```cpp
class Base {
public:
    virtual void process() { }
    virtual void calculate(int x) { }
};

class Derived : public Base {
public:
    void process() override { }  // OK
    
    // void process(int x) override { }  // ERROR: Not overriding anything
    // void calculate() override { }     // ERROR: Wrong signature
    
    void calculate(int x) override { }  // OK
};
```

## Static Members and Methods

```cpp
class Counter {
private:
    static int count;  // Shared by all instances
    int id;
    
public:
    Counter() {
        id = ++count;
        cout << "Counter " << id << " created" << endl;
    }
    
    // Static method - can't access non-static members
    static int getCount() {
        return count;
        // return id;  // ERROR: Can't access non-static member
    }
    
    int getId() const { return id; }
};

// Must define static member outside class
int Counter::count = 0;

// Usage
Counter c1;  // Counter 1 created
Counter c2;  // Counter 2 created
Counter c3;  // Counter 3 created

cout << "Total counters: " << Counter::getCount() << endl;  // 3
```

## Nested Classes

```cpp
class OuterClass {
private:
    int outerData;
    
    // Nested class
    class InnerClass {
    private:
        int innerData;
        
    public:
        void accessOuter(OuterClass& outer) {
            // Can access private members of outer class
            outer.outerData = 10;
        }
    };
    
    InnerClass inner;
    
public:
    OuterClass() : outerData(0) {}
    
    void useInner() {
        inner.accessOuter(*this);
    }
};
```

## Friend Classes and Functions

```cpp
class BankAccount {
private:
    double balance;
    string accountNumber;
    
    // Friend function can access private members
    friend void printAccountDetails(const BankAccount& account);
    
    // Friend class can access private members
    friend class BankAuditor;
    
public:
    BankAccount(string num, double bal) 
        : accountNumber(num), balance(bal) {}
};

// Friend function definition
void printAccountDetails(const BankAccount& account) {
    cout << "Account: " << account.accountNumber << endl;
    cout << "Balance: $" << account.balance << endl;
}

// Friend class definition
class BankAuditor {
public:
    void audit(const BankAccount& account) {
        cout << "Auditing account " << account.accountNumber << endl;
        cout << "Balance: $" << account.balance << endl;
        
        if (account.balance < 0) {
            cout << "Warning: Negative balance!" << endl;
        }
    }
};
```

## Practical Example: Game Engine Entity System

```cpp
class Transform {
private:
    float x, y, z;
    
public:
    Transform(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    
    void setPosition(float newX, float newY, float newZ) {
        x = newX; y = newY; z = newZ;
    }
    
    void getPosition(float& outX, float& outY, float& outZ) const {
        outX = x; outY = y; outZ = z;
    }
};

class Component {
public:
    virtual void update(float deltaTime) = 0;
    virtual ~Component() = default;
};

class RenderComponent : public Component {
public:
    void update(float deltaTime) override {
        cout << "Rendering..." << endl;
    }
};

class PhysicsComponent : public Component {
public:
    void update(float deltaTime) override {
        cout << "Updating physics..." << endl;
    }
};

class Entity {
private:
    unique_ptr<Transform> transform;
    vector<unique_ptr<Component>> components;
    string name;
    
public:
    Entity(string n) : name(n) {
        transform = make_unique<Transform>();
    }
    
    // Perfect forwarding to create components
    template<typename T, typename... Args>
    T* addComponent(Args&&... args) {
        auto component = make_unique<T>(forward<Args>(args)...);
        T* ptr = component.get();
        components.push_back(move(component));
        return ptr;
    }
    
    void update(float deltaTime) {
        for (auto& component : components) {
            component->update(deltaTime);
        }
    }
    
    Transform* getTransform() { return transform.get(); }
};

// Usage
Entity player("Player");
player.addComponent<RenderComponent>();
player.addComponent<PhysicsComponent>();

player.getTransform()->setPosition(10, 5, 0);
player.update(0.016f);  // 60 FPS
```

## Best Practices Summary

1. **Use smart pointers** instead of raw pointers
2. **Follow Rule of Zero** when possible
3. **Use move semantics** for large objects
4. **Mark overrides** with `override` keyword
5. **Use `final`** to prevent unwanted inheritance
6. **Prefer `unique_ptr`** over `shared_ptr` when possible
7. **Use `make_unique`/`make_shared`** instead of `new`
8. **Avoid manual memory management**
9. **Use `const` wherever possible**
10. **Leverage RAII** (Resource Acquisition Is Initialization)

## Summary

Modern C++ OOP features:
- **Smart pointers**: Automatic memory management
- **Move semantics**: Efficient resource transfer
- **Rule of Five/Zero**: Resource management guidelines
- **`override` and `final`**: Better inheritance control
- **Virtual inheritance**: Solving diamond problem
- **Static members**: Class-level data and methods
- **Friend classes**: Controlled access to private members

These features make C++ OOP safer, more efficient, and easier to maintain!

---

**Congratulations!** You've completed the comprehensive OOP mastery course. You now understand everything from basic concepts to advanced C++ features!