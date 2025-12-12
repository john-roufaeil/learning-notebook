package librarymgmt.utils;

import librarymgmt.exception.ObjectNotValidException;

@FunctionalInterface
public interface MagazineValidator {
    void validate(String title, int pages) throws ObjectNotValidException;
}