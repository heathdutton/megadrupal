<?php
/**
 * @file
 * Webcam editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Image Editor Webcam'),
  'description' => 'NOT WORKING: ' . t('Take an image from a Webcam'),
  'class' => 'imageeditor-webcam',
  'site' => 'http://www.xarg.org/project/jquery-webcam-plugin/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'image',
  'options' => array(
    'launch_url' => url('imageeditor/webcam'),
  ),
  // 'initialize_callback' => 'imageeditor_imageeditor_webcam_initialize_callback',
  'save_callback' => 'imageeditor_imageeditor_webcam_save_callback',
  'js' => 'imageeditor_webcam.js',
  'css' => 'imageeditor_webcam.css',
);

function imageeditor_imageeditor_webcam_initialize_callback(&$editor) {
}

function imageeditor_imageeditor_webcam_save_callback() {
  $directory = imageeditor_temporary_directory();
  $image = '';
  if ($postdata = file_get_contents('php://input')) {
    $destination = $directory . '/' . md5($postdata) . '.jpg';
    $file = file_unmanaged_save_data(pack('H*', $postdata), $destination);
    $image = file_create_url($file);
  }
  $js_code = 'if (parent) {';
  $js_code .= 'parent.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'parent.Drupal.imageeditor.save();';
  $js_code .= 'parent.Drupal.imageeditor.overlay.hide();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}
