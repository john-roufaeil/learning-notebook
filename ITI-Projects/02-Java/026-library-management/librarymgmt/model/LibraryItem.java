package librarymgmt.model;

import librarymgmt.exception.ItemNotAvailableException;

public abstract class LibraryItem {
    private static int itemsCount = 0;
    private final int id;
    private String title;
    private int stock;

    public LibraryItem(String title, int stock) {
        id = ++itemsCount;
        this.title = title;
        this.stock = stock;
    }

    public int getId() { return id; }
    public String getTitle() { return title; }
    public int getStock() { return stock; }
    public boolean isAvailable() { return stock > 0; }

    public void setTitle(String newTitle) { title = newTitle; }
    public void setStock(int stock) { this.stock = stock; }
    public void incrementStock() { stock++; }
    public void decrementStock() throws ItemNotAvailableException {
        if (stock <= 0) {
            throw new ItemNotAvailableException("Item not available for borrowing.");
        }
        stock--;
    }

    public abstract String getItemDetails();

    @Override
    public String toString() {
        return "Item ID: " + id + ", Title: " + title + ", Stock: " + stock;
    }
}
