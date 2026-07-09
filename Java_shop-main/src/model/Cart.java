package model;

import java.text.DecimalFormat;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import model.Exceptions.InvalidQuantityException;

public class Cart {
    private Supplier supplier;
    private ObservableList<Product> catalogue;
    private User user;
    private ObservableList<Order> orders;
    
    public Cart(Supplier supplier, User user) {
        this.orders = FXCollections.<Order>observableArrayList();
        this.supplier = supplier;
        this.user = user;
        this.catalogue = FXCollections.<Product>observableArrayList(supplier.getProducts().getAvailableProducts());
    }

    public Supplier getSupplier() {
        return this.supplier;
    }

    public ObservableList<Product> getCatalogue() {
        return this.catalogue;
    }

    public ObservableList<Order> getOrders() {
        return this.orders;
    }

    public User getUser() {
        return this.user;
    }

    public void addOrder(Order o) throws InvalidQuantityException {
        if (!o.getProduct().has(o.getQuantity()) || o.getQuantity() < 1) {
            throw new InvalidQuantityException();
        }
        this.orders.add(o);
    }

    public void removeOrder(Order order) {
        this.orders.remove(order);
    }

    public Order order(Product product) {
        for (Order o : orders) {
            if (o.getProduct().equals(product)) {
                return o;
            }
        }
        return null;
    }

    public double getTotalCost(){
        return user instanceof Manager ? 0 : orders.stream().mapToDouble(x -> x.getProfit()).sum();
    }

    @Override
    public String toString() {
        String s = "Order from ";
        s += this.supplier.getRegion() + ": \n";
        for (Order o : orders) {
            s += o + "\n";
        }
        s += "Total Cost: " + formatted(getTotalCost());
        return s;
    }

    public String formatted(double price) {
        return new DecimalFormat("###,##0.00").format(price);
    }

}
