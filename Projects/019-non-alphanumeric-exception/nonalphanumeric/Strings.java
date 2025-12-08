package nonalphanumeric;

public class Strings {

    public static void validate(String input, String message) throws NonAlphaNumericException {
        if (!input.matches("[A-Za-z0-9]+")) {
            throw new NonAlphaNumericException(message);
        }
    }

    public static void validateUsername(String username) throws NonAlphaNumericException {
        validate(username, "Error: Username must be alphanumeric. Input: " + username);
    }

    public static void validatePassword(String password) throws NonAlphaNumericException {
        validate(password, "Error: Password must be alphanumeric. Input: " + password);
    }

    public static void validateString(String input) throws NonAlphaNumericException {
        validate(input, "Error: Input contains non-alphanumeric characters. Input: " + input);
    }
}