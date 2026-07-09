package model;


public class Organisation {
    private Suppliers suppliers;
    private Users users;
    private static User loggedInUser;

    public Organisation(Suppliers suppliers, Users users) {
        this.suppliers = suppliers;
        this.users = users;
    }

    public Suppliers getSuppliers() {
        return suppliers;
    }

    public Users getUsers() {
        return this.users;
    }

    public static User getLoggedInUser() {
        return loggedInUser;
    }

    public static void setLoggedInUser(User user) {
        loggedInUser = user;
    }
}
