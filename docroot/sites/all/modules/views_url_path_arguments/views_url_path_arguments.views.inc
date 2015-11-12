<?php

/**
 * @file
 * Provide views data and handlers.
 */

/**
 * Implements hook_views_plugins().
 */
function views_url_path_arguments_views_plugins() {
  return array(
    'argument validator' => array(
      'url_path_validator' => array(
        'title' => t('Validate URL path alias'),
        'handler' => 'views_plugin_argument_validate_url_path',
      ),
    ),
    'argument default' => array(
      'url_path_default_' => array(
        'title' => t('URL path alias from URL'),
        'handler' => 'views_plugin_argument_default_url_path',
      ),
    ),
  );
}
