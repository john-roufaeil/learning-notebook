package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.ClientValidator;

public class Client {
    private static int clientsCount = 0;
    private final int id;
    private String name;
    private String email;
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