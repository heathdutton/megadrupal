<?php
/**
 * @file
 * 
 * Contains Drush hook implementations for this module
 */

/**
 * Implements hook_drush_cache_clear
 * 
 * Adds a cache clear option for Petfinder
 */
function petfinder_drush_cache_clear(&$types) {
  $types['petfinder'] = 'petfinder_cache_clear';
}
