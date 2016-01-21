<?php
/**
 * @file
 * Image Editor Inline Revert editor for Image Editor module.
 */

/**
 * Plugin declaration.
 */
$plugin = array(
  'name' => t('Image Editor Inline Revert'),
  'description' => t('Image Editor Inline: Revert to the previous image version'),
  'class' => 'imageeditor-inline-revert',
  'site' => url(''),
  'api_key' => FALSE,
  'launch_type' => 'overlay',
  'image_url_param' => 'image',
  'options' => array(
    'launch_url' => url('imageeditor_inline/revert'),
  ),
  'js' => 'imageeditor_inline_revert.js',
  'css' => 'imageeditor_inline_revert.css',
);
