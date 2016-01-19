<?php
/**
 * @file
 * Provides the views plugin information.
 */

/**
 * Implements hook_views_plugin().
 */
function views_infinite_table_scroll_views_plugins() {
  return array(
    'module' => 'views_infinite_table_scroll',
    'pager' => array(
      'infinite_table_scroll' => array(
        'title' => t('Infinite Table Scroll'),
        'help' => t('views_infinite_table_scroll'),
        'handler' => 'views_plugin_pager_infinite_table_scroll',
        'uses options' => TRUE,
        'parent' => 'full',
      ),
    ),
  );
}
