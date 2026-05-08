import javax.swing.JFrame;
import javax.swing.JLabel;

public class TextBannerApp extends JFrame implements Runnable {
    Thread thread;
    JLabel marquee = new JLabel("Hello, World!");

    public TextBannerApp() {
        setTitle("Text Banner Java Application");

        setLayout(null);
        marquee.setBounds(0, 100, 100, 30);
        add(marquee);

        setBounds(50,50,600,400);
        setVisible(true);

        thread = new Thread(this);
        thread.start();
    }

    public void run() {
        while (true) { 
            int x = marquee.getX() + 1;

            if (x >= getWidth()) {
                x = -marquee.getWidth();
            }

            marquee.setLocation(x, marquee.getY());

            try {
                Thread.sleep(10); 
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        TextBannerApp app = new TextBannerApp();
    }
}
