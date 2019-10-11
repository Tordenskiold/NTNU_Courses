package sushibar.model;

import sushibar.SushiBar;

public class Waitress implements Runnable {

    private WaitingArea waitingArea;
    private Customer customer;

    public Waitress(WaitingArea waitingArea) {
        this.waitingArea = waitingArea;
    }

    @Override
    public void run() {
        while (SushiBar.isOpen || !waitingArea.isEmpty()) {
            customer = waitingArea.next();
            if (customer != null) {
                SushiBar.write(Thread.currentThread().getName() + ": Customer #" + customer.getCustomerID() + " is now fetched.");
                try {
                    Thread.sleep(SushiBar.waitressWait);
                }
                catch (InterruptedException e) {

                }
                SushiBar.write(Thread.currentThread().getName() + ": Customer #" + customer.getCustomerID() + " is now eating.");
                customer.order();
                try {
                    Thread.sleep(customer.getOrdersBar() * SushiBar.customerWait);
                }
                catch (InterruptedException e) {

                }
                SushiBar.write(Thread.currentThread().getName() + ": Customer #" + customer.getCustomerID() + " is now leaving.");
            }
        }
    }

}