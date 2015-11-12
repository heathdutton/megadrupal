<?php
/**
 * @file
 * Views hook.
 * See http://drupalcontrib.org/api/drupal/contributions%21views%21views.api.php/function/hook_views_plugins/7
 */

/**
 * Implements hook_views_plugins().
 */
function views_mediatable_views_plugins() {
  $theme_path = drupal_get_path('module', 'views_mediatable') . '/views';
  return array(
    'module' => 'views_mediatable',
    'style' => array(
      'mediatable' => array(
        'title' => t('MediaTable Table'),
        'help' => t('Adds the ability to have responsive options to the table display.'),
        'path' => $theme_path,
        'theme path' => $theme_path,
        'handler' => 'views_mediatable_plugin_style_table',
        'parent' => 'table',
        'theme' => 'views_mediatable_view',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
