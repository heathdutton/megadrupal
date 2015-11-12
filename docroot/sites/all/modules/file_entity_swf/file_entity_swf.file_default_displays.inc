<?php

/**
 * @file
 * Default display configuration for Flash file type.
 */

/**
 * Implements hook_file_default_displays().
 */
function file_entity_swf_file_default_displays() {
  $file_displays = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'flash__default__file_swf';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array();
  $file_displays['flash__default__file_swf'] = $file_display;

  // Flash previews should be displayed using a large filetype icon.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'flash__preview__file_field_media_large_icon';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $file_displays['flash__preview__file_field_media_large_icon'] = $file_display;

  // Flash media_wysiwyg should be displayed using a large filetype icon.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'flash__wysiwyg__file_field_media_large_icon';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = '';
  $file_displays['flash__wysiwyg__file_field_media_large_icon'] = $file_display;

  return $file_displays;
}
