<?php
/**
 * @file
 * Provide Views data and handlers for views_media_browser.module
 */

/**
 * Implements hook_views_plugins().
 */
function views_media_browser_views_plugins() {
  $plugins = array();

  include_once('views_media_browser_style_plugin.inc');

  // Style plugin.
  $plugins['style']['views_media_browser'] = array(
    'title' => t('Views Media Browser'),
    'help' => t('Separately rendered rows in a table.'),
    'handler' => 'views_media_browser_style_plugin',
    'theme' => 'views_view_views_media_browser_table',
    'theme path' => drupal_get_path('module', 'views_media_browser') . '/includes',
    'base' => array('file_managed'),
    'uses row plugin' => TRUE,
    'uses row class' => TRUE,
    'uses options' => TRUE,
    'uses fields' => TRUE,
    'type' => 'normal',
    'help topic' => 'style-views-media-browser',
    'path' => drupal_get_path('module', 'views_media_browser') . '/includes',
  );

  return $plugins;
}