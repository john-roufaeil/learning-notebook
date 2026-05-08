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


Additional Requirements

1. Relation between Client and Library Items:
Establish a relationship between Client and LibraryItem where a client can rent or borrow items.
Each Client should have a collection of borrowed library items.

2. Borrow/Rent Feature:
Add a stock property to each library item to manage multiple copies of the same item.
Alternatively, manage a single copy per item with an availability property
Add two methods in the menu:
borrowItem(userId, itemId): Allows a client to borrow an item.
returnItem(itemId, userId): Allows a client to return an item.

3. Use of Streams:
Use Java streams for operations such as filtering library items or finding a client by ID.
Ensure streams are used wherever applicable for efficiency and simplicity.

4. Validations:
Ensure unique IDs across all entities (LibraryItem and Client).
Validate user input, such as checking for item availability or verifying client and item IDs before processing requests.

5. Interfaces:
Create a CRUD interface implemented by both LibraryItem and Client.
Define methods like create, read, update, and delete in the interface.

6. User-Friendly Menu:
Design a clear and user-friendly menu system with descriptive instructions.
Include meaningful error messages for invalid operations.
