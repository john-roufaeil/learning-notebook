
import java.util.Scanner;

public class IsOnlyAlphabet {
    public static void main(String[] args) throws IllegalArgumentException {
        System.out.print("Enter your string: ");
        Scanner scanner = new Scanner(System.in);
        String input = scanner.nextLine();

        try {
            for (int i = 0; i < input.length(); i++) {
                if (!Character.isLetter(input.charAt(i))) {
                    throw new IllegalArgumentException("The input string \"" + input + "\" is not alphabetic :(");
                }        
            }
            System.out.println("The input string \"" + input + "\" is alphabetic :)");
        } catch (IllegalArgumentException e) {
            System.err.println(e.getMessage());
        }
    }
}