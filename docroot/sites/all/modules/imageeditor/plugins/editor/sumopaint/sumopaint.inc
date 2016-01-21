<?php
/**
 * @file
 * Sumopaint editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Sumo Paint'),
  'description' => 'NOT WORKING: ' . t('Advanced image editor'),
  'class' => 'sumopaint',
  'site' => 'http://www.sumopaint.com/home/',
  'api_key' => TRUE,
  'api_key_codename' => 'imageeditor_sumopaint_api_key',
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'url',
  'options' => array(
    'key' => variable_get('imageeditor_sumopaint_api_key', ''),
    'launch_url' => 'http://www.sumopaint.com/app/',
    'service' => 'Save to ' . variable_get('site_name', ''),
    //'opt' => 'test',
    'target' => url(imageeditor_ajax_save_path() . '/sumopaint', array('absolute' => TRUE)),
  ),
  // 'initialize_callback' => 'imageeditor_sumopaint_initialize_callback',
  'save_callback' => 'imageeditor_sumopaint_save_callback',
  'settings_form_callback' => 'imageeditor_sumopaint_settings_form_callback',
  'js' => 'sumopaint.js',
  'css' => 'sumopaint.css',
);

function imageeditor_sumopaint_initialize_callback(&$editor) {
}

function imageeditor_sumopaint_save_callback() {
  $directory = imageeditor_temporary_directory();
  $image = '';
  if (isset($_POST['data'])) {
    if ($_POST['url']) {
      $destination = $directory . '/' . basename($_POST['url']);
    }
    else {
      $destination = $directory . '/' . md5($_POST['data']) . '.png';
    }
    $file = file_unmanaged_save_data(base64_decode($_POST['data']), $destination);
    $image = file_create_url($file);
  }
  $js_code = 'if (opener) {';
  $js_code .= 'opener.Drupal.settings.imageeditor.save.image = "' . $image . '";';
  $js_code .= 'opener.Drupal.imageeditor.save();';
  $js_code .= 'self.close();';
  $js_code .= '}';
  drupal_add_js($js_code, 'inline');
}

function imageeditor_sumopaint_settings_form_callback() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Sumo Paint'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $link = 'http://www.sumopaint.com/getpartner/';
  $form['imageeditor_sumopaint_api_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('API key'),
    '#description'   => l($link, $link, array('attributes' => array('target' => '_blank'))),
    '#default_value' => variable_get('imageeditor_sumopaint_api_key', ''),
    '#size'          => 50,
    '#maxlength'     => 100,
    '#required'      => FALSE,
    '#weight'        => 25,
  );

  return $form;
}
