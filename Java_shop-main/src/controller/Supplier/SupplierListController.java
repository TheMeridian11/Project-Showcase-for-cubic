package controller.Supplier;

import au.edu.uts.ap.javafx.*;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class SupplierListController extends Controller<Organisation> {
    
    @FXML private Label supplierSelectLabel;
    @FXML private ListView<Supplier> supplierListView;
    @FXML private Button shopButton;

    public final Organisation getSuppliersList() { return model; }

    public final User getUserLoggedIn() {
        return Organisation.getLoggedInUser();
    }
    User user = getUserLoggedIn();

    @FXML private void initialize() {
        shopButton.setDisable(true);
        
        if (user instanceof Manager) {
            Manager userManager = (Manager) user;
            supplierListView.setItems(userManager.getSuppliers().getSuppliers());
        } else if (user instanceof Customer) {
            supplierListView.setItems(getSuppliersList().getSuppliers().getSuppliers());
        }

        supplierListView.getSelectionModel().selectedItemProperty().addListener(
            (observable, oldItem, newItem) -> {
                shopButton.setDisable(newItem == null);
            }
        );

    }

    // this method obtains the supplier that the user has selected and wants to buy/manage from
    public Supplier supplierSelection() {
        Supplier selectedSupplier = supplierListView.getSelectionModel().getSelectedItem();
        return selectedSupplier;
    }

    public String supplierTitle() {
        return supplierSelection().getRegion();
    }

    

    @FXML public void userShop() throws Exception{
        if (user instanceof Manager) {
            Stage stage = new FixedStage("/image/supplier_icon.png");
            ViewLoader.showStage(supplierSelection(), "/view/Supplier/SupplierManagerView.fxml", "Supplier: " + supplierTitle(), stage);
        } else if (user instanceof Customer) {
            Stage stage = new FixedStage("/image/supplier_icon.png");
            ViewLoader.showStage(supplierSelection(), "/view/Supplier/SupplierCustomerView.fxml", "Supplier: " + supplierTitle(), stage);
        }
        stage.close();
    }

    @FXML
    public void userExit() {
        stage.close();
    }
}
