# Lambda Functions - Functional Programming in Java

## What Are Lambda Functions?

**Lambda expressions** (introduced in Java 8) are anonymous functions - functions without a name. They provide a clear and concise way to implement single-method interfaces (functional interfaces).

### Before Lambda vs After Lambda

```java
// BEFORE Lambda (Java 7) - Anonymous Inner Class
Runnable runnable = new Runnable() {
    @Override
    public void run() {
        System.out.println("Hello World");
    }
};

// AFTER Lambda (Java 8+) - Clean and Concise!
Runnable runnableLambda = () -> System.out.println("Hello World");
```

**The difference**: 5 lines reduced to 1 line!

## Lambda Syntax

### Basic Syntax

```
(parameters) -> expression
```

or

```
(parameters) -> { statements; }
```

### Examples of Different Syntaxes

```java
// No parameters
() -> System.out.println("Hello")

// One parameter (parentheses optional)
x -> x * x
(x) -> x * x  // Same as above

// Multiple parameters
(x, y) -> x + y

// Multiple statements (need curly braces)
(x, y) -> {
    int sum = x + y;
    return sum;
}

// With type declarations
(int x, int y) -> x + y

// Return statement (explicit)
(x, y) -> { return x + y; }
```

## Functional Interfaces

Lambdas can only be used with **functional interfaces** - interfaces with exactly one abstract method.

### @FunctionalInterface Annotation

```java
@FunctionalInterface
interface Calculator {
    int calculate(int a, int b);
    
    // Can have default methods
    default void printResult(int result) {
        System.out.println("Result: " + result);
    }
    
    // Can have static methods
    static void info() {
        System.out.println("Calculator interface");
    }
}

// Using lambda with this interface
Calculator add = (a, b) -> a + b;
Calculator multiply = (a, b) -> a * b;

System.out.println(add.calculate(5, 3));       // 8
System.out.println(multiply.calculate(5, 3));  // 15
```

## Built-in Functional Interfaces

Java provides many built-in functional interfaces in `java.util.function` package.

### 1. Predicate<T> - Test a Condition

Takes one argument, returns boolean.

```java
import java.util.function.Predicate;

// Lambda syntax
Predicate<Integer> isEven = num -> num % 2 == 0;
Predicate<String> isLongString = str -> str.length() > 5;

System.out.println(isEven.test(4));          // true
System.out.println(isEven.test(7));          // false
System.out.println(isLongString.test("Hi")); // false

// Chaining predicates
Predicate<Integer> isPositive = num -> num > 0;
Predicate<Integer> isEvenAndPositive = isEven.and(isPositive);

System.out.println(isEvenAndPositive.test(4));  // true
System.out.println(isEvenAndPositive.test(-4)); // false
```

### 2. Function<T, R> - Transform Input to Output

Takes one argument, returns a result.

```java
import java.util.function.Function;

// Lambda syntax
Function<String, Integer> stringLength = str -> str.length();
Function<Integer, Integer> square = num -> num * num;

System.out.println(stringLength.apply("Hello"));  // 5
System.out.println(square.apply(5));              // 25

// Chaining functions
Function<String, String> toUpperCase = str -> str.toUpperCase();
Function<String, Integer> lengthOfUpper = toUpperCase.andThen(stringLength);

System.out.println(lengthOfUpper.apply("hello")); // 5 (length of "HELLO")

// Composition
Function<Integer, Integer> addTwo = x -> x + 2;
Function<Integer, Integer> multiplyByThree = x -> x * 3;
Function<Integer, Integer> combined = addTwo.compose(multiplyByThree);

System.out.println(combined.apply(5)); // 17 (5*3=15, 15+2=17)
```

### 3. Consumer<T> - Consume Input, Return Nothing

Takes one argument, returns void.

```java
import java.util.function.Consumer;

// Lambda syntax
Consumer<String> print = str -> System.out.println(str);
Consumer<Integer> printSquare = num -> System.out.println(num * num);

print.accept("Hello World");  // Hello World
printSquare.accept(5);        // 25

// Chaining consumers
Consumer<String> printUpperCase = str -> System.out.println(str.toUpperCase());
Consumer<String> printBoth = print.andThen(printUpperCase);

printBoth.accept("hello");
// Output:
// hello
// HELLO
```

### 4. Supplier<T> - Supply a Value

Takes no arguments, returns a value.

```java
import java.util.function.Supplier;

// Lambda syntax
Supplier<Double> randomNumber = () -> Math.random();
Supplier<String> greeting = () -> "Hello World";

System.out.println(randomNumber.get());  // Random number
System.out.println(greeting.get());      // Hello World

// Practical example: Lazy initialization
class ExpensiveObject {
    public ExpensiveObject() {
        System.out.println("Creating expensive object...");
    }
}

Supplier<ExpensiveObject> supplier = () -> new ExpensiveObject();
// Object not created yet!

ExpensiveObject obj = supplier.get();  // Created only when needed
```

### 5. BiFunction<T, U, R> - Two Arguments Function

Takes two arguments, returns a result.

```java
import java.util.function.BiFunction;

// Lambda syntax
BiFunction<Integer, Integer, Integer> add = (a, b) -> a + b;
BiFunction<String, String, String> concat = (s1, s2) -> s1 + s2;
BiFunction<Integer, Integer, Double> divide = (a, b) -> (double) a / b;

System.out.println(add.apply(5, 3));              // 8
System.out.println(concat.apply("Hello", "World")); // HelloWorld
System.out.println(divide.apply(10, 3));          // 3.333...
```

### 6. UnaryOperator<T> and BinaryOperator<T>

Special cases of Function where input and output types are the same.

```java
import java.util.function.UnaryOperator;
import java.util.function.BinaryOperator;

// UnaryOperator<T> is Function<T, T>
UnaryOperator<Integer> square = x -> x * x;
UnaryOperator<String> toUpper = str -> str.toUpperCase();

System.out.println(square.apply(5));        // 25
System.out.println(toUpper.apply("hello")); // HELLO

// BinaryOperator<T> is BiFunction<T, T, T>
BinaryOperator<Integer> multiply = (a, b) -> a * b;
BinaryOperator<String> concat = (s1, s2) -> s1 + " " + s2;

System.out.println(multiply.apply(5, 3));           // 15
System.out.println(concat.apply("Hello", "World")); // Hello World
```

## Lambda with Collections

### forEach

```java
List<String> names = Arrays.asList("Alice", "Bob", "Charlie");

// Before Lambda
for (String name : names) {
    System.out.println(name);
}

// With Lambda
names.forEach(name -> System.out.println(name));

// Even shorter with method reference (next lesson!)
names.forEach(System.out::println);
```

### Filtering with removeIf

```java
List<Integer> numbers = new ArrayList<>(Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8));

// Remove all even numbers
numbers.removeIf(num -> num % 2 == 0);

System.out.println(numbers);  // [1, 3, 5, 7]
```

### Sorting with Comparator

```java
List<String> names = Arrays.asList("Charlie", "Alice", "Bob");

// Before Lambda
Collections.sort(names, new Comparator<String>() {
    @Override
    public int compare(String s1, String s2) {
        return s1.compareTo(s2);
    }
});

// With Lambda
names.sort((s1, s2) -> s1.compareTo(s2));

// Even shorter
names.sort(String::compareTo);

System.out.println(names);  // [Alice, Bob, Charlie]

// Custom sorting
List<Person> people = Arrays.asList(
    new Person("Alice", 30),
    new Person("Bob", 25),
    new Person("Charlie", 35)
);

// Sort by age
people.sort((p1, p2) -> Integer.compare(p1.getAge(), p2.getAge()));

// Sort by name length
people.sort((p1, p2) -> Integer.compare(
    p1.getName().length(), 
    p2.getName().length()
));
```

### Map Operations

```java
Map<String, Integer> map = new HashMap<>();
map.put("Alice", 30);
map.put("Bob", 25);
map.put("Charlie", 35);

// forEach with lambda
map.forEach((name, age) -> 
    System.out.println(name + " is " + age + " years old")
);

// computeIfAbsent
map.computeIfAbsent("David", name -> name.length() * 10);

// replaceAll
map.replaceAll((name, age) -> age + 1);  // Increment all ages
```

## Stream API with Lambdas

Streams provide a powerful way to process collections using lambdas.

### Basic Stream Operations

```java
List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

// Filter even numbers
List<Integer> evens = numbers.stream()
    .filter(num -> num % 2 == 0)
    .collect(Collectors.toList());
System.out.println(evens);  // [2, 4, 6, 8, 10]

// Map - transform each element
List<Integer> squares = numbers.stream()
    .map(num -> num * num)
    .collect(Collectors.toList());
System.out.println(squares);  // [1, 4, 9, 16, 25, ...]

// Reduce - combine all elements
int sum = numbers.stream()
    .reduce(0, (a, b) -> a + b);
System.out.println(sum);  // 55

// Count
long count = numbers.stream()
    .filter(num -> num > 5)
    .count();
System.out.println(count);  // 5
```

### Complex Stream Operations

```java
class Product {
    String name;
    double price;
    String category;
    
    public Product(String name, double price, String category) {
        this.name = name;
        this.price = price;
        this.category = category;
    }
    
    // Getters
    public String getName() { return name; }
    public double getPrice() { return price; }
    public String getCategory() { return category; }
}

List<Product> products = Arrays.asList(
    new Product("Laptop", 1000, "Electronics"),
    new Product("Phone", 500, "Electronics"),
    new Product("Desk", 300, "Furniture"),
    new Product("Chair", 150, "Furniture"),
    new Product("Monitor", 200, "Electronics")
);

// Find all electronics under $600
List<Product> cheapElectronics = products.stream()
    .filter(p -> p.getCategory().equals("Electronics"))
    .filter(p -> p.getPrice() < 600)
    .collect(Collectors.toList());

// Get names of all furniture
List<String> furnitureNames = products.stream()
    .filter(p -> p.getCategory().equals("Furniture"))
    .map(Product::getName)
    .collect(Collectors.toList());

// Calculate total price of electronics
double totalElectronicsPrice = products.stream()
    .filter(p -> p.getCategory().equals("Electronics"))
    .mapToDouble(Product::getPrice)
    .sum();

// Group by category
Map<String, List<Product>> byCategory = products.stream()
    .collect(Collectors.groupingBy(Product::getCategory));

// Find most expensive product
Optional<Product> mostExpensive = products.stream()
    .max((p1, p2) -> Double.compare(p1.getPrice(), p2.getPrice()));

// Average price per category
Map<String, Double> avgPriceByCategory = products.stream()
    .collect(Collectors.groupingBy(
        Product::getCategory,
        Collectors.averagingDouble(Product::getPrice)
    ));
```

## Closures and Variable Capture

Lambdas can access variables from their enclosing scope, but there are rules.

### Effectively Final Variables

```java
public void demonstrateClosure() {
    int multiplier = 10;  // Must be final or effectively final
    
    Function<Integer, Integer> multiply = num -> num * multiplier;
    
    System.out.println(multiply.apply(5));  // 50
    
    // multiplier = 20;  // ERROR! Can't modify after lambda uses it
}
```

### Why Effectively Final?

```java
public void problematicExample() {
    List<Runnable> runnables = new ArrayList<>();
    
    for (int i = 0; i < 5; i++) {
        final int value = i;  // Create effectively final variable
        runnables.add(() -> System.out.println(value));
    }
    
    runnables.forEach(Runnable::run);  // 0, 1, 2, 3, 4
}
```

## Real-World Examples

### Example 1: Button Click Handlers

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
        System.out.println("Button '" + label + "' clicked");
        if (onClick != null) {
            onClick.run();
        }
    }
}

class Application {
    private int counter = 0;
    
    public void setupUI() {
        Button submitButton = new Button("Submit");
        Button cancelButton = new Button("Cancel");
        Button incrementButton = new Button("Increment");
        
        // Lambda for submit
        submitButton.setOnClick(() -> {
            System.out.println("Submitting form...");
            validateForm();
            sendData();
        });
        
        // Lambda for cancel
        cancelButton.setOnClick(() -> System.out.println("Cancelled!"));
        
        // Lambda accessing instance variable
        incrementButton.setOnClick(() -> {
            counter++;
            System.out.println("Counter: " + counter);
        });
        
        submitButton.click();
        cancelButton.click();
        incrementButton.click();
        incrementButton.click();
    }
    
    private void validateForm() { System.out.println("Validating..."); }
    private void sendData() { System.out.println("Sending data..."); }
}
```

### Example 2: Custom Retry Logic

```java
@FunctionalInterface
interface RetryableOperation<T> {
    T execute() throws Exception;
}

class RetryHelper {
    public static <T> T retry(RetryableOperation<T> operation, int maxAttempts) {
        for (int attempt = 1; attempt <= maxAttempts; attempt++) {
            try {
                System.out.println("Attempt " + attempt);
                return operation.execute();
            } catch (Exception e) {
                System.out.println("Failed: " + e.getMessage());
                if (attempt == maxAttempts) {
                    throw new RuntimeException("All attempts failed", e);
                }
            }
        }
        throw new RuntimeException("Should not reach here");
    }
}

// Usage
String result = RetryHelper.retry(() -> {
    // Simulate network call that might fail
    if (Math.random() < 0.7) {
        throw new IOException("Network error");
    }
    return "Success!";
}, 5);
```

### Example 3: Template Method Pattern with Lambdas

```java
class DataProcessor {
    public void process(
        Supplier<String> dataReader,
        Function<String, String> dataTransformer,
        Consumer<String> dataWriter
    ) {
        // Template method
        String data = dataReader.get();
        String transformed = dataTransformer.apply(data);
        dataWriter.accept(transformed);
    }
}

// Usage - Different implementations
DataProcessor processor = new DataProcessor();

// CSV processing
processor.process(
    () -> "name,age\nAlice,30\nBob,25",           // Read CSV
    data -> data.toUpperCase(),                   // Transform
    result -> System.out.println("CSV:\n" + result) // Write
);

// JSON processing
processor.process(
    () -> "{\"name\":\"Alice\"}",                    // Read JSON
    data -> data.replace("Alice", "Bob"),           // Transform
    result -> System.out.println("JSON: " + result) // Write
);
```

### Example 4: Strategy Pattern with Lambdas

```java
class PaymentProcessor {
    @FunctionalInterface
    interface PaymentStrategy {
        boolean processPayment(double amount);
    }
    
    public boolean processPayment(double amount, PaymentStrategy strategy) {
        System.out.println("Processing payment: $" + amount);
        return strategy.processPayment(amount);
    }
}

// Usage
PaymentProcessor processor = new PaymentProcessor();

// Different payment strategies
processor.processPayment(100, amount -> {
    System.out.println("Processing credit card payment");
    return true;
});

processor.processPayment(50, amount -> {
    System.out.println("Processing PayPal payment");
    return true;
});

processor.processPayment(200, amount -> {
    System.out.println("Processing Bitcoin payment");
    return amount <= 1000;  // Limit for Bitcoin
});
```

## Lambda Best Practices

### 1. Keep Lambdas Short and Simple

```java
// BAD - Too complex
list.stream().filter(item -> {
    boolean condition1 = item.getPrice() > 100;
    boolean condition2 = item.getCategory().equals("Electronics");
    boolean condition3 = item.getStock() > 0;
    return condition1 && condition2 && condition3;
}).collect(Collectors.toList());

// GOOD - Extract to method
list.stream()
    .filter(this::isExpensiveElectronicInStock)
    .collect(Collectors.toList());

private boolean isExpensiveElectronicInStock(Item item) {
    return item.getPrice() > 100 
        && item.getCategory().equals("Electronics")
        && item.getStock() > 0;
}
```

### 2. Use Method References When Possible

```java
// GOOD but verbose
list.forEach(item -> System.out.println(item));

// BETTER - Method reference
list.forEach(System.out::println);
```

### 3. Prefer Standard Functional Interfaces

```java
// Don't create custom when standard exists
// BAD
@FunctionalInterface
interface StringProcessor {
    String process(String input);
}

// GOOD - Use Function<String, String>
Function<String, String> processor = input -> input.toUpperCase();
```

### 4. Document Complex Lambdas

```java
// Complex lambda - add comment
List<Order> urgentOrders = orders.stream()
    // Find orders that are urgent: placed > 2 days ago and not shipped
    .filter(order -> {
        long daysSincePlaced = ChronoUnit.DAYS.between(
            order.getPlacedDate(), 
            LocalDate.now()
        );
        return daysSincePlaced > 2 && !order.isShipped();
    })
    .collect(Collectors.toList());
```

## Summary

**Lambda Expressions**:
- **Syntax**: `(parameters) -> expression` or `(parameters) -> { statements }`
- **Purpose**: Concise way to implement functional interfaces
- **Benefits**: Less boilerplate, more readable code, enables functional programming

**Key Points**:
- Can only be used with functional interfaces (one abstract method)
- Can access effectively final variables from enclosing scope
- Make collections and streams easier to work with
- Replace verbose anonymous inner classes

**Common Functional Interfaces**:
- `Predicate<T>`: Test condition
- `Function<T,R>`: Transform data
- `Consumer<T>`: Consume data
- `Supplier<T>`: Supply data
- `BiFunction<T,U,R>`: Two-argument function

---

**Next Lesson**: We'll explore Method References - an even more concise way to write lambdas!