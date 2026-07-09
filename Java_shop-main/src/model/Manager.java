package model;

import javafx.collections.ObservableList;

public class Manager extends User{
    
    private Suppliers suppliers;

    public Manager(String firstName, String lastName, String username, String password, ObservableList<Supplier> stores) {
        super(firstName, lastName, username, password);
        this.suppliers = new Suppliers(stores);
    }

    public Suppliers getSuppliers() {
        return this.suppliers;
    }

    @Override
    public String toString() {
        return firstName + " " + lastName + ", manager for: " + suppliers.toString();
    }
}
