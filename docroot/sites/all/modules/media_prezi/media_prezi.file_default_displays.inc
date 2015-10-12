<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_prezi_file_default_displays() {
  $file_displays = array();
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'presentation__default__media_prezi_presentation';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '550',
    'height' => '400',
    'lock_to_path' => '0',
  );
  $file_displays['presentation__default__media_prezi_presentation'] = $file_display;
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__preview__media_prezi_image';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'media_thumbnail',
  );
  $file_displays['presentation__preview__media_prezi_image'] = $file_display;
  return $file_displays;
}
