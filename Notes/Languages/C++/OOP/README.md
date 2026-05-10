# OOP

## Car

```cpp
class Car {
    // private by default
    public:
    string brand;
    string model;
    int year;

    void displayInfo() {
        cout << "Brand :" << brand << endl;
        cout << "Model :" << model << endl;
        cout << "Year  :" << year << endl;
    }
}

int main() {
    Car car1;
    car1.brand = "Mercedes";
    car1.model = "C180";
    car1.year = 2020;
}
```

### ClassName:: — Access class-level items

Use scope resolution (::) to access things that belong to the class itself, not to a particular object.

Examples:

```cpp
//Static member
class Math {
public:
    static int count;
};

int Math::count = 0;

Static method
Math::count;

// Enum inside the class
Car::Type::Sedan;

// Defining a method outside the class
void Car::start() {
    // code
}
```

Why?
Because :: tells the compiler “this belongs to the class, not an instance.”

Ask yourself:
Am I referencing something tied to the class itself and not a specific object?
If yes → use ::


|Situation|	Example	|Use|
|---|---|---|
|Accessing a member of an object	|obj.run()|	.|
|Accessing a member through a pointer	|ptr->run()|	->|
|Accessing static members|	Class::member	|::|
|Defining class methods outside class|	ReturnType Class::method()|	::|
|Accessing enum or type inside class|	Class::NestedType	|::|

To master this, ask yourself where does the thing I want live?

In an object? → .

Behind a pointer? → ->

In the class itself? → ::

This is the mindset you need to build.

A question for you

Imagine you have this:

class Player {
public:
    static int totalPlayers;
    int health;

    void heal() { health += 10; }
};


And you write:

Player* p = new Player();


Which one is valid and why?

p.totalPlayers

p->heal()

Player.health

Player::totalPlayers

Tell me your answers and why you chose them.
Your reasoning is more important than the answer.