package nonalphanumeric;

public class Main {
    public static void main(String[] args) {
        String username1 = "johnf";
        String password1 = "john123";
        String message1 = "abcdefghij";
        String username2 = "john@f";
        String password2 = "john@123";
        String message2 = "Hello!";
        
        try {
            Strings.validateUsername(username1);
            System.out.println(username1 + " is a valid username");
        } catch (NonAlphaNumericException e) {
            System.err.println(e.getMessage());
        }

        try {
            Strings.validateUsername(username2);
            System.out.println(username2 + " is a valid username");
        } catch (NonAlphaNumericException e) {
            System.err.println(e.getMessage());
        }

        try {
            Strings.validatePassword(password1);
            System.out.println(password1 + " is a valid password");
        } catch (NonAlphaNumericException e) {
            System.err.println(e.getMessage());
        }

        try {
            Strings.validatePassword(password2);
            System.out.println(password2 + " is a valid password");
        } catch (NonAlphaNumericException e) {
            System.err.println(e.getMessage());
        }

        try {
            Strings.validateString(message1);
            System.out.println(message1 + " is a valid alphanumeric string");
        } catch (NonAlphaNumericException e) {
            System.err.println(e.getMessage());
        }

        try {
            Strings.validateString(message2);
            System.out.println(message2 + " is a valid alphanumeric string");
        } catch (NonAlphaNumericException e) {
            System.err.println(e.getMessage());
        }
    }
}