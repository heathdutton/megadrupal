<?php

/**
 * @file
 * Views custom cache views plugins.
 */

/**
 * Implements hook_views_plugins().
 */
function views_custom_cache_views_plugins() {
  return array(
    'cache' => array(
      'views_custom_cache' => array(
        'path' => drupal_get_path('module', 'views_custom_cache') . '/views',
        'title' => t("Custom cache: view's first argument"),
        'help' => t("Cache based on view's first argument."),
        'help topic' => 'cache-custom',
        'handler' => 'views_custom_cache_plugin_cache',
        'uses options' => TRUE,
      ),
      'views_custom_multi_cache' => array(
        'path' => drupal_get_path('module', 'views_custom_cache') . '/views',
        'title' => t("Custom cache: view's all arguments"),
        'help' => t("Cache based on view's all arguments."),
        'help topic' => 'cache-custom-multi',
        'handler' => 'views_custom_cache_plugin_multi_cache',
        'uses options' => TRUE,
      ),
    ),
  );
}
