<?php
/**
 * @file
 * SVG-edit editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('SVG-edit'),
  'description' => 'EXPERIMENTAL: ' . t('Open-source SVG image editor'),
  'class' => 'svgedit',
  'site' => 'http://code.google.com/p/svg-edit/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'image',
  'options' => array(
    'saveurl' => url(imageeditor_ajax_save_path() . '/svgedit', array('absolute' => TRUE)),
  ),
  'initialize_callback' => 'imageeditor_svgedit_initialize_callback',
  'save_callback' => 'imageeditor_svgedit_save_callback',
  'js' => 'svgedit.js',
  'css' => 'svgedit.css',
);

function imageeditor_svgedit_initialize_callback(&$editor) {
  $language_default = language_default();
  $language_default->prefix = '';
  if (module_exists('libraries')) {
    $editor['options']['launch_url'] = url(libraries_get_path('svgedit') . '/svg-editor.html', array('query' => array('extensions' => 'ext-markers.js,ext-connector.js,ext-eyedropper.js,ext-arrows.js,ext-imageeditor.js'), 'absolute' => TRUE, 'language' => $language_default));
  }
  else {
    $editor['options']['launch_url'] = url('sites/all/libraries/svgedit' . '/svg-editor.html', array('query' => array('extensions' => 'ext-markers.js,ext-connector.js,ext-eyedropper.js,ext-arrows.js,ext-imageeditor.js'), 'absolute' => TRUE, 'language' => $language_default));
  }
}

function imageeditor_svgedit_save_callback() {
  $directory = imageeditor_temporary_directory();
  $image = '';
  if (isset($_POST['output_png'])) {
    $destination = $directory . '/' . md5($_POST['output_png']) . '.png';
    $file = file_unmanaged_save_data(base64_decode(substr($_POST['output_png'], 22)), $destination);
    $image = file_create_url($file);
  }
  $js_code = 'if (parent.parent) {';
  $js_code .= 'parent.parent.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'parent.parent.Drupal.imageeditor.save();';
  $js_code .= 'parent.parent.Drupal.imageeditor.overlay.hide();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}
