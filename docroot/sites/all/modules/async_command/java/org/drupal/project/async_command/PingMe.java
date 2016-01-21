package org.drupal.project.async_command;

/**
 * Simple command to test Drupal database connection.
 */
public class PingMe extends AsyncCommand {

    private String pingMessage;
    private String pongMessage;
    private Long sleepTime;

    public PingMe(CommandRecord record, Druplet druplet) {
        super(record, druplet);
    }

    @Override
    protected void beforeExecute() {
        super.beforeExecute();
        this.pingMessage = record.getString1();
        this.sleepTime = DrupletUtils.getLong(record.getNumber1());
    }

    @Override
    protected void execute() {
        if (sleepTime != null) {
            try {
                logger.info("PingMe sleeps for " + sleepTime + " milliseconds.");
                Thread.currentThread().sleep(sleepTime);
            } catch (InterruptedException e) {
                commandStatus = Status.FAILURE;
                pongMessage = "Interrupted while PingMe sleeps.";
                return;
            }
        }

        if (pingMessage != null) {
            pongMessage = "Pong with message: " + pingMessage;
        } else {
            pongMessage = "Pong.";
        }
        //commandStatus = Status.SUCCESS;
    }

    @Override
    protected void afterExecute() {
        // success status is set in super.afterExecute()
        super.afterExecute();
        record.setMessage(pongMessage);
    }
}
