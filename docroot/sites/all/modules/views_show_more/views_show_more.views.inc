<?php
/**
 * @file
 * Views Pager plugin definition.
 */

/**
 * Implements hook_views_plugins().
 */
function views_show_more_views_plugins() {
  $plugins = array(
    'pager' => array(
      'show_more' => array(
        'title' => t('Show more pager'),
        'help' => t('Ajax based show more pager.'),
        'handler' => 'ViewsShowMorePlugin',
        'help topic' => 'pager-show-more',
        'uses options' => TRUE,
        'parent' => 'full',
      ),
    ),
  );

  return $plugins;
}
