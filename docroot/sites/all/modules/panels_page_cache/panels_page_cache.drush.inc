<?php
/**
 * @file
 * Drush integration of panels_page_cache.
 *
 * drush cache-clear panels-page-cache - Clears the panels page specific caches.
 */

/**
 * Implements hook_drush_cache_clear().
 */
function panels_page_cache_drush_cache_clear(&$types) {
  $types['panels-page-cache'] = 'panels_page_cache_invalidate';
}
