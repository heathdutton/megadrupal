package org.drupal.project.async_command.exception;

/**
 * Handles any Drupal related exceptions.
 */
public class DrupletException extends RuntimeException {

    public DrupletException(Throwable t) {
        super(t);
    }

    public DrupletException(String msg) {
        super(msg);
    }

    public DrupletException(String message, Throwable cause) {
        super(message, cause);
    }
}
