// Use generics and wildcards to allow adding and retrieving items of any subclass of LibraryItem.
// Implement methods to add, retrieve, and display items.
package librarymgmt.service;

import java.util.ArrayList;
import librarymgmt.model.LibraryItem;

public class ItemsService {
    private static ArrayList<LibraryItem> items;

    private ItemsService() {
        items = new ArrayList<>();
    }
}