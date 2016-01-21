<?php
/**
 * @file
 * fancyBox overlay for Image Editor.
 */

/**
 * Plugin declaration.
 */
if (module_exists('fancybox')) {
  $plugin = array(
    'name' => t('fancyBox'),
    //'initialize_callback' => 'imageeditor_overlay_fancybox_initialize_callback',
    'js' => 'fancybox.js',
  );
}

function imageeditor_overlay_fancybox_initialize_callback() {
  // Add fancyBox library defined in fancybox module.
  //drupal_add_library('fancybox', 'fancybox');
}
