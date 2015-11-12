<?php
/**
 * @file
 * Views integration for Views CSS Table.
 */

/**
 * Implements hook_views_plugins().
 */
function views_css_tables_views_plugins() {
  $path = drupal_get_path('module', 'views_css_tables');
  $plugins = array(
    'style' => array(
      'views_css_tables_style' => array(
        'title' => t('CSS Table'),
        'help' => t('Table constructed using DIVs and CSS, useful for responsive sites.'),
        'handler' => 'views_plugin_style_table',
        'path' => $path . '/views',
        'theme' => 'views_css_tables_table',
        'theme path' => $path . '/views',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'uses row plugin' => FALSE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );

  return $plugins;
}
