<?php
/**
 * @file
 *   Defines main menu callback for the JStats module, and some utility
 *   functions.
 */

/**
 * Writes access record in jstats_accesslog.
 *
 * @param $return boolean
 *   If TRUE, the inserted record will be returned to the calling code.
 *   If FALSE, the json representation of the record will be printed.
 *   Defaults to FALSE.
 */
function jstats_callback($return = FALSE) {
  global $user;

  // Get the most reliable uid we can have without
  // further bootstraping Drupal.
  if (!empty($user) && is_object($user)) {
    $uid = $user->uid;
  }
  elseif (isset($_COOKIE['DRUPAL_UID'])) {
    $uid = $_COOKIE['DRUPAL_UID'];
  }
  else {
    $uid = 0;
  }

  // Retrieve the nid from the POST value if there's one
  $nid = isset($_POST['nid']) ? $_POST['nid'] : 0;

  $record = new stdClass();
  $record->path = $_POST['path'];
  $record->nid = $nid;
  $record->uid = $uid;
  $record->timestamp = REQUEST_TIME;

  $record->aid = db_insert('jstats_accesslog')
    ->fields((array)$record)
    ->execute();

  if ($return) {
    return $record;
  }
  else {
    drupal_json_output($record);
  }
}
