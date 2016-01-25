<?php

/**
 * Implements hook_drush_cache_clear().
 */
function passive_drush_cache_clear(&$types) {
  if (module_exists('passive')) {
    $new_types = array(
      'all' => $types['all'],
    );
    if (variable_get('passive_flush')) {
      $new_types['all-non-passive'] = 'drush_cache_clear_all_non_passive';
    }
    else {
      $new_types['all-passive'] = 'drush_cache_clear_all_passive';
    }
    $types = $new_types + $types;
  }
}

/**
 * Flushes all non-passive caches.
 */
function drush_cache_clear_all_non_passive() {
  global $conf;
  $conf['passive_flush_override'] = FALSE;
  drush_cache_clear_both();
}

/**
 * Flushes all caches including passive.
 */
function drush_cache_clear_all_passive() {
  global $conf;
  $conf['passive_flush_override'] = TRUE;
  drush_cache_clear_both();
}
