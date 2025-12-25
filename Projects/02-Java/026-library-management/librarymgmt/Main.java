package librarymgmt;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.Book;
import librarymgmt.model.Client;
import librarymgmt.model.LibraryItem;
import librarymgmt.model.Magazine;
import librarymgmt.model.Newspaper;
import librarymgmt.service.ClientService;
import librarymgmt.service.ItemService;
import librarymgmt.view.MainMenu;

public class Main {
    public static void main(String[] args) {
        ItemService<LibraryItem> itemService = new ItemService<>();
        ClientService clientService = new ClientService(itemService);


        try {
            itemService.add(Book.create("Title", "Author", 10, 2));
            itemService.add(Magazine.create("Magazine", 10, 1));
            itemService.add(Newspaper.create("News", "Journal", 3));
            clientService.add(Client.create("John", "John@email.com"));
        } catch (ObjectNotValidException e) {
            e.printStackTrace();
        }
        
        MainMenu.show(itemService, clientService);
    }
}
