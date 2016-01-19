<?php
/**
 * @file
 * Error reporting/handling
 */

$plugin = array (
  'title' => t('Error reporting'),
  'description' => t('Library for advanced error reporting.'),
  'xu_type' => 'library'
);

/**
 * Default error reporting function
 *
 * @param $message
 * @param $status
 */
function xu_set_message($message, $status) {
  drupal_set_message($message, $status);
}
