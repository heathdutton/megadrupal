<?php

/**
 * @file
 * This file contains the drush integration for the Server Monitor module.
 */

/**
 * Implements hook_drush_command().
 */
function server_monitor_drush_command() {
  $items = array();
  $items['server-monitor'] = array(
    'description' => 'Run the server monitor.',
    'drupal dependencies' => array('server_monitor'),
    'aliases' => array('smon'),
    'callback' => 'server_monitor_check_all',
  );
  return $items;
}
