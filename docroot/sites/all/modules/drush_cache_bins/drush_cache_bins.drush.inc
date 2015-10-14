<?php

/**
 * @file
 * Exposes all core cache bins.
 *
 * @todo Drupal 8.
 */

/**
 * Implements hook_drush_cache_clear().
 *
 * Adds a cache clear option for the rest of core's bins.
 *
 * @todo How does Drush handle Drupal core version code? I know they have a
 * special mechanism.
 */
function drush_cache_bins_drush_cache_clear(&$types) {
  if (!drush_has_boostrapped(DRUSH_BOOTSTRAP_DRUPAL_FULL)) {
    return;
  }

  // Special case for main cache bin.
  $types['default'] = _drush_cache_bins_create_function('cache');

  switch (drush_drupal_major_version()) {
    case 6:
      _drush_cache_bins_drush_cache_clear_6($types);
      break;

    case 7:
      _drush_cache_bins_drush_cache_clear_7($types);
      break;
  }
}

/**
 * Adds cache bins for Drupal 6.
 */
function _drush_cache_bins_drush_cache_clear_6(&$types) {
  $bins = array(
    'filter',
    'form',
    'page',
  );

  foreach ($bins as $bin) {
    $types[$bin] = _drush_cache_bins_create_function('cache_' . $bin);
  }
}

/**
 * Add cache bins for Drupal 7.
 */
function _drush_cache_bins_drush_cache_clear_7(&$types) {
  // Cache bins that are installed by system module or whatnot.
  // Drush handles cache_block, and cache_menu for us.
  $bins = array(
    'form',
    'page',
    'path',
    'bootstrap',
  );

  foreach ($bins as $bin) {
    $types[$bin] = _drush_cache_bins_create_function('cache_' . $bin);
  }

  $modules = array(
    'filter',
    'image',
  );

  foreach ($modules as $module) {
    if (module_exists($module)) {
      $types[$module] = _drush_cache_bins_create_function('cache_' . $module);
    }
  }

  // Field module has its own cache_clear function.
  if (module_exists('field')) {
    $types['field'] = 'field_cache_clear';
  }
}

/**
 * Creates the cache bin specific cache_clear_all() function.
 */
function _drush_cache_bins_create_function($bin) {
  return create_function('', "cache_clear_all('*', '$bin', TRUE);");
}
