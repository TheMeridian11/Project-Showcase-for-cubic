package controller;

import au.edu.uts.ap.javafx.*;
import javafx.beans.binding.BooleanBinding;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.stage.Stage;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class LoginController extends Controller<Organisation>{

    @FXML private TextField userInput;
    @FXML private PasswordField passInput;
    @FXML private Button loginButton;

    public final Organisation getOrganisation() { return model; }

    // Initialiser method that allows for FX:ID's in FXML file to be initialised and allow for functioning of App
    @FXML private void initialize() {
        loginButton.setDisable(true);
        
        userInput.textProperty().addListener(
            (observable, oldText, newTxt) -> {
                if (userInput.getText().isEmpty() == true || passInput.getText().isEmpty() == true) {
                    loginButton.setDisable(true);
                } else {
                    loginButton.setDisable(false);
                }
            }
        );
        passInput.textProperty().addListener(
            (obserable, oldPass, newPass) -> {
                if (userInput.getText().isEmpty() == true || passInput.getText().isEmpty() == true) {
                    loginButton.setDisable(true);
                } else {
                    loginButton.setDisable(false);
                }
            }
        );
    }


    // This FXML event handler, allows us to login to the application through either as a customer or a manager
    @FXML private void userLogin() throws Exception {
        try {
            User user = getOrganisation().getUsers().validateUser(userInput.getText(), passInput.getText());
            Organisation.setLoggedInUser(user);
            if (user instanceof Customer) {
                Stage stage = new FixedStage("/image/user_icon.png");
                ViewLoader.showStage(getOrganisation(), "/view/User/UserDashboardView.fxml", "Customer Dashboard", stage);
            } else if (user instanceof Manager) {
                Stage stage = new FixedStage("/image/user_icon.png");
                ViewLoader.showStage(getOrganisation(), "/view/User/UserDashboardView.fxml", "Manager Dashboard", stage);
            }
            stage.close();
            
        } catch (NoSuchUserException exception) {
            ErrorModel loginError = new ErrorModel(exception, "Invalid credentials");
            ViewLoader.showErrorStage(loginError);
        }
        
    } 

    // provided from scaffold, allows the user to exit the application if they dont want to login
    @FXML
    public void userExit() {
        stage.close();
    }

}
