package model;

import java.text.DecimalFormat;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.ReadOnlyDoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.collections.ObservableList;

public class Supplier {

    private String name;
    private String region;
    private String address;
    private Products products;
    private DoubleProperty profit;

    public Supplier(String name, String region, String address, ObservableList<Product> products) {
        this.name = name;
        this.region = region;
        this.address = address;
        this.products = new Products(products);
        this.profit = new SimpleDoubleProperty(0);
    }
    
    public String getName() {
        return this.name;
    }

    public String getRegion() {
        return this.region;
    }

    public Products getProducts() {
        return this.products;
    }
    
    public String getProfit() {
        return formatted(profit.get());
    }

    public ReadOnlyDoubleProperty profitProperty() {
        return profit;
    }

    public void processCart(Cart cart) {
        for (Order order : cart.getOrders()) {
            order.getProduct().sell(order.getQuantity());    
        }
        this.profit.set(profit.get() + cart.getTotalCost());
    }

    public Product product(String name) {
        for (Product p : products.getAllProducts()) {
            if (p.getName().equals(name)) {
                return p;
            }
        }
        return null;
    }

    @Override
    public String toString() {
        return name + " (" + region + "), " + address;
    }

    public String formatted(double price) {
        return new DecimalFormat("###,##0.00").format(price);
    }

}
