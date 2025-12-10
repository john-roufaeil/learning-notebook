package shapes;
import java.util.ArrayList;

public class Test {
    public static <T extends Shape> void testDraw(ArrayList<T> shapes) {
        for (int i = 0; i < shapes.size(); i++) {
            shapes.get(i).draw();
        }
    }
}