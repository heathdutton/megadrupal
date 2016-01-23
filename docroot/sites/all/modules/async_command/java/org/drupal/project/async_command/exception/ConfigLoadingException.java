package org.drupal.project.async_command.exception;

public class ConfigLoadingException extends DrupletException {

    public ConfigLoadingException(String message) {
        super(message);
    }

    public ConfigLoadingException(Throwable cause) {
        super(cause);
    }

    public ConfigLoadingException(String message, Throwable cause) {
        super(message, cause);
    }
}
