package sushibar.model;

import sushibar.SushiBar;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Timer;
import java.util.TimerTask;

public class Clock {
    Timer timer;


    public Clock(int seconds) {
        timer = new Timer();  //At this line a new Thread will be created
        timer.schedule(new RemindTask(), seconds * 1000); //delay in milliseconds
    }

    class RemindTask extends TimerTask {
        public void run() {
            SushiBar.isOpen = false; //prevents creating new customers.
            timer.cancel();
        }
    }

    public static String getTime() {
        // get current time
        Calendar cal = Calendar.getInstance();
        SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss:SS");
        return sdf.format(cal.getTime());
    }

}
