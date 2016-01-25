package org.drupal.project.async_command;

import org.drupal.project.async_command.exception.DatabaseRuntimeException;

import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;

/**
 * Database record for this AsyncCommand.
 * This is the "boundary object" between AsyncCommand and Druplet, where Druplet knows how to transfer data into the AsyncCommand,
 * And AsyncCommand only cares about the program logic.
 */
public class CommandRecord implements Comparable<CommandRecord> {

    // this variables can't change after construction.
    private final Long id;
    private final String app;
    private final String command;
    private final String description;
    private final Long uid; // nullable, can't change during runtime.
    private final Long eid; // nullable, can't change during runtime.
    private final Long created;

    // these are input/output parameters
    private final byte[] input;
    private byte[] output;
    private Long id1;
    private Long id2;
    private Long id3;
    private Long id4;
    private Float number1;
    private Float number2;
    private Float number3;
    private Float number4;
    private String string1;
    private String string2;
    private String string3;
    private String string4;

    // these are status/control parameters.
    private String status;
    private String control;
    private String message;
    private Long weight;
    private Long start;
    private Long end;
    private Long checkpoint;
    private Float progress;

    // other useful variables
    private final DrupalConnection drupalConnection;

    protected static Logger logger = DrupletUtils.getPackageLogger();


    /**
     * Given the database query result, construct a AsyncCommand Record object.
     *
     * @param row Database row for this record, should exact match the record.
     * @param drupalConnection
     */
    public CommandRecord(Map<String, Object> row, DrupalConnection drupalConnection) {
        assert row != null;
        // drupalConnection could be null for ad hoc record.
        //assert drupalConnection != null;

        // don't need to do assertion here. if row doesn't contain the key, the value is just null.
        //assert row.containsKey("id") && row.containsKey("app") && row.containsKey("command") && row.containsKey("description")
        //        && row.containsKey("uid") && row.containsKey("eid") && row.containsKey("created") && row.containsKey("input")
        //        && row.containsKey("output") && row.containsKey("id1") && row.containsKey("id2") && row.containsKey("number1")
        //        && row.containsKey("number2") && row.containsKey("string1") && row.containsKey("string2") && row.containsKey("status")
        //        && row.containsKey("control") && row.containsKey("message") && row.containsKey("dependency") && row.containsKey("start")
        //        && row.containsKey("end") && row.containsKey("checkpoint") && row.containsKey("progress");

        this.drupalConnection = drupalConnection;
        //drupalConnection.connect();

        this.id = DrupletUtils.getLong(row.get("id")); // if id is null, it means this is a dummy record.
        this.app = (String) row.get("app");     // could be null for dummys, assert app != null;
        this.command = (String) row.get("command");    // could be null for dummys, assert app != null;
        this.description = (String) row.get("description");   // note: even though this can be null, class type cast would still work.
        this.uid = DrupletUtils.getLong(row.get("uid"));
        this.eid = DrupletUtils.getLong(row.get("eid"));
        this.created = DrupletUtils.getLong(row.get("created"));

        this.input = (byte[]) row.get("input");
        this.output = (byte[]) row.get("output");
        this.id1 = DrupletUtils.getLong(row.get("id1"));
        this.id2 = DrupletUtils.getLong(row.get("id2"));
        this.id3 = DrupletUtils.getLong(row.get("id3"));
        this.id4 = DrupletUtils.getLong(row.get("id4"));
        this.number1 = (Float) row.get("number1");
        this.number2 = (Float) row.get("number2");
        this.number3 = (Float) row.get("number3");
        this.number4 = (Float) row.get("number4");
        this.string1 = (String) row.get("string1");
        this.string2 = (String) row.get("string2");
        this.string3 = (String) row.get("string3");
        this.string4 = (String) row.get("string4");

        this.status = (String) row.get("status");
        this.control = (String) row.get("control");
        this.message = (String) row.get("message");
        this.weight = DrupletUtils.getLong(row.get("weight"));   // could be null for dummies, assert weight != null;
        this.start = DrupletUtils.getLong(row.get("start"));
        this.end = DrupletUtils.getLong(row.get("end"));
        this.checkpoint = DrupletUtils.getLong(row.get("checkpoint"));
        this.progress = (Float) row.get("progress");
    }

    /**
     * Update the result and status part of the command record.
     */
    public void persistResult() {
        if (isForged()) {
            logger.info("The CommandRecord is forged. Persist to logger. ");
            // TODO: print record to logger.
        }

        String sql = "UPDATE {async_command} SET output=?, id1=?, id2=?, id3=?, id4=?, number1=?, number2=?, number3=?, number4=?, " +
                "string1=?, string2=?, string3=?, string4=?, status=?, control=?, message=?, weight=?, " +
                "start=?, end=?, checkpoint=?, progress=? WHERE id=?";
        try {
            drupalConnection.update(sql, output, id1, id2, id3, id4, number1, number2, number3, number4, string1, string2, string3, string4,
                    status, control, message, weight, start, end, checkpoint, progress, id);
        } catch (SQLException e) {
            AsyncCommand.logger.severe("Cannot update command record. Fatal error. Record id: " + id);
            throw new DatabaseRuntimeException(e);
        }
    }

    /**
     * Update a single field in the {async_command} table.
     *
     * @param fieldName  Can only be status, control, message, weight, start, end, checkpoint and progress.
     * @param fieldValue The value of the field. Doesn't have to match to the class member field.
     */
    public void persistField(String fieldName, Object fieldValue) {
        assert fieldName.equals("status") || fieldName.equals("control") || fieldName.equals("message")
                || fieldName.equals("weight") || fieldName.equals("start") || fieldName.equals("end")
                || fieldName.equals("checkpoint") || fieldName.equals("progress");
        assert drupalConnection != null;

        try {
            drupalConnection.update("UPDATE {async_command} SET " + fieldName + "=? WHERE id=?", fieldValue, id);
        } catch (SQLException e) {
            AsyncCommand.logger.severe("Cannot update command record for field '" + fieldName + "'. Record id: " + id);
            throw new DatabaseRuntimeException(e);
        }
    }


    /**
     * The smaller the weight, created, or id, the smaller the record. Smaller record would get executed first.
     *
     * @param o
     * @return
     */
    @Override
    public int compareTo(CommandRecord o) {
        // first, compare weight
        int compareWeight = this.weight.compareTo(o.weight);
        if (compareWeight != 0) {
            return compareWeight;
        }
        // then compare created
        int compareCreated = this.created.compareTo(o.created);
        if (compareCreated != 0) {
            return compareCreated;
        }
        // finally compare id.
        return this.id.compareTo(o.id);
    }

    /**
     * Not supported yet!! Factory method, Create a record and save in the database.
     *
     * @param fields
     * @param drupalConnection
     * @return
     */
    public static CommandRecord create(Map<String, Object> fields, DrupalConnection drupalConnection) {
        long id = drupalConnection.insertCommandRecord(fields);
        return drupalConnection.retrieveCommandRecord(id);
    }

    /**
     * Created a forge record which is only used in memory.
     *
     * @param fields
     * @return
     */
    public static CommandRecord forge(Map<String, Object> fields) {
        if (fields == null) {
            fields = new HashMap<String, Object>();
        }
        return new CommandRecord(fields, null);
    }

    /**
     *
     * @return
     */
    public boolean isForged() {
        if (id == null || id <= 0 || drupalConnection == null) {
            return true;
        } else {
            return false;
        }
    }



    ///////////////////////////// getters and setters /////////////////////


    public String getCommand() {
        return command;
    }

    public void setStatus(AsyncCommand.Status status) {
        this.status = status.toString();
        assert this.status.length() == 4;
    }

    public AsyncCommand.Status getStatus() {
        if (status != null) {
            return AsyncCommand.Status.parse(status);
        } else {
            return null;
        }
    }

    public Long getId() {
        return id;
    }

    public String getApp() {
        return app;
    }

    public String getDescription() {
        return description;
    }

    public Long getUid() {
        return uid;
    }

    public Long getEid() {
        return eid;
    }

    public Long getCreated() {
        return created;
    }

    public byte[] getInput() {
        return input;
    }

    public byte[] getOutput() {
        return output;
    }

    public Long getId1() {
        return id1;
    }

    public Long getId2() {
        return id2;
    }
    
    public Long getId3() {
        return id3;
    }

    public Long getId4() {
        return id4;
    }

    public Float getNumber1() {
        return number1;
    }

    public Float getNumber2() {
        return number2;
    }

    public Float getNumber3() {
        return number3;
    }

    public Float getNumber4() {
        return number4;
    }

    public String getString1() {
        return string1;
    }

    public String getString2() {
        return string2;
    }
    
    public String getString3() {
        return string3;
    }

    public String getString4() {
        return string4;
    }

    public AsyncCommand.Control getControl() {
        if (control != null) {
            return AsyncCommand.Control.parse(control);
        } else {
            return null;
        }
    }

    public String getMessage() {
        return message;
    }

    public Long getWeight() {
        return weight;
    }

    public Long getStart() {
        return start;
    }

    public Long getEnd() {
        return end;
    }

    public Long getCheckpoint() {
        return checkpoint;
    }

    public Float getProgress() {
        return progress;
    }

    public void setOutput(byte[] output) {
        this.output = output;
    }

    public void setId1(Long id1) {
        this.id1 = id1;
    }

    public void setId2(Long id2) {
        this.id2 = id2;
    }
    
    public void setId3(Long id3) {
        this.id1 = id3;
    }

    public void setId4(Long id4) {
        this.id4 = id4;
    }

    public void setNumber1(Float number1) {
        this.number1 = number1;
    }

    public void setNumber2(Float number2) {
        this.number2 = number2;
    }

    public void setNumber3(Float number3) {
        this.number3 = number3;
    }

    public void setNumber4(Float number4) {
        this.number4 = number4;
    }

    public void setString1(String string1) {
        this.string1 = string1;
    }

    public void setString2(String string2) {
        this.string2 = string2;
    }
    
    public void setString3(String string3) {
        this.string3 = string3;
    }

    public void setString4(String string4) {
        this.string4 = string4;
    }

    public void setStatus(String status) {
        this.status = status;
    }

    public void setControl(AsyncCommand.Control control) {
        this.control = control.toString();
        assert this.control.length() == 4;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public void setWeight(Long weight) {
        this.weight = weight;
    }

    public void setStart(Long start) {
        this.start = start;
    }

    public void setEnd(Long end) {
        this.end = end;
    }

    public void setCheckpoint(Long checkpoint) {
        this.checkpoint = checkpoint;
    }

    public void setProgress(Float progress) {
        this.progress = progress;
    }
}
