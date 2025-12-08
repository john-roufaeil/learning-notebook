package nonalphanumeric;

public class NonAlphaNumericException extends Exception {
    public NonAlphaNumericException() {}

    public NonAlphaNumericException(String message) {
        super(message);
    }
}