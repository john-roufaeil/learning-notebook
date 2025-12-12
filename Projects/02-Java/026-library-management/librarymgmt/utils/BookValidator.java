package librarymgmt.utils;

import librarymgmt.exception.ObjectNotValidException;

@FunctionalInterface
public interface BookValidator {
    void validate(String title, String author, int pages) throws ObjectNotValidException;
}