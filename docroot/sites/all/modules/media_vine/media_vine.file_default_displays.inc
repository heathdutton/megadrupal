<?php

/**
 * @file
 * Default display configuration for the default file types.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_vine_file_default_displays() {
  $file_displays = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__default__media_vine_video';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '600',
    'height' => '600',
    'embed_format' => 'simple',
    'autoplay_audio' => 0,
  );
  $file_displays['video__default__media_vine_video'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__preview__media_vine_image';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'media_thumbnail',
  );
  $file_displays['video__preview__media_vine_image'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__teaser__media_vine_video';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '320',
    'height' => '320',
    'embed_format' => 'simple',
    'autoplay_audio' => 0,
  );
  $file_displays['video__teaser__media_vine_video'] = $file_display;

  return $file_displays;
}
