# Library Management System

OOP, exception handling, generics, and wildcards

1. Design a class hierarchy for Library Management System:
   - Create an abstract class `LibraryItem` with properties like `id` and `title`.
   - Extend `LibraryItem` to create subclasses `Book` and `Magazine`.
   - Add a method `getItemDetails()` to return item details.

2. Implement a custom exception `ItemNotFoundException` that is thrown when a library item is not found.

3. Create a `Library` class to manage a collection of `LibraryItem` objects:
   - Use generics and wildcards to allow adding and retrieving items of any subclass of `LibraryItem`.
   - Implement methods to add, retrieve, and display items.

4. Design a class hierarchy for library clients:
   - Create a `Client` class with properties like `id`, `name`, and `email`.
   - Add a method `getClientDetails()` to display client information.

5. Implement a menu system to provide CRUD (Create, Read, Update, Delete) functionalities for:
   - Library items (e.g., add books/magazines, retrieve item details, update or delete items).
   - Library clients (e.g., add clients, retrieve client details, update or delete clients).

6. In the main method:
   - Create instances of `Book`, `Magazine`, and `Client`.
   - Use the menu system to demonstrate CRUD operations for both library items and clients.



## Requirements

### 1. Library Item Class Hierarchy
- Create an abstract class `LibraryItem` with properties:
  - `id` (String)
  - `title` (String)
  - `available` (boolean)
- Extend `LibraryItem` to create concrete subclasses:
  - `Book`: Add properties like `author`, `isbn`, `publicationYear`
  - `Magazine`: Add properties like `issueNumber`, `publicationMonth`
- Implement abstract method `getItemDetails()` to return formatted item information

### 2. Client Entity
- Create a `Client` class with properties:
  - `id` (String)
  - `name` (String)
  - `email` (String)
  - `phoneNumber` (String)
- Add method `getClientDetails()` to display client information

### 3. Borrow Entity
- Create a `Borrow` class to track borrowing transactions:
  - `borrowId` (String)
  - `client` (Client)
  - `item` (LibraryItem)
  - `borrowDate` (LocalDate)
  - `dueDate` (LocalDate)
  - `returnDate` (LocalDate) - nullable
  - `status` (enum: ACTIVE, RETURNED, OVERDUE)
- Implement methods:
  - `isOverdue()`: Check if borrow is past due date
  - `calculateLateFee()`: Calculate fee for overdue items
  - `getDaysOverdue()`: Calculate days past due date

### 4. Custom Exceptions
Implement custom exception classes:
- `ItemNotFoundException`: Thrown when a library item is not found
- `ClientNotFoundException`: Thrown when a client is not found
- `BorrowNotFoundException`: Thrown when a borrow record is not found
- `ItemNotAvailableException`: Thrown when attempting to borrow an unavailable item

### 5. Service Layer with Generics and Wildcards

**LibraryService:**
- Use generics to manage collections of `LibraryItem` and its subclasses
- Implement methods:
  - `addItem(T item)` where T extends LibraryItem
  - `getItemById(String id)`: Returns Optional<LibraryItem>
  - `getAllItems()`: Returns all items
  - `updateItem(String id, LibraryItem updatedItem)`
  - `deleteItem(String id)`
  - `displayAllItems()`: Print all item details

**ClientService:**
- Manage CRUD operations for clients:
  - `addClient(Client client)`
  - `getClientById(String id)`: Returns Optional<Client>
  - `getAllClients()`
  - `updateClient(String id, Client updatedClient)`
  - `deleteClient(String id)`

**BorrowService:**
- Manage borrowing operations:
  - `borrowItem(String clientId, String itemId)`: Create new borrow record
  - `returnItem(String borrowId)`: Mark item as returned
  - `getBorrowById(String borrowId)`: Returns Optional<Borrow>
  - `getActiveBorrows()`: Returns all active borrows
  - `getBorrowsByClient(String clientId)`: Returns client's borrow history

### 6. Lambda Expressions and Method References

Implement the following using lambdas and method references:

**Sorting Operations:**
- Sort items by title: `Comparator.comparing(LibraryItem::getTitle)`
- Sort items by ID: `Comparator.comparing(LibraryItem::getId)`
- Sort clients by name: `Comparator.comparing(Client::getName)`
- Sort borrows by due date: `Comparator.comparing(Borrow::getDueDate)`

**Filtering with Predicates:**
- Filter available items: `items.stream().filter(LibraryItem::isAvailable)`
- Filter books only: `items.stream().filter(Book.class::isInstance)`
- Find overdue borrows: `borrows.stream().filter(Borrow::isOverdue)`
- Search items by title keyword: `items.stream().filter(item -> item.getTitle().contains(keyword))`

**Statistical Operations:**
- Count total books: `items.stream().filter(Book.class::isInstance).count()`
- Calculate total late fees: `borrows.stream().mapToDouble(Borrow::calculateLateFee).sum()`
- Group items by type: `items.stream().collect(Collectors.groupingBy(item -> item.getClass().getSimpleName()))`
- Find average overdue days: `borrows.stream().filter(Borrow::isOverdue).mapToInt(Borrow::getDaysOverdue).average()`

**forEach with Method References:**
- Display all items: `items.forEach(System.out::println)`
- Send overdue notifications: `overdueborrows.forEach(notificationService::sendReminder)`

**Custom Functional Interface:**
```java
@FunctionalInterface
interface ItemValidator {
    boolean validate(LibraryItem item);
}
```
Use it to create flexible validation: `validateItems(item -> item.getTitle().length() > 3)`

### 7. Menu System (UI Layer)

Create an interactive console menu with the following options:

**Library Items Menu:**
1. Add new item (Book/Magazine)
2. View item by ID
3. View all items
4. Update item details
5. Delete item
6. Sort items (by title, ID)
7. Filter items (available only, by type)
8. Search items by keyword

**Clients Menu:**
1. Add new client
2. View client by ID
3. View all clients
4. Update client details
5. Delete client
6. Sort clients by name

**Borrowing Menu:**
1. Borrow an item
2. Return an item
3. View all active borrows
4. View overdue borrows
5. View client borrow history
6. Calculate total late fees
7. View borrow statistics

### 8. Main Method Requirements

In your main method, demonstrate:
- Creating instances of `Book`, `Magazine`, and `Client`
- Adding items and clients to the system
- Performing borrow operations
- Handling exceptions appropriately
- Using lambda expressions for:
  - Sorting collections
  - Filtering data
  - Statistical calculations
- Using method references where appropriate
- Displaying results using the menu system

### 9. Additional Features (Optional Enhancements)

- Implement data persistence using file I/O
- Add input validation using lambda-based validators
- Create a notification system using functional interfaces

## Learning Objectives

By completing this project, you will demonstrate:
- Generics and wildcards for type-safe collections
- Lambda expressions for functional programming
- Method references for cleaner code
- Stream API for data processing
- Predicate, Function, and Consumer functional interfaces
- Comparator with lambda expressions
- Optional for null-safety
