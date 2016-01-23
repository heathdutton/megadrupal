<?php
/**
 * Hooks for db remote.
 */

/**
 * Provide a schema definition for a remote database table.
 *
 * This is identical to hook_schema(), except that the table will be created
 * using the database connect key specified in $conf['db_remote'].
 *
 * @see hook_schema().
 */
function hook_db_remote_schema() {
}
