package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.NewspaperValidator;

public class Newspaper extends LibraryItem {
    private String journal;
    private static final NewspaperValidator validator = (title, journal, stock) -> {
        if (title == null || title.isBlank()) {
            throw new ObjectNotValidException("Newspaper title cannot be empty.");
        } else if (journal == null || journal.isBlank()) {
            throw new ObjectNotValidException("Newspaper journal cannot be empty.");
        } else if (stock <= 0) {
            throw new ObjectNotValidException("Stock must be positive");
        }
    };

    public Newspaper(String title, String journal, int stock) throws ObjectNotValidException {
        super(title, stock);
        this.journal = journal;
    }

    public static Newspaper create(String title, String journal, int stock) 
        throws ObjectNotValidException {
        validator.validate(title, journal, stock);
        return new Newspaper(title, journal, stock);
    }

    public String getJournal() { return journal; }

    public void setJournal(String newJournal) throws ObjectNotValidException { 
        validator.validate(getTitle(), newJournal, getStock());
        journal = newJournal;
    }

    @Override
    public String getItemDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("| Item #").append(getId()).append(" Details");
        sb.append("\n| Type: Newspaper");
        sb.append("\n| Title: ").append(getTitle());
        sb.append("\n| Journal: ").append(journal);
        sb.append(isAvailable()? "\n| Available" :"\n| Borrowed");
        sb.append("\n+-----------------");

        return sb.toString();
    }
}