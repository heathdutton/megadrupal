<?php
/**
 * @file
 * ScriptCam editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Image Editor ScriptCam'),
  'description' => 'NOT WORKING: ' . t('Take an image from a Webcam'),
  'class' => 'imageeditor-scriptcam',
  'site' => 'http://www.scriptcam.com/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'image',
  'options' => array(
    'launch_url' => url('imageeditor_webcams/scriptcam'),
  ),
  // 'initialize_callback' => 'imageeditor_imageeditor_scriptcam_initialize_callback',
  'save_callback' => 'imageeditor_imageeditor_scriptcam_save_callback',
  'js' => 'imageeditor_scriptcam.js',
  'css' => 'imageeditor_scriptcam.css',
);

function imageeditor_imageeditor_scriptcam_initialize_callback(&$editor) {
}

function imageeditor_imageeditor_scriptcam_save_callback() {

}
