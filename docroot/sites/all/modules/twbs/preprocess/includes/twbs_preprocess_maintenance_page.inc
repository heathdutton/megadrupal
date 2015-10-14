<?php

/**
 * Process variables for maintenance-page.tpl.php.
 *
 * The variables array generated here is a mirror of
 * twbs_preprocess_page(). This preprocessor will run its course when
 * theme_maintenance_page() is invoked. An alternate template file of
 * maintenance-page--offline.tpl.php can be used when the database is offline to
 * hide errors and completely replace the content.
 *
 * The $variables array contains the following arguments:
 * - $content
 *
 * @see maintenance-page.tpl.php
 */
function twbs_preprocess_maintenance_page(&$variables) {
  // Add favicon
  if (theme_get_setting('toggle_favicon')) {
    $favicon = theme_get_setting('favicon');
    $type = theme_get_setting('favicon_mimetype');
    drupal_add_html_head_link(array('rel' => 'shortcut icon', 'href' => drupal_strip_dangerous_protocols($favicon), 'type' => $type));
  }

  global $theme;
  // Retrieve the theme data to list all available regions.
  $theme_data = list_themes();
  $regions = $theme_data[$theme]->info['regions'];

  // Get all region content set with drupal_add_region_content().
  foreach (array_keys($regions) as $region) {
    // Assign region to a region variable.
    $region_content = drupal_get_region_content($region);
    isset($variables[$region]) ? $variables[$region] .= $region_content : $variables[$region] = $region_content;
  }

  // Setup layout variable.
  $variables['layout'] = 'col-md-12';
  if (!empty($variables['sidebar_first'])) {
    $variables['layout'] = 'col-md-9';
  }
  if (!empty($variables['sidebar_second'])) {
    $variables['layout'] = ($variables['layout'] == 'col-md-9') ? 'col-md-6' : 'col-md-9';
  }

  // Construct page title
  if (drupal_get_title()) {
    $head_title = array(
      'title' => strip_tags(drupal_get_title()),
      'name' => variable_get('site_name', 'Drupal'),
    );
  }
  else {
    $head_title = array('name' => variable_get('site_name', 'Drupal'));
    if (variable_get('site_slogan', '')) {
      $head_title['slogan'] = variable_get('site_slogan', '');
    }
  }

  // set the default language if necessary
  $language = isset($GLOBALS['language']) ? $GLOBALS['language'] : language_default();

  $variables['head_title_array']  = $head_title;
  $variables['head_title']        = implode(' | ', $head_title);
  $variables['base_path']         = base_path();
  $variables['front_page']        = url();
  $variables['breadcrumb']        = '';
  $variables['feed_icons']        = '';
  $variables['help']              = '';
  $variables['language']          = $language;
  $variables['language']->dir     = $language->direction ? 'rtl' : 'ltr';
  $variables['logo']              = theme_get_setting('logo');
  $variables['messages']          = $variables['show_messages'] ? theme('status_messages') : '';
  $variables['main_menu']         = array();
  $variables['secondary_menu']    = array();
  $variables['site_name']         = (theme_get_setting('toggle_name') ? variable_get('site_name', 'Drupal') : '');
  $variables['site_slogan']       = (theme_get_setting('toggle_slogan') ? variable_get('site_slogan', '') : '');
  $variables['tabs']              = '';
  $variables['title']             = drupal_get_title();

  // Compile a list of classes that are going to be applied to the body element.
  $variables['classes_array'][] = 'in-maintenance';
  if (isset($variables['db_is_active']) && !$variables['db_is_active']) {
    $variables['classes_array'][] = 'db-offline';
  }
  if ($variables['layout'] == 'both') {
    $variables['classes_array'][] = 'two-sidebars';
  }
  elseif ($variables['layout'] == 'none') {
    $variables['classes_array'][] = 'no-sidebars';
  }
  else {
    $variables['classes_array'][] = 'one-sidebar sidebar-' . $variables['layout'];
  }

  // Dead databases will show error messages so supplying this template will
  // allow themers to override the page and the content completely.
  if (isset($variables['db_is_active']) && !$variables['db_is_active']) {
    $variables['theme_hook_suggestion'] = 'maintenance_page__offline';
  }
}
