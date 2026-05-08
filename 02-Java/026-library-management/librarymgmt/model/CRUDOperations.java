package librarymgmt.model;

import java.util.ArrayList;
import librarymgmt.exception.ObjectNotFoundException;
import librarymgmt.exception.ObjectNotValidException;

public interface CRUDOperations<T> {
    void add(T obj) throws ObjectNotValidException;
    T read(int id) throws ObjectNotFoundException;
    ArrayList<T> readAll();
    void update(T newObj) throws ObjectNotFoundException;
    void delete(int id) throws ObjectNotFoundException;
    void deleteAll();
}