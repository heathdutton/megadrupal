package org.drupal.project.async_command.exception;

import java.sql.SQLException;

/**
 * Handles database related exceptions.
 * Only supports rethrow SQLException.
 */
public class DatabaseRuntimeException extends DrupletException {
    public DatabaseRuntimeException(SQLException t) {
        super(t);
    }
}
