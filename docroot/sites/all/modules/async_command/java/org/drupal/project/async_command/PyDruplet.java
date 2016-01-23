package org.drupal.project.async_command;

import java.util.Properties;

public class PyDruplet extends Druplet {

    /**
     * This calls a function initialize() for other initialization stuff.
     * Jython doesn't support function overloading, so we can only do this.
     *
     * @param drupalConnection Connection to a Drupal database that has the {async_command} table.
     */
    @Deprecated
    public PyDruplet(DrupalConnection drupalConnection) {
        super(drupalConnection);
        logger.fine("Constructor called for PyDruplet");
        initialize();
    }

    @Deprecated
    public PyDruplet(Properties config) {
       super(config);
       logger.fine("Constructor called for PyDruplet");
       initialize();
    }

    public PyDruplet(DrupletConfig config) {
       super(config);
       logger.fine("Constructor called for PyDruplet");
       initialize();
    }

    protected void initialize() {
        // do nothing here. for overriding purpose. Usually register command here.
    }

    /**
     * anually set DrupalConnection and call initialize() required.
     */
    protected PyDruplet() {
        super();
        logger.fine("Default constructor called for PyDruplet. Manually set DrupalConnection and call initialize() required.");
    }

    /**
     * Derived function has to override it.
     */
    @Override
    public String getIdentifier() {
        throw new IllegalArgumentException("Please override getIdentifier()");
    }

    /**
     * Has to use a different name for the function because Jython/Python doesn't support function overloading.
     * @param identifier
     * @param commandClass
     */
    protected void registerCommandClassWithIdentifier(String identifier, Class<? extends AsyncCommand> commandClass) {
        registerCommandClass(identifier, commandClass);
    }
}
