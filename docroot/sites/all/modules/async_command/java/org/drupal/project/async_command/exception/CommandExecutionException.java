package org.drupal.project.async_command.exception;

/**
 * This is the exception that can be handled by the Druplet.
 */
public class CommandExecutionException extends DrupletException {
    // Commands should set error status and message in CommandRecord.
    //private AsyncCommand.Status status;

//    public CommandExecutionException(String message, Throwable e, AsyncCommand.Status status) {
//        super(message, e);
//        this.status = status;
//    }
//
//    public CommandExecutionException(String message) {
//        super(message);
//        status = AsyncCommand.Status.FAILURE;
//    }

//    public CommandExecutionException(String message, AsyncCommand.Status status) {
//        super(message);
//        this.status = status;
//    }
//
//    public AsyncCommand.Status getStatus() {
//        return status;
//    }

    public CommandExecutionException (Throwable cause) {
        super(cause);
    }
}
