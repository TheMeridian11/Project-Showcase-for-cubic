import java.io.IOException;

import au.edu.uts.ap.javafx.*;
import javafx.application.Application;
import javafx.stage.Stage;
import model.*;

public class Prog2WarehouseApp extends Application{
    public static void main(String[] args){
        launch(args);
    }

    @Override
    public void start(Stage stage) throws Exception {
        Suppliers seededSuppliers = new Suppliers().seedData();
        Users seededUsers = new Users().seedData(seededSuppliers);
        ViewLoader.showStage(new Organisation(seededSuppliers, seededUsers), "/view/LoginView.fxml", "Login", new FixedStage("/image/login_icon.png"));
    }
}

