package org.drupal.project.async_command;

import org.drupal.project.async_command.exception.CommandExecutionException;
import org.drupal.project.async_command.exception.CommandParseException;
import org.drupal.project.async_command.exception.ConfigLoadingException;
import org.drupal.project.async_command.exception.DrupletException;

import java.io.File;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.*;
import java.util.concurrent.*;
import java.util.logging.Logger;

/**
 * This is the new Druplet class. A launcher has a field to launch Druplet, rather than Druplet has a field of launcher.
 */
public class Druplet implements Runnable {

    /**
     * Various running mode of the Druplet
     */
    public static enum RunningMode {
        /**
         * Only execute the next available command in the queue, and then exit.
         */
        ONCE,

        /**
         * Retrieve a list of commands, execute them in serial, and then exit.
         */
        SERIAL,

        /**
         * Retrieves a list of commands, execute them in parallel, and then exit.
         */
        PARALLEL,

        /**
         * Continuously retrieve new commands from the queue, execute them in parallel (serial just means thread=1), and exit only when told to.
         */
        NONSTOP
    }

    private RunningMode runningMode = RunningMode.SERIAL;

    public static enum AccessMode {
        /**
         * The Druplet and Drupal are located on the same computer.
         * Druplet can access Drupal API via (Drush) and can access settings.php.
         * Drupal home is saved in drupalHome (File.isDirectory() == true)
         */
        LOCAL,

        /**
         * The Druplet is located on a remote server from Drupal installation.
         * It's not safe for Druplet to call DrupalAPI (perhaps needs services module). settings.php is not available.
         * drupalSiteUrl is set with "drupal_site_url".
         *
         */
        REMOTE
    }

    protected AccessMode accessMode = AccessMode.LOCAL;

    protected DrupalConnection drupalConnection;

    @Deprecated
    protected Properties config;

    protected DrupletConfig drupletConfig;

    protected static Logger logger = DrupletUtils.getPackageLogger();

    /**
     * Default executor, run single thread.
     * This is useful for command that will create another command to run (either in 1 thread or in multiple thread)
     */
    protected ExecutorService executor = Executors.newSingleThreadExecutor();

    /**
     * Class name: the class object.
     */
    protected Map<String, Class> acceptableCommandClass = new HashMap<String, Class>();

    /**
     * Any Druplet should have at least one drupal database connection. Otherwise it's not "Druplet" anymore.
     * Use Druplet(Properties config) instead.
     *
     * @param drupalConnection Connection to a Drupal database that has the {async_command} table.
     */
    @Deprecated
    public Druplet(DrupalConnection drupalConnection) {
        this(drupalConnection.config);
        logger.fine("Constructor called for Druplet(DrupalConnection)");
    }

    /**
     * This constructor requires manually set DrupalConnection.
     * Access is "protected".
     */
    protected Druplet() {
        logger.fine("Default constructor called for Druplet. Manually set DrupalConnection required.");
    }


    @Deprecated
    public Druplet(Properties config) {
        logger.fine("Constructor called for Druplet(Properties)");
        if (config == null) {
            throw new ConfigLoadingException("Druplet config cannot be null.");
        }
        DrupletUtils.prepareConfig(config);
        this.config = config;

        // init drupalConnection
        DrupalConnection drupalConnection = new DrupalConnection(this.config);
        setDrupalConnection(drupalConnection);

        // set access_mode.
        if (config.containsKey("drupal_access_mode")) {
            try {
                accessMode = AccessMode.valueOf(config.getProperty("drupal_access_mode").toUpperCase());
            } catch (IllegalArgumentException e) {
                logger.warning("Incorrect drupal_access_mode setting. Set default to: local");
                accessMode = AccessMode.LOCAL;
            }
        }
        logger.info("Drupal access mode is: " + accessMode);

        // set drupalHome so that local Druplet can call DrupalAPI directly from drush.
        if (accessMode == AccessMode.LOCAL) {
            // attention: currently we delegate to callers to check if DRUPAL_HOME is valid.
            String drupalHomePath = null;
            if (config.containsKey("drupal_home")) {
                drupalHomePath = config.getProperty("drupal_home");
            } else if (System.getenv("DRUPAL_HOME") != null) {
                drupalHomePath = System.getenv("DRUPAL_HOME");
            }

            if (drupalHomePath != null) {
                File f = new File(drupalHomePath);
                if (f.exists() && f.isDirectory()) {
                } else {
                    logger.warning("Please specify valid DRUPAL_HOME (system environment variable) or drupal_home (in config.properties)");
                }
            }
        } else if (accessMode == AccessMode.REMOTE) {
            // set drupalSiteUrl so that Druplet can access the services module.
            if (config.containsKey("drupal_site_url")) {
                try {
                    URL drupalSiteUrl = new URL(config.getProperty("drupal_site_url"));
                } catch (MalformedURLException e) {
                    logger.warning("Please specify a valid drupal_site_url.");
                    e.printStackTrace();
                }
            }
        }

        // register command classes
        registerCommandClass(PingMe.class);
    }


    /**
     * This should be the default constructor to initiate Druplet with the configuration.
     * Register acceptable AsyncCommand classes in constructor. By default, this registers PingMe command.
     *
     * @param drupletConfig
     */
    public Druplet(DrupletConfig drupletConfig) {
        logger.fine("Constructor called for Druplet(DrupletConfig)");
        if (drupletConfig == null) {
            throw new ConfigLoadingException("Druplet config cannot be null.");
        }
        this.drupletConfig = drupletConfig;

        // init drupalConnection
        DrupalConnection drupalConnection = new DrupalConnection(drupletConfig);
        setDrupalConnection(drupalConnection);

        accessMode = drupletConfig.getAccessMode();
        logger.info("Drupal access mode is: " + accessMode);

        // register command classes
        registerCommandClass(PingMe.class);
    }


    /////////////////////////////////////////////////


    public void setRunningMode(RunningMode runningMode) {
        this.runningMode = runningMode;
    }

    /**
     * Only derived class can change drupal connection.
     *
     * @param drupalConnection
     */
    protected void setDrupalConnection(DrupalConnection drupalConnection) {
        assert drupalConnection != null;
        this.drupalConnection = drupalConnection;
    }

    public DrupalConnection getDrupalConnection() {
        return this.drupalConnection;
    }

    /**
     * Specifies the name this Druplet is known as. By default is the class name. You can override default value too.
     * @return The identifier of the app.
     */
    public String getIdentifier() {
        return DrupletUtils.getIdentifier(this.getClass());
    }


    /**
     * Create an object of AsyncCommand based on the CommandRecord.
     * This function can't be moved into CommandRecord because CommandRecord is not award of different AsyncCommand classes.
     *
     * @param record
     * @return
     */
    AsyncCommand parseCommand(CommandRecord record) throws CommandParseException {
        if (acceptableCommandClass.containsKey(record.getCommand())) {
            Class commandClass = acceptableCommandClass.get(record.getCommand());
            try {
                Constructor<AsyncCommand> constructor = commandClass.getConstructor(CommandRecord.class, Druplet.class);
                return constructor.newInstance(record, this);
            } catch (NoSuchMethodException e) {
                throw new DrupletException(e);
            } catch (InvocationTargetException e) {
                throw new DrupletException(e);
            } catch (InstantiationException e) {
                throw new DrupletException(e);
            } catch (IllegalAccessException e) {
                throw new DrupletException(e);
            }
        } else {
            throw new CommandParseException("Invalid command or not registered with the Druplet. Command: " + record.getCommand());
        }
    }

    /**
     * Register a command with the Druplet.
     *
     * @param commandClass
     */
    public void registerCommandClass(Class<? extends AsyncCommand> commandClass) {
        // the command class has to be a subclass of AsyncCommand
        // assert AsyncCommand.class.isAssignableFrom(commandClass);
        String id = DrupletUtils.getIdentifier(commandClass);
        acceptableCommandClass.put(id, commandClass);
    }

    /**
     * Register a command with arbitrary identifier.
     *
     * @param identifier
     * @param commandClass
     */
    public void registerCommandClass(String identifier, Class commandClass) {
        // the command class has to be a subclass of AsyncCommand
        assert AsyncCommand.class.isAssignableFrom(commandClass);
        acceptableCommandClass.put(identifier, commandClass);
    }


    @Override
    public void run() {
        assert drupalConnection != null;  // this shouldn't be a problem in runtime.
        drupalConnection.connect();  // if it's not connected yet. make the connection.

        switch (runningMode) {
            case ONCE:
                runOnce();
                break;
            case SERIAL:
                runSerial();
                break;
            case PARALLEL:
                runParallel();
                break;
            case NONSTOP:
                throw new UnsupportedOperationException("NONSTOP running mode not supported yet.");
                //break;
        }
        // even though serial didn't use executor, we still shut it down in case any commands used it.
        logger.info("Shutdown parallel executor.");
        executor.shutdown();

        // close drupalConnection here is odd because Druplet didn't create the connection.
        // whoever create the connection should be responsible closing it.
        drupalConnection.close();
        drupalConnection = null;
        logger.info("Running the Druplet is accomplished.");
    }

    protected void runOnce() {
        StringBuffer sqlWhere = new StringBuffer();
        sqlWhere.append("WHERE app='").append(this.getIdentifier()).append("' AND status IS NULL AND control='ONCE'");
        List<CommandRecord> records = drupalConnection.retrieveAnyCommandRecord(sqlWhere.toString());
        if (records.size() != 1) {
            logger.warning("You can only have 1 command marked with 'ONCE' unhandled for execution. Found " + records.size() + ". Exist.");
            return;
        }

        CommandRecord record = records.get(0);
        AsyncCommand command = null;
        try {
            // attention: might need to set "records.start" here in order to handle UREC case.
            command = parseCommand(record);
            logger.info("Executing command: " + record.getCommand());
            command.run();
        } catch (Throwable e) {
            handleException(record, e);
        }

        if (record.getEnd() == null) {
            record.setEnd(DrupletUtils.getLocalUnixTimestamp());
        }
        logger.info("Command finished running with status: " + record.getStatus().toString());
        // if persistResult throws exception, then we simply will exit running the Druplet unless we fix the problem.
        // that's why we don't catch exception here and have launcher handle the exception.
        record.persistResult();
    }

    /**
     * Handle any exception (Throwable) occurred during command execution.
     * AsyncCommand should delegate exception handling to Druplet.
     *
     * @param record
     * @param exception
     */
    protected void handleException(CommandRecord record, Throwable exception) {
        if (CommandParseException.class.isInstance(exception)) {
                record.setStatus(AsyncCommand.Status.UNRECOGNIZED);
                record.setMessage(exception.getMessage());
        } else if (CommandExecutionException.class.isInstance(exception)) {
            logger.severe("Cannot run command '" + record.getCommand() + "' for application '" + this.getIdentifier() + "'. Error message: " + exception.getMessage());
            // assume status and message are already set in the command execution
            if (record.getStatus() == null) {
                record.setStatus(AsyncCommand.Status.FAILURE);
            }
            if (record.getMessage() == null) {
                record.setMessage(exception.getMessage());
            }
        } else if (DrupletException.class.isInstance(exception)) {
            logger.severe("Unexpected error happens for command: " + record.getCommand());
            record.setStatus(AsyncCommand.Status.FAILURE);
            record.setMessage(exception.getMessage());
        } else {
            logger.severe("System error or programming bug.");
            record.setStatus(AsyncCommand.Status.INTERNAL_ERROR);
            record.setMessage("System error or programming bug. See execution log for more details. Error type: " + exception.getClass().getName());
        }
        exception.printStackTrace();
    }

    /**
     * Run in serial mode. We don't use the single thread "executor" here to have more simplicity.
     */
    protected void runSerial() {
        List<CommandRecord> records = drupalConnection.retrieveNewCommandRecord(this.getIdentifier());
        logger.info("Total number of commands to run: " + records.size());
        logger.fine("Sorting commands.");
        Collections.sort(records);
        for (CommandRecord record : records) {
            AsyncCommand command = null;
            try {
                // attention: might need to set "records.start" here in order to handle UREC case.
                command = parseCommand(record);
                logger.info("Executing command: " + record.getCommand());
                command.run();
            } catch (Throwable e) {
                handleException(record, e);
            }

            if (record.getEnd() == null) {
                record.setEnd(DrupletUtils.getLocalUnixTimestamp());
            }
            logger.info("Command finished running with status: " + record.getStatus().toString());
            // if persistResult throws exception, then we simply will exit running the Druplet unless we fix the problem.
            // that's why we don't catch exception here and have launcher handle the exception.
            record.persistResult();
        }
    }

    protected void runParallel() {
        int coreThreadNum = drupletConfig.getIntProperty("thread_core_number", 2);
        int maxThreadNum = drupletConfig.getIntProperty("thread_max_number", 2);
        if (maxThreadNum < coreThreadNum) {
            logger.warning("thread_max_number cannot be smaller than thread_core_number.");
            maxThreadNum = coreThreadNum;
        }
        // set the executor to be pooled executor.
        //executor = new ThreadPoolExecutor(coreThreadNum, maxThreadNum, 1, TimeUnit.SECONDS,
        //        new PriorityBlockingQueue<Runnable>(), new CommandThreadFactory(), new ThreadPoolExecutor.AbortPolicy());
        executor = new ThreadPoolExecutor(coreThreadNum, maxThreadNum, 0, TimeUnit.SECONDS, new PriorityBlockingQueue<Runnable>(), new ThreadPoolExecutor.AbortPolicy()) {
            @Override
            protected void afterExecute(Runnable runnable, Throwable throwable) {
                super.afterExecute(runnable, throwable);
                if (throwable != null) {
                    assert runnable instanceof Future<?>;
                    CommandRecord record = null;
                    try {
                        record = (CommandRecord) (((Future<?>) runnable).get());
                    } catch (InterruptedException e) {
                        assert false;
                    } catch (ExecutionException e) {
                        assert false;
                    }
                    // delegate to the Druplet to handle exception.
                    handleException(record, throwable);
                }
            }
        };

        List<CommandRecord> records = drupalConnection.retrieveNewCommandRecord(this.getIdentifier());
        //logger.fine("Sorting commands.");
        //Collections.sort(records);
        List<Future<CommandRecord>> futuresList = new ArrayList<Future<CommandRecord>>();
        // this is to save the map if future.get generates no results.
        Map<Future<CommandRecord>, CommandRecord> futuresMap = new HashMap<Future<CommandRecord>, CommandRecord>();

        for (CommandRecord record : records) {
            try {
                AsyncCommand command = parseCommand(record);
                Future<CommandRecord> future = executor.submit(command, record);
                futuresList.add(future);
                futuresMap.put(future, record);
            } catch (CommandParseException e) {
                record.setStatus(AsyncCommand.Status.UNRECOGNIZED);
                record.setMessage(e.getMessage());
                // if any record generate errors when persisting, we stop the main program by not handling any RuntimeException.
                record.persistResult();
                e.printStackTrace();
            }
        }
        logger.info("Total number of commands to run in parallel: " + futuresList.size());

        for (Future<CommandRecord> future : futuresList) {
            CommandRecord record = null;
            try {
                // TODO: could enforce timeout here.
                record = future.get();
            } catch (Throwable e) {
                if (record == null) {
                    record = futuresMap.get(future);
                }
                handleException(record, e);
            }
            record.persistResult();
        }
    }

    @Deprecated
    public Properties getConfig() {
        return config;
    }

    public DrupletConfig getDrupletConfig () {
        return drupletConfig;
    }
}
