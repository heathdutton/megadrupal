<?php

/**
 * Implementation of hook_views_plugins().
 */
function views_access_callback_views_plugins() {
  return array(
    'access' => array(
      'parent' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_access',
        'parent' => '',
      ),
      'views_access_callback' => array(
        'title' => t('Callback function'),
        'help' => t('Access will be granted to users based on result returned by callback function.'),
        'handler' => 'views_access_callback_plugin_access_callback',
        'uses options' => TRUE,
        'path' => drupal_get_path('module', 'views_access_callback'),
      ),
    ),
  );
}
