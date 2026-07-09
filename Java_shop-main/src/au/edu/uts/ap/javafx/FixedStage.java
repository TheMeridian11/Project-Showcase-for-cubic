package au.edu.uts.ap.javafx;

import javafx.scene.image.Image;
import javafx.stage.Stage;

public class FixedStage extends Stage{
    public FixedStage(String icon_path) {
        super();
        setResizable(false);
        setOnCloseRequest(x -> x.consume());
        getIcons().add(new Image(icon_path));
    }
}
