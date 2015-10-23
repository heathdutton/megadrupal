<?php
/**
 * Implements hook_views_plugins().
 */
function views_supertable_views_plugins() {
  return array(
    'style' => array(
      'views_supertable' => array(
        'title' => t('jQuery Supertable'),
        'help' => t('Display the results as a jQuery supertable.'),
        'handler' => 'views_plugin_style_supertable',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'type' => 'normal',
        'parent' => 'table',
        'path' => drupal_get_path('module', 'views_supertable'),
        'theme' => 'views_supertable',
        'theme path' => drupal_get_path('module', 'views_supertable') . '/theme',
        'theme file' => 'views_supertable.theme.inc',
      ),
    ),
  );
}