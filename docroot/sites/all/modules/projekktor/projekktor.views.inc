<?php
/**
 * @file
 * Enables Views module support.
 */

/**
 * Implements hook_views_plugins().
 *
 * The style plugin for Projekktor views.
 */
function projekktor_views_plugins() {
  return array(
    'module' => 'projekktor',
    'style' => array(
      'projekktor' => array(
        'title' => t('Projekktor'),
        'help' => t('Creates playlists in the Projekktor HTML5 media player.'),
        'handler' => 'views_plugin_style_projekktor',
        'theme' => 'views_view_projekktor',
        'theme file' => 'theme.inc',
        'theme path' => drupal_get_path('module', 'projekktor') . '/theme',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
