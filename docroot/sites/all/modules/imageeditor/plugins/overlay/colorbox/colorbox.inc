<?php
/**
 * @file
 * Colorbox overlay for Image Editor.
 */

/**
 * Plugin declaration.
 */
if (module_exists('colorbox')) {
  $plugin = array(
    'name' => t('Colorbox'),
    //'initialize_callback' => 'imageeditor_overlay_colorbox_initialize_callback',
    'js' => 'colorbox.js',
  );
}

function imageeditor_overlay_colorbox_initialize_callback() {
  // Add Colorbox library defined in colorbox module.
  //drupal_add_library('colorbox', 'colorbox');
}
