package librarymgmt.view;

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
            if (choice == Integer.MIN_VALUE) return;

            switch (choice) {
                case 1 -> addClient();
                case 3 -> viewClients();
                case 8 -> { return; }
            }
        }
    }

    private static void addClient() {
        String name = ConsoleHelper.readString("Enter client name: ");
        String email = ConsoleHelper.readString("Enter client email: ");
        try {
            Client newClient = Client.create(name, email);
            clientService.add(newClient);
            System.out.println("Client added successfully!");
        } catch (ObjectNotValidException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    private static void viewClients() {
        clientService.readAll().forEach(c -> System.out.println(c.getClientDetails()));
        ConsoleHelper.pressEnterToReturn();
    }
}
