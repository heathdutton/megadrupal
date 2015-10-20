<?php

/**
 * @file
 * Provides database driver specific commands.
 */

/**
 * Generate code for creating table.
 */
function dba_mysql_show_create_table($table) {
  $query = ".schema $table;";
  $create = db_query($query)->fetchAssoc();
  // TODO: return proper information from this query
}
