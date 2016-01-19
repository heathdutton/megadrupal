<?php

/**
 * @file
 * Provides the Views integration for views content cache.
 */

/**
 * Implements hook_views_plugins().
 */
function views_row_cache_views_plugins() {
  return array(
    'cache' => array(
      'views_row_cache' => array(
        'path' => drupal_get_path('module', 'views_row_cache') . '/views',
        'title' => t('Row-based'),
        'help' => t('Row-based caching options for views.'),
        'handler' => 'views_row_cache_plugin_cache',
        'uses options' => TRUE,
      ),
    ),
  );
}
