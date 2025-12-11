package librarymgmt.model;

public class Book extends LibraryItem {
    private String author;
    private int pages;

    public Book(String title, String author, int pages) {
        super(title);
        this.author = author;
        this.pages = pages;
    }

    public String getAuthor() { return author; }

    public int getPages() { return pages; }

    public void setAuthor(String newAuthor) { author = newAuthor; }

    public void setPages(int newPages) { pages = newPages; }

    @Override
    public String getItemDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("\n__________________");
        sb.append("\n| Book #").append(getId()).append(" Details");
        sb.append("\n| Title: ").append(getTitle());
        sb.append("\n| Author: ").append(author);
        sb.append("\n| Pages: ").append(pages);
        sb.append(isAvailable()? "\n| Available" :"\n| Borrowed");
        sb.append("\n__________________\n");

        return sb.toString();
    }
}