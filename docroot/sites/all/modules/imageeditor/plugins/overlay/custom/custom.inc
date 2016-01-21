<?php
/**
 * @file
 * Custom overlay for Image Editor.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Image Editor custom overlay'),
  //'initialize_callback' => 'imageeditor_overlay_custom_initialize_callback',
  'js' => 'custom_overlay.js',
  'css' => 'custom_overlay.css',
);

function imageeditor_overlay_custom_initialize_callback() {
}
