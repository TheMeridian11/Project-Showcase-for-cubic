package model;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.ReadOnlyIntegerProperty;
import javafx.beans.property.ReadOnlyStringProperty;
import javafx.beans.property.SimpleIntegerProperty;

public class Order{
    private Product product;
    private IntegerProperty quantity;
    private final Cart cart;

    public Order(Product product, int quantity, Cart cart) {
        this.product = product;
        this.quantity = new SimpleIntegerProperty(quantity);
        this.cart = cart;
    }

    public Product getProduct() {
        return product;
    }

    public int getQuantity() {
        return quantity.get();
    }

    public Cart getCart() {
        return this.cart;
    }

    public ReadOnlyStringProperty nameProperty() {
        return this.product.nameProperty();
    }

    public ReadOnlyIntegerProperty quantityProperty() {
        return this.quantity;
    }

    public void setQuantity(int quantity) {
        this.quantity.set(quantity);
    }

    public double getProfit() {
        return product.getPrice() * quantity.get();
    }

    @Override
    public String toString() {
        return product.getName() + " (" + quantity.get() + ")";
    }
}
