package org.drupal.project.async_command;

import org.apache.commons.dbutils.QueryRunner;
import org.drupal.project.async_command.exception.DatabaseRuntimeException;
import org.drupal.project.async_command.exception.DrupletException;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.logging.Logger;

/**
 * This is thread that upload data through JDBC concurrently.
 * TODO: submit this to commons.dbutils.
 */
public class BatchUploader extends Thread {

    private static Logger logger = DrupletUtils.getPackageLogger();
    private final BlockingQueue<Object[]> queue = new LinkedBlockingQueue<Object[]>();
    private final Connection connection;
    private final PreparedStatement preparedSql;
    private int batchSize;

    // check whether to use AtomicBoolean. perhaps don't need to as it's synchronized.
    private boolean accomplished = false;

    /**
     * Equivalent to BatchUploader(null, threadName, connection, sql, 0)
     *
     * @param threadName
     * @param connection
     * @param sql
     */
    public BatchUploader(String threadName, Connection connection, String sql) {
        this(null, threadName, connection, sql, 0);
    }

    /**
     * Initialize the BatchUploader.
     *
     * @param threadGroup The thread group this thread is associated with.
     * @param threadName A name for this BatchUploader for identification purpose in multi-threading environment. Could be null.
     * @param connection The connection to the Drupal database.
     * @param sql The full SQL for this BatchUploader. Use d() to parse any tables enclosed with {}.
     * @param batchSize
     */
    public BatchUploader(ThreadGroup threadGroup, String threadName, Connection connection, String sql, int batchSize) {
        super(threadGroup, threadName);
        this.connection = connection;
        this.batchSize = batchSize;
        try {
            this.preparedSql = connection.prepareStatement(sql);
        } catch (SQLException e) {
            throw new DatabaseRuntimeException(e);
        }
    }

    synchronized public void put(Object... row) {
        try {
            queue.put(row);
        } catch (InterruptedException e) {
            throw new DrupletException(e);
        } finally {
            notifyAll();
        }
    }

    synchronized public void accomplish() {
        accomplished = true;
        notifyAll();
    }

    @Override
    synchronized public void run() {
        QueryRunner qr = new QueryRunner(); // this is a dummy so we can use fillStatement().
        List<Object[]> rows = new ArrayList<Object[]>();
        int num = 0;
        int index = 0;

        while (true) {
            // retrieve items and save them.
            num = queue.drainTo(rows);
            if (num > 0) {
                logger.finest("Processing " + num + " rows in BatchUploader " + getName());
                try {
                    for (index = 0; index < num; index++) {
                        qr.fillStatement(preparedSql, rows.get(index));
                        preparedSql.addBatch();
                        // periodically execute batch
                        if ((batchSize > 0) && (index > 0) && (index % batchSize == 0)) {
                            preparedSql.executeBatch();
                        }
                    }
                    // finally execute batch for the rest of the items.
                    preparedSql.executeBatch();
                    rows.clear();
                } catch (SQLException e) {
                    throw new DatabaseRuntimeException(e);
                }
            }

            // test if accomplished or not.
            if (accomplished) {
                break;
            } else {
                //logger.finest("No rows in queue. Wait.");
                try {
                    wait();
                } catch (InterruptedException e) {
                    throw new DrupletException(e);
                }
            }
        }
        logger.fine("Finished processing BatchUploader: " + getName());
    }
}
