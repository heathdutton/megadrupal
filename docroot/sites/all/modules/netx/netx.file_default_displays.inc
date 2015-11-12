<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function netx_file_default_displays() {
  $file_displays = array();

  // Configure default displays with initial image styles.
  $default_image_styles = array(
    'teaser' => 'medium',
    'preview' => 'media_thumbnail',
    'default' => ''
  );
  foreach ($default_image_styles as $view_mode => $image_style) {
    $display_name = 'netx__' . $view_mode . '__netx_image';
    $file_displays[$display_name] = (object) array(
      'api_version' => 1,
      'name' => $display_name,
      'status' => 1,
      'weight' => 2,
      'settings' => array('image_style' => $image_style),
    );
  }
  return $file_displays;
}
