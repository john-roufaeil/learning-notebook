import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.TreeMap;

public class WordDictionary extends TreeMap<Character, List<String>> {

    public void addWord(String word) {
        if (word == null || word.isEmpty()) return;

        char firstLetter = Character.toLowerCase(word.charAt(0));
        List<String> wordsBeginWithLetter = get(firstLetter);

        if (wordsBeginWithLetter == null) {
            wordsBeginWithLetter = new ArrayList<>();
            put(firstLetter, wordsBeginWithLetter);
        }

        wordsBeginWithLetter.add(word.toLowerCase());
        Collections.sort(wordsBeginWithLetter);
    }

    public void printWordsOfLetter(Character letter) {
        letter = Character.toLowerCase(letter);
        char upperLetter = Character.toUpperCase(letter);
        List<String> words = get(letter);
        if (words == null) {
            System.out.println("No words saved begin with the letter " + upperLetter);
        } else {
            System.out.print( upperLetter + ": ");
            for (int i = 0; i < words.size() - 1; i++) {
                System.out.print(words.get(i) + ", ");
            }
            System.out.println(words.get(words.size() - 1));
        }
    }

    public void printAll() {
        System.out.println("\nPrinting dictionary values");
        for (Character letter : keySet()) {
            System.out.print("| ");
            printWordsOfLetter(letter);
        }
        System.out.println("--------------------------\n");
    }
}
