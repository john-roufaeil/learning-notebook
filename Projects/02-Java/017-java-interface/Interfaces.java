import java.util.function.Function;

public class Interfaces {
    public static void main(String[] args) {
        
        Function<Double, Double> convertCelToFahr =
                cel -> (cel * 9/5) + 32;

        Function<Double[], Double[]> roots = arr -> {
            double a = arr[0];
            double b = arr[1];
            double c = arr[2];

            double pos = (-b + Math.sqrt(b*b - 4*a*c)) / (2 * a);
            double neg = (-b - Math.sqrt(b*b - 4*a*c)) / (2 * a);
            return new Double[]{pos, neg};
        };

        System.out.println(convertCelToFahr.apply(37.0));
        System.out.println(roots.apply(new Double[]{1.0, -3.0, 2.0})[0]);
        System.out.println(roots.apply(new Double[]{1.0, -3.0, 2.0})[1]);
    }
}
