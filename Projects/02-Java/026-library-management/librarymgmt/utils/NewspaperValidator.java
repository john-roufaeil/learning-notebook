package librarymgmt.utils;

import librarymgmt.exception.ObjectNotValidException;

@FunctionalInterface
public interface NewspaperValidator {
    void validate(String title, String journal) throws ObjectNotValidException;
}