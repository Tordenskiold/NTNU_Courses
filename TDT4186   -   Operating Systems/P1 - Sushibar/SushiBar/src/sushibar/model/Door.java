package sushibar.model;

import sushibar.SushiBar;

public class Door implements Runnable {

    WaitingArea waitingArea;

    public Door(WaitingArea waitingArea) {
        this.waitingArea = waitingArea;
    }

    @Override
    public void run() {
        while (SushiBar.isOpen) {
            waitingArea.enter(new Customer());
            try {
                Thread.sleep(SushiBar.doorWait);
            }
            catch (InterruptedException e) {

            }
            synchronized (waitingArea) {
                waitingArea.notifyAll();
            }
        }
    }

}