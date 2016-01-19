<?php
/**
 * @file
 * Drush commands for Codit and helper functions for Drush.
 */

/**
 * Outputs messages object that has properties ->error[], ->msg[], ->success[].
 *
 * @param object $object
 *   Object can contain arrays in ->error, ->msg, ->success that contain
 *   errors, messages, or success statements for the log. After they are output
 *   they are removed from the object
 */
function codit_drush_process_messages(&$object = '') {
  // Output errors.
  foreach (!empty($object->error) && ((is_array($object->error) || is_object($object->error))) ? $object->error : array() as $error) {
    // Output each error.
    drush_log($error, 'error');
  }
  $object->errorFlag = TRUE;
  // Reset the errors.
  $object->error = array();

  // Output Messages.
  foreach (!empty($object->msg) && ((is_array($object->msg) || is_object($object->msg))) ? $object->msg : array() as $msg) {
    // Output each message.
    // This log is too intense but 'notice' does not display:
    // drush_log($msg, 'warning');
    drush_print("* $msg ");
  }
  // Reset the messages.
  $object->msg = array();

  // Output Success.
  foreach (!empty($object->success) && ((is_array($object->success) || is_object($object->success))) ? $object->success : array() as $success) {
    // Output each error.
    drush_log($success, 'success');
  }
  // Reset the Successes.
  $object->success = array();
}
