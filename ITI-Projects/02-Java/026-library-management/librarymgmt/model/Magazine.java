package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.MagazineValidator;

public class Magazine extends LibraryItem {
    private int pages;
    private static final MagazineValidator validator = (title, pages, stock) -> {
        if (title == null || title.isBlank()) {
            throw new ObjectNotValidException("Magazine title cannot be empty.");
        } else if (pages <= 0) {
            throw new ObjectNotValidException("Page count must be positive");
        } else if (stock <= 0) {
            throw new ObjectNotValidException("Stock must be positive");
        }
    };

    public Magazine(String title, int pages, int stock) throws ObjectNotValidException {
        super(title, stock);
        this.pages = pages;
    }

    public static Magazine create(String title, int pages, int stock) 
        throws ObjectNotValidException {
        validator.validate(title, pages, stock);
        return new Magazine(title, pages, stock);
    }

    public int getPages() { return pages; }

    public void setPages(int newPages) throws ObjectNotValidException { 
        validator.validate(getTitle(), newPages, getStock());
        pages = newPages;
    }

    @Override
    public String getItemDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("| Item #").append(getId()).append(" Details");
        sb.append("\n| Type: Magazine");
        sb.append("\n| Title: ").append(getTitle());
        sb.append("\n| Pages: ").append(pages);
        sb.append(isAvailable()? "\n| Available" :"\n| Borrowed");
        sb.append("\n+-----------------");

        return sb.toString();
    }
}
