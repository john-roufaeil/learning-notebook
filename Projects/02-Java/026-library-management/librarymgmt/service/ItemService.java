package librarymgmt.service;

import java.util.ArrayList;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.CRUDOperations;
import librarymgmt.model.LibraryItem;

public class ItemService<T extends LibraryItem> implements CRUDOperations<T> {
    private ArrayList<T> items;

    public ArrayList<T> getItems() { return items; }
    public void setItems(ArrayList<T> newItems) { items = newItems; }

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
        for (T item : items) {
            if (item.getId() == id) {
                return item;
            }
        }
        throw new ObjectNotFoundException("The item with id " + id + " is not found.");
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

    public void displayItems() {
        for (T item : items) {
            System.out.println(item.getItemDetails());
        }
    }
}
