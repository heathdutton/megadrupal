<?php

/**
 * @file
 * Default file display configuration for the media_theplatform_mpx module.
 */

/**
 * Implements hook_file_default_displays().
 */
function media_theplatform_mpx_file_default_displays() {
  $file_displays = array();

  // Default settings for displaying as a video.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__default__file_field_media_theplatform_mpx_player_iframe';
  $file_display->weight = 1;
  $file_display->status = TRUE;
  $file_display->settings = array();
  $file_displays['video__default__file_field_media_theplatform_mpx_player_iframe'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__teaser__file_field_media_theplatform_mpx_player_iframe';
  $file_display->weight = 1;
  $file_display->status = TRUE;
  $file_display->settings = array();
  $file_displays['video__teaser__file_field_media_theplatform_mpx_player_iframe'] = $file_display;

  // Default settings for displaying a video preview image. We enable preview
  // images even for view modes that also play video, for use inside a running
  // WYSIWYG editor. The higher weight ensures that the video display is used
  // where possible.
  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__default__media_theplatform_mpx_image';
  $file_display->weight = 2;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
  );
  $file_displays['video__default__media_theplatform_mpx_image'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__teaser__media_theplatform_mpx_image';
  $file_display->weight = 2;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'large',
  );
  $file_displays['video__teaser__media_theplatform_mpx_image'] = $file_display;

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'video__preview__media_theplatform_mpx_image';
  $file_display->weight = -50;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'media_thumbnail',
  );
  $file_displays['video__preview__media_theplatform_mpx_image'] = $file_display;

  return $file_displays;
}
