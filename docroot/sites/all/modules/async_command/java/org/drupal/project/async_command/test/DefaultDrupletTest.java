package org.drupal.project.async_command.test;

import org.drupal.project.async_command.*;
import org.junit.Test;

import java.sql.SQLException;
import java.util.List;

import static junit.framework.Assert.assertTrue;

public class DefaultDrupletTest {

    @Test
    public void testPingMe() throws SQLException {
        // create a pingme command.
        // attention: this drupal connection is the one Drush is using. could be different from the one in the drupalConnection created earlier
        String output = DrupletUtils.executeDrush("async-command", "default", "PingMe", "From UnitTest", "string1=Hello");
        System.out.println("Drush output: " + output);

        DrupalConnection drupalConnection = new DrupalConnection(DrupletConfig.load());
        drupalConnection.connect();
        Long id = DrupletUtils.getLong(drupalConnection.queryValue("SELECT max(id) FROM {async_command}"));
        DefaultDruplet druplet = new DefaultDruplet(DrupletConfig.load());
        druplet.run();

        // run() closes the drupal connection, so we recreate again.
        drupalConnection.connect(true);
        assertTrue(output.trim().endsWith(id.toString()));
        CommandRecord record = drupalConnection.retrieveCommandRecord(id);
        assertTrue(record.getStatus().equals(AsyncCommand.Status.SUCCESS));
        assertTrue(record.getMessage().endsWith("Hello"));
    }

    @Test
    public void testParallel() throws SQLException {
        DrupalConnection drupalConnection = new DrupalConnection(DrupletConfig.load());
        drupalConnection.connect();

        // create a pingme command.
        // attention: this drupal connection is the one Drush is using. could be different from the one in the drupalConnection created earlier
        DrupletUtils.executeDrush("async-command", "default", "PingMe", "Test parallel (from UnitTest)", "string1=medium|number1=5000|number3=-1");
        Long id1 = DrupletUtils.getLong(drupalConnection.queryValue("SELECT max(id) FROM {async_command}"));
        DrupletUtils.executeDrush("async-command", "default", "PingMe", "Test parallel (from UnitTest)", "string1=short|number1=1000|number3=-1");
        Long id2 = DrupletUtils.getLong(drupalConnection.queryValue("SELECT max(id) FROM {async_command}"));
        DrupletUtils.executeDrush("async-command", "default", "PingMe", "Test parallel (from UnitTest)", "string1=long|number1=10000|number3=-1");
        Long id3 = DrupletUtils.getLong(drupalConnection.queryValue("SELECT max(id) FROM {async_command}"));

        DefaultDruplet druplet = new DefaultDruplet(DrupletConfig.load());
        druplet.setRunningMode(Druplet.RunningMode.PARALLEL);
        druplet.run();

        // run() closes the drupal connection, so we recreate again.
        drupalConnection.connect(true);
        List<Object[]> results = drupalConnection.queryArray("SELECT id FROM {async_command} WHERE end IS NOT NULL AND app='default' AND command='PingMe' AND number3=-1 ORDER BY end DESC");
        //System.out.println((results.get(0))[0] + ", " + (results.get(1))[0] + ", " + (results.get(2))[0]);
        //System.out.println(id3 + ", " + id1 + ", " + id2);
        // since there's waiting time, this should be the correct order
        assertTrue(id3.equals(DrupletUtils.getLong((results.get(0))[0])));
        assertTrue(id1.equals(DrupletUtils.getLong((results.get(1))[0])));
        assertTrue(id2.equals(DrupletUtils.getLong((results.get(2))[0])));
    }

    @Test
    public void testOnce() throws SQLException {
        DrupalConnection drupalConnection = new DrupalConnection(DrupletConfig.load());
        drupalConnection.connect();

        // create a pingme command.
        // attention: this drupal connection is the one Drush is using. could be different from the one in the drupalConnection created earlier
        String output1 = DrupletUtils.executeDrush("async-command", "default", "PingMe", "From UnitTest, stub", "string1=NonRunning");
        //System.out.println("Drush output: " + output);
        Long id1 = DrupletUtils.getLong(drupalConnection.queryValue("SELECT max(id) FROM {async_command}"));

        String output2 = DrupletUtils.executeDrush("async-command", "default", "PingMe", "From UnitTest", "string1=Hello|control=ONCE");
        //System.out.println("Drush output: " + output);
        Long id2 = DrupletUtils.getLong(drupalConnection.queryValue("SELECT max(id) FROM {async_command}"));

        DefaultDruplet druplet = new DefaultDruplet(DrupletConfig.load());
        druplet.setRunningMode(Druplet.RunningMode.ONCE);
        druplet.run();

        // run() closes the drupal connection, so we recreate again.
        drupalConnection.connect(true);
        CommandRecord record1 = drupalConnection.retrieveCommandRecord(id1);
        assertTrue(record1.getStatus() == null);
        record1.persistField("status", AsyncCommand.Status.FORCED_STOP.toString());

        CommandRecord record2 = drupalConnection.retrieveCommandRecord(id2);
        assertTrue(record2.getStatus() == AsyncCommand.Status.SUCCESS);
    }
}
