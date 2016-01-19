<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for hooks in the standard Drupal manner.
 */

/**
 * @smart_import hooks
 * @{
 */


/**
 * Alter row from import file. 
 * 
 * @param array $row
 *   An array of xls file row data.
 * @param object $ewrapper
 *   Node type entity wrapper variable to save.
 */
function hook_smart_import_row_alter($row, &$ewrapper) {
  // Alter body value.
  $ewrapper->body->set(array('value' => $row['body']));
}
