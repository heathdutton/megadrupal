<?php
/**
 * @file
 * Paintweb editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('PaintWeb'),
  'description' => t('Open-source HTML 5 image editor'),
  'class' => 'paintweb',
  'site' => 'http://code.google.com/p/paintweb/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'options' => array(
    'configFile' => 'config-example.json',
    'saveurl' => url(imageeditor_ajax_save_path() . '/paintweb', array('absolute' => TRUE)),
  ),
  'initialize_callback' => 'imageeditor_paintweb_initialize_callback',
  'save_callback' => 'imageeditor_paintweb_save_callback',
  'js' => 'paintweb.js',
  'css' => 'paintweb.css',
);

function imageeditor_paintweb_initialize_callback(&$editor) {
  if (module_exists('libraries')) {
    drupal_add_js(libraries_get_path('paintweb') . '/build/paintweb.src.js');
  }
  else {
    drupal_add_js('sites/all/libraries/paintweb' . '/build/paintweb.src.js');
  }
}

function imageeditor_paintweb_save_callback() {
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
