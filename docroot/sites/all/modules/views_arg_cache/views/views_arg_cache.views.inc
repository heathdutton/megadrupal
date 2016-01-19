<?php

/**
 * Implements hook_views_plugins().
 */
function views_arg_cache_views_plugins() {
  return array(
    'cache' => array(
      'views_arg_cache' => array(
        'path' => drupal_get_path('module', 'views_arg_cache') . '/views',
        'title' => t('Argument-based'),
        'help' => t('Simple caching using keys based on the arguments.'),
        'handler' => 'views_arg_cache_plugin_cache',
        'uses options' => TRUE,
      ),
    ),
  );
}
