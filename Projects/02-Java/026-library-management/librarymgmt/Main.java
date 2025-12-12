package librarymgmt;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.model.Book;
import librarymgmt.model.Client;
import librarymgmt.model.Magazine;
import librarymgmt.model.Newspaper;
import librarymgmt.service.ClientService;
import librarymgmt.service.ItemService;
import librarymgmt.view.MainMenu;

public class Main {
    public static void main(String[] args) {
        ItemService itemService = new ItemService();
        ClientService clientService = new ClientService(itemService);


        try {
            itemService.create(new Book("Title", "Author", 10, 2));
            itemService.create(new Magazine("Magazine", 10, 1));
            itemService.create(new Newspaper("News", "Journal", 3));
            clientService.create(new Client("John", "John@email.com"));
        } catch (ObjectNotValidException e) {
            e.printStackTrace();
        }
        
        MainMenu.show(itemService, clientService);
    }
}
