import java.awt.BorderLayout;
import java.util.Date;
import javax.swing.JFrame;
import javax.swing.JLabel;

public class DateTimeApp extends JFrame implements Runnable {
    Thread thread;
    Date date = new Date();     
    JLabel timeLabel = new JLabel();

    public DateTimeApp() {
        setTitle("DateTime Java Application");
        timeLabel.setHorizontalAlignment(JLabel.CENTER);
        timeLabel.setVerticalAlignment(JLabel.CENTER);
        add(timeLabel, BorderLayout.CENTER);
        thread = new Thread(this);
        thread.start();
    }

    public void run() {
        while (true) { 
            date = new Date();
            timeLabel.setText(date.toString());
            try {
                Thread.sleep(1000); 
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        DateTimeApp app = new DateTimeApp();
        app.setBounds(50,50,600,400);
        app.setVisible(true);
    }
}
