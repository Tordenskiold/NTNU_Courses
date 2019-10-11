package sushibar;

import sushibar.model.*;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;

public class SushiBar {

    private static int waitingAreaCapacity = 15;
    private static int waitressCount = 8;
    private static int duration = 4;
    public static int maxOrder = 10;
    public static int waitressWait = 50;
    public static int customerWait = 2000;
    public static int doorWait = 100;
    public static boolean isOpen = true;

    private static File log;
    private static String path = "./";

    public static SynchronizedInteger customerCounter;
    public static SynchronizedInteger servedOrders;
    public static SynchronizedInteger takeawayOrders;
    public static SynchronizedInteger totalOrders;


    public static void main(String[] args) {
        log = new File(path + "log.txt");

        customerCounter = new SynchronizedInteger(0);
        totalOrders = new SynchronizedInteger(0);
        servedOrders = new SynchronizedInteger(0);
        takeawayOrders = new SynchronizedInteger(0);

        WaitingArea waitingArea = new WaitingArea(waitingAreaCapacity);
        Clock clock = new Clock(duration);
        Thread doorThread = new Thread(new Door(waitingArea));
        doorThread.start();

        ArrayList<Thread> waitresses = new ArrayList<>();
        for (int i = 0; i < waitressCount; i++) {
            Thread waitressThread = new Thread(new Waitress(waitingArea));
            waitresses.add(waitressThread);
            waitressThread.start();
        }

        try {
            doorThread.join();
            for (int i = 0; i < waitressCount; i++) {
                waitresses.get(i).join();
            }
        }
        catch (InterruptedException e) {
            e.printStackTrace();
        }

        SushiBar.lineBreak();
        SushiBar.write("***** NO MORE CUSTOMERS - THE SHOP IS CLOSED NOW. *****\n");

        SushiBar.write("Total orders: " + SushiBar.totalOrders.get());
        SushiBar.write("Total takeaways: " + SushiBar.takeawayOrders.get());
        SushiBar.write("Total bar orders: " + SushiBar.servedOrders.get());
    }

    public static void write(String str) {
        try {
            FileWriter fw = new FileWriter(log.getAbsoluteFile(), true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write(Clock.getTime() + ", " + str + "\n");
            bw.close();
            System.out.println(Clock.getTime() + ", " + str);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static void lineBreak() {
        try {
            FileWriter fw = new FileWriter(log.getAbsoluteFile(), true);
            BufferedWriter bw = new BufferedWriter(fw);
            bw.write("\n");
            bw.close();
            System.out.println();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
