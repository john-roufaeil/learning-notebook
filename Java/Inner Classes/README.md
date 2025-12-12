# Inner Classes - All Four Types

## What Are Inner Classes?

**Inner classes** (also called **nested classes**) are classes defined within another class. Java provides four types of inner classes, each serving different purposes.

## Why Inner Classes?

1. **Logical grouping**: Group classes that are only used in one place
2. **Encapsulation**: Hide implementation details
3. **Readable code**: Keep related code together
4. **Access to outer class**: Can access private members of outer class

## The Four Types of Inner Classes

1. **Member Inner Class** (Non-static nested class)
2. **Static Nested Class**
3. **Local Inner Class**
4. **Anonymous Inner Class**

---

## 1. Member Inner Class (Non-Static)

A regular inner class that's a member of the outer class.

### Basic Example

```java
class Outer {
    private int outerData = 100;
    
    // Member inner class
    class Inner {
        private int innerData = 50;
        
        void display() {
            // Can access outer class private members!
            System.out.println("Outer data: " + outerData);
            System.out.println("Inner data: " + innerData);
        }
        
        void modifyOuter() {
            outerData = 200;  // Can modify outer class members
        }
    }
    
    void createInner() {
        Inner inner = new Inner();
        inner.display();
    }
}

// Usage
Outer outer = new Outer();
outer.createInner();

// Can also create inner class from outside
Outer.Inner inner = outer.new Inner();  // Notice: outer.new
inner.display();
```

### Key Characteristics

- **Cannot have static members** (except static final constants)
- **Has implicit reference** to outer class instance
- **Can access all members** of outer class (even private)
- **Requires outer class instance** to be created

### Real-World Example: LinkedList Node

```java
class LinkedList {
    private Node head;
    private int size = 0;
    
    // Member inner class - Node only makes sense within LinkedList
    private class Node {
        int data;
        Node next;
        
        Node(int data) {
            this.data = data;
            this.next = null;
        }
    }
    
    public void add(int data) {
        Node newNode = new Node(data);
        
        if (head == null) {
            head = newNode;
        } else {
            Node current = head;
            while (current.next != null) {
                current = current.next;
            }
            current.next = newNode;
        }
        size++;
    }
    
    public void display() {
        Node current = head;
        while (current != null) {
            System.out.print(current.data + " -> ");
            current = current.next;
        }
        System.out.println("null");
    }
    
    public int getSize() {
        return size;  // Inner class can access outer class fields
    }
}

// Usage
LinkedList list = new LinkedList();
list.add(10);
list.add(20);
list.add(30);
list.display();  // 10 -> 20 -> 30 -> null
```

### Advanced Example: Iterator Pattern

```java
class MyCollection {
    private String[] items;
    private int size = 0;
    
    public MyCollection(int capacity) {
        items = new String[capacity];
    }
    
    public void add(String item) {
        if (size < items.length) {
            items[size++] = item;
        }
    }
    
    // Member inner class implementing Iterator
    class MyIterator {
        private int currentIndex = 0;
        
        public boolean hasNext() {
            return currentIndex < size;  // Access outer class 'size'
        }
        
        public String next() {
            if (!hasNext()) {
                throw new RuntimeException("No more elements");
            }
            return items[currentIndex++];  // Access outer class 'items'
        }
    }
    
    public MyIterator iterator() {
        return new MyIterator();
    }
}

// Usage
MyCollection collection = new MyCollection(5);
collection.add("Apple");
collection.add("Banana");
collection.add("Cherry");

MyCollection.MyIterator iterator = collection.iterator();
while (iterator.hasNext()) {
    System.out.println(iterator.next());
}
```

---

## 2. Static Nested Class

A nested class marked with `static` keyword. It's associated with the outer class, not an instance.

### Basic Example

```java
class Outer {
    private static int staticOuterData = 100;
    private int instanceOuterData = 200;
    
    // Static nested class
    static class StaticNested {
        void display() {
            // Can access static members of outer class
            System.out.println("Static outer data: " + staticOuterData);
            
            // CANNOT access instance members directly
            // System.out.println(instanceOuterData);  // ERROR!
        }
        
        void displayWithInstance(Outer outer) {
            // Can access through outer class reference
            System.out.println("Instance data: " + outer.instanceOuterData);
        }
    }
}

// Usage - No outer instance needed!
Outer.StaticNested nested = new Outer.StaticNested();
nested.display();

// To access instance members, pass outer instance
Outer outer = new Outer();
nested.displayWithInstance(outer);
```

### Key Characteristics

- **No implicit reference** to outer class instance
- **Can only access static members** of outer class directly
- **Can have static members** (unlike member inner class)
- **More memory efficient** (no outer class reference stored)
- **Acts like a regular class** that happens to be nested

### When to Use Static Nested Class

Use when the nested class doesn't need access to outer class instance members.

### Real-World Example: Builder Pattern

```java
class Person {
    private final String firstName;
    private final String lastName;
    private final int age;
    private final String email;
    private final String phone;
    
    // Private constructor - only Builder can create Person
    private Person(Builder builder) {
        this.firstName = builder.firstName;
        this.lastName = builder.lastName;
        this.age = builder.age;
        this.email = builder.email;
        this.phone = builder.phone;
    }
    
    // Static nested Builder class
    public static class Builder {
        // Required parameters
        private final String firstName;
        private final String lastName;
        
        // Optional parameters
        private int age = 0;
        private String email = "";
        private String phone = "";
        
        public Builder(String firstName, String lastName) {
            this.firstName = firstName;
            this.lastName = lastName;
        }
        
        public Builder age(int age) {
            this.age = age;
            return this;
        }
        
        public Builder email(String email) {
            this.email = email;
            return this;
        }
        
        public Builder phone(String phone) {
            this.phone = phone;
            return this;
        }
        
        public Person build() {
            return new Person(this);
        }
    }
    
    @Override
    public String toString() {
        return "Person{" +
                "firstName='" + firstName + '\'' +
                ", lastName='" + lastName + '\'' +
                ", age=" + age +
                ", email='" + email + '\'' +
                ", phone='" + phone + '\'' +
                '}';
    }
}

// Usage - Beautiful fluent API!
Person person = new Person.Builder("John", "Doe")
    .age(30)
    .email("john@example.com")
    .phone("555-1234")
    .build();

System.out.println(person);
```

### Example: HashMap Entry

```java
class MyHashMap<K, V> {
    private Entry<K, V>[] table;
    private int size = 0;
    
    // Static nested class for entries
    static class Entry<K, V> {
        final K key;
        V value;
        Entry<K, V> next;
        
        Entry(K key, V value, Entry<K, V> next) {
            this.key = key;
            this.value = value;
            this.next = next;
        }
        
        public K getKey() { return key; }
        public V getValue() { return value; }
        public void setValue(V value) { this.value = value; }
    }
    
    @SuppressWarnings("unchecked")
    public MyHashMap(int capacity) {
        table = new Entry[capacity];
    }
    
    public void put(K key, V value) {
        int index = key.hashCode() % table.length;
        Entry<K, V> entry = new Entry<>(key, value, table[index]);
        table[index] = entry;
        size++;
    }
    
    public V get(K key) {
        int index = key.hashCode() % table.length;
        Entry<K, V> entry = table[index];
        
        while (entry != null) {
            if (entry.key.equals(key)) {
                return entry.value;
            }
            entry = entry.next;
        }
        return null;
    }
}
```

---

## 3. Local Inner Class

A class defined inside a method or block.

### Basic Example

```java
class Outer {
    void outerMethod() {
        final int localVariable = 100;  // Effectively final
        
        // Local inner class
        class LocalInner {
            void display() {
                // Can access outer class members
                // Can access local variables (must be final or effectively final)
                System.out.println("Local variable: " + localVariable);
            }
        }
        
        // Create and use local inner class within the method
        LocalInner local = new LocalInner();
        local.display();
    }
}

// Usage
Outer outer = new Outer();
outer.outerMethod();
```

### Key Characteristics

- **Scope limited to the block** where it's defined
- **Cannot have access modifiers** (public, private, etc.)
- **Cannot be static**
- **Can access local variables** that are final or effectively final
- **Can access outer class members**

### Why "Effectively Final"?

```java
class Example {
    void method() {
        int x = 10;  // Not explicitly final, but never modified
        
        class LocalClass {
            void display() {
                System.out.println(x);  // OK - x is effectively final
            }
        }
        
        // x = 20;  // If you uncomment this, LocalClass won't compile!
        
        LocalClass local = new LocalClass();
        local.display();
    }
}
```

### Real-World Example: Validation

```java
class UserService {
    public void registerUser(String username, String password, int age) {
        // Local inner class for validation
        class Validator {
            boolean isValidUsername() {
                return username != null && username.length() >= 3;
            }
            
            boolean isValidPassword() {
                return password != null && password.length() >= 8;
            }
            
            boolean isValidAge() {
                return age >= 18 && age <= 120;
            }
            
            void validate() {
                if (!isValidUsername()) {
                    throw new IllegalArgumentException("Invalid username");
                }
                if (!isValidPassword()) {
                    throw new IllegalArgumentException("Invalid password");
                }
                if (!isValidAge()) {
                    throw new IllegalArgumentException("Invalid age");
                }
            }
        }
        
        // Use the validator
        Validator validator = new Validator();
        validator.validate();
        
        // Proceed with registration
        System.out.println("User registered: " + username);
    }
}

// Usage
UserService service = new UserService();
service.registerUser("john_doe", "password123", 25);
```

### Example: Comparator

```java
class StudentService {
    public void sortStudents(List<Student> students, String sortBy) {
        
        if (sortBy.equals("name")) {
            // Local inner class for name comparison
            class NameComparator implements Comparator<Student> {
                @Override
                public int compare(Student s1, Student s2) {
                    return s1.getName().compareTo(s2.getName());
                }
            }
            Collections.sort(students, new NameComparator());
            
        } else if (sortBy.equals("grade")) {
            // Local inner class for grade comparison
            class GradeComparator implements Comparator<Student> {
                @Override
                public int compare(Student s1, Student s2) {
                    return Double.compare(s1.getGrade(), s2.getGrade());
                }
            }
            Collections.sort(students, new GradeComparator());
        }
    }
}
```

---

## 4. Anonymous Inner Class

A class without a name, defined and instantiated in a single expression.

### Basic Example

```java
interface Greeting {
    void greet();
}

class Example {
    public void demonstrateAnonymous() {
        // Anonymous inner class implementing interface
        Greeting greeting = new Greeting() {
            @Override
            public void greet() {
                System.out.println("Hello from anonymous class!");
            }
        };
        
        greeting.greet();
    }
}
```

### Key Characteristics

- **No class name**
- **Defined and instantiated in one expression**
- **Can extend a class or implement one interface** (not both)
- **Cannot have constructors** (but can have instance initializers)
- **Can access outer class members and final local variables**

### Implementing Interface

```java
interface Calculator {
    int calculate(int a, int b);
}

class MathOperations {
    public void performOperations() {
        // Anonymous class for addition
        Calculator adder = new Calculator() {
            @Override
            public int calculate(int a, int b) {
                return a + b;
            }
        };
        
        // Anonymous class for multiplication
        Calculator multiplier = new Calculator() {
            @Override
            public int calculate(int a, int b) {
                return a * b;
            }
        };
        
        System.out.println("5 + 3 = " + adder.calculate(5, 3));
        System.out.println("5 * 3 = " + multiplier.calculate(5, 3));
    }
}
```

### Extending Class

```java
class Animal {
    void makeSound() {
        System.out.println("Some sound");
    }
}

class Zoo {
    public void demonstrateAnimals() {
        // Anonymous class extending Animal
        Animal dog = new Animal() {
            @Override
            void makeSound() {
                System.out.println("Woof!");
            }
        };
        
        Animal cat = new Animal() {
            @Override
            void makeSound() {
                System.out.println("Meow!");
            }
        };
        
        dog.makeSound();
        cat.makeSound();
    }
}
```

### Real-World Example: Event Handling

```java
class Button {
    private String label;
    private OnClickListener listener;
    
    public Button(String label) {
        this.label = label;
    }
    
    public void setOnClickListener(OnClickListener listener) {
        this.listener = listener;
    }
    
    public void click() {
        System.out.println("Button '" + label + "' clicked");
        if (listener != null) {
            listener.onClick();
        }
    }
    
    interface OnClickListener {
        void onClick();
    }
}

class Application {
    public void setupUI() {
        Button submitButton = new Button("Submit");
        
        // Anonymous inner class for click handler
        submitButton.setOnClickListener(new Button.OnClickListener() {
            @Override
            public void onClick() {
                System.out.println("Form submitted!");
                validateForm();
                sendData();
            }
            
            private void validateForm() {
                System.out.println("Validating form...");
            }
            
            private void sendData() {
                System.out.println("Sending data...");
            }
        });
        
        submitButton.click();
    }
}
```

### Example: Thread Creation

```java
class ThreadExample {
    public void demonstrateThreads() {
        // Anonymous inner class extending Thread
        Thread thread1 = new Thread() {
            @Override
            public void run() {
                for (int i = 0; i < 5; i++) {
                    System.out.println("Thread 1: " + i);
                }
            }
        };
        
        // Anonymous inner class implementing Runnable
        Thread thread2 = new Thread(new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 5; i++) {
                    System.out.println("Thread 2: " + i);
                }
            }
        });
        
        thread1.start();
        thread2.start();
    }
}
```

### Instance Initializer in Anonymous Class

```java
interface Counter {
    int getCount();
}

class Example {
    public void demonstrate() {
        Counter counter = new Counter() {
            private int count;
            
            // Instance initializer block
            {
                count = 0;
                System.out.println("Counter initialized");
            }
            
            @Override
            public int getCount() {
                return ++count;
            }
        };
        
        System.out.println(counter.getCount());  // 1
        System.out.println(counter.getCount());  // 2
    }
}
```

---

## Comparison Table

| Feature | Member Inner | Static Nested | Local Inner | Anonymous Inner |
|---------|-------------|---------------|-------------|-----------------|
| **Can access outer instance members** | ✅ Yes | ❌ No | ✅ Yes | ✅ Yes |
| **Can access outer static members** | ✅ Yes | ✅ Yes | ✅ Yes | ✅ Yes |
| **Can have static members** | ❌ No (except final) | ✅ Yes | ❌ No | ❌ No |
| **Needs outer instance to create** | ✅ Yes | ❌ No | ✅ Yes | ✅ Yes |
| **Can have access modifiers** | ✅ Yes | ✅ Yes | ❌ No | ❌ No |
| **Scope** | Class level | Class level | Method/block level | Expression level |
| **Has a name** | ✅ Yes | ✅ Yes | ✅ Yes | ❌ No |
| **Can have constructor** | ✅ Yes | ✅ Yes | ✅ Yes | ❌ No |

## When to Use Each Type

### Member Inner Class
- When inner class needs access to outer instance state
- Iterator implementations
- Linked list nodes
- Helper classes that work with outer class data

### Static Nested Class
- When inner class doesn't need outer instance
- Builder pattern
- Helper classes for grouping
- Entry classes (like HashMap.Entry)

### Local Inner Class
- When class is needed only in one method
- Temporary validators
- Method-specific comparators
- Scoped implementations

### Anonymous Inner Class
- One-time use implementations
- Event handlers
- Quick interface implementations
- Thread creation
- **Note**: Often replaced by lambdas in modern Java!

---

**Next Lesson**: We'll explore Lambda Functions and how they simplify anonymous inner classes!