<?php
// $Id;$

/**
 * @file
 * Ooyala module drush integration.
 */

/**
 * Implementation of hook_drush_command().
 */
function ooyala_drush_command() {
  $items = array();
  $items['ooyala-sync'] = array(
    'description' => 'Synchronize video status/length data from Ooyala.',
    'drupal dependencies' => array('ooyala'),
    'options' => array(
      '--limit' => 'Number of items to synchronize.',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function ooyala_drush_help($section) {
  switch($section) {
    case 'drush:ooyala-sync':
      return dt('Query Ooyala API for status/length information for all videos that are missing the status/length values.');
  }
}

/**
 * Query Ooyala API for status/length data.
 */
function drush_ooyala_sync() {
  $limit = drush_get_option('limit', 0);
  $updated_nodes = _ooyala_sync_data($limit);
  foreach ($updated_nodes as $nid => $title) {
    $message = dt('Updated status/length for %title', array('%title' => $title));
    drush_log($message, 'ok');
    watchdog('ooyala', $message);
  }
}
