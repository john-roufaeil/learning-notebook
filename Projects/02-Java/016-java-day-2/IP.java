import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class IP {
	public static void main(String[] args) {
		String ip = args[0];
		String reg = "-?(\\d\\d\\d).(\\d\\d\\d).(\\d\\d\\d).(\\d\\d\\d)";
		if (ip.matches(reg)) {
			Pattern pattern = Pattern.compile(reg);
			Matcher matcher = pattern.matcher(ip);
			if (matcher.find()) {
			    System.out.println("octet 1: " + matcher.group(1));
			    System.out.println("octet 2: " + matcher.group(2));
			    System.out.println("octet 3: " + matcher.group(3));
			    System.out.println("octet 4: " + matcher.group(4));
			}
		} else {
			System.out.println("You entered an incorrect IP address");
		}
	}
}
