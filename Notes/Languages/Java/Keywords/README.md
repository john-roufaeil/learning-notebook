# Java Keywords & Comparisons - Complete Guide

## What Are Keywords?

**Keywords** (also called reserved words) are predefined words in Java that have special meaning and cannot be used as identifiers (variable names, method names, class names, etc.).

Java has **51 keywords** divided into several categories.

---

# Part 1: All Java Keywords by Category

## 1. Data Type Keywords (8)

```java
// Primitive types
byte myByte = 127;
short myShort = 32000;
int myInt = 2147483647;
long myLong = 9223372036854775807L;
float myFloat = 3.14f;
double myDouble = 3.14159265359;
char myChar = 'A';
boolean myBoolean = true;
```

## 2. Control Flow Keywords (12)

### Conditional Statements

```java
// if, else
int age = 20;
if (age >= 18) {
    System.out.println("Adult");
} else {
    System.out.println("Minor");
}

// switch, case, default
String day = "Monday";
switch (day) {
    case "Monday":
        System.out.println("Start of week");
        break;
    case "Friday":
        System.out.println("Almost weekend!");
        break;
    default:
        System.out.println("Regular day");
}
```

### Loops

```java
// for
for (int i = 0; i < 5; i++) {
    System.out.println(i);
}

// while
int count = 0;
while (count < 5) {
    System.out.println(count);
    count++;
}

// do (with while)
int num = 0;
do {
    System.out.println(num);
    num++;
} while (num < 5);
```

### Loop Control

```java
// break - exit loop
for (int i = 0; i < 10; i++) {
    if (i == 5) {
        break;  // Exit loop when i is 5
    }
    System.out.println(i);
}

// continue - skip iteration
for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
        continue;  // Skip even numbers
    }
    System.out.println(i);  // Prints odd numbers only
}

// return - exit method
public int calculate(int x) {
    if (x < 0) {
        return -1;  // Exit method early
    }
    return x * 2;
}
```

## 3. Class and Object Keywords (10)

```java
// class - define class
class MyClass {
    // fields and methods
}

// interface - define interface
interface Printable {
    void print();
}

// enum - define enumeration
enum Color {
    RED, GREEN, BLUE
}

// extends - inheritance
class Dog extends Animal {
}

// implements - implement interface
class Printer implements Printable {
    public void print() { }
}

// new - create object
MyClass obj = new MyClass();

// this - reference current object
class Person {
    private String name;
    
    public Person(String name) {
        this.name = name;  // 'this' refers to instance variable
    }
}

// super - reference parent class
class Child extends Parent {
    public void method() {
        super.method();  // Call parent's method
    }
}

// instanceof - check object type
if (obj instanceof String) {
    System.out.println("It's a string");
}

// package - declare package
package com.example.myapp;

// import - import classes
import java.util.ArrayList;
```

## 4. Modifier Keywords (11)

```java
// Access modifiers
public class PublicClass { }
protected void protectedMethod() { }
private int privateField;
// default (no keyword)

// Non-access modifiers
static int staticVariable;
final int CONSTANT = 100;
abstract class AbstractClass { }
synchronized void syncMethod() { }
volatile boolean flag;
transient String tempData;
native void nativeMethod();
strictfp class StrictClass { }
```

## 5. Exception Handling Keywords (5)

```java
try {
    // Code that might throw exception
    int result = 10 / 0;
} catch (ArithmeticException e) {
    // Handle exception
    System.out.println("Cannot divide by zero");
} finally {
    // Always executes
    System.out.println("Cleanup code");
}

// throw - throw exception manually
public void checkAge(int age) {
    if (age < 18) {
        throw new IllegalArgumentException("Too young");
    }
}

// throws - declare exceptions
public void readFile() throws IOException {
    // Method might throw IOException
}
```

## 6. Other Important Keywords (5)

```java
// void - no return value
public void printMessage() {
    System.out.println("Hello");
}

// assert - debugging (must enable with -ea flag)
assert x > 0 : "x must be positive";

// const - reserved but not used (use final instead)
// const int MAX = 100;  // ❌ ERROR

// goto - reserved but not used
// goto label;  // ❌ ERROR
```

---

# Part 2: Detailed Keyword Explanations

## break and continue with Labels

```java
// break with label - exit outer loop
outerLoop:
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (i == 1 && j == 1) {
            break outerLoop;  // Break out of outer loop
        }
        System.out.println(i + "," + j);
    }
}

// continue with label - skip outer loop iteration
outerLoop:
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (i == 1 && j == 1) {
            continue outerLoop;  // Continue outer loop
        }
        System.out.println(i + "," + j);
    }
}
```

## switch with Modern Enhancements (Java 12+)

```java
// Traditional switch
String result;
switch (day) {
    case "Monday":
    case "Tuesday":
    case "Wednesday":
        result = "Weekday";
        break;
    case "Saturday":
    case "Sunday":
        result = "Weekend";
        break;
    default:
        result = "Unknown";
}

// Modern switch expression (Java 12+)
String result = switch (day) {
    case "Monday", "Tuesday", "Wednesday" -> "Weekday";
    case "Saturday", "Sunday" -> "Weekend";
    default -> "Unknown";
};

// Switch with yield (Java 13+)
String result = switch (day) {
    case "Monday", "Tuesday", "Wednesday" -> {
        System.out.println("Calculating...");
        yield "Weekday";
    }
    case "Saturday", "Sunday" -> "Weekend";
    default -> "Unknown";
};
```

## this vs super

```java
class Parent {
    protected int value = 10;
    
    public void display() {
        System.out.println("Parent display");
    }
    
    public Parent(String message) {
        System.out.println("Parent: " + message);
    }
}

class Child extends Parent {
    private int value = 20;
    
    public Child() {
        super("Initializing");  // Call parent constructor (must be first)
    }
    
    public void showValues() {
        System.out.println("This value: " + this.value);      // 20
        System.out.println("Super value: " + super.value);    // 10
    }
    
    public void display() {
        super.display();     // Call parent's method
        System.out.println("Child display");
    }
}
```

## instanceof and Type Checking

```java
public class InstanceOfExample {
    public static void main(String[] args) {
        Object obj = "Hello";
        
        // Check type
        if (obj instanceof String) {
            String str = (String) obj;
            System.out.println("Length: " + str.length());
        }
        
        // Pattern matching (Java 16+)
        if (obj instanceof String str) {
            // 'str' is automatically cast
            System.out.println("Length: " + str.length());
        }
        
        // With inheritance
        Animal animal = new Dog();
        if (animal instanceof Dog) {
            Dog dog = (Dog) animal;
            dog.bark();
        }
        
        // Null check
        Object nullObj = null;
        System.out.println(nullObj instanceof String);  // false
    }
}
```

## assert for Debugging

```java
public class AssertExample {
    public static void main(String[] args) {
        int age = -5;
        
        // Simple assertion
        assert age >= 0;  // Throws AssertionError if false
        
        // Assertion with message
        assert age >= 0 : "Age cannot be negative: " + age;
        
        // Assertions must be enabled with -ea flag
        // java -ea AssertExample
    }
    
    public static int divide(int a, int b) {
        assert b != 0 : "Divisor cannot be zero";
        return a / b;
    }
}
```

---

# Part 3: Comparison Operators

## Relational Operators

```java
int a = 10;
int b = 20;

// Equal to
System.out.println(a == b);  // false

// Not equal to
System.out.println(a != b);  // true

// Greater than
System.out.println(a > b);   // false

// Less than
System.out.println(a < b);   // true

// Greater than or equal to
System.out.println(a >= b);  // false

// Less than or equal to
System.out.println(a <= b);  // true
```

## Comparing Primitives vs Objects

```java
public class ComparisonExample {
    public static void main(String[] args) {
        // Primitives: use ==
        int x = 5;
        int y = 5;
        System.out.println(x == y);  // true - compares values
        
        // Objects: == compares references (memory addresses)
        String str1 = new String("Hello");
        String str2 = new String("Hello");
        System.out.println(str1 == str2);  // false - different objects
        
        // Objects: use .equals() to compare content
        System.out.println(str1.equals(str2));  // true - same content
        
        // String literal pool exception
        String s1 = "Hello";
        String s2 = "Hello";
        System.out.println(s1 == s2);  // true - same object in pool
    }
}
```

## equals() vs ==

```java
public class EqualsExample {
    public static void main(String[] args) {
        // == for primitives
        int a = 10;
        int b = 10;
        System.out.println(a == b);  // true
        
        // == for objects (reference comparison)
        Integer num1 = new Integer(10);
        Integer num2 = new Integer(10);
        System.out.println(num1 == num2);  // false - different objects
        System.out.println(num1.equals(num2));  // true - same value
        
        // String comparison
        String str1 = new String("Java");
        String str2 = new String("Java");
        System.out.println(str1 == str2);  // false
        System.out.println(str1.equals(str2));  // true
        
        // Array comparison
        int[] arr1 = {1, 2, 3};
        int[] arr2 = {1, 2, 3};
        System.out.println(arr1 == arr2);  // false
        System.out.println(arr1.equals(arr2));  // false - arrays don't override equals
        System.out.println(Arrays.equals(arr1, arr2));  // true - use Arrays.equals()
    }
}
```

## Implementing equals() Correctly

```java
public class Person {
    private String name;
    private int age;
    
    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }
    
    @Override
    public boolean equals(Object obj) {
        // 1. Check if same reference
        if (this == obj) {
            return true;
        }
        
        // 2. Check if null or different class
        if (obj == null || getClass() != obj.getClass()) {
            return false;
        }
        
        // 3. Cast and compare fields
        Person person = (Person) obj;
        return age == person.age && 
               Objects.equals(name, person.name);
    }
    
    @Override
    public int hashCode() {
        // Must override hashCode() when overriding equals()
        return Objects.hash(name, age);
    }
}

// Usage
Person p1 = new Person("Alice", 25);
Person p2 = new Person("Alice", 25);
System.out.println(p1 == p2);      // false - different objects
System.out.println(p1.equals(p2)); // true - same content
```

## compareTo() for Ordering

```java
public class Student implements Comparable<Student> {
    private String name;
    private int grade;
    
    public Student(String name, int grade) {
        this.name = name;
        this.grade = grade;
    }
    
    @Override
    public int compareTo(Student other) {
        // Return negative if this < other
        // Return 0 if this == other
        // Return positive if this > other
        return this.grade - other.grade;
    }
    
    // Or compare by name
    public int compareToByName(Student other) {
        return this.name.compareTo(other.name);
    }
}

// Usage
Student s1 = new Student("Alice", 85);
Student s2 = new Student("Bob", 92);

System.out.println(s1.compareTo(s2));  // negative (85 < 92)
System.out.println(s2.compareTo(s1));  // positive (92 > 85)

// Sorting
List<Student> students = new ArrayList<>();
students.add(s1);
students.add(s2);
Collections.sort(students);  // Sorts by grade
```

## Comparator for Custom Sorting

```java
import java.util.*;

public class ComparatorExample {
    public static void main(String[] args) {
        List<Person> people = new ArrayList<>();
        people.add(new Person("Alice", 25));
        people.add(new Person("Bob", 30));
        people.add(new Person("Charlie", 20));
        
        // Sort by age
        Collections.sort(people, new Comparator<Person>() {
            public int compare(Person p1, Person p2) {
                return p1.getAge() - p2.getAge();
            }
        });
        
        // Lambda version
        Collections.sort(people, (p1, p2) -> p1.getAge() - p2.getAge());
        
        // Method reference
        Collections.sort(people, Comparator.comparing(Person::getAge));
        
        // Reverse order
        Collections.sort(people, Comparator.comparing(Person::getAge).reversed());
        
        // Multiple criteria
        Collections.sort(people, 
            Comparator.comparing(Person::getAge)
                      .thenComparing(Person::getName));
    }
}
```

---

# Part 4: Logical Operators

## AND, OR, NOT

```java
boolean a = true;
boolean b = false;

// AND (&&) - both must be true
System.out.println(a && b);  // false

// OR (||) - at least one must be true
System.out.println(a || b);  // true

// NOT (!) - inverts boolean
System.out.println(!a);      // false
System.out.println(!b);      // true

// Compound conditions
int age = 25;
boolean hasLicense = true;

if (age >= 18 && hasLicense) {
    System.out.println("Can drive");
}

// XOR (^) - exactly one must be true
System.out.println(a ^ b);   // true
System.out.println(a ^ a);   // false
```

## Short-Circuit Evaluation

```java
public class ShortCircuit {
    public static void main(String[] args) {
        int x = 5;
        int y = 0;
        
        // && short-circuits: second condition not evaluated if first is false
        if (y != 0 && x / y > 2) {  // Safe! y != 0 is false, so x / y never runs
            System.out.println("Result: " + (x / y));
        }
        
        // || short-circuits: second condition not evaluated if first is true
        if (x > 0 || y > 0) {  // x > 0 is true, so y > 0 never checked
            System.out.println("At least one is positive");
        }
        
        // Non-short-circuit: & and | always evaluate both sides
        // if (y != 0 & x / y > 2) {  // ❌ ArithmeticException! Both sides evaluated
        //     System.out.println("Result");
        // }
    }
}
```

---

# Part 5: Ternary Operator

## Basic Ternary Operator

```java
// Syntax: condition ? valueIfTrue : valueIfFalse

int age = 20;
String status = (age >= 18) ? "Adult" : "Minor";
System.out.println(status);  // "Adult"

// Without ternary (longer)
String status2;
if (age >= 18) {
    status2 = "Adult";
} else {
    status2 = "Minor";
}

// Finding max
int a = 10;
int b = 20;
int max = (a > b) ? a : b;
System.out.println("Max: " + max);  // 20

// Nested ternary (use sparingly!)
int score = 85;
String grade = (score >= 90) ? "A" : 
               (score >= 80) ? "B" : 
               (score >= 70) ? "C" : "F";
System.out.println("Grade: " + grade);  // "B"
```

## Ternary in Method Calls

```java
public class TernaryExamples {
    public static void main(String[] args) {
        int number = -5;
        
        // In method arguments
        System.out.println(number >= 0 ? "Positive" : "Negative");
        
        // In return statements
        String result = getStatus(15);
        System.out.println(result);
        
        // In assignments with methods
        int absValue = (number >= 0) ? number : -number;
    }
    
    public static String getStatus(int age) {
        return (age >= 18) ? "Eligible" : "Not eligible";
    }
}
```

---

# Part 6: null Checks and Comparisons

## Checking for null

```java
public class NullChecks {
    public static void main(String[] args) {
        String str = null;
        
        // Check for null before using
        if (str != null) {
            System.out.println(str.length());
        }
        
        // Using ternary
        int length = (str != null) ? str.length() : 0;
        
        // Objects.isNull() and Objects.nonNull()
        if (Objects.isNull(str)) {
            System.out.println("String is null");
        }
        
        if (Objects.nonNull(str)) {
            System.out.println("String is not null");
        }
        
        // Objects.requireNonNull() - throws exception if null
        try {
            String result = Objects.requireNonNull(str, "String cannot be null");
        } catch (NullPointerException e) {
            System.out.println(e.getMessage());
        }
    }
}
```

## Optional for null Safety (Java 8+)

```java
import java.util.Optional;

public class OptionalExample {
    public static void main(String[] args) {
        // Create Optional
        Optional<String> optional1 = Optional.of("Hello");
        Optional<String> optional2 = Optional.empty();
        Optional<String> optional3 = Optional.ofNullable(null);  // Safe
        
        // Check if present
        if (optional1.isPresent()) {
            System.out.println(optional1.get());
        }
        
        // ifPresent with lambda
        optional1.ifPresent(value -> System.out.println(value));
        
        // orElse - provide default value
        String result1 = optional2.orElse("Default");
        System.out.println(result1);  // "Default"
        
        // orElseGet - provide supplier
        String result2 = optional2.orElseGet(() -> "Generated Default");
        
        // orElseThrow - throw exception if empty
        try {
            String result3 = optional2.orElseThrow(() -> 
                new IllegalStateException("Value not present"));
        } catch (IllegalStateException e) {
            System.out.println(e.getMessage());
        }
        
        // map - transform value
        Optional<Integer> length = optional1.map(String::length);
        System.out.println(length.get());  // 5
        
        // filter
        Optional<String> filtered = optional1.filter(s -> s.length() > 3);
    }
}
```

---

# Part 7: Special Comparisons

## Comparing Arrays

```java
import java.util.Arrays;

public class ArrayComparison {
    public static void main(String[] args) {
        int[] arr1 = {1, 2, 3};
        int[] arr2 = {1, 2, 3};
        int[] arr3 = {1, 2, 4};
        
        // == compares references
        System.out.println(arr1 == arr2);  // false
        
        // equals() doesn't work for arrays
        System.out.println(arr1.equals(arr2));  // false
        
        // Use Arrays.equals() for content comparison
        System.out.println(Arrays.equals(arr1, arr2));  // true
        System.out.println(Arrays.equals(arr1, arr3));  // false
        
        // For multi-dimensional arrays
        int[][] matrix1 = {{1, 2}, {3, 4}};
        int[][] matrix2 = {{1, 2}, {3, 4}};
        System.out.println(Arrays.deepEquals(matrix1, matrix2));  // true
    }
}
```

## Comparing Collections

```java
import java.util.*;

public class CollectionComparison {
    public static void main(String[] args) {
        List<String> list1 = Arrays.asList("A", "B", "C");
        List<String> list2 = Arrays.asList("A", "B", "C");
        List<String> list3 = new ArrayList<>(list1);
        
        // equals() works for collections
        System.out.println(list1.equals(list2));  // true
        System.out.println(list1.equals(list3));  // true
        
        // == compares references
        System.out.println(list1 == list2);  // false
        System.out.println(list1 == list3);  // false
        
        // Sets
        Set<Integer> set1 = new HashSet<>(Arrays.asList(1, 2, 3));
        Set<Integer> set2 = new HashSet<>(Arrays.asList(3, 2, 1));  // Different order
        System.out.println(set1.equals(set2));  // true - order doesn't matter
        
        // Maps
        Map<String, Integer> map1 = new HashMap<>();
        map1.put("A", 1);
        map1.put("B", 2);
        
        Map<String, Integer> map2 = new HashMap<>();
        map2.put("B", 2);
        map2.put("A", 1);
        
        System.out.println(map1.equals(map2));  // true
    }
}
```

## Comparing Strings

```java
public class StringComparison {
    public static void main(String[] args) {
        String s1 = "Hello";
        String s2 = "Hello";
        String s3 = new String("Hello");
        String s4 = "HELLO";
        
        // == compares references
        System.out.println(s1 == s2);  // true (string pool)
        System.out.println(s1 == s3);  // false (different objects)
        
        // equals() compares content
        System.out.println(s1.equals(s3));  // true
        
        // equalsIgnoreCase() - ignore case
        System.out.println(s1.equalsIgnoreCase(s4));  // true
        
        // compareTo() - lexicographic order
        System.out.println(s1.compareTo(s2));  // 0 (equal)
        System.out.println("Apple".compareTo("Banana"));  // negative
        System.out.println("Zebra".compareTo("Apple"));   // positive
        
        // compareToIgnoreCase()
        System.out.println(s1.compareToIgnoreCase(s4));  // 0
        
        // contains()
        System.out.println(s1.contains("ell"));  // true
        
        // startsWith() and endsWith()
        System.out.println(s1.startsWith("He"));  // true
        System.out.println(s1.endsWith("lo"));    // true
    }
}
```

---

# Part 8: Comparison Best Practices

## Rule Summary

```java
public class ComparisonRules {
    public static void main(String[] args) {
        // ✅ Primitives: use ==
        int a = 5;
        int b = 5;
        if (a == b) { }
        
        // ✅ Objects: use .equals()
        String s1 = "Hello";
        String s2 = "Hello";
        if (s1.equals(s2)) { }
        
        // ✅ null-safe comparison
        if (Objects.equals(s1, s2)) { }  // Handles null
        
        // ✅ Arrays: use Arrays.equals()
        int[] arr1 = {1, 2, 3};
        int[] arr2 = {1, 2, 3};
        if (Arrays.equals(arr1, arr2)) { }
        
        // ✅ Ordering: implement Comparable or use Comparator
        List<String> list = Arrays.asList("C", "A", "B");
        Collections.sort(list);  // Natural ordering
        Collections.sort(list, Comparator.reverseOrder());
        
        // ✅ Check null before calling methods
        if (s1 != null && s1.equals("Hello")) { }
        
        // ❌ Common mistakes
        // if (s1 == s2) { }  // Wrong for objects!
        // if (arr1.equals(arr2)) { }  // Doesn't work for arrays!
        // s1.equals(null);  // Returns false, but might throw NPE elsewhere
    }
}
```

## Defensive Comparison

```java
public class DefensiveComparison {
    // ✅ Good: constant first prevents NullPointerException
    public boolean isActive(String status) {
        return "ACTIVE".equals(status);  // Safe even if status is null
    }
    
    // ❌ Bad: might throw NullPointerException
    public boolean isActiveBad(String status) {
        return status.equals("ACTIVE");  // NPE if status is null
    }
    
    // ✅ Best: use Objects.equals()
    public boolean areEqual(String s1, String s2) {
        return Objects.equals(s1, s2);  // Handles null for both
    }
}
```

---

# Summary

**All 51 Java Keywords:**

**Data Types (8):** byte, short, int, long, float, double, char, boolean

**Control Flow (12):** if, else, switch, case, default, for, while, do, break, continue, return

**Class/Object (10):** class, interface, enum, extends, implements, new, this, super, instanceof, package, import

**Modifiers (11):** public, protected, private, static, final, abstract, synchronized, volatile, transient, native, strictfp

**Exceptions (5):** try, catch, finally, throw, throws

**Other (5):** void, assert, const*, goto*

*Reserved but not used

**Comparison Rules:**
- Primitives: `==`
- Objects: `.equals()`
- Arrays: `Arrays.equals()`
- Null-safe: `Objects.equals()`
- Ordering: `compareTo()` or `Comparator`
- Strings: `.equals()` or `.equalsIgnoreCase()`

**Best Practices:**
- Always check for null before calling methods
- Override `equals()` and `hashCode()` together
- Use `Objects.equals()` for null safety
- Implement `Comparable` for natural ordering
- Use `Comparator` for custom sorting
- Prefer constants first in comparisons: `"ACTIVE".equals(status)`