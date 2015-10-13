<?php


/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_views_plugins().
 */
function clientcert_views_plugins() {
  return array(
    'access' => array(
      'clientcert' => array(
        'title' => t('Clientcert item permission'),
        'handler' => 'clientcert_plugin_access_perm',
        'uses options' => TRUE,
      ),
    ),
  );
}