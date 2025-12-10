package shapes;
import java.util.ArrayList;

public class Main {
    public static void main(String[] args) {
        ArrayList<Shape> shapes = new ArrayList<>();
        shapes.add(new Rectangle());
        shapes.add(new Rectangle());
        shapes.add(new Circle());
        shapes.add(new Circle());

        ArrayList<Rectangle> rects = new ArrayList<>();
        rects.add(new Rectangle());
        rects.add(new Rectangle());

        Test.testDraw(shapes);
        Test.testDraw(rects);
    }
}