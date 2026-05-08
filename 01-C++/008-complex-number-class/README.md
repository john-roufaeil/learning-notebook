# Complex Number Class

An implementation of complex number class to practice copy constructor, pass-by-reference, and operator overloading.

## 1. Copy Constructor & Assignment
```cpp
// Copy constructor - creates new object from existing one
Complex c1(3, 4);
Complex c2(c1);      // Calls copy constructor
Complex c3 = c1;     // Also calls copy constructor

// Assignment operator - assigns to existing object
Complex c4(1, 2);
c4 = c1;             // Calls operator=
```

**Implementation:**
```cpp
Complex(const Complex& other);           // Copy constructor
Complex& operator=(const Complex& other); // Copy assignment
```

## 2. Pass-by-Reference vs Pass-by-Value

| Method | Syntax | When to Use |
|--------|--------|-------------|
| **Const reference** | `const Complex&` | Read-only parameter (no copy) |
| **Reference** | `Complex&` | Modify parameter (no copy) |
| **Value** | `Complex` | Need a copy or returning new object |

**Examples in this class:**
```cpp
// Read only - no copy needed
Complex operator+(const Complex& other) const;  // ✓ Efficient

// Modify and return - enables chaining
Complex& operator+=(const Complex& other);      // ✓ Returns reference

// Create new object - must return by value
Complex operator-() const;                      // ✓ Returns new object
```

## 3. Operator Overloading

```cpp
Complex a(3, 4), b(1, 2);
Complex c = a + b;       // Binary +
Complex d = -a;          // Unary -
Complex e = a * 2;       // Member function
Complex f = 2 * a;       // Non-member function
```

## Notes

### 1. Return Type
- **Reference (`Complex&`)**: When returning `*this` (compound assignment, prefix ++/--)
- **Value (`Complex`)**: When creating new objects (arithmetic, postfix ++/--)
- **`const` methods**: Operations that don't modify the object

### 2. Const Correctness
```cpp
double getReal() const;              // Getter doesn't modify
Complex operator+(const Complex& other) const;  // Arithmetic doesn't modify
void setReal(double real);           // Setter modifies (not const)
```

### 3. Friend vs Non-Friend Functions
```cpp
// Non-member function
Complex operator+(double n, const Complex& c);  // Uses public interface

// Friend function
friend Complex operator+(double n, const Complex& c);  // Direct member access
```

### Implementation Notes

- **Epsilon comparison**: Floating-point equality uses `EPSILON = 1e-9` tolerance
- **Division by zero**: Throws `std::invalid_argument` exception
- **Self-assignment**: Protected with `if (this != &other)` check
- **Move semantics**: Omitted (no performance benefit for primitive types)

## References

- [Copy constructors](https://en.cppreference.com/w/cpp/language/copy_constructor)
- [Operator overloading](https://en.cppreference.com/w/cpp/language/operators)
- [Pass by reference](https://en.cppreference.com/w/cpp/language/reference)