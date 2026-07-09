package model.Exceptions;

public class ErrorModel {
    private Exception exception;
    private String message;

    public ErrorModel(Exception ex, String message) {
        this.exception = ex;
        this.message = message;
    }

    public String getException() {
        return exception.getClass().getSimpleName();
    }

    public String getMessage() {
        return this.message;
    }
}
