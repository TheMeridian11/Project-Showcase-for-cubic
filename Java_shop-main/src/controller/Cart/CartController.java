package controller.Cart;

import au.edu.uts.ap.javafx.*;
import javafx.beans.binding.Bindings;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.*;
import model.Exceptions.*;

import java.util.Observable;
import java.util.logging.*;

public class CartController extends Controller<Supplier> {

    @FXML private Label productSelection;
    @FXML private TableView<Product> supplierProductsListView;
    @FXML private Button addButton;
    @FXML private Button viewButton;
    @FXML private Button checkoutButton;

    public final Supplier getSupplier() { return model; }

    @FXML private void initialize() {
        productSelection.textProperty().bind(Bindings.concat("Ordering from " + getSupplier().getName()));
        supplierProductsListView.getSelectionModel().setSelectionMode(SelectionMode.MULTIPLE);
        supplierProductsListView.setItems(getSupplier().getProducts().getAvailableProducts());

    }


    @FXML public void addProduct() throws Exception {
        
    }

    @FXML public void viewCart() {

    }

    @FXML public void checkoutOrder() {

    }

    @FXML
    public void userExit() {
        stage.close();
    }

}

