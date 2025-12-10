public class App {
    public static void main(String[] args) {
        WordDictionary dict = new WordDictionary();
        dict.addWord("Alphabet");
        dict.addWord("Apple");
        dict.addWord("AB");
        dict.addWord("aab");
        dict.addWord("Boy");
        dict.addWord("Banana");
        dict.addWord("Camel");
        dict.addWord("Zambia");
        dict.addWord("Nicaragua");

        dict.printAll();
        
        dict.printWordsOfLetter('a');
        dict.printWordsOfLetter('F');
    }
}