import java.util.StringTokenizer;

public class Count {
	public static int countWithIndexOf(String sentence, String word) {
		int count = 0;
		int idxFound = sentence.indexOf(word);
		while (idxFound != -1) {
			count++;
			idxFound = sentence.indexOf(word, idxFound + word.length());
		}
		return count;
	}
	
	public static int countWithSplit(String sentence, String word) {
		sentence = " " + sentence + " ";
		String[] parts = sentence.split(word);
		return parts.length - 1;
	}
	
	public static int countWithTokenizer(String sentence, String word) {
		StringTokenizer st = new StringTokenizer(sentence);
		int count = 0;
		while (st.hasMoreTokens()) {
			if (st.nextToken().equals(word)) {
				count++;
			}
		}
		return count	;
	}
	
	public static void main(String[] args) {
		if (args.length !=2) {
			System.out.println("Enter two arguments: sentence then word");
		} else {
			System.out.println("Count with index of:  " + countWithIndexOf(args[0], args[1]));
			System.out.println("Count with split:     " + countWithSplit(args[0], args[1]));
			System.out.println("Count with tokenizer: " + countWithTokenizer(args[0], args[1]));
		}
	}
}
