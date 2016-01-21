<?php
/**
 * @file
 * Pixlr uploader for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Pixlr Upload'),
  'description' => 'CLOSED: ' . t('Upload to http://pixlr.com service'),
  'class' => 'pixlr-upload',
  'site' => 'http://pixlr.com/',
  'api_key' => FALSE,
  'upload_url' => 'http://pixlr.com/store/',
  'parameters' => array(
    'callback_url' => url(imageeditor_ajax_upload_path() . '/pixlr_upload', array('absolute' => TRUE)),
  ),
  'post_callback' => 'imageeditor_uploader_pixlr_upload_post_callback',
  'response_callback' => 'imageeditor_uploader_pixlr_upload_response_callback',
  'js' => 'pixlr_upload.js',
  'css' => 'pixlr_upload.css',
);

function imageeditor_uploader_pixlr_upload_post_callback($filepath) {
  return array(
    'image' => $filepath,
  );
}

function imageeditor_uploader_pixlr_upload_response_callback($response) {
  $output = array(
    'url' => $response,
  );
  drupal_json_output($output);
  drupal_exit();
}
