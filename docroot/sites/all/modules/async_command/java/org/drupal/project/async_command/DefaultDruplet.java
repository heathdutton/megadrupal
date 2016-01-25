package org.drupal.project.async_command;


import java.util.Properties;

/**
 * This Druplet is mostly used for testing/demo purposes.
 */
@Identifier("default")
public class DefaultDruplet extends Druplet {

    @Deprecated
    public DefaultDruplet(DrupalConnection drupalConnection) {
        super(drupalConnection);
    }

    @Deprecated
    public DefaultDruplet(Properties config) {
        super(config);
    }

    public DefaultDruplet(DrupletConfig config) {
        super(config);
    }

    public static void main(String[] args) {
        logger.finest("You have set the log level to be finest. Expect to see many messages.");
        CommandLineLauncher launcher = new CommandLineLauncher(DefaultDruplet.class);
        launcher.launch(args);
    }
}
