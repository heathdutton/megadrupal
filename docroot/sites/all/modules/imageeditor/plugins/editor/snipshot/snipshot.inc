<?php
/**
 * @file
 * Snipshot editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Snipshot'),
  'description' => 'CLOSED: ' . t('Snipshot editor'),
  'class' => 'snipshot',
  'site' => 'http://snipshot.com/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'snipshot_input',
  'options' => array(
    'launch_url' => 'http://services.snipshot.com',
    'snipshot_callback' => url(imageeditor_ajax_save_path() . '/snipshot', array('absolute' => TRUE)),
    'snipshot_callback_agent' => 'user', //either 'snipshot' or 'user'
    'snipshot_output' => 'image',
  ),
  // 'initialize_callback' => 'imageeditor_snipshot_initialize_callback',
  'save_callback' => 'imageeditor_snipshot_save_callback',
  'js' => 'snipshot.js',
  'css' => 'snipshot.css',
);

function imageeditor_snipshot_initialize_callback(&$editor) {
}

function imageeditor_snipshot_save_callback() {
  $image = '';
  if (isset($_GET['image'])) {
    $image = rawurldecode($_GET['image']);
  }
  $js_code = 'if (parent) {';
  $js_code .= 'parent.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'parent.Drupal.imageeditor.save();';
  $js_code .= 'parent.Drupal.imageeditor.overlay.hide();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}
