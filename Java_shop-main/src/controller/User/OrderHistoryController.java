package controller.User;

import au.edu.uts.ap.javafx.*;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class OrderHistoryController extends Controller<User> {

    @FXML private Label userOrderHistoryLabel;
    @FXML private ListView<Cart> userPurchaseHistory;

    public final User getUsers() { return model; }

    @FXML private void initialise() {
        userPurchaseHistory.setItems(getUsers().getPurchaseHistory());
    }


    @FXML
    public void userExit() {
        stage.close();
    }
}

