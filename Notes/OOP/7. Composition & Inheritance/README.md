# OOP Lesson 7: Composition vs Inheritance & Design Patterns

## Composition vs Inheritance

**"Favor composition over inheritance"** - Gang of Four (Design Patterns book)

This is one of the most important principles in OOP design.

### What is Composition?

**Composition** = Building complex objects by combining simpler objects (HAS-A relationship)

**Inheritance** = Building new classes from existing ones (IS-A relationship)

### The Key Difference

```cpp
// Inheritance: IS-A relationship
class Car : public Vehicle {
    // Car IS-A Vehicle
};

// Composition: HAS-A relationship
class Car {
private:
    Engine engine;      // Car HAS-AN Engine
    Wheels wheels;      // Car HAS Wheels
    Transmission trans; // Car HAS-A Transmission
};
```

## Why Favor Composition?

### Problem 1: Inheritance Creates Tight Coupling

```cpp
// Base class
class Animal {
protected:
    int health;
    
public:
    virtual void eat() { health += 10; }
    virtual void move() = 0;
};

// What if we change Animal's internal implementation?
class Dog : public Animal {
public:
    void move() override {
        // Dog depends on Animal's protected 'health' variable
        // If Animal changes 'health' implementation, Dog might break!
    }
};
```

### Problem 2: Inflexible Hierarchies

```cpp
// Rigid inheritance hierarchy
class Employee {
    virtual double getSalary() = 0;
};

class Manager : public Employee {
    double getSalary() override { return 100000; }
    void managePeople() { }
};

class Engineer : public Employee {
    double getSalary() override { return 80000; }
    void writeCode() { }
};

// What if someone is both Manager AND Engineer?
// Multiple inheritance gets messy!
```

### Solution: Composition

```cpp
// Define roles as separate components
class Role {
public:
    virtual double getSalary() const = 0;
    virtual void performRole() = 0;
    virtual ~Role() = default;
};

class ManagerRole : public Role {
public:
    double getSalary() const override { return 100000; }
    void performRole() override {
        cout << "Managing people..." << endl;
    }
};

class EngineerRole : public Role {
public:
    double getSalary() const override { return 80000; }
    void performRole() override {
        cout << "Writing code..." << endl;
    }
};

// Employee HAS roles (composition)
class Employee {
private:
    string name;
    vector<Role*> roles;  // Can have multiple roles!
    
public:
    Employee(string n) : name(n) {}
    
    void addRole(Role* role) {
        roles.push_back(role);
    }
    
    double getTotalSalary() const {
        double total = 0;
        for (const Role* role : roles) {
            total += role->getSalary();
        }
        return total;
    }
    
    void performAllRoles() {
        for (Role* role : roles) {
            role->performRole();
        }
    }
};

// Now someone can be BOTH manager and engineer!
Employee bob("Bob");
bob.addRole(new ManagerRole());
bob.addRole(new EngineerRole());
cout << "Total salary: $" << bob.getTotalSalary() << endl;
bob.performAllRoles();
```

### Problem 3: Gorilla-Banana Problem

> "You wanted a banana but what you got was a gorilla holding the banana and the entire jungle." - Joe Armstrong

```cpp
// Inheritance: Want one method, get entire class hierarchy
class JungleAnimal {
    void breathe() { }
    void eat() { }
    void sleep() { }
    void reproduce() { }
    // ... 50 more methods
};

class Gorilla : public JungleAnimal {
    void holdBanana() { }  // This is what we want
    // But we inherit 50+ methods we don't need!
};

// Composition: Take only what you need
class BananaHolder {
    void holdBanana() { }
};

class Gorilla {
private:
    BananaHolder bananaHolder;  // Only what we need
    
public:
    void hold() {
        bananaHolder.holdBanana();
    }
};
```

## Composition Patterns

### 1. Strategy Pattern (Behavior Composition)

```cpp
// Different algorithms as separate components
class FlyingBehavior {
public:
    virtual void fly() = 0;
    virtual ~FlyingBehavior() = default;
};

class FlyWithWings : public FlyingBehavior {
public:
    void fly() override {
        cout << "Flying with wings!" << endl;
    }
};

class FlyWithRocket : public FlyingBehavior {
public:
    void fly() override {
        cout << "Flying with rocket!" << endl;
    }
};

class NoFlying : public FlyingBehavior {
public:
    void fly() override {
        cout << "Can't fly!" << endl;
    }
};

// Duck uses composition
class Duck {
private:
    FlyingBehavior* flyingBehavior;
    
public:
    Duck(FlyingBehavior* fb) : flyingBehavior(fb) {}
    
    void performFly() {
        flyingBehavior->fly();
    }
    
    // Can change behavior at runtime!
    void setFlyingBehavior(FlyingBehavior* fb) {
        flyingBehavior = fb;
    }
};

// Usage
Duck mallard(new FlyWithWings());
mallard.performFly();  // Flying with wings!

// Change behavior dynamically!
mallard.setFlyingBehavior(new FlyWithRocket());
mallard.performFly();  // Flying with rocket!
```

### 2. Decorator Pattern (Adding Functionality)

```cpp
// Base component
class Coffee {
public:
    virtual double cost() const = 0;
    virtual string description() const = 0;
    virtual ~Coffee() = default;
};

// Concrete component
class SimpleCoffee : public Coffee {
public:
    double cost() const override {
        return 2.0;
    }
    
    string description() const override {
        return "Simple coffee";
    }
};

// Decorator base
class CoffeeDecorator : public Coffee {
protected:
    Coffee* coffee;
    
public:
    CoffeeDecorator(Coffee* c) : coffee(c) {}
};

// Concrete decorators
class Milk : public CoffeeDecorator {
public:
    Milk(Coffee* c) : CoffeeDecorator(c) {}
    
    double cost() const override {
        return coffee->cost() + 0.5;
    }
    
    string description() const override {
        return coffee->description() + ", milk";
    }
};

class Sugar : public CoffeeDecorator {
public:
    Sugar(Coffee* c) : CoffeeDecorator(c) {}
    
    double cost() const override {
        return coffee->cost() + 0.2;
    }
    
    string description() const override {
        return coffee->description() + ", sugar";
    }
};

class Whip : public CoffeeDecorator {
public:
    Whip(Coffee* c) : CoffeeDecorator(c) {}
    
    double cost() const override {
        return coffee->cost() + 0.7;
    }
    
    string description() const override {
        return coffee->description() + ", whip";
    }
};

// Usage - wrap decorators around each other!
Coffee* myCoffee = new SimpleCoffee();
cout << myCoffee->description() << " = $" << myCoffee->cost() << endl;

myCoffee = new Milk(myCoffee);
cout << myCoffee->description() << " = $" << myCoffee->cost() << endl;

myCoffee = new Sugar(myCoffee);
cout << myCoffee->description() << " = $" << myCoffee->cost() << endl;

myCoffee = new Whip(myCoffee);
cout << myCoffee->description() << " = $" << myCoffee->cost() << endl;

// Output:
// Simple coffee = $2.0
// Simple coffee, milk = $2.5
// Simple coffee, milk, sugar = $2.7
// Simple coffee, milk, sugar, whip = $3.4
```

## Essential Design Patterns

### 1. Factory Pattern

Creates objects without specifying exact class.

```cpp
// Product hierarchy
class Animal {
public:
    virtual void speak() = 0;
    virtual ~Animal() = default;
};

class Dog : public Animal {
public:
    void speak() override { cout << "Woof!" << endl; }
};

class Cat : public Animal {
public:
    void speak() override { cout << "Meow!" << endl; }
};

class Duck : public Animal {
public:
    void speak() override { cout << "Quack!" << endl; }
};

// Factory
class AnimalFactory {
public:
    static Animal* createAnimal(const string& type) {
        if (type == "dog") {
            return new Dog();
        } else if (type == "cat") {
            return new Cat();
        } else if (type == "duck") {
            return new Duck();
        }
        return nullptr;
    }
};

// Usage - client doesn't know about concrete classes
Animal* pet = AnimalFactory::createAnimal("dog");
pet->speak();  // Woof!

Animal* pet2 = AnimalFactory::createAnimal("cat");
pet2->speak();  // Meow!
```

### 2. Singleton Pattern

Ensures only one instance of a class exists.

```cpp
class DatabaseConnection {
private:
    static DatabaseConnection* instance;
    
    // Private constructor
    DatabaseConnection() {
        cout << "Database connection established" << endl;
    }
    
public:
    // Delete copy constructor and assignment operator
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    
    static DatabaseConnection* getInstance() {
        if (instance == nullptr) {
            instance = new DatabaseConnection();
        }
        return instance;
    }
    
    void query(const string& sql) {
        cout << "Executing: " << sql << endl;
    }
};

DatabaseConnection* DatabaseConnection::instance = nullptr;

// Usage
DatabaseConnection* db1 = DatabaseConnection::getInstance();
DatabaseConnection* db2 = DatabaseConnection::getInstance();

// Both point to same instance!
cout << (db1 == db2) << endl;  // true
```

### 3. Observer Pattern

One-to-many dependency between objects.

```cpp
// Observer interface
class Observer {
public:
    virtual void update(float temperature) = 0;
    virtual ~Observer() = default;
};

// Subject (observable)
class WeatherStation {
private:
    vector<Observer*> observers;
    float temperature;
    
public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    
    void removeObserver(Observer* observer) {
        observers.erase(
            remove(observers.begin(), observers.end(), observer),
            observers.end()
        );
    }
    
    void setTemperature(float temp) {
        temperature = temp;
        notifyObservers();
    }
    
    void notifyObservers() {
        for (Observer* observer : observers) {
            observer->update(temperature);
        }
    }
};

// Concrete observers
class PhoneDisplay : public Observer {
public:
    void update(float temperature) override {
        cout << "Phone: Temperature is " << temperature << "°C" << endl;
    }
};

class TVDisplay : public Observer {
public:
    void update(float temperature) override {
        cout << "TV: Temperature is " << temperature << "°C" << endl;
    }
};

class WebDisplay : public Observer {
public:
    void update(float temperature) override {
        cout << "Website: Temperature is " << temperature << "°C" << endl;
    }
};

// Usage
WeatherStation station;

PhoneDisplay phone;
TVDisplay tv;
WebDisplay web;

station.addObserver(&phone);
station.addObserver(&tv);
station.addObserver(&web);

station.setTemperature(25.0);
// All displays are notified!
```

### 4. Template Method Pattern

Defines skeleton of algorithm, subclasses fill in details.

```cpp
class DataProcessor {
public:
    // Template method - defines the algorithm structure
    void process() {
        readData();
        processData();
        writeData();
    }
    
protected:
    virtual void readData() = 0;      // Must implement
    virtual void processData() = 0;   // Must implement
    virtual void writeData() = 0;     // Must implement
    
    virtual ~DataProcessor() = default;
};

class CSVProcessor : public DataProcessor {
protected:
    void readData() override {
        cout << "Reading CSV file..." << endl;
    }
    
    void processData() override {
        cout << "Processing CSV data..." << endl;
    }
    
    void writeData() override {
        cout << "Writing CSV results..." << endl;
    }
};

class JSONProcessor : public DataProcessor {
protected:
    void readData() override {
        cout << "Reading JSON file..." << endl;
    }
    
    void processData() override {
        cout << "Processing JSON data..." << endl;
    }
    
    void writeData() override {
        cout << "Writing JSON results..." << endl;
    }
};

// Usage
DataProcessor* processor = new CSVProcessor();
processor->process();
// Reading CSV file...
// Processing CSV data...
// Writing CSV results...
```

### 5. Adapter Pattern

Makes incompatible interfaces work together.

```cpp
// Target interface (what we want)
class MediaPlayer {
public:
    virtual void play(const string& filename) = 0;
    virtual ~MediaPlayer() = default;
};

// Adaptee (existing incompatible class)
class AdvancedMediaPlayer {
public:
    virtual void playVLC(const string& filename) {
        cout << "Playing VLC file: " << filename << endl;
    }
    
    virtual void playMP4(const string& filename) {
        cout << "Playing MP4 file: " << filename << endl;
    }
};

// Adapter - makes AdvancedMediaPlayer compatible with MediaPlayer
class MediaAdapter : public MediaPlayer {
private:
    AdvancedMediaPlayer* advancedPlayer;
    
public:
    MediaAdapter() {
        advancedPlayer = new AdvancedMediaPlayer();
    }
    
    void play(const string& filename) override {
        if (filename.find(".vlc") != string::npos) {
            advancedPlayer->playVLC(filename);
        } else if (filename.find(".mp4") != string::npos) {
            advancedPlayer->playMP4(filename);
        } else {
            cout << "Invalid format" << endl;
        }
    }
    
    ~MediaAdapter() {
        delete advancedPlayer;
    }
};

// Usage
MediaPlayer* player = new MediaAdapter();
player->play("movie.vlc");  // Playing VLC file: movie.vlc
player->play("song.mp4");   // Playing MP4 file: song.mp4
```

## When to Use Inheritance vs Composition

### Use Inheritance When:

1. **True IS-A relationship exists**
```cpp
Dog IS-A Animal  ✓
```

2. **Need polymorphism**
```cpp
void feedAnimal(Animal* animal) {
    animal->eat();  // Works with any animal type
}
```

3. **Shared interface and behavior**
```cpp
class Shape {
    virtual double area() = 0;
};
```

### Use Composition When:

1. **HAS-A relationship**
```cpp
Car HAS-AN Engine  ✓
```

2. **Need flexibility**
```cpp
// Can swap components easily
car.setEngine(new ElectricEngine());
```

3. **Want to avoid deep hierarchies**
```cpp
// Instead of: Vehicle → Car → SportsCar → Ferrari
// Use: Car { Engine, Transmission, Suspension }
```

4. **Need multiple behaviors**
```cpp
class Character {
    WeaponBehavior* weapon;
    MovementBehavior* movement;
    // Can mix and match!
};
```

## Practical Example: Game Character System

### ❌ Inheritance Approach (Problematic)

```cpp
class Character { };
class Warrior : public Character { };
class Mage : public Character { };
class Archer : public Character { };

// What if we want a Warrior who can cast spells?
// What if we want an Archer who can melee attack?
// Inheritance becomes a nightmare!
```

### ✅ Composition Approach (Flexible)

```cpp
// Components
class AttackBehavior {
public:
    virtual void attack() = 0;
    virtual ~AttackBehavior() = default;
};

class MeleeAttack : public AttackBehavior {
public:
    void attack() override { cout << "Sword slash!" << endl; }
};

class RangedAttack : public AttackBehavior {
public:
    void attack() override { cout << "Bow shot!" << endl; }
};

class MagicAttack : public AttackBehavior {
public:
    void attack() override { cout << "Fireball!" << endl; }
};

class DefenseBehavior {
public:
    virtual void defend() = 0;
    virtual ~DefenseBehavior() = default;
};

class ShieldDefense : public DefenseBehavior {
public:
    void defend() override { cout << "Blocking with shield!" << endl; }
};

class DodgeDefense : public DefenseBehavior {
public:
    void defend() override { cout << "Dodging!" << endl; }
};

// Character uses composition
class Character {
private:
    string name;
    AttackBehavior* attack;
    DefenseBehavior* defense;
    
public:
    Character(string n, AttackBehavior* a, DefenseBehavior* d)
        : name(n), attack(a), defense(d) {}
    
    void performAttack() {
        attack->attack();
    }
    
    void performDefense() {
        defense->defend();
    }
    
    // Can change behaviors at runtime!
    void setAttack(AttackBehavior* a) { attack = a; }
    void setDefense(DefenseBehavior* d) { defense = d; }
};

// Create any combination!
Character warrior("Conan", new MeleeAttack(), new ShieldDefense());
Character archer("Legolas", new RangedAttack(), new DodgeDefense());
Character battlemage("Gandalf", new MagicAttack(), new ShieldDefense());

// Can switch strategies
warrior.setAttack(new MagicAttack());  // Warrior learns magic!
```

## Summary

**Composition vs Inheritance**:
- Inheritance: IS-A, rigid, tight coupling
- Composition: HAS-A, flexible, loose coupling
- **Prefer composition** for flexibility and maintainability

**Key Design Patterns**:
- Factory: Object creation
- Singleton: One instance
- Observer: Event notification
- Strategy: Interchangeable algorithms
- Decorator: Add functionality dynamically
- Adapter: Interface compatibility
- Template Method: Algorithm skeleton

**Remember**: "Favor composition over inheritance, but use inheritance when it makes sense!"

---

**Next Lesson**: We'll explore **Advanced C++ OOP Features** including move semantics, smart pointers, and modern C++ best practices!