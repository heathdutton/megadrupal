<?php

/**
 * Add local.css stylesheet.
 */
function coolness_preprocess_html(&$variables) {
  $path = drupal_get_path('theme', 'coolness');
  if (file_exists($path . '/css/local.css')) {
    drupal_add_css($path . '/css/local.css', array('group' => CSS_THEME, 'every_page' => TRUE));
  }
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function coolness_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  drupal_add_css(drupal_get_path('theme', 'coolness') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function coolness_process_maintenance_page(&$variables) {
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
}
