package org.drupal.project.async_command;

/**
 * Stub class for commands written in Jython.
 */
abstract public class PyAsyncCommand extends AsyncCommand {

    /**
     * Call to a function to initialize in Jython, because Jython doesn't support function overloading.
     *
     * @param record
     * @param druplet
     */
    public PyAsyncCommand(CommandRecord record, Druplet druplet) {
        super(record, druplet);
        initialize();
    }

    protected PyAsyncCommand() {
        super();
        logger.fine("Default constructor called for PyAsyncCommand. Manually set record and druplet and call initialize() required.");
    }

    protected void initialize() {
        // do nothing here. for overriding purpose
    }

    /**
     * Derived function has to override it.
     */
    @Override
    public String getIdentifier() {
        throw new IllegalArgumentException("Please override getIdentifier()");
    }

}
