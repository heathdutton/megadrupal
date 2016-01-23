<?php
/**
 * @file
 * Imm.io uploader for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('imm.io Upload'),
  'description' => 'CLOSED: ' . t('Upload to http://imm.io service'),
  'class' => 'immio-upload',
  'site' => 'http://imm.io/api/',
  'api_key' => FALSE,
  'upload_url' => 'http://imm.io/store/',
  'parameters' => array(
    'callback_url' => url(imageeditor_ajax_upload_path() . '/immio_upload', array('absolute' => TRUE)),
  ),
  'post_callback' => 'imageeditor_uploader_immio_upload_post_callback',
  'response_callback' => 'imageeditor_uploader_immio_upload_response_callback',
  'js' => 'immio_upload.js',
  'css' => 'immio_upload.css',
);

function imageeditor_uploader_immio_upload_post_callback($filepath) {
  return array(
    'image' => $filepath,
  );
}

function imageeditor_uploader_immio_upload_response_callback($response) {
  $result = drupal_json_decode($response);
  if ($result['success']) {
    $output = array(
      'url' => $result['payload']['uri'],
    );
  }
  else {
    $output = array(
      'error' => $result['payload'],
    );
  }
  drupal_json_output($output);
  drupal_exit();
}
