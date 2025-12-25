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
            System.out.println("8. Delete all Items");
            System.out.println("9. Back");
            int choice = ConsoleHelper.readChoice("Select option: ", 1, 9);

            switch (choice) {
                case 1 -> addBook();
                case 2 -> addMagazine();
                case 3 -> addNewspaper();
                case 4 -> viewItem();
                case 5 -> viewItems();
                case 6 -> editItem();
                case 7 -> deleteItem();
                case 8 -> deleteAllItems();
                case 9 -> { return; }
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
    ConsoleHelper.clearConsole();
    System.out.println("=== Edit a Library Item ===\n");

    int itemId = ConsoleHelper.readInt("Enter Item ID to edit: ");
    LibraryItem item;

    try {
        item = itemService.read(itemId);
    } catch (ObjectNotFoundException e) {
        System.out.println("\nError: " + e.getMessage());
        ConsoleHelper.pressEnterToReturn();
        return;
    }

    boolean done = false;
    while (!done) {
        ConsoleHelper.clearConsole();
        System.out.println("Editing Item ID #" + item.getId());
        System.out.println("\n+-----------------");
        System.out.println(item.getItemDetails());
        System.out.println("\nSelect field to edit:");

        if (item instanceof Book book) {
            System.out.println("1. Title");
            System.out.println("2. Author");
            System.out.println("3. Pages");
            System.out.println("4. Stock");
            System.out.println("5. Done");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 5);
            try {
                switch (choice) {
                    case 1 -> book.setTitle(ConsoleHelper.readString("Enter new title: "));
                    case 2 -> book.setAuthor(ConsoleHelper.readString("Enter new author: "));
                    case 3 -> book.setPages(ConsoleHelper.readInt("Enter new page count: "));
                    case 4 -> book.setStock(ConsoleHelper.readInt("Enter new stock: "));
                    case 5 -> done = true;
                }
            } catch (ObjectNotValidException ex) {
                System.out.println("\nError: " + ex.getMessage());
                ConsoleHelper.pressEnterToReturn();
            }

        } else if (item instanceof Magazine magazine) {
            System.out.println("1. Title");
            System.out.println("2. Pages");
            System.out.println("3. Stock");
            System.out.println("4. Done");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 4);
            try {
                switch (choice) {
                    case 1 -> magazine.setTitle(ConsoleHelper.readString("Enter new title: "));
                    case 2 -> magazine.setPages(ConsoleHelper.readInt("Enter new page count: "));
                    case 3 -> magazine.setStock(ConsoleHelper.readInt("Enter new stock: "));
                    case 4 -> done = true;
                }
            } catch (ObjectNotValidException ex) {
                System.out.println("\nError: " + ex.getMessage());
                ConsoleHelper.pressEnterToReturn();
            }

        } else if (item instanceof Newspaper newspaper) {
            System.out.println("1. Title");
            System.out.println("2. Journal");
            System.out.println("3. Stock");
            System.out.println("4. Done");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 4);
            try {
                switch (choice) {
                    case 1 -> newspaper.setTitle(ConsoleHelper.readString("Enter new title: "));
                    case 2 -> newspaper.setJournal(ConsoleHelper.readString("Enter new journal: "));
                    case 3 -> newspaper.setStock(ConsoleHelper.readInt("Enter new stock: "));
                    case 4 -> done = true;
                }
            } catch (ObjectNotValidException ex) {
                System.out.println("\nError: " + ex.getMessage());
                ConsoleHelper.pressEnterToReturn();
            }
        }
    }
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

    public static void deleteAllItems() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Delete All Items ===");
        boolean confirm = ConsoleHelper.confirmAction("This action will delete all items in the library. Are you sure you want to proceed?");
        if (confirm) {
            itemService.deleteAll();
            System.out.println("All library items have been deleted successfully");
            ConsoleHelper.pressEnterToReturn();
        } else {
            System.out.println("Deletion canceled");
            ConsoleHelper.pressEnterToReturn();
        }
    }
}
 