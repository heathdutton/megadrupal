<?php
/**
 * @file
 * this is a template file
 */

/**
 * This is to call the jQuery Cycle JavaScript.
 *
 * Implements hook_preprocess_HOOK()
 *
 * The name of the template being rendered ("html" in this case.)
 */
function whitebull_preprocess_html(&$vars) {
  if (drupal_is_front_page()) {
    drupal_add_js(
    drupal_get_path('theme', 'whitebull') . '/scripts/jquery.cycle.all.js');
  }
}


/**
 * This is for maintenance page.
 *
 * Implements hook_preprocess_HOOK()
 *
 * The name of the template being rendered ("maintenance_page" in this case.)
 */
function whitebull_preprocess_maintenance_page(&$variables) {
  if (!$variables['db_is_active']) {
    unset($variables['site_name']);
  }
  drupal_add_css(drupal_get_path('theme', 'whitebull') . '/styles/maintenance.css');
}
