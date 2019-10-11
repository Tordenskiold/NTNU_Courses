package sushibar.model;

import sushibar.SushiBar;

import java.util.concurrent.ThreadLocalRandom;

public class Customer {

    private int id;
    private int ordersBar;

    public Customer() {
        id = SushiBar.customerCounter.get();
        SushiBar.customerCounter.increment();
    }

    public synchronized void order(){
        ordersBar = ThreadLocalRandom.current().nextInt(0, SushiBar.maxOrder + 1);
        int ordersTakeAway = ThreadLocalRandom.current().nextInt((ordersBar == 0 ? 1 : 0), SushiBar.maxOrder - ordersBar + 1);
        addOrders(ordersBar, ordersTakeAway);
    }

    public int getCustomerID() {
        return id;
    }

    public int getOrdersBar() {
        return ordersBar;
    }

    private void addOrders(int bar, int takeAway) {
        SushiBar.servedOrders.add(bar);
        SushiBar.takeawayOrders.add(takeAway);
        SushiBar.totalOrders.add(bar + takeAway);
    }

}
