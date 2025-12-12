package librarymgmt.model;

import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;

public interface CRUDOperations<T> {
    void create(T obj) throws ObjectNotValidException;
    T read(int id) throws ObjectNotFoundException;
    void update(T newObj) throws ObjectNotFoundException;
    void delete(int id) throws ObjectNotFoundException;
}