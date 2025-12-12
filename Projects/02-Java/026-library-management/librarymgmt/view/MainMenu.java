package librarymgmt.view;

import librarymgmt.service.ClientService;
import librarymgmt.service.ItemService;
import librarymgmt.utils.ConsoleHelper;

public class MainMenu {

    public static void show(ItemService is, ClientService cs) {
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("\n=== Library Management System ===");
            System.out.println("1. Library Items");
            System.out.println("2. Clients & Borrowing");
            System.out.println("3. Exit");
            int choice = ConsoleHelper.readChoice("Select an option: ", 1, 3);

            switch (choice) {
                case 1 -> LibraryMenu.show(is);
                case 2 -> ClientMenu.show(cs);
                case 3 -> { return; }
            }
        }
    }
}
