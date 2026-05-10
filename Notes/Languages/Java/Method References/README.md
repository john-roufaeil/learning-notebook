# Method References - Even Cleaner Than Lambdas

## What Are Method References?

**Method references** are shorthand notation of lambda expressions to call a method. They make code even more concise and readable.

### Lambda vs Method Reference

```java
// Lambda expression
list.forEach(item -> System.out.println(item));

// Method reference - Same thing!
list.forEach(System.out::println);
```

**The `::` operator** is used to create method references.

## Why Use Method References?

1. **More concise** - Less code to write
2. **More readable** - Clear intent
3. **Better performance** - JVM can optimize better
4. **Cleaner** - No need for parameter declarations

## Four Types of Method References

1. **Static Method Reference**: `ClassName::staticMethod`
2. **Instance Method Reference (Specific Object)**: `object::instanceMethod`
3. **Instance Method Reference (Arbitrary Object)**: `ClassName::instanceMethod`
4. **Constructor Reference**: `ClassName::new`

---

## 1. Static Method Reference

Reference to a static method: `ClassName::staticMethodName`

### Basic Example

```java
// Lambda
Function<String, Integer> parser1 = s -> Integer.parseInt(s);

// Method reference - cleaner!
Function<String, Integer> parser2 = Integer::parseInt;

System.out.println(parser2.apply("123"));  // 123
```

### More Examples

```java
class MathOperations {
    public static int square(int x) {
        return x * x;
    }
    
    public static boolean isEven(int x) {
        return x % 2 == 0;
    }
    
    public static int max(int a, int b) {
        return a > b ? a : b;
    }
}

// Using static method references
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);

// Lambda
List<Integer> squares1 = numbers.stream()
    .map(n -> MathOperations.square(n))
    .collect(Collectors.toList());

// Method reference - better!
List<Integer> squares2 = numbers.stream()
    .map(MathOperations::square)
    .collect(Collectors.toList());

// Filtering with static method
List<Integer> evens = numbers.stream()
    .filter(MathOperations::isEven)
    .collect(Collectors.toList());

// Reducing with static method
int maximum = numbers.stream()
    .reduce(MathOperations::max)
    .orElse(0);
```

### Real-World Example: Utility Methods

```java
class StringUtils {
    public static boolean isNotEmpty(String s) {
        return s != null && !s.isEmpty();
    }
    
    public static String capitalize(String s) {
        if (s == null || s.isEmpty()) return s;
        return s.substring(0, 1).toUpperCase() + s.substring(1);
    }
}

List<String> names = Arrays.asList("alice", "", "bob", null, "charlie");

// Filter and transform
List<String> processed = names.stream()
    .filter(StringUtils::isNotEmpty)       // Static method reference
    .map(StringUtils::capitalize)          // Static method reference
    .collect(Collectors.toList());

System.out.println(processed);  // [Alice, Bob, Charlie]
```

---

## 2. Instance Method Reference (Specific Object)

Reference to an instance method of a particular object: `object::instanceMethodName`

### Basic Example

```java
class Printer {
    private String prefix;
    
    public Printer(String prefix) {
        this.prefix = prefix;
    }
    
    public void print(String message) {
        System.out.println(prefix + message);
    }
}

Printer printer = new Printer("[LOG] ");

List<String> messages = Arrays.asList("Starting", "Processing", "Done");

// Lambda
messages.forEach(msg -> printer.print(msg));

// Method reference - cleaner!
messages.forEach(printer::print);
```

### More Examples

```java
class NumberFormatter {
    private String format;
    
    public NumberFormatter(String format) {
        this.format = format;
    }
    
    public String formatNumber(double number) {
        return String.format(format, number);
    }
}

NumberFormatter currencyFormatter = new NumberFormatter("$%.2f");
NumberFormatter percentFormatter = new NumberFormatter("%.1f%%");

List<Double> values = Arrays.asList(10.5, 20.75, 30.125);

// Using instance method reference
List<String> currencies = values.stream()
    .map(currencyFormatter::formatNumber)
    .collect(Collectors.toList());

List<String> percents = values.stream()
    .map(percentFormatter::formatNumber)
    .collect(Collectors.toList());

System.out.println(currencies);  // [$10.50, $20.75, $30.13]
System.out.println(percents);    // [10.5%, 20.8%, 30.1%]
```

### Real-World Example: Validation

```java
class Validator {
    private int minLength;
    private int maxLength;
    
    public Validator(int minLength, int maxLength) {
        this.minLength = minLength;
        this.maxLength = maxLength;
    }
    
    public boolean isValid(String input) {
        return input != null 
            && input.length() >= minLength 
            && input.length() <= maxLength;
    }
}

Validator usernameValidator = new Validator(3, 20);
Validator passwordValidator = new Validator(8, 50);

List<String> usernames = Arrays.asList("ab", "alice", "bob", "verylongusername123456");
List<String> passwords = Arrays.asList("short", "validPassword123", "ok");

// Filter using instance method references
List<String> validUsernames = usernames.stream()
    .filter(usernameValidator::isValid)
    .collect(Collectors.toList());

List<String> validPasswords = passwords.stream()
    .filter(passwordValidator::isValid)
    .collect(Collectors.toList());
```

---

## 3. Instance Method Reference (Arbitrary Object of a Type)

Reference to an instance method where the instance is provided: `ClassName::instanceMethodName`

**Key difference**: The object is supplied by the lambda parameter.

### Basic Example

```java
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");

// Lambda - parameter becomes the object
names.sort((s1, s2) -> s1.compareToIgnoreCase(s2));

// Method reference - first parameter becomes the object!
names.sort(String::compareToIgnoreCase);
```

### How It Works

```java
// Lambda form
(str1, str2) -> str1.compareToIgnoreCase(str2)

// Method reference form
String::compareToIgnoreCase

// What it means:
// Take first parameter (str1) as the object
// Call its method compareToIgnoreCase with second parameter (str2)
```

### More Examples

```java
List<String> words = Arrays.asList("apple", "BANANA", "Cherry");

// Lambda
words.stream()
    .map(s -> s.toUpperCase())
    .forEach(System.out::println);

// Method reference
words.stream()
    .map(String::toUpperCase)  // Each string calls toUpperCase on itself
    .forEach(System.out::println);

// Checking if strings are empty
List<Boolean> emptiness = words.stream()
    .map(String::isEmpty)  // Each string calls isEmpty on itself
    .collect(Collectors.toList());
```

### Practical Example: Person Comparison

```java
class Person {
    private String name;
    private int age;
    
    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }
    
    public String getName() { return name; }
    public int getAge() { return age; }
    
    public boolean isOlderThan(Person other) {
        return this.age > other.age;
    }
}

List<Person> people = Arrays.asList(
    new Person("Alice", 30),
    new Person("Bob", 25),
    new Person("Charlie", 35)
);

// Lambda - explicit parameters
people.sort((p1, p2) -> p1.getName().compareTo(p2.getName()));

// Method reference - cleaner!
people.sort((p1, p2) -> p1.getName().compareTo(p2.getName()));
// Can be written as:
Comparator<Person> byName = Comparator.comparing(Person::getName);
people.sort(byName);

// Or even simpler with chaining
people.sort(Comparator.comparing(Person::getName)
                      .thenComparing(Person::getAge));
```

### String Operations

```java
List<String> strings = Arrays.asList("  hello  ", "  world  ", "  java  ");

// Trim all strings
List<String> trimmed = strings.stream()
    .map(String::trim)  // Each string trims itself
    .collect(Collectors.toList());

// Get length of each string
List<Integer> lengths = strings.stream()
    .map(String::length)  // Each string returns its length
    .collect(Collectors.toList());

// Convert to char array
List<char[]> charArrays = strings.stream()
    .map(String::toCharArray)  // Each string converts itself
    .collect(Collectors.toList());
```

---

## 4. Constructor Reference

Reference to a constructor: `ClassName::new`

### Basic Example

```java
// Lambda
Supplier<List<String>> listSupplier1 = () -> new ArrayList<>();

// Constructor reference
Supplier<List<String>> listSupplier2 = ArrayList::new;

List<String> list = listSupplier2.get();
```

### Creating Objects from Data

```java
class Person {
    private String name;
    private int age;
    
    public Person(String name) {
        this.name = name;
        this.age = 0;
    }
    
    public Person(String name, int age) {
        this.name = name;
        this.age = age;
    }
    
    @Override
    public String toString() {
        return name + " (" + age + ")";
    }
}

// Using constructor reference with one parameter
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");

// Lambda
List<Person> people1 = names.stream()
    .map(name -> new Person(name))
    .collect(Collectors.toList());

// Constructor reference - cleaner!
List<Person> people2 = names.stream()
    .map(Person::new)  // Calls Person(String name)
    .collect(Collectors.toList());
```

### Array Constructor Reference

```java
// Lambda
Function<Integer, String[]> arrayCreator1 = size -> new String[size];

// Array constructor reference
Function<Integer, String[]> arrayCreator2 = String[]::new;

String[] array = arrayCreator2.apply(10);  // Creates array of size 10

// Useful with streams
List<String> list = Arrays.asList("A", "B", "C");
String[] array = list.stream().toArray(String[]::new);
```

### Factory Methods

```java
class Product {
    private String name;
    private double price;
    
    public Product(String name, double price) {
        this.name = name;
        this.price = price;
    }
    
    public static Product fromString(String data) {
        String[] parts = data.split(",");
        return new Product(parts[0], Double.parseDouble(parts[1]));
    }
}

List<String> productData = Arrays.asList(
    "Laptop,1000.00",
    "Mouse,25.50",
    "Keyboard,75.00"
);

// Using static factory method reference
List<Product> products = productData.stream()
    .map(Product::fromString)
    .collect(Collectors.toList());
```

---

## Method Reference Equivalents

### Common Patterns

```java
// Pattern: Single parameter passed directly to method
x -> System.out.println(x)  →  System.out::println

// Pattern: Single parameter as method call target
x -> x.isEmpty()            →  String::isEmpty
x -> x.length()             →  String::length

// Pattern: Two parameters, first is target
(x, y) -> x.compareTo(y)    →  String::compareTo

// Pattern: No parameters, create new object
() -> new ArrayList<>()     →  ArrayList::new

// Pattern: Static method call
x -> Integer.parseInt(x)    →  Integer::parseInt
x -> Math.abs(x)            →  Math::abs
```

## When You CANNOT Use Method References

### 1. When Lambda Has Additional Logic

```java
// Cannot use method reference - has additional logic
list.stream()
    .map(x -> {
        System.out.println("Processing: " + x);
        return x.toUpperCase();
    })
    .collect(Collectors.toList());
```

### 2. When Parameters Are Modified

```java
// Cannot use method reference - parameter is modified
list.stream()
    .map(x -> x.trim().toUpperCase())
    .collect(Collectors.toList());
```

### 3. When Using Different Parameter Order

```java
// Cannot use method reference - parameters swapped
(x, y) -> y.compareTo(x)  // Not possible as method reference
```

## Real-World Examples

### Example 1: Data Processing Pipeline

```java
class DataProcessor {
    public static String clean(String data) {
        return data.trim().toLowerCase();
    }
    
    public static boolean isValid(String data) {
        return !data.isEmpty() && data.length() > 2;
    }
    
    public static String transform(String data) {
        return data.substring(0, 1).toUpperCase() + data.substring(1);
    }
}

List<String> rawData = Arrays.asList("  ALICE  ", "  bob  ", " ", "CHARLIE");

List<String> processed = rawData.stream()
    .map(DataProcessor::clean)        // Static method reference
    .filter(DataProcessor::isValid)   // Static method reference
    .map(DataProcessor::transform)    // Static method reference
    .collect(Collectors.toList());

System.out.println(processed);  // [Alice, Bob, Charlie]
```

### Example 2: Event Handler System

```java
class Button {
    private String label;
    private Runnable onClick;
    
    public Button(String label) {
        this.label = label;
    }
    
    public void setOnClick(Runnable action) {
        this.onClick = action;
    }
    
    public void click() {
        if (onClick != null) onClick.run();
    }
}

class Application {
    public void handleSubmit() {
        System.out.println("Submitting form...");
    }
    
    public void handleCancel() {
        System.out.println("Cancelling...");
    }
    
    public void handleReset() {
        System.out.println("Resetting form...");
    }
    
    public void setupUI() {
        Button submitBtn = new Button("Submit");
        Button cancelBtn = new Button("Cancel");
        Button resetBtn = new Button("Reset");
        
        // Method references to instance methods
        submitBtn.setOnClick(this::handleSubmit);
        cancelBtn.setOnClick(this::handleCancel);
        resetBtn.setOnClick(this::handleReset);
        
        submitBtn.click();  // Submitting form...
        cancelBtn.click();  // Cancelling...
        resetBtn.click();   // Resetting form...
    }
}
```

### Example 3: Repository Pattern

```java
class User {
    private int id;
    private String name;
    
    public User(int id, String name) {
        this.id = id;
        this.name = name;
    }
    
    public int getId() { return id; }
    public String getName() { return name; }
}

class UserRepository {
    private List<User> users = new ArrayList<>();
    
    public void save(User user) {
        users.add(user);
    }
    
    public void saveAll(List<User> usersToSave) {
        // Method reference for forEach
        usersToSave.forEach(this::save);
    }
    
    public List<User> findAll() {
        return new ArrayList<>(users);
    }
}

// Usage
UserRepository repo = new UserRepository();
List<User> newUsers = Arrays.asList(
    new User(1, "Alice"),
    new User(2, "Bob"),
    new User(3, "Charlie")
);

// Using method reference
repo.saveAll(newUsers);
```

## Best Practices

### 1. Prefer Method References When Possible

```java
// Less clear
names.forEach(n -> System.out.println(n));

// More clear
names.forEach(System.out::println);
```

### 2. Don't Force Method References

```java
// Forced and unclear
numbers.stream()
    .map(Optional::of)
    .map(Optional::get)  // Unnecessary complexity

// Clear and simple
numbers.stream()
    .filter(n -> n > 0)  // Lambda is clearer here
```

### 3. Extract Methods for Readability

```java
// BAD - Complex lambda
list.stream()
    .filter(item -> item.getPrice() > 100 && 
                    item.getStock() > 0 && 
                    item.getCategory().equals("Electronics"))
    .collect(Collectors.toList());

// GOOD - Extract to method, use method reference
list.stream()
    .filter(this::isAvailableElectronics)
    .collect(Collectors.toList());

private boolean isAvailableElectronics(Item item) {
    return item.getPrice() > 100 
        && item.getStock() > 0 
        && item.getCategory().equals("Electronics");
}
```

## Summary

**Method References** - Shorthand for lambdas:

| Type | Syntax | Example |
|------|--------|---------|
| Static method | `Class::staticMethod` | `Integer::parseInt` |
| Instance method (specific) | `object::instanceMethod` | `printer::print` |
| Instance method (arbitrary) | `Class::instanceMethod` | `String::toUpperCase` |
| Constructor | `Class::new` | `ArrayList::new` |

**When to use**:
- ✅ When lambda just calls a single method
- ✅ When method reference makes code clearer
- ✅ When no additional logic needed

**When NOT to use**:
- ❌ When lambda has multiple statements
- ❌ When parameters need modification
- ❌ When method reference reduces clarity

---

**Next Lesson**: We'll explore all Java Modifiers - access modifiers, non-access modifiers, and where they can be applied!