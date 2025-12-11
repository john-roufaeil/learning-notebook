package librarymgmt.model;

public class Magazine extends LibraryItem {
    private int pages;

    public Magazine(String title, int pages) {
        super(title);
        this.pages = pages;
    }

    public int getPages() { return pages; }

    public void setPages(int pages) throws IllegalArgumentException { 
        if (pages <= 0) {
            throw new IllegalArgumentException(
                "Magazine pages count cannot be non positive"
            );
        }
        this.pages = pages;
    }

    @Override
    public String getItemDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("\n__________________");
        sb.append("\n| Magazine #").append(getId()).append(" Details");
        sb.append("\n| Title: ").append(getTitle());
        sb.append("\n| Pages: ").append(pages);
        sb.append(isAvailable()? "\n| Available" :"\n| Borrowed");
        sb.append("\n__________________\n");

        return sb.toString();
    }

    // @Override
    // public void markAsBorrowed() {
    //     setAvailable(false);
    // }
    
    // @Override
    // public void markAsReturned() {
    //     setAvailable(true);
    // }
}