<?php
/**
 * @file
 * DeviantART Muro editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('DeviantART Muro'),
  'description' => t('Advanced HTML5 editor'),
  'class' => 'deviantartmuro',
  'site' => 'http://sta.sh/muro/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'options' => array(
    'saveurl' => url(imageeditor_ajax_save_path() . '/deviantartmuro', array('absolute' => TRUE)),
  ),
  'initialize_callback' => 'imageeditor_deviantartmuro_initialize_callback',
  'js' => 'deviantartmuro.js',
  'css' => 'deviantartmuro.css',
);

function imageeditor_deviantartmuro_initialize_callback(&$editor) {
  if (module_exists('libraries')) {
    $path = libraries_get_path('deviantartmuro');
    drupal_add_js($path . '/jquery.deviantartmuro.js');
    $editor['options']['sandbox'] = file_create_url($path . '/html/deviantart_muro_sandbox.html');
  }
  else {
    drupal_add_js('sites/all/libraries/deviantartmuro' . '/jquery.deviantartmuro.js');
    $editor['options']['sandbox'] = file_create_url('sites/all/libraries/deviantartmuro' . '/html/deviantart_muro_sandbox.html');
  }
}
