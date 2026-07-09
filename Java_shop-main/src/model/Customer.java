package model;

public class Customer extends User{
    
    public Customer(String firstName, String lastName, String username, String password) {
        super(firstName, lastName, username, password);
    }

    @Override
    public String toString() {
        return firstName + " " + lastName;
    }
}
