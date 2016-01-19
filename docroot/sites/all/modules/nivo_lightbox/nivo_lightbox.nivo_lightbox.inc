<?php

/**
 * @file
 * Declare default themes included with the Nivo Lightbox jQuery plugin.
 */

/**
 * Implements hook_nivo_lightbox_theme_info().
 */
function nivo_lightbox_nivo_lightbox_theme_info() {
  // Find the path to the Nivo Lightbox jQuery plugin
  $library_path = libraries_get_path('nivo-lightbox');

  return array(
    'default' => array(
      'name' => t('Default'),
      'description' => t('The default skin for the Nivo Lightbox.'),
      'thumb_support' => TRUE,
      'resources' => array(
        'css' => array(
          $library_path . '/themes/default/default.css',
        ),
      ),
    ),
  );
}
