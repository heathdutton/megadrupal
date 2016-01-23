<?php
/**
 * @file
 * Mail.ru Camera for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Image Editor Mail.ru Camera'),
  'description' => t('Take an image from a webcam'),
  'class' => 'imageeditor-mailru-camera',
  'site' => 'http://mailru.github.io/FileAPI/#en/FileAPI.Camera',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'options' => array(
    'launch_url' => url('imageeditor_webcams/mailru_camera'),
  ),
  'parameters' => array(
    'callback_url' => url(imageeditor_ajax_save_path() . '/imageeditor_mailru_camera', array('absolute' => TRUE)),
  ),
  //'initialize_callback' => 'imageeditor_imageeditor_mailru_camera_initialize_callback',
  'save_callback' => 'imageeditor_imageeditor_mailru_camera_save_callback',
  'js' => 'imageeditor_mailru_camera.js',
  'css' => 'imageeditor_mailru_camera.css',
);

function imageeditor_imageeditor_mailru_camera_initialize_callback(&$editor) {
}

function imageeditor_imageeditor_mailru_camera_save_callback() {
  $directory = imageeditor_temporary_directory();
  if (isset($_POST['data'])) {
    if ($_POST['url'] && $_POST['url'] != 'undefined') {
      $destination = $directory . '/' . basename($_POST['url']);
    }
    else {
      $destination = $directory . '/' . md5($_POST['data']) . '.png';
    }
    list(, $data) = explode(',', $_POST['data']);
    $file = file_unmanaged_save_data(base64_decode($data), $destination);
    drupal_json_output(file_create_url($file));
    drupal_exit();
  }
}
