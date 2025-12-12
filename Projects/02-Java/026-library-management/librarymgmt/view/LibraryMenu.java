package librarymgmt.view;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.*;
import librarymgmt.service.ItemService;
import librarymgmt.utils.ConsoleHelper;

public class LibraryMenu {
    private static ItemService itemService;
    
    public static void show(ItemService is) {
        itemService = is;
        
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("\n--- Library Items Menu ---");
            System.out.println("1. Add Book");
            System.out.println("2. Add Magazine");
            System.out.println("3. Add Newspaper");
            System.out.println("4. View an Item");
            System.out.println("5. View Items");
            System.out.println("6. Edit an Item");
            System.out.println("7. Delete an Item");
            System.out.println("8. Back");
            int choice = ConsoleHelper.readChoice("Select option: ", 1, 8);
            if (choice == Integer.MIN_VALUE) return;

            switch (choice) {
                case 1: addBook(); break;
                case 5: viewItems(); break;
                case 8: return;
            }
        }
    }

    private static void addBook() {
        String title = ConsoleHelper.readString("Enter book title: ");
        String author = ConsoleHelper.readString("Enter author name: ");
        int pages = ConsoleHelper.readInt("Enter number of pages: ");
        int stock = ConsoleHelper.readInt("Enter stock: ");
        try {
            Book book = new Book(title, author, pages, stock);
            itemService.create(book);
            System.out.println("Book added successfully!");
        } catch (ObjectNotValidException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    public static void viewItems() {
        itemService.displayItems();
        ConsoleHelper.pressEnterToContinue();
    }
}
 