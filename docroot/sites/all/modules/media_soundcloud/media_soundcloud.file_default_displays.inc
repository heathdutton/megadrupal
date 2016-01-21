<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_soundcloud_file_default_displays() {
  $file_displays = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__default__media_soundcloud_audio';
  $file_display->weight = 1;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '640',
    'height' => '450',
    'color' => '',
    'protocol_specify' => 0,
    'protocol' => 'https://',
    'auto_play' => 0,
    'visual' => 1,
    'show_artwork' => 1,
    'show_comments' => 1,
    'api' => 0,
  );
  $file_displays['audio__default__media_soundcloud_audio'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__preview__media_soundcloud_image';
  $file_display->weight = 2;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'media_thumbnail',
  );
  $file_displays['audio__preview__media_soundcloud_image'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'audio__teaser__media_soundcloud_audio';
  $file_display->weight = 1;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '100%',
    'height' => '166',
    'color' => '',
    'protocol_specify' => 0,
    'protocol' => 'https://',
    'auto_play' => 0,
    'visual' => 0,
    'show_artwork' => 1,
    'show_comments' => 1,
    'api' => 0,
  );
  $file_displays['audio__teaser__media_soundcloud_audio'] = $file_display;

  return $file_displays;
}
