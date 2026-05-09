package librarymgmt.view;

import librarymgmt.exception.ItemNotAvailableException;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.Client;
import librarymgmt.service.ClientService;
import librarymgmt.utils.ConsoleHelper;

public class ClientMenu {
    private static ClientService clientService;

    public static void show(ClientService cs) {
        clientService = cs;
        
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("\n=== Client Menu ===");
            System.out.println("1. Add Client");
            System.out.println("2. View a Client");
            System.out.println("3. View Clients");
            System.out.println("4. Edit a Client");
            System.out.println("5. Delete a Client");
            System.out.println("6. Borrow an Item");
            System.out.println("7. Return an Item");
            System.out.println("8. View Borrowings of a User");
            System.out.println("9. Back");
            int choice = ConsoleHelper.readChoice("Select option: ", 1, 9);

            switch (choice) {
                case 1 -> addClient();
                case 2 -> viewClient();
                case 3 -> viewClients();
                case 4 -> editClient();
                case 5 -> deleteClient();
                case 6 -> borrowItem();
                case 7 -> returnItem();
                case 8 -> viewBorrowings();
                case 9 -> { return; }
            }
        }
    }

    private static void addClient() {
        while (true) {
            ConsoleHelper.clearConsole();
            System.out.println("=== Add Client ===\n");

            String name = ConsoleHelper.readString("Enter client name: ");
            String email = ConsoleHelper.readString("Enter client email: ");

            try {
                Client newClient = Client.create(name, email);
                clientService.add(newClient);
                System.out.println("\nClient added successfully!");
            } catch (ObjectNotValidException e) {
                System.out.println("\nError: " + e.getMessage());
            }

            System.out.println("\n1. Add Another");
            System.out.println("2. Go Back");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 2);
            if (choice == 2) return;
        }
    }

    private static void viewClient() {
        ConsoleHelper.clearConsole();
        System.out.println("=== View Client ===\n");

        int clientId = ConsoleHelper.readInt("Enter Client ID: ");

        try {
            System.out.println("+-----------------");
            Client client = clientService.read(clientId);
            System.out.println(client.getClientDetails());
        } catch (ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }

    private static void viewClients() {
        ConsoleHelper.clearConsole();
        System.out.println("=== View Clients ===\n");
        System.out.println("+-----------------");
        clientService.readAll().forEach(c -> System.out.println(c.getClientDetails()));
        ConsoleHelper.pressEnterToReturn();
    }

    private static void editClient() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Edit Client ===\n");

        int clientId = ConsoleHelper.readInt("Enter Client ID to edit: ");

        Client client;
        try {
            client = clientService.read(clientId);
        } catch (ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
            ConsoleHelper.pressEnterToReturn();
            return;
        }

        boolean done = false;
        while (!done) {
            ConsoleHelper.clearConsole();
            System.out.println("=== Edit Client ===\n");
            System.out.println("+-----------------");
            System.out.println(client.getClientDetails());

            System.out.println("\n1. Edit Name");
            System.out.println("2. Edit Email");
            System.out.println("3. Done");

            int choice = ConsoleHelper.readChoice("Select option: ", 1, 3);

            try {
                switch (choice) {
                    case 1 -> client.setName(ConsoleHelper.readString("Enter new name: "));
                    case 2 -> client.setEmail(ConsoleHelper.readString("Enter new email: "));
                    case 3 -> done = true;
                }
            } catch (ObjectNotValidException e) {
                System.out.println("\nError: " + e.getMessage());
                ConsoleHelper.pressEnterToReturn();
            }
        }
    }

    private static void deleteClient() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Delete Client ===\n");

        int clientId = ConsoleHelper.readInt("Enter Client ID: ");

        try {
            clientService.delete(clientId);
            System.out.println("\nClient deleted successfully.");
        } catch (ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }

    private static void borrowItem() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Borrow Item ===\n");

        int clientId = ConsoleHelper.readInt("Enter Client ID: ");
        int itemId = ConsoleHelper.readInt("Enter Item ID: ");

        try {
            clientService.borrowItem(clientId, itemId);
            System.out.println("\nItem borrowed successfully.");
        } catch (ItemNotAvailableException | ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }

    private static void returnItem() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Return Item ===\n");

        int clientId = ConsoleHelper.readInt("Enter Client ID: ");
        int itemId = ConsoleHelper.readInt("Enter Item ID: ");

        try {
            clientService.returnItem(clientId, itemId);
            System.out.println("\nItem returned successfully.");
        } catch (ItemNotAvailableException | ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }

    private static void viewBorrowings() {
        ConsoleHelper.clearConsole();
        System.out.println("=== Client Borrowings ===\n");

        int clientId = ConsoleHelper.readInt("Enter Client ID: ");

        try {
            System.out.println("\n+-----------------");
            Client client = clientService.read(clientId);
            client.getBorrowedItems()
                  .forEach(item -> System.out.println(item.getItemDetails()));
        } catch (ObjectNotFoundException e) {
            System.out.println("\nError: " + e.getMessage());
        }

        ConsoleHelper.pressEnterToReturn();
    }
}
