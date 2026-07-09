package model;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public abstract class User {

    class Credentials {
        public String username;
        public String password;

        public Credentials(String username, String password) {
            this.username = username;
            this.password = password;
        }
    }

    protected String firstName;
    protected String lastName;
    protected ObservableList<Cart> purchases;
    protected Credentials credentials;

    public User(String firstName, String lastName, String username,  String password) {
        this.firstName = firstName;
        this.lastName = lastName;
        credentials = new Credentials(username, password);
        purchases = FXCollections.<Cart>observableArrayList();
    }

    public String getFirstName() {
        return this.firstName;
    }

    public String getFullName() {
        return firstName + " " + lastName;
    }

    public void addPurchase(Cart cart) {
        purchases.add(cart);
    }

    public ObservableList<Cart> getPurchaseHistory() {
        return this.purchases;
    }

    public abstract String toString();

}
