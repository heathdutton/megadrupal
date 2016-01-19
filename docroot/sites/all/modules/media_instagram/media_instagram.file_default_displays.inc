<?php

/**
 * @file
 * Default display configuration for the default Instagram file type.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_instagram_file_default_displays() {
  $file_displays = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'instagram__default__media_instagram';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array('maxwidth' => '', 'hidecaption' => FALSE);
  $file_displays['instagram__default__media_instagram'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'instagram__media_preview__media_instagram';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array('maxwidth' => 320, 'hidecaption' => TRUE);
  $file_displays['instagram__media_preview__media_instagram'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'instagram__media_small__media_instagram';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array('maxwidth' => 320, 'hidecaption' => TRUE);
  $file_displays['instagram__media_small__media_instagram'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'instagram__media_original__media_instagram';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array('maxwidth' => '', 'hidecaption' => FALSE);
  $file_displays['instagram__media_original__media_instagram'] = $file_display;

  return $file_displays;
}
