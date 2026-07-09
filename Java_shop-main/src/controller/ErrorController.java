package controller;

import au.edu.uts.ap.javafx.*;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import model.*;
import model.Exceptions.*;
import java.util.logging.*;

public class ErrorController extends Controller<ErrorModel> {
    
    @FXML private Label errorException;
    @FXML private Label errorMessage;

    public final ErrorModel getErrorModel() { return model; }

    @FXML private void Initialize() {
        //errorException.textProperty().bind(getErrorModel().getException());
    }

    @FXML
    public void userExit() {
        stage.close();
    }

}
