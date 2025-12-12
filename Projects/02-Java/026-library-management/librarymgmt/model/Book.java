package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.BookValidator;

public class Book extends LibraryItem {
    private String author;
    private int pages;
    private static final BookValidator validator = (title, author, pages, stock) -> {
        if (title == null || title.isBlank()) {
            throw new ObjectNotValidException("Book title cannot be empty.");
        } else if (author == null || author.isBlank()) {
            throw new ObjectNotValidException("Author name cannot be empty.");
        } else if (pages <= 0) {
            throw new ObjectNotValidException("Page count must be positive");
        } else if (stock <= 0) {
            throw new ObjectNotValidException("Stock must be positive");
        }
    };

    public Book(String title, String author, int pages, int stock) throws ObjectNotValidException {
        super(title, stock);
        this.author = author;
        this.pages = pages;
    }

    public static Book create(String title, String author, int pages, int stock) 
        throws ObjectNotValidException {
        validator.validate(title, author, pages, stock);
        return new Book(title, author, pages, stock);
    }

    public String getAuthor() { return author; }
    public int getPages() { return pages; }

    public void setAuthor(String newAuthor) throws ObjectNotValidException {
        validator.validate(getTitle(), newAuthor, pages, getStock());
        author = newAuthor;
    }

    public void setPages(int newPages) throws ObjectNotValidException { 
        validator.validate(getTitle(), author, newPages, getStock());
        pages = newPages;
    }

    @Override
    public String getItemDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("| Item #").append(getId()).append(" Details");
        sb.append("\n| Type: Book");
        sb.append("\n| Title: ").append(getTitle());
        sb.append("\n| Author: ").append(author);
        sb.append("\n| Pages: ").append(pages);
        sb.append(isAvailable()? "\n| Available" :"\n| Borrowed");
        sb.append("\n+-----------------");
        return sb.toString();
    }
}
