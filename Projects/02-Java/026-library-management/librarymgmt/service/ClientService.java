package librarymgmt.service;

import java.util.ArrayList;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.CRUDOperations;
import librarymgmt.model.Client;

public class ClientService implements CRUDOperations<Client> {
    private ArrayList<Client> clients;

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
        for (Client client : clients) {
            if (client.getId() == id) {
                return client;
            }
        }
        throw new ObjectNotFoundException("The client with id " + id + " is not found.");
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

    public void displayClients() {
        for (Client client : clients) {
            System.out.println(client.getClientDetails());
        }
    }
}