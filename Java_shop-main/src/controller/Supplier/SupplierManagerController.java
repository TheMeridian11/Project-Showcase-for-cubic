package controller.Supplier;

import au.edu.uts.ap.javafx.*;
import javafx.beans.binding.Bindings;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class SupplierManagerController extends Controller<Supplier> {

    @FXML private Label productSelection;
    @FXML private CheckBox productFilter;
    @FXML private TableView<Product> supplierProductsListView;
    @FXML private Button manageButton;
    @FXML private Button orderButton;

    public final Supplier getSupplier() { return model; }

    @FXML private void initialize() {
        productSelection.textProperty().bind(Bindings.concat("Welcome to " + getSupplier().getName() + " (Total Profit: " + getSupplier().getProfit() + ")"));
        
        // Initially display all products
        supplierProductsListView.setItems(getSupplier().getProducts().getAvailableProducts());

        productFilter.selectedProperty().addListener(
            (observable, wasSelected, isSelected) -> {
                if (isSelected) {
                    supplierProductsListView.setItems(getSupplier().getProducts().getAvailableProducts());
                } else {
                    supplierProductsListView.setItems(getSupplier().getProducts().getAllProducts());
                }
            }
        );
    }


    @FXML public void manageProducts() throws Exception {
        Stage stage = new FixedStage("/image/supplier_icon.png");
        ViewLoader.showStage(model, "/view/Supplier/ManageProductsView.fxml", "Managing", stage);
    }

    @FXML public void managerOrder() throws Exception {
        Stage stage = new FixedStage("/image/cart_icon.png");
        ViewLoader.showStage(model, "/view/Cart/CartView.fxml", "Cart", stage);
    }

    @FXML
    public void userExit() {
        stage.close();
    }
}
