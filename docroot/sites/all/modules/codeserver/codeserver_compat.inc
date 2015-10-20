<?php
/*
 * @file
 * Part of Code Server module for Drupal by netgenius.co.uk
 */

// Set a custom header, handling various Drupal versions.
function codeserver_set_header($name, $value) {
  // For D7 we have drupal_add_http_header().
  drupal_add_http_header($name, $value);
}

// Log a message to the system log, handling D6 and D7.
function _codeserver_log_message($message, $variables, $type = 'notice') {
  $module = 'codeserver';
  // For Drupal 6 and 7, log the message in the watchdog log.
  $severity = ($type == 'error')? WATCHDOG_ERROR : (($type == 'warning')? WATCHDOG_WARNING : WATCHDOG_NOTICE);
  watchdog($module, $message, $variables, $severity);
}

// Get form arguments.
function codeserver_get_form_args($args) {
  // For D7, $args will be $form and $form_state.
  return array('form' => $args[0], 'form_state' => &$args[1]);
}
