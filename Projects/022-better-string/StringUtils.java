public class StringUtils {
    
    @FunctionalInterface
    public interface Tester<T> {
        public Boolean test(T item1, T item2);
    }

    public static String betterString(String s1, String s2, Tester<String> condition) {
        if (condition.test(s1, s2)) {
            return s1;
        } else {
            return s2;
        }
    }

    public static void main(String[] args) {
        String string1 = "short";
        String string2 = "loooooooooong";

        String longer = StringUtils.betterString(string1, string2, (s1, s2) -> s1.length() > s2.length());
        String first = StringUtils.betterString(string1, string2, (s1, s2) -> true);
        String startsWithL = StringUtils.betterString(string1, string2, (s1, s2) -> s1.startsWith("l"));
        String startsWithS = StringUtils.betterString(string1, string2, (s1, s2) -> s1.startsWith("s"));

        System.out.println("The longer string is \"" + longer + "\"");
        System.out.println("The first string is \"" + first + "\"");
        System.out.println("The startsWithL string is \"" + startsWithL + "\"");
        System.out.println("The startsWithS string is \"" + startsWithS + "\"");
    }
}