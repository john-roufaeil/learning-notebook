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