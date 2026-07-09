package model;

import java.util.Arrays;
import java.util.LinkedList;

import javafx.collections.FXCollections;
import model.Exceptions.NoSuchUserException;

public class Users {
    private LinkedList<User> users;

    public Users() {
        this.users = new LinkedList<User>();
    }

    public Users(LinkedList<User> initialUsers) {
        this.users = initialUsers;
    }

    public LinkedList<User> getUsers() {
        return this.users;
    }

    public Users seedData(Suppliers stores) {
        this.users.add(new Customer("David", "Dyer", "davey", "pass"));
        this.users.add(new Customer("Aziz", "Shavershian", "1", "1"));
        this.users.add(new Customer("Lee", "Yeoreum", "lee123", "wjsn"));
        this.users.add(new Customer("Kim", "Dahyun", "dah-yun", "twice"));

        

        this.users.add(new Manager("David", "Dyer", "2", "2", FXCollections.<Supplier>observableArrayList(Arrays.asList(
            stores.getByRegion("Penshurst"),
            stores.getByRegion("Hurstville"),
            stores.getByRegion("Allawah"),
            stores.getByRegion("Carlton"),
            stores.getByRegion("Kogarah"),
            stores.getByRegion("Waterfall"),
            stores.getByRegion("Engadine")
        ))));
        this.users.add(new Manager("Big", "Paulie", "paul89", "huge", FXCollections.<Supplier>observableArrayList(Arrays.asList(
            stores.getByRegion("Heathcote"),
            stores.getByRegion("Loftus"),
            stores.getByRegion("Sutherland"),
            stores.getByRegion("Mortdale")
        ))));
        this.users.add(new Manager("Rishik", "Sood", "rishik", "pass", FXCollections.<Supplier>observableArrayList(Arrays.asList(
            stores.getByRegion("Banksia"),
            stores.getByRegion("Arncliffe")
        ))));
        this.users.add(new Manager("Angela", "Huo", "admin", "admin", FXCollections.<Supplier>observableArrayList(Arrays.asList(
            stores.getByRegion("Wolli Creek"),
            stores.getByRegion("Sydenham")
        ))));
        this.users.add(new Manager("Zohair", "Gandhi", "zohair45", "ted", FXCollections.<Supplier>observableArrayList(Arrays.asList(
            stores.getByRegion("Redfern"),
            stores.getByRegion("Central")
        ))));
        return this;
    }

    public User validateUser(String username, String password) throws NoSuchUserException {
        for (User u : this.users) {
            if (username.equals(u.credentials.username) && password.equals(u.credentials.password)) {
                return u;
            }
        }
        throw new NoSuchUserException();
    }
}
