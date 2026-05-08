public class Main {
    public static void main(String[] args) {
        Complex<Integer> c1 = new Complex<>(3, -2);
        System.out.print("Complex 1 (integer): ");
        c1.print();

        Complex<Float> c2 = new Complex<>(1f, -1f);
        System.out.print("Complex 2 (float): ");
        c2.print();
        
        Complex<Double> c3 = new Complex<>(5.0, 5.0);
        System.out.print("Complex 3 (double): ");
        c3.print();
        
        Complex<Double> c4 = c1.sub(c2);
        System.out.print("Complex 4 (double): ");
        c4.print();
        Complex<Double> c5 = c1.sub(c3);
        System.out.print("Complex 5 (double): ");
        c5.print();
        
        Complex<Double> c6 = c2.sub(c1);
        System.out.print("Complex 6 (double): ");
        c6.print();
        Complex<Double> c7 = c2.sub(c3);
        System.out.print("Complex 7 (double): ");
        c7.print();
        
        Complex<Double> c8 = c3.sub(c1);
        System.out.print("Complex 8 (double): ");
        c8.print();
        Complex<Double> c9 = c3.sub(c2);
        System.out.print("Complex 9 (double): ");
        c9.print();
    }
}