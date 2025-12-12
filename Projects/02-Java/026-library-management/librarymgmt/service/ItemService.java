package librarymgmt.service;

import java.util.ArrayList;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.CRUDOperations;
import librarymgmt.model.LibraryItem;

public class ItemService<T extends LibraryItem> implements CRUDOperations<T> {
    private final ArrayList<T> items = new ArrayList<>();

    @Override
    public void create(T item) throws ObjectNotValidException {
        for (T existingItem : items) {
            if (existingItem.getId() == item.getId()) {
                throw new ObjectNotValidException("This item with id " + item.getId() + " already exists.");
            }
        }
        items.add(item);
    }

    @Override
    public T read(int id) throws ObjectNotFoundException {
        return items.stream()
            .filter(item -> item.getId() == id)
            .findFirst()
            .orElseThrow(
                () -> new ObjectNotFoundException("The item with id " + id + " is not found.")
            );
    }

    @Override
    public ArrayList<T> readAll() {
        return items;
    }

    @Override
    public void update(T newObj) throws ObjectNotFoundException {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).getId() == newObj.getId()) {
                items.set(i, newObj);
                return;
            }
        }
        throw new ObjectNotFoundException("The item with id " + newObj.getId() + " is not found.");
    }

    @Override
    public void delete(int id) throws ObjectNotFoundException {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).getId() == id) {
                items.remove(i);
                return;
            }
        }
        throw new ObjectNotFoundException("The item with id " + id + " is not found.");
    }

    @Override
    public void deleteAll() {
        items.clear();
    }

    public void displayItems() {
        for (T item : items) {
            System.out.println(item.getItemDetails());
        }
    }
}
