<?php

/**
 * @file
 * Implements views's hooks.
 */

/**
 * Implements hook_views_plugins().
 *
 * Define new view display format.
 */
function views_table_rowspan_views_plugins() {
  $plugins = array(
    'style' => array(
      'table_rowspan' => array(
        'title' => t('Table Rowspan'),
        'help' => t('Merge duplicate rows in group use rowspan attribute'),
        'handler' => 'views_plugin_display_table_rowspan',
        'theme' => 'views_view_table',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-table',
        'parent' => 'views_plugin_style_table',
      ),
    ),
  );
  return $plugins;
}
