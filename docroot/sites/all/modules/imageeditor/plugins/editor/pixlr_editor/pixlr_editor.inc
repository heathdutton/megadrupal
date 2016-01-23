<?php
/**
 * @file
 * Pixlr editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Pixlr Editor'),
  'description' => t('Advanced Photoshop like editor'),
  'class' => 'pixlr-editor',
  'site' => 'http://pixlr.com',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'image',
  'options' => array(
    //'referrer' => 'Drupal: ' . url('', array('absolute' => TRUE)),
    'referrer' => 'Drupal',
    //'icon' => '',
    'exit' => url(imageeditor_ajax_close_path() . '/pixlr_editor', array('absolute' => TRUE)),
    //'image' => '',
    //'title' => '',
    //'type' => '',
    'method' => 'GET',
    'target' => url(imageeditor_ajax_save_path() . '/pixlr_editor', array('absolute' => TRUE)),
    //'redirect' => 'false',
    //'locktarget' => '',
    //'locktitle' => '',
    //'locktype' => '',
    //'quality' => 80,
    //'copy' => '',
    //'maxwidth' => 0,
    //'maxheight' => 0,
    //'wmode' => '',
  ),
  'initialize_callback' => 'imageeditor_pixlr_editor_initialize_callback',
  'save_callback' => 'imageeditor_pixlr_editor_save_callback',
  'js' => 'pixlr_editor.js',
  'css' => 'pixlr_editor.css',
);

function imageeditor_pixlr_editor_initialize_callback(&$editor) {
  global $user, $language, $is_https;
  $editor['options']['launch_url'] = ($is_https ? 'https' : 'http') . '://pixlr.com/editor/';
  $editor['options']['loc'] = property_exists($user, 'language') ? $user->language : $language->language;
}

function imageeditor_pixlr_editor_save_callback() {
  $image = '';
  if (isset($_GET['image'])) {
    $directory = imageeditor_temporary_directory();
    $data = drupal_http_request(rawurldecode($_GET['image']));
    $destination = $directory . '/' . $_GET['title'] . '.' . $_GET['type'];
    $file = file_unmanaged_save_data($data->data, $destination, FILE_EXISTS_REPLACE);
    $image = file_create_url($file);
  }
  $js_code = 'if (parent) {';
  $js_code .= 'parent.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'parent.Drupal.imageeditor.save();';
  $js_code .= 'parent.Drupal.imageeditor.overlay.hide();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}
