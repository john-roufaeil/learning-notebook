package librarymgmt.model;

import librarymgmt.exception.ObjectNotValidException;
import librarymgmt.utils.NewspaperValidator;

public class Newspaper extends LibraryItem {
    private String journal;
    private static final NewspaperValidator validator = (title, journal) -> {
        if (title == null || title.isBlank()) {
            throw new IllegalArgumentException("Newspaper title cannot be empty.");
        } else if (journal == null || journal.isBlank()) {
            throw new IllegalArgumentException("Newspaper journal cannot be empty.");
        }
    };

    public Newspaper(String title, String journal) throws ObjectNotValidException {
        super(title);
        validator.validate(title, journal);
        this.journal = journal;
    }

    public String getJournal() { return journal; }

    public void setJournal(String newJournal) throws ObjectNotValidException { 
        validator.validate(getTitle(), newJournal);
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
        sb.append("\n------------------");

        return sb.toString();
    }
}