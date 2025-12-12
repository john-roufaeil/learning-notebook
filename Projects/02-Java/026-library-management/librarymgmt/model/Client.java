package librarymgmt.model;

import java.util.ArrayList;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.items.LibraryItem;
import librarymgmt.utils.ClientValidator;

public class Client {
    private static int clientsCount = 0;
    private final int id;
    private String name;
    private String email;
    private final ArrayList<LibraryItem> borrowedItems = new ArrayList<>();
    private static final ClientValidator validator = (name, email) -> {
        if (name == null || name.isBlank()) {
            throw new ObjectNotValidException("Client name cannot be empty.");
        } else if (email == null || email.isBlank()) {
            throw new ObjectNotValidException("Client email cannot be empty.");
        } else if (!email.matches("^[A-Za-z0-9_.-]+@[A-Za-z.-]+$")) {
            throw new ObjectNotValidException("Client email is invalid.");
        }
    };

    public Client(String name, String email) throws ObjectNotValidException {
        validator.validate(name, email);
        this.id = ++clientsCount;
        this.name = name;
        this.email = email;
    }

    public int getId() { return id; }
    public String getName() { return name; }
    public String getEmail() { return email; }
    public ArrayList<LibraryItem> getBorrowedItems() { return borrowedItems; }
    public LibraryItem getBorrowedItem(int itemId) throws ObjectNotFoundException { 
        return borrowedItems.stream()
            .filter(item -> item.getId() == itemId)
            .findFirst()
            .orElse(null);
            // .orElseThrow(() -> new ObjectNotFoundException(name + " has not borrowed item with id " + itemId + "."));
    }

    public void setName(String newName) throws ObjectNotValidException {
        validator.validate(newName, email);
        name = newName;
    }
    public void setEmail(String newEmail) throws ObjectNotValidException {
        validator.validate(name, newEmail);
        email = newEmail;
    }

    public String getClientDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("| Client #").append(id).append(" Details");
        sb.append("\n| Name: ").append(name);
        sb.append("\n| Email: ").append(email);
        sb.append("\n------------------");
        return sb.toString();
    }
}