<?php

/**
 * @file
 * Views include file for zoundation_views.
 */

/**
 * Implements hook_views_plugins().
 */
function zoundation_views_views_plugins() {
  $plugins = array();

  $plugins['style']['zoundation_views_block_grid'] = array(
    'title' => t('Zoundation block grid'),
    'help' => t('Displays rows in a responsive grid'),
    'handler' => 'views_plugin_style_zoundation_views_block_grid',
    'theme' => 'zoundation_views_block_grid',
    'uses row plugin' => TRUE,
    'uses row class' => TRUE,
    'uses options' => TRUE,
    'type' => 'normal',
    'path' => drupal_get_path('module', 'zoundation_views') . '/views',
  );

  return $plugins;
}
