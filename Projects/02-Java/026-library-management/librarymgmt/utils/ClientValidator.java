package librarymgmt.utils;

import librarymgmt.exception.ObjectNotValidException;

@FunctionalInterface
public interface ClientValidator {
    void validate(String name, String email) throws ObjectNotValidException;
}