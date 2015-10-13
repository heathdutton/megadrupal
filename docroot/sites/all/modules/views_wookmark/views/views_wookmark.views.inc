<?php

function views_wookmark_views_plugins() {
  $module_path = drupal_get_path('module', 'views_wookmark');
  return array(
    'module' => 'views_wookmark', // This just tells our themes are elsewhere.
    'style' => array(
      'fluid_grid' => array(
        'title' => t('Wookmark'),
        'help' => t('Displays items in a wookmark grid layout.'),
        'handler' => 'views_wookmark_plugin_style',
        'path' => $module_path . '/views',
        'theme path' => $module_path . '/theme',
        'theme file' => 'theme.inc',
        'theme' => 'views_wookmark_plugin_style',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-fluid-grid',
      ),
    ),
  );
}
