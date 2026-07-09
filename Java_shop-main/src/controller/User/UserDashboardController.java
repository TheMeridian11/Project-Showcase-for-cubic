package controller.User;

import au.edu.uts.ap.javafx.*;
import javafx.beans.binding.Bindings;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class UserDashboardController extends Controller<Organisation> {

    @FXML private Label userTypeDashboardMessage;
    @FXML private Button userTypeInteractButton;

    public final Organisation getUser() { return model; }
    public final User getUserLoggedIn() {
        return Organisation.getLoggedInUser();
    }
    User user = getUserLoggedIn();

    @FXML private void initialize() {
        if (user instanceof Customer) {
            userTypeDashboardMessage.textProperty().bind(Bindings.concat("Welcome to the Customer Dashboard ", user.getFirstName()));
            userTypeInteractButton.setText("Shop");
        } else if (user instanceof Manager) {
            userTypeDashboardMessage.textProperty().bind(Bindings.concat("Welcome to the Manager Dashboard ", user.getFirstName()));
            userTypeInteractButton.setText("Manage");
        }
    }
    

    @FXML public void viewUserOrderHistory() throws Exception {
        Stage stage = new FixedStage("/image/user_icon.png");
        ViewLoader.showStage(model, "/view/User/OrderHistoryView.fxml", "Order History", stage);
    }

    @FXML public void userTypeInteract() throws Exception {
        if (user instanceof Manager) {
            Stage stage = new FixedStage("/image/supplier_icon.png");
            ViewLoader.showStage(model, "/view/Supplier/SupplierListView.fxml", "Supplier List", stage);
        } else if (user instanceof Customer) {
            Stage stage = new FixedStage("/image/supplier_icon.png");
            ViewLoader.showStage(model, "/view/Supplier/SupplierListView.fxml", "Supplier List", stage);
        }
    }

    @FXML
    public void userExit() {
        stage.close();
    }
}
