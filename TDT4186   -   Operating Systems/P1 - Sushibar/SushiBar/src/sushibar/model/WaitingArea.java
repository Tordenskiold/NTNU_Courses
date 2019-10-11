package sushibar.model;

import sushibar.SushiBar;

import java.util.LinkedList;
import java.util.Queue;

public class WaitingArea {

    private static int capacity;
    private Queue<Customer> customers;

    public WaitingArea(int size) {
        // TODO Implement required functionality
        capacity = size;
        customers = new LinkedList<Customer>();
    }

    public synchronized void enter(Customer customer) {
        while (customers.size() >= capacity)
            try {
                this.wait();
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        if (SushiBar.isOpen) {
            this.notifyAll();
            customers.add(customer);
            SushiBar.write(Thread.currentThread().getName() + ": Customer #" + customer.getCustomerID() + " is now waiting.");
        }
    }

    public synchronized Customer next() {
        while (SushiBar.isOpen && customers.isEmpty())
            try {
                this.wait();
            }
            catch (InterruptedException e) {
                e.printStackTrace();
            }
        if (!customers.isEmpty()) {
            this.notifyAll();
            return customers.remove();
        }
        return null;
    }

    public boolean isEmpty() {
        return customers.isEmpty();
    }

}
