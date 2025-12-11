package librarymgmt.model;

public abstract class LibraryItem {
    private static int itemsCount = 1;
    private final int id;
    private String title;
    private boolean available;

    public LibraryItem(String title) {
        id = itemsCount + 1;
        this.title = title;
        available = true;
        itemsCount++;
    }

    public int getId() { return id; }

    public String getTitle() { return title; }

    public boolean isAvailable() { return available; }

    public void setTitle(String newTitle) { title = newTitle; }

    public void setAvailable(boolean available) { this.available = available; }

    public abstract String getItemDetails();

    @Override
    public String toString() {
        return "Item ID: " + id + ", Title: " + title + (available ? ", available.\n" : ", borrowed.\n");
    }
}