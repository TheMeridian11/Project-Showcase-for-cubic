package model;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class Products {
    private ObservableList<Product> products;

    public Products() {
        this.products = FXCollections.<Product>observableArrayList();
    }

    public Products(ObservableList<Product> initialProducts) {
        this.products = initialProducts;
    }

    public ObservableList<Product> getAllProducts() {
        return this.products;
    }

    //Yes, this returns a copy. This shouldn't matter, as you should only be manipulating the total products list
    public ObservableList<Product> getAvailableProducts() {
        ObservableList<Product> availableProducts = FXCollections.<Product>observableArrayList();
        products.forEach((prod) -> {if (prod.isAvailable()) availableProducts.add(prod);});
        return availableProducts;
    }

    public void removeProduct(Product product){
        this.products.remove(product);
    }

    public void clearProducts() {
        this.products.clear();
    }
}
