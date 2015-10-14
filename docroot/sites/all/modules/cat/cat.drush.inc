<?php

/**
 * @file Drush integration for the CAT module.
 *
 * @author Oleksiy Kalinichenko <aexchecker@gmail.com>
 */

/**
 * Implements hook_drush_cache_clear().
 */
function cat_drush_cache_clear(&$types) {
  if (function_exists('module_exists') && module_exists('cat')) {
    $types['cat-items'] = 'drush_cat_cache_clear_cat_items';
  }
}

/**
 * Clear caches internal to CAT module.
 */
function drush_cat_cache_clear_cat_items() {
  cat_clear_cache();
}

