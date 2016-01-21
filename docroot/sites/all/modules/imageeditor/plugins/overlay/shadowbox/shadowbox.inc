<?php
/**
 * @file
 * Shadowbox overlay for Image Editor.
 */

/**
 * Plugin declaration.
 */
if (module_exists('shadowbox')) {
  $plugin = array(
    'name' => t('Shadowbox'),
    //'initialize_callback' => 'imageeditor_overlay_shadowbox_initialize_callback',
    'js' => 'shadowbox.js',
  );
}

function imageeditor_overlay_shadowbox_initialize_callback() {
  // Add Shadowbox library defined in shadowbox module.
  //drupal_add_library('shadowbox', 'shadowbox');
}
