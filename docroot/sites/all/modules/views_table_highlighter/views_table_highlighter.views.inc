<?php

/**
 * Implements hook_views_plugins().
 */
function views_table_highlighter_views_plugins() {
  return array(
    'style' => array(
      'table_highlighter' => array(
        'title' => t('Table Highlighter'),
        'help' => t('Highlights table rows based on simple PHP code.'),
        'handler' => 'views_table_highlighter_plugin_style',
        'parent' => 'table',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'theme' => 'views_view_table',
        'even empty' => TRUE,
      )
    )
  );
}
