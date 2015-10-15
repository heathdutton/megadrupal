<?php

/**
 * Implements hook_views_plugins().
 */
function autocache_views_plugins() {
  return array(
    'cache' => array(
      'autocache' => array(
        'title' => t('Autocache'),
        'help' => t('Use automatic caching for this view.'),
        'handler' => 'autocache_views_cache_plugin',
        'help topic' => 'autocache',
        'path' => drupal_get_path('module', 'autocache') . '/plugins/views',
      ),
    ),
  );
}
