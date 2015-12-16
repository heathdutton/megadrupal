<?php

/**
 * @file
 * Commerce_xls_import.errors.inc.
 *
 * Error functions.
 */

/**
 * Get error message if it exists.
 */
function commerce_xls_import_get_error($row, $column) {
  $message = '';

  $result = db_query("SELECT message FROM {commerce_xls_import_errors} WHERE row=:row AND `column`=:column", array(':row' => $row, ':column' => $column))->fetchField();
  if ($result) {
    $message = $result;
  }

  return $message;
}

/**
 * Get whether or not the row has an error.
 */
function commerce_xls_import_get_row_error($row) {
  $result = db_query('SELECT message FROM {commerce_xls_import_errors} WHERE row=:row', array(':row' => $row))->fetchField();

  if ($result) {
    return TRUE;
  }

  return FALSE;
}

/**
 * Set error.
 */
function commerce_xls_import_set_error($row, $column, $message) {
  db_insert('commerce_xls_import_errors')
    ->fields(array(
      'row' => $row,
      '`column`' => $column,
      'message' => $message,
    ))
    ->execute();
  variable_set('commerce_xls_import_error_count', variable_get('commerce_xls_import_error_count') + 1);
}

/**
 * Gets the columns status.
 *
 * Determines the column status (error, success) for the
 * phpexport export hook to color code warnings and errors.
 *
 * If whole line is good, mark green, else mark white and highlight warnings and
 * errors.
 */
function commerce_xls_import_get_column_status($row, $column) {
  $error = commerce_xls_import_get_error($row, $column);

  if ($error) {
    return COMMERCE_XLS_IMPORT_DATA_ERROR;
  }
  else {
    return COMMERCE_XLS_IMPORT_DATA_SUCCESS;
  }
}
