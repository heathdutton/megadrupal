<?php

/**
 * @file
 * Views integration for the Ooyala module.
 */

/**
 * Implements hook_views_plugin().
 */
function ooyala_views_plugins() {
  return array(
    'module' => 'ooyala',
    'style' => array(
      'ooyala_sharedplayer' => array(
        'title' => t('Ooyala shared player'),
        'theme' => 'ooyala_sharedplayer_view',
        'theme path' => drupal_get_path('module', 'ooyala') . '/includes',
        'path' => drupal_get_path('module', 'ooyala') . '/includes',
        'help' => t('Show a video player followed by the resulting nodes of this view. The nodes will play on the same player when clicked.'),
        'handler' => 'ooyala_style_plugin',
        'uses row plugin' => FALSE,
        'uses row class' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => FALSE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

