package controller.Supplier;

import au.edu.uts.ap.javafx.*;
import javafx.beans.binding.Bindings;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class SupplierCustomerController extends Controller<Supplier> {

    @FXML private Label productSelection;
    @FXML private TableView<Product> supplierProductsListView;
    @FXML private Button orderButton;

    public final Supplier getSupplier() { return model; }

    @FXML private void initialize() {
        productSelection.textProperty().bind(Bindings.concat("Welcome to " + getSupplier().getName() + " (Total Profit: " + getSupplier().getProfit() + ")"));
        
        // Initially display all products
        supplierProductsListView.setItems(getSupplier().getProducts().getAvailableProducts());
        supplierProductsListView.setSelectionModel(null);

    }

    @FXML public void customerOrder() throws Exception {
        Stage stage = new FixedStage("/image/cart_icon.png");
        ViewLoader.showStage(model, "/view/Cart/CartView.fxml", "Cart", stage);
    }

    @FXML
    public void userExit() {
        stage.close();
    }

}

