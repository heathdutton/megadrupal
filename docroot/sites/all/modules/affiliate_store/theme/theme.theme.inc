<?php

/**
 * @file
 * Theme and template preprocess functions.
 */

/**
 * Implements theme passthrough hook.
 */
function _affiliate_store_theme_theme(&$theme) {
  $path = drupal_get_path('module', 'affiliate_store');
  $theme['affiliate_store_currency'] = array(
    'variables' => array('code' => 'USD'),
    'file' => 'theme.inc',
    'path' => "$path/theme",
  );
  $theme['affiliate_store_go'] = array(
    'variables' => array('text' => '', 'url' => ''),
    'file' => 'theme.inc',
    'path' => "$path/theme",
  );
  $theme['affiliate_store_image'] = array(
    'variables' => array('element' => '', 'url' => '', 'title' => ''),
    'file' => 'theme.inc',
    'path' => "$path/theme",
  );
  $theme['affiliate_store_menu'] = array(
    'variables' => array('tree' => array()),
    'file' => 'theme.inc',
    'path' => "$path/theme",
  );
  $theme['views_view__affiliate_store'] = array(
    'variables' => array('view_array' => array(), 'view' => NULL),
    'file' => 'theme.inc',
    'template' => 'views-view--affiliate-store',
    'base hook' => 'views_view',
    'path' => "$path/theme",
    'preprocess functions' => array(
      'template_preprocess',
      'template_preprocess_views_view',
      'affiliate_store_preprocess_views_view',
      'template_preprocess_views_view__affiliate_store',
    ),
  );
}
