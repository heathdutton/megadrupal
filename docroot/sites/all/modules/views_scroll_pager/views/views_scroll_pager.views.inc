<?php

/**
 * @file
 * Views integration hooks and functions.
 */

/**
 * Implements hook_views_plugin().
 */
function views_scroll_pager_views_plugins() {
  return array(
    'module' => 'views_scroll_pager',
    'pager' => array(
      'horizontal_scroll_pager' => array(
        'title' => t('Horizontal scroll pager'),
        'help' => t('Horizontal pager with scrolling.'),
        'help topic' => 'pager-some',
        'handler' => 'views_scroll_pager_plugin_horizontal_scroll',
        'uses options' => TRUE,
        'parent' => 'full',
      ),
    ),
  );
}
