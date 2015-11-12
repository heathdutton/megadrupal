<?php

/**
 * @file
 * Implements all drush commands for the User-Agent Cache Bypass module.
 */


/**
 * Implements hook_drush_command().
 */
function ua_cache_bypass_drush_command() {
  $items = array();

  $items['cache-get-stale'] = array(
    'description' => "Returns a list of URLs whose page cache entries are stale.",
    'arguments' => array(
      'timeago' => 'The age (in seconds) when considering staleness of cache entries (defaults to an hour).',
    ),
    'examples' => array(
      'drush cgs 900' => 'Returns URL of page cache entries created more than 15 minutes ago.',
    ),
    'aliases' => array('cgs'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_DATABASE,
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function ua_cache_bypass_drush_help($section) {
  switch ($section) {
    case 'drush:cache-get-stale':
      return dt("Returns a list of URLs whose page cache entries are older than a given amount of time ago.");
  }
}

/**
 * Drush command callback for cache-get-stale.
 */
function drush_ua_cache_bypass_cache_get_stale($timeago = 3600) {
  // Drupal 7 uses REQUEST_TIME instead of time() for setting cache.
  $time = REQUEST_TIME - filter_var($timeago, FILTER_SANITIZE_NUMBER_INT);

  // Query the DB for caches that are older than $time.
  $cids = db_select('cache_page', 'cp')
    ->fields('cp', array('cid'))
    ->condition('created', $time, '<=')
    ->orderBy('created', 'ASC')
    ->execute()
    ->fetchCol();

  // Print results.
  if (is_array($cids)) {
    foreach ($cids as $cid) {
      drush_print($cid);
    }
  }
}
