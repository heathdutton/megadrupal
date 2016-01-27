package org.drupal.project.async_command;

import java.util.logging.Logger;

/**
 * Individual command to be executed. Each command is also registered with a druplet.
 * A command doesn't necessarily know a DrupalConnection. If needed, it can get from druplet.
 * The Record inner class needs to know a DrupalConnection in order to do database operations.
 *     f
 * Subclass can also write a initialize(...) function, which initialize parameters for the app and is used for CLI evaluation.
 */
abstract public class AsyncCommand implements Runnable {

    protected static Logger logger = DrupletUtils.getPackageLogger();

    protected DrupalConnection getDrupalConnection() {
        return druplet.getDrupalConnection();
    }

    protected Druplet getDruplet() {
        return druplet;
    }

    // TODO: should it be in CommandRecord?
    public static enum Status {
        SUCCESS("OKOK"),
        FAILURE("FAIL"),
        RUNNING("RUNN"),
        FORCED_STOP("STOP"),
        UNRECOGNIZED("NREC"),
        PENDING("PEND"),
        INTERNAL_ERROR("INTR");  // this is the error that should be fixed in the program.


        private final String statusToken;

        Status(String token) {
            assert token.length() == 4;
            this.statusToken = token;
        }

        @Override
        public String toString() {
            return statusToken;
        }

        public static Status parse(String token) {
            assert token != null && token.length() == 4;
            for (Status status : Status.class.getEnumConstants()) {
                if (status.statusToken.equals(token)) {
                    return status;
                }
            }
            // this shouldn't happen in real application.
            throw new AssertionError("Cannot parse status token: " + token);
        }
    }

    public static enum Control {
        ONCE("ONCE"),   // the once running mode.
        CANCEL("CNCL"),  // set to cancel
        REMOTE("TRAN"); // set to run as using data transfer.


        private final String controlToken;

        Control(String token) {
            assert token.length() == 4;
            this.controlToken = token;
        }

        @Override
        public String toString() {
            return controlToken;
        }

        public static Control parse(String token) {
            assert token != null && token.length() == 4;
            for (Control control : Control.class.getEnumConstants()) {
                if (control.controlToken.equals(token)) {
                    return control;
                }
            }
            // this shouldn't happen in real application.
            throw new AssertionError("Cannot parse control token: " + token);
        }
    }

    /**
     * The druplet this command is associated with.
     * Should be "final", but since we have the default constructor, it can't be final.
     * Use the default constructor in order to accommodate PyAsyncCommand
     */
    protected Druplet druplet;

    /**
     * The database record this command is associated with.
     * Should be "final", but since we have the default constructor, it can't be final.
     * Use the default constructor in order to accommodate PyAsyncCommand
     */
    protected CommandRecord record;

    /**
     * The command status. Will be set directly into CommandRecord.setStatus()
     */
    protected Status commandStatus;

    /**
     * Command message. Will be set directly into CommandRecord.setMessage().
     */
    protected String commandMessage;

    /**
     * Constructor should prepare the command to run "run()". Set member fields by using data in "record"; don't use record directly in execution.
     *
     * @param record
     * @param druplet
     */
    public AsyncCommand(CommandRecord record, Druplet druplet) {
        assert record != null && druplet != null;
        this.record = record;
        this.druplet = druplet;
    }

    protected AsyncCommand() {
        logger.fine("Default constructor called for AsyncCommand. Please manually set record and druplet.");
    }

    protected void setRecord(CommandRecord record) {
        this.record = record;
    }

    public CommandRecord getRecord() {
        return this.record;
    }

    protected void setDruplet(Druplet druplet) {
        this.druplet = druplet;
    }


    /**
     * Specifies the name this command is known as. By default is the class name. You can override default value too.
     * Deprecated because we don't want to use the default identifier. Instead, when register the command with an app, you can specify the identifier for this command on that specific
     *
     * @return The identifier of the command.
     */
    public String getIdentifier() {
        return DrupletUtils.getIdentifier(this.getClass());
    }

    /**
     * Run this command.
     * Attention: as of now, please override run() directly rather than using the execute() logic, which is unstable.
     * Before running the command, all input information is in 'record'.
     * After running the command, all output should also be in 'record'.
     * This method is not responsible to save status results back to the database. It's only responsible to run the command and set the command record.
     * This method is not responsible to handle exceptions. Exceptions should be handled by the enclosing druplet.
     */
    @Override
    public void run() {
        beforeExecute();
        // TODO: might catch exception here?
        execute();
        afterExecute();
    }

    /**
     * Usually initialize the command from 'record'
     */
    protected void beforeExecute() {
        record.setStart(DrupletUtils.getLocalUnixTimestamp());
    }

    /**
     * Usually save results back to 'record'
     */
    protected void afterExecute() {
        record.setEnd(DrupletUtils.getLocalUnixTimestamp());
        if (record.getStatus() == null) {
            if (commandStatus != null)
                record.setStatus(commandStatus);
            else
                record.setStatus(Status.SUCCESS);
        }
        if (record.getMessage() == null && commandMessage != null) {
            record.setMessage(commandMessage);
        }
    }

    /**
     * Execute this command. All parameters should be set before executing this command, preferably in the beforeExecute().
     * After execution, the results could be handled in afterExecute().
     * Either override this one, or the "run()" method.
     */
    protected void execute() {
    }


}
