package org.drupal.project.async_command;

import org.apache.commons.cli.*;
import org.drupal.project.async_command.exception.ConfigLoadingException;
import org.drupal.project.async_command.exception.DrupletException;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.util.logging.Logger;

/**
 * This handler deals with CommandLineInterface launching Druplets.
 * Different launchers (CLI, web, etc) have different interfaces.
 */
public class CommandLineLauncher {

    private static Logger logger = DrupletUtils.getPackageLogger();

    private Class drupletClass;
    private Druplet druplet;
    private File configFile;
    // this is the default running mode.
    private Druplet.RunningMode runningMode;
    private boolean debugMode = false;

    /**
     * Initialize the launcher with a druplet class.
     *
     * @param drupletClass
     */
    public CommandLineLauncher(Class<? extends Druplet> drupletClass) {
        // assert Druplet.class.isAssignableFrom(drupletClass);
        this.drupletClass = drupletClass;
    }


    /**
     * Launches  CLI; users of the class should call this function.
     *
     * @param args arguments from main()
     */
    public void launch(String[] args) {
        logger.info("Druplet VERSION: " + DrupletUtils.VERSION);
        if (!DrupletUtils.checkJavaVersion()) {
            logger.warning("Please install Java 1.6 or greater version. Your Java version is " + System.getProperty("java.version"));
        }

        // build command parser
        Options options = buildOptions();
        CommandLineParser parser = new PosixParser();
        CommandLine shellCommand = null;
        try {
            shellCommand = parser.parse(options, args);
        } catch (ParseException exp) {
            logger.severe("Cannot parse parameters. Please use -h to see help. Error message: " + exp.getMessage());
            return;
        }

        try {
            // get configurable settings. non-exclusive.
            handleSettings(shellCommand);
            // construct druplet object
            constructDruplet();
            // handle executable options, mutual exclusive
            handleExecutables(shellCommand);
            logger.info("Mission accomplished.");
        } catch (Throwable e) {
            logger.severe(e.getMessage());
            e.printStackTrace();
            logger.info("Unexpected errors. Program exits.");
            System.exit(-1);
        }
    }


    private void handleExecutables(CommandLine command) {
        // mutual exclusive options.
        if (command.hasOption('h')) {
            // print help message and exit.
            HelpFormatter formatter = new HelpFormatter();
            formatter.printHelp("Druplet -- " + druplet.getIdentifier(), buildOptions());
        }
        else if (command.hasOption('t')) {
            // test connection and exit.
            druplet.getDrupalConnection().connect();
            druplet.getDrupalConnection().testConnection();
        }
        else if (command.hasOption('e')) {
            // first create an command using Drush (which means it can only work locally), and then run the app using ONCE running mode.
            String[] args = command.getArgs();
            String options = "control=ONCE";
            if (args.length < 2 || args.length > 3) {
                logger.severe("You need to have at least 2 parameters for evaluation. Please see drush help async-command.");
                return;
            } else if (args.length == 2) {
                // options remain the same.
            } else if (args.length == 3) {
                options = args[2] + "|" + options;
            }
            DrupletUtils.executeDrush("async-command", druplet.getIdentifier(), args[0], args[1], options); // app, command, description, options.
            druplet.setRunningMode(Druplet.RunningMode.ONCE);
            druplet.run();
        }
        else {
            // run the application command by command. could be time-consuming
            druplet.run();
        }
    }

    /**
     * Handle non-executible settings.
     *
     * @param shellCommand
     */
    private void handleSettings(CommandLine shellCommand) {
        if (shellCommand.hasOption('c')) {
            configFile = new File(shellCommand.getOptionValue('c'));
            if (!configFile.exists()) {
                throw new ConfigLoadingException("Config file at '" + configFile.getPath() + "' does not exist.");
            }
            logger.info("Set configuration file as: " + configFile.getPath());
        }

        if (shellCommand.hasOption('r')) {
            String rm = shellCommand.getOptionValue('r');
            try {
                runningMode = Druplet.RunningMode.valueOf(rm.toUpperCase());
            } catch (Exception e) {
                throw new ConfigLoadingException("Cannot recognize running mode. Use -h to see the options available", e);
            }
        } else {
            runningMode = Druplet.RunningMode.SERIAL;
        }

        if (shellCommand.hasOption('d')) {
            debugMode = true;
        } else {
            debugMode = false;
        }
    }

    private void constructDruplet() {
        DrupletConfig drupletConfig;
        if (configFile != null && configFile.exists()) {
            drupletConfig = new DrupletConfig(configFile);
        } else {
            drupletConfig = DrupletConfig.load();
        }

        try {
            Constructor<Druplet> constructor = drupletClass.getConstructor(DrupletConfig.class);
            druplet = constructor.newInstance(drupletConfig);
        } catch (NoSuchMethodException e) {
            throw new DrupletException(e);
        } catch (InvocationTargetException e) {
            throw new DrupletException(e);
        } catch (InstantiationException e) {
            throw new DrupletException(e);
        } catch (IllegalAccessException e) {
            throw new DrupletException(e);
        }
        druplet.setRunningMode(runningMode);
    }


    /**
     * If subclass needs to set properties, Use config.properties instead of override the method here.
     */
    private Options buildOptions() {
        Options options = new Options();
        options.addOption("c", true, "database configuration file");
        options.addOption("r", true, "program running mode: 'first', 'serial' (default), 'parallel', or nonstop");
        options.addOption("d", "debug", false, "enable debug mode");
        options.addOption("h", "help", false, "print this message");
        options.addOption("t", "test", false, "test connection to Drupal database");
        options.addOption("e", "eval", false, "evaluate a command call directly for this Druplet. eg., -e app command description options");

        return options;
    }

}
