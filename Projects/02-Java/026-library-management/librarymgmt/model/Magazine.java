package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.MagazineValidator;

public class Magazine extends LibraryItem {
    private int pages;
    private static final MagazineValidator validator = (title, pages) -> {
        if (title == null || title.isBlank()) {
            throw new IllegalArgumentException("Magazine title cannot be empty.");
        } else if (pages <= 0) {
            throw new IllegalArgumentException("Page count must be positive");
        }
    };

    public Magazine(String title, int pages) throws ObjectNotValidException {
        super(title);
        validator.validate(title, pages);
        this.pages = pages;
    }

    public int getPages() { return pages; }

    public void setPages(int newPages) throws ObjectNotValidException { 
        validator.validate(getTitle(), newPages);
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
        sb.append("\n------------------");

        return sb.toString();
    }
}
