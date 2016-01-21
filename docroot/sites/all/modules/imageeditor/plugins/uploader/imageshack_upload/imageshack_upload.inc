<?php
/**
 * @file
 * Imageshack uploader for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('ImageShack Upload'),
  'description' => t('Upload to http://imageshack.us service'),
  'class' => 'imageshack-upload',
  'site' => 'http://imageshack.us/',
  'api_key' => TRUE,
  'api_key_codename' => 'imageeditor_imageshack_api_key',
  'upload_url' => 'http://www.imageshack.us/upload_api.php',
  'parameters' => array(
    'callback_url' => url(imageeditor_ajax_upload_path() . '/imageshack_upload', array('absolute' => TRUE)),
  ),
  'post_callback' => 'imageeditor_uploader_imageshack_upload_post_callback',
  'response_callback' => 'imageeditor_uploader_imageshack_upload_response_callback',
  'settings_form_callback' => 'imageeditor_imageshack_upload_settings_form_callback',
  'js' => 'imageshack_upload.js',
  'css' => 'imageshack_upload.css',
);

function imageeditor_uploader_imageshack_upload_post_callback($filepath) {
  // Imageshack needs mimetype to be added.
  $type = file_get_mimetype($filepath);
  $filepath = $filepath . ';type=' . $type;
  return array(
    'fileupload' => $filepath,
    'key' => variable_get('imageeditor_imageshack_api_key', ''),
    'xml' => 'yes',
  );
}

function imageeditor_uploader_imageshack_upload_response_callback($response) {
  $xml = simplexml_load_string($response);
  if ($xml) {
    if (isset($xml->error)) {
      $output = array(
        'error' => (string) $xml->error,
      );
    }
    else {
      $output = array(
        'url' => (string) $xml->links->image_link,
      );
    }
  }
  else {
    $output = array(
      'error' => t('An error occurred.'),
    );
  }
  drupal_json_output($output);
  drupal_exit();
}

function imageeditor_imageshack_upload_settings_form_callback() {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('ImageShack Upload'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $link = 'http://stream.imageshack.us/xmlapi/';
  $form['imageeditor_imageshack_api_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('API key'),
    '#description'   => l($link, $link, array('attributes' => array('target' => '_blank'))),
    '#default_value' => variable_get('imageeditor_imageshack_api_key', ''),
    '#size'          => 50,
    '#maxlength'     => 100,
    '#required'      => FALSE,
    '#weight'        => 25,
  );

  return $form;
}
