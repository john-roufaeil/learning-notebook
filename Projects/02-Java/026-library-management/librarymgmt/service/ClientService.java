package librarymgmt.service;

import java.util.ArrayList;
import librarymgmt.exception.ItemNotAvailableException;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.CRUDOperations;
import librarymgmt.model.Client;
import librarymgmt.model.LibraryItem;

public class ClientService implements CRUDOperations<Client> {
    private final ArrayList<Client> clients = new ArrayList<>();
    private ItemService itemService;

    @Override
    public void create(Client newClient) throws ObjectNotValidException {
        for (Client existingClient : clients) {
            if (existingClient.getId() == newClient.getId()) {
                throw new ObjectNotValidException("This client with id " + newClient.getId() + " already exists.");
            }
        }
        clients.add(newClient);
    }

    @Override
    public Client read(int id) throws ObjectNotFoundException {
        return clients.stream()
                .filter(client -> client.getId() == id)
                .findFirst()
                .orElseThrow(() -> new ObjectNotFoundException("The client with id " + id + " is not found."));
    }

    @Override
    public ArrayList<Client> readAll() {
        return clients;
    }

    @Override
    public void update(Client updatedClient) throws ObjectNotFoundException {
        for (int i = 0; i < clients.size(); i++) {
            if (clients.get(i).getId() == updatedClient.getId()) {
                clients.set(i, updatedClient);
                return;
            }
        }
        throw new ObjectNotFoundException("The client with id " + updatedClient.getId() + " is not found.");
    }

    @Override
    public void delete(int id) throws ObjectNotFoundException {
        for (int i = 0; i < clients.size(); i++) {
            if (clients.get(i).getId() == id) {
                clients.remove(i);
                return;
            }
        }
        throw new ObjectNotFoundException("The client with id " + id + " is not found.");
    }

    @Override
    public void deleteAll() {
        clients.clear();
    }

    public void displayClients() {
        for (Client client : clients) {
            System.out.println(client.getClientDetails());
        }
    }

    public void borrowItem(int clientId, int itemId) throws ObjectNotFoundException, ItemNotAvailableException {
        Client client = read(clientId);
        LibraryItem item = itemService.read(itemId);

        item.decrementStock();
        client.getBorrowedItems().add(item);
        System.out.println(client.getName() + " borrowed " + item.getTitle());
    }

    public void returnItem(int clientId, int itemId) throws ObjectNotFoundException, ItemNotAvailableException {
        Client client = read(clientId);
        LibraryItem item = client.getBorrowedItem(itemId);

        item.incrementStock();
        client.getBorrowedItems().remove(item);
        System.out.println(client.getName() + " returned " + item.getTitle());
    }
}
