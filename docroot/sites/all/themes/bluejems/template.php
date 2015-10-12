<?php
/**
 * @file
 * this is a template file
 */


/**
 * This is for maintenance page.
 *
 * Implements hook_preprocess_HOOK()
 *
 * The name of the template being rendered ("maintenance_page" in this case.)
 */
function bluejems_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'bluejems') . '/styles/maintenance.css');
}
