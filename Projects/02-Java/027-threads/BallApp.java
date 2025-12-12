import java.awt.*;
import java.util.Random;
import javax.swing.*;

public class BallApp extends JPanel implements Runnable {

    private int x, y;
    private int dx, dy;

    private static final int BALL_SIZE = 40;
    private final Random rand = new Random();

    public BallApp() {
        setBackground(Color.BLACK);

        x = rand.nextInt(300);
        y = rand.nextInt(200);

        dx = rand.nextBoolean() ? 3 : -3;
        dy = rand.nextBoolean() ? 4 : -4;

        new Thread(this).start();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        g.setColor(Color.GREEN);
        g.fillOval(x, y, BALL_SIZE, BALL_SIZE);
    }

    @Override
    public void run() {
        while (true) {
            x += dx;
            y += dy;

            if (x <= 0 || x >= getWidth() - BALL_SIZE) dx = -dx;
            if (y <= 0 || y >= getHeight() - BALL_SIZE) dy = -dy;

            repaint();

            try {
                Thread.sleep(20);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                break;
            }
        }
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Bouncing Ball");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(500, 400);
        frame.setLocationRelativeTo(null);
        frame.add(new BallApp());
        frame.setVisible(true);
    }
}
