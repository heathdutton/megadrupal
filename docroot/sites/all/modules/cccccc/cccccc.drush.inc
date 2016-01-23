<?php

/**
 * Implementation of hook_drush_command().
 *
 * @return
 *   An associative array of commands.
 */
function cccccc_drush_command() {
  $items = array();
  $items['civicrm-cccccc'] = array(
    'callback' => '_cccccc_clear_cached_creditcards',
    'arguments' => array(
      'seconds' => 'Retain cached data within this number of seconds.'
    ),
    'description' => dt('Flush the CiviCRM cache of sensitive data.'),
    'examples' => array(
      'drush civicrm-cccccc' => dt('Flush the CiviCRM cache of sensitive data.'),
    ),
  );
  return $items;
}
