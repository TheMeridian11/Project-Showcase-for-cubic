package controller.Supplier;

import au.edu.uts.ap.javafx.*;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import model.*;
import model.Exceptions.*;

import java.util.Observable;
import java.util.logging.*;

public class ManageProductsController extends Controller<Supplier> {

    @FXML private Label editSupplierLabel;
    @FXML private ListView<Product> productsListView;
    @FXML private Button removeButton;
    @FXML private Button delistButton;

    public final Supplier getSupplier() { return model; }

    @FXML private void initialize() {
        productsListView.setItems(getSupplier().getProducts().getAllProducts());
        
        productsListView.getSelectionModel().selectedItemProperty().addListener(
            (observable, oldItem, newItem) -> {
                if (newItem != null) {
                    removeButton.setDisable(false);
                    delistButton.setDisable(!newItem.isAvailable());
                } else {
                    delistButton.setDisable(true);
                    removeButton.setDisable(true);
                }
            }
        );

    }

    public Product productSelection() {
        Product selectedProduct = productsListView.getSelectionModel().getSelectedItem();
        return selectedProduct;
    }

    @FXML public void removeProduct() {
        Product selectedProduct = productSelection();
        if (selectedProduct != null) {
            getSupplier().getProducts().removeProduct(selectedProduct);
        }
    } 

    @FXML public void delistProduct() {
        Product selectedProduct = productSelection();
        if (selectedProduct != null) {
            selectedProduct.delist();
            delistButton.setDisable(true);
        }
    }

    @FXML
    public void userExit() {
        stage.close();
    }
}
