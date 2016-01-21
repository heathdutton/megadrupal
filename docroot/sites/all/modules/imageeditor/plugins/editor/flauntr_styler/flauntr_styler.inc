<?php
/**
 * @file
 * flauntR stylR editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('flauntR stylR'),
  'description' => 'NOT SAVING BACK: ' . t('Advanced image editor'),
  'class' => 'flauntr-styler',
  'site' => 'http://www.flauntr.com/',
  'api_key' => FALSE,
  'image_creation' => TRUE,
  'launch_type' => 'overlay',
  'image_url_param' => 'url',
  'options' => array(
    'launch_url' => 'http://www.flauntr.com/flauntr/applauncher.do',
    'app' => 'styler',
  ),
  // 'initialize_callback' => 'imageeditor_flauntr_styler_initialize_callback',
  'js' => 'flauntr_styler.js',
  'css' => 'flauntr_styler.css',
);

function imageeditor_flauntr_styler_initialize_callback(&$editor) {
}
