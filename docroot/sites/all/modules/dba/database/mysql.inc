<?php

/**
 * @file
 * Provides database driver specific commands.
 */

/**
 * Generate code for creating table.
 */
function dba_mysql_show_create_table($table) {
  $query = "SHOW CREATE TABLE $table;";
  $create = db_query($query)->fetchAssoc();
  return str_replace('"', '`', $create['Create Table']);
}

/**
 * Generate code for describing a table.
 */
function dba_mysql_describe_table($table) {
  $conn = db_set_active(variable_get('dba_active_database', 'default'));
  $rows = db_query("DESCRIBE $table;");
  db_set_active($conn);
  return $rows;
}
