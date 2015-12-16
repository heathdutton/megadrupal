<?php
/**
 * @file
 * Blocksit views hooks.
 */

/**
 * Implements hook_views_plugins().
 */
function views_blocksit_views_plugins() {
  $module_path = drupal_get_path('module', 'views_blocksit');
  return array(
    'style' => array(
      'blocksit' => array(
        'title' => t('Blocksit'),
        'help' => t('Displays content using the Blocksit library'),
        'handler' => 'views_blocksit_plugin_style_blocksit',
        'theme' => 'views_blocksit_grid',
        'theme path' => $module_path . '/theme',
        'theme file' => 'views_blocksit.theme.inc',
        'uses row plugin' => TRUE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => FALSE,
      ),
    ),
  );
}
