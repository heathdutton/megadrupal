<?php
/**
 * @file views_grouped_table.views.inc
 * Contains views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function views_grouped_table_views_plugins() {
  $plugins = array(
    'style' => array(
      'table_grouped_relationship' => array(
        'title' => t('Relationship table'),
        'help' => t('Displays rows in a table grouped by relationship.'),
        'handler' => 'views_plugin_style_table_grouped_relationship',
        'theme' => 'views_grouped_table_view_table_grouped_relationship',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-table', // TODO: change me
      ),
    ),
  );

  return $plugins;
}
