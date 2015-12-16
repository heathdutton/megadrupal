<?php
/**
 * @file
 * Settings for the style of this plugin.
 */

/**
 * Implements hook_views_plugins().
 */
function views_column_class_views_plugins() {
  return array(
    'style' => array(
      'views_column_class' => array(
        'title' => t('Views Column Class'),
        'help' => t('Assign classes to row items relevant to their position in the column structure as determined by your css.'),
        'help topic' => 'style-column-class',
        'handler' => 'views_column_class_style_plugin',
        'uses options' => TRUE,
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'type' => 'normal',
        'theme' => 'views_view_column_class',
        'theme path' => drupal_get_path('module', 'views_column_class') . '/theme',
        'theme file' => 'views_column_class.theme.inc',
      ),
    ),
  );
}
