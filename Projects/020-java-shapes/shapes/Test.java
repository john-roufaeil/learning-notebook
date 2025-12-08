package shapes;
import java.util.ArrayList;

public class Test {
    public static void testDraw(ArrayList<? extends Shape> shapes) {
        for (int i = 0; i < shapes.size(); i++) {
            shapes.get(i).draw();
        }
    }
}