<?php 
/**
 * @file
 * Drush functionality.
 */

/**
 * Adds a cache clear option for Netforum.
 */
function netforum_drush_cache_clear(&$types) {
  if (function_exists('module_exists') && module_exists('token')) {
    $types['netforum'] = 'netforum_invalidate_cache';
  }
}
