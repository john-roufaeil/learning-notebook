package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.BookValidator;

public class Book extends LibraryItem {
    private String author;
    private int pages;
    private static final BookValidator validator = (title, author, pages) -> {
        if (title == null || title.isBlank()) {
            throw new IllegalArgumentException("Book title cannot be empty.");
        } else if (author == null || author.isBlank()) {
            throw new IllegalArgumentException("Author name cannot be empty.");
        } else if (pages <= 0) {
            throw new IllegalArgumentException("Page count must be positive");
        }
    };

    public Book(String title, String author, int pages) throws ObjectNotValidException {
        super(title);
        validator.validate(title, author, pages);
        this.author = author;
        this.pages = pages;
    }

    public String getAuthor() { return author; }
    public int getPages() { return pages; }

    public void setAuthor(String newAuthor) throws ObjectNotValidException {
        validator.validate(getTitle(), newAuthor, pages);
        author = newAuthor;
    }

    public void setPages(int newPages) throws ObjectNotValidException { 
        validator.validate(getTitle(), author, newPages);
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
        sb.append("\n------------------");
        return sb.toString();
    }
}
