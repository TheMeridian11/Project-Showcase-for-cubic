package model;

import java.text.DecimalFormat;
import javafx.beans.property.*;

public class Product {
    private StringProperty name;
    private DoubleProperty price;
    private IntegerProperty stock;
    private BooleanProperty available;

    public Product(String name, double price, int initialStock) {
        this.name = new SimpleStringProperty(name);
        this.price = new SimpleDoubleProperty(price);
        this.stock = new SimpleIntegerProperty(initialStock);
        available = new SimpleBooleanProperty(true);
    }

    public String getName() {
        return name.get();
    }

    public double getPrice() {
        return price.get();
    }

    public double getStock() {
        return stock.get();
    }

    public ReadOnlyStringProperty nameProperty() {
        return this.name;
    }

    public ReadOnlyDoubleProperty priceProperty() {
        return this.price;
    }

    public ReadOnlyIntegerProperty stockProperty() {
        return this.stock;
    }
    
    public boolean isAvailable() {
        return available.get();
    }

    public boolean has(int stock) {
        return this.stock.get() >= stock;
    }

    public double sell(int amount) {
        stock.set(stock.get() - amount);
        return amount * price.get();
    }

    public void delist() {
        available.set(false);
    }

    @Override
    public String toString() {
        return name.get() + " at $" + formatted(price.get()) + " (" + stock.get() + ")";
    }

    public String formatted(double price) {
        return new DecimalFormat("###,##0.00").format(price);
    }
}
