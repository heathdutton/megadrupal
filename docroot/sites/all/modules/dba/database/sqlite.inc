<?php

/**
 * @file
 * Provides database driver specific commands.
 */

/**
 * Generate code for creating table.
 */
function dba_sqlite_show_create_table($table) {
  $result = db_select('sqlite_master', 'sm')
    ->fields('sm', array('sql'))
    ->condition('tbl_name', $table, '=')
    ->execute();
  $create = '';
  foreach ($result as $row) {
    $create .= "$row->sql;\n";
  }
  return $create;
}

/**
 * Generate code for describing a table.
 */
function dba_sqlite_describe_table($table) {
  $conn = db_set_active(variable_get('dba_active_database', 'default'));
  db_set_active($conn);
  $rows = db_query("PRAGMA table_info([$table])");
  return $rows;
}
