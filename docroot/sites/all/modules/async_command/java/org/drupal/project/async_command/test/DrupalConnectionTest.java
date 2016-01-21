package org.drupal.project.async_command.test;

import org.drupal.project.async_command.DrupalConnection;
import org.drupal.project.async_command.DrupletConfig;
import org.drupal.project.async_command.DrupletUtils;
import org.junit.Before;
import org.junit.Test;

import java.util.List;
import java.util.Map;
import java.util.Properties;

import static org.junit.Assert.*;

public class DrupalConnectionTest {

    @Before
    public void setUp() throws Exception {
    }

    @Test
    public void testBatchUpdateAndVariables() throws Exception {
        Properties prop = DrupletUtils.loadProperties(DrupletUtils.getConfigPropertiesFile());
        prop.setProperty("db_max_batch_size", "2");
        DrupalConnection conn = new DrupalConnection(new DrupletConfig(prop));
        conn.connect();

        String s1 = DrupletUtils.evalPhp("echo serialize(2);");
        Object[][] params1 = {{"async_command_test1", "1".getBytes()}, {"async_command_test2", s1.getBytes()}, {"async_command_test3", "3".getBytes()}};
        conn.batch("INSERT INTO {variable}(name, value) VALUES(?, ?)", params1);

        long num1 = (Long) conn.queryValue("SELECT COUNT(*) FROM {variable} WHERE name LIKE 'async_command_test%'");
        assertEquals(3, num1);

        int v1 = (Integer) conn.variableGet("async_command_test2");
        assertEquals(v1, 2);

        // test variable set.
        conn.variableSet("async_command_test2", 100);
        v1 = (Integer) conn.variableGet("async_command_test2");
        assertEquals(v1, 100);
        conn.variableSet("async_command_test2", "Hello");
        s1 = (String) conn.variableGet("async_command_test2");
        assertEquals(s1, "Hello");


        String s2 = DrupletUtils.evalPhp("echo serialize('abc');");
        Object[][] params2 = {{s2.getBytes(), "async_command_test1"}};
        conn.batch("UPDATE {variable} SET value=? WHERE name=?", params2);
        String v2 = (String) conn.variableGet("async_command_test1");
        assertEquals(v2, "abc");

        // re-establish connection
        conn.close();
        prop = DrupletUtils.loadProperties(DrupletUtils.getConfigPropertiesFile());
        prop.setProperty("db_max_batch_size", "0");
        conn = new DrupalConnection(new DrupletConfig(prop));
        conn.connect();

        Object[][] params3 = {{"async_command_test1"}, {"async_command_test2"}, {"async_command_test3"}};
        conn.batch("DELETE FROM {variable} WHERE name=?", params3);
        long num2 = (Long) conn.queryValue("SELECT COUNT(*) FROM {variable} WHERE name LIKE 'async_command_test%'");
        assertEquals(num2, 0);
        conn.close();
    }

    @Test
    public void testQuery() throws Exception {
        DrupalConnection conn = new DrupalConnection(DrupletConfig.load());
        conn.connect();
        long uid = (Long) conn.queryValue("SELECT uid FROM {users} WHERE uid=1");
        assertTrue(uid == 1L);
        Object result = conn.queryValue("SELECT uid FROM {users} WHERE uid=-1");
        assertNull(result);
        Object o = conn.queryValue("SELECT nid FROM {node} WHERE nid=0");
        assertNull(o);
        long i = (Long) conn.queryValue("SELECT COUNT(*) FROM {variable}");
        assertTrue(i > 0);

        // test query array
        List<Object[]> lst = conn.queryArray("SELECT uid, name FROM {users} WHERE uid < ?", 5);
        for (Object[] row : lst) {
            assertEquals(row.length, 2);
            System.out.println("" + row[0] + " : " + row[1]);
        }

        conn.close();
    }

    @Test
    public void testAutoIncrement() throws Exception {
        DrupalConnection conn = new DrupalConnection(DrupletConfig.load());
        conn.connect();

        long max = (Long) conn.queryValue("SELECT max(rid) FROM {role}");
        long r1 = conn.insertAutoIncrement("INSERT INTO {role}(name, weight) VALUE(?, ?)", "test1", 0);
        assertTrue(r1 > max);
        long r2 = conn.insertAutoIncrement("INSERT INTO {role}(name, weight) VALUE(?, ?)", "test2", 0);
        assertTrue(r2 - r1 == 1);
        Object[][] params = {{r1}, {r2}};
        conn.batch("DELETE FROM {role} WHERE rid=?", params);
        long r3 = (Long) conn.queryValue("SELECT max(rid) FROM {role}");
        assertEquals(r3, max);

        conn.close();
    }

    @Test
    public void testColumnLabel() throws Exception {
        DrupalConnection conn = new DrupalConnection(DrupletConfig.load());
        conn.connect();
        List<Map<String, Object>> results = conn.query("SELECT uid FROM {users} WHERE uid=1");
        assertTrue(results.size() == 1);
        long uid = (Long) results.get(0).get("uid");
        assertEquals(1, uid);

        results = conn.query("SELECT uid AS id FROM {users} WHERE uid=1");
        assertTrue(results.size() == 1);
        uid = (Long) results.get(0).get("id");
        assertEquals(1, uid);
        assertTrue(!results.get(0).containsKey("uid"));
    }

}
