<?php
/**
 * @file
 * jQuery UI Dialog overlay for Image Editor.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('jQuery UI Dialog'),
  'initialize_callback' => 'imageeditor_overlay_jqueryuidialog_initialize_callback',
  'js' => 'jqueryuidialog.js',
);

function imageeditor_overlay_jqueryuidialog_initialize_callback() {
  // Add jQueryUI.dialog.
  drupal_add_library('system', 'ui.dialog');
}
