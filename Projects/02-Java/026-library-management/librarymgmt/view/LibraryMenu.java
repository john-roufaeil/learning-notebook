package librarymgmt.view;

import librarymgmt.exception.ObjectNotFoundException;
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
            System.out.println("\n=== Library Items Menu ===");
            System.out.println("1. Add Book");
            System.out.println("2. Add Magazine");
            System.out.println("3. Add Newspaper");
            System.out.println("4. View an Item");
            System.out.println("5. View Items");
            System.out.println("6. Edit an Item");
            System.out.println("7. Delete an Item");
            System.out.println("8. Back");
            int choice = ConsoleHelper.readChoice("Select option: ", 1, 8);

            switch (choice) {
                case 1 -> addBook();
                case 2 -> addMagazine();
                case 3 -> addNewspaper();
                case 4 -> viewItem();
                case 5 -> viewItems();
                case 6 -> editItem();
                case 7 -> deleteItem();
                case 8 -> { return; }
            }
        }
    }

    private static void addBook() {
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("=== Add a New Book ===\n");

            String title = ConsoleHelper.readString("Enter book title: ");
            String author = ConsoleHelper.readString("Enter author name: ");
            int pages = ConsoleHelper.readInt("Enter number of pages: ");
            int stock = ConsoleHelper.readInt("Enter stock: ");

            try {
                Book newBook = Book.create(title, author, pages, stock);
                itemService.add(newBook);
                System.out.println("\nBook added successfully!");
            } catch (ObjectNotValidException e) {
                System.out.println("\nError: " + e.getMessage());
            }

            System.out.println("\n1. Add Another");
            System.out.println("2. Go Back");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 2);
            if (choice == 2) return;
        }
    }

    private static void addMagazine() {
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("=== Add a New Magazine ===\n");

            String title = ConsoleHelper.readString("Enter magazine title: ");
            int pages = ConsoleHelper.readInt("Enter number of pages: ");
            int stock = ConsoleHelper.readInt("Enter stock: ");

            try {
                Magazine newMagazine = Magazine.create(title, pages, stock);
                itemService.add(newMagazine);
                System.out.println("\nMagazine added successfully!");
            } catch (ObjectNotValidException e) {
                System.out.println("\nError: " + e.getMessage());
            }

            System.out.println("\n1. Add Another");
            System.out.println("2. Go Back");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 2);
            if (choice == 2) return;
        }
    }


    private static void addNewspaper() {
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("=== Add a New Newspaper ===\n");

            String title = ConsoleHelper.readString("Enter magazine title: ");
            String journal = ConsoleHelper.readString("Enter journal name: ");
            int stock = ConsoleHelper.readInt("Enter stock: ");

            try {
                Newspaper newNewspaper = Newspaper.create(title, journal, stock);
                itemService.add(newNewspaper);
                System.out.println("\nNewspaper added successfully!");
            } catch (ObjectNotValidException e) {
                System.out.println("\nError: " + e.getMessage());
            }

            System.out.println("\n1. Add Another");
            System.out.println("2. Go Back");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 2);
            if (choice == 2) return;
        }
    }

    public static void viewItem() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Library Item  ===\n");
        
        int itemId = ConsoleHelper.readInt("Enter Item ID: ");

        try {
            System.out.println("+-----------------");
            System.out.println(itemService.read(itemId).getItemDetails());
        } catch (ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }

    public static void viewItems() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Library Items Stored ===\n");
        System.out.println("+-----------------");
        itemService.displayItems();
        ConsoleHelper.pressEnterToReturn();
    }

    public static void editItem() {

    }

    public static void deleteItem() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Delete a Library Item ===");
        int itemId = ConsoleHelper.readInt("Enter Item ID: ");

        try {
            itemService.delete(itemId);
            System.out.println("Item with id " + itemId + " has been successfully deleted.");
        } catch (ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }
}
 