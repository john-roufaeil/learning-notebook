package librarymgmt.model;

public class Newspaper extends LibraryItem {
    private String journal;

    public Newspaper(String title, String journal) {
        super(title);
        this.journal = journal;
    }

    public String getJournal() { return journal; }

    public void setJournal(String journal) { 
        this.journal = journal;
    }

    @Override
    public String getItemDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("\n__________________");
        sb.append("\n| Newspaper #").append(getId()).append(" Details");
        sb.append("\n| Title: ").append(getTitle());
        sb.append("\n| Journal: ").append(journal);
        sb.append(isAvailable()? "\n| Available" :"\n| Borrowed");
        sb.append("\n__________________\n");

        return sb.toString();
    }
}