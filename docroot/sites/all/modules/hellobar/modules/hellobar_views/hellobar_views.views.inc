<?php
/**
 * @file
 * Provide views data and handlers.
 */

/**
 * Implements hook_views_plugins().
 */
function hellobar_views_views_plugins() {
  $module_path = drupal_get_path('module', 'hellobar_views');

  return array(
    'style' => array(
      'hellobar_views_plugin_style_default' => array(
        'title' => t('HelloBar'),
        'help' => t('Push hellobar message'),
        'path' => "$module_path/plugins",
        'handler' => 'HelloBarViewsPluginStyleDefault',
        'parent' => 'default',
        'uses row plugin' => FALSE,
        'uses grouping' => FALSE,
        'uses options' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}
