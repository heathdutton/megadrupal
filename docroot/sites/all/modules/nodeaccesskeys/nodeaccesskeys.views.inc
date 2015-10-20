<?php

/**
 * @file
 * Provides the views plugin information.
 */

/**
 * Implements hook_views_plugin().
 */
function nodeaccesskeys_views_plugins() {
  return array(
    'module' => 'nodeaccesskeys',
    'access' => array(
      'nodeaccesskeys' => array(
        'title' => t('Node Access Keys'),
        'help' => t('This plugin extends the Views access rules to include support for node access keys.'),
        'handler' => 'ViewsPluginAccessNodeAccessKeys',
        'uses options' => TRUE,
      ),
    ),
  );
}
