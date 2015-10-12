<?php

/**
 * @file
 * File hooks for the media_youtube_upload module.
 */

/**
 * Implements hook_file_default_types().
 */
function media_youtube_upload_file_default_types() {
  return array(
    'youtube' => (object) array(
      'api_version' => 1,
      'type' => 'youtube',
      'label' => t('YouTube video'),
      'description' => t("A <em>YouTube video</em> is a video file that has been uploaded through the Media YouTube Upload module."),
      'mimetypes' => array(
        'youtube/*',
      ),
    ),
  );
}

/**
 * Implements hook_file_mimetype_mapping_alter().
 */
function media_youtube_upload_file_mimetype_mapping_alter(&$mapping) {
  $mapping['mimetypes'][] = 'youtube/video';
  $mapping['mimetypes'][] = 'youtube/config';
}

/**
 * Implements hook_file_delete().
 */
function media_youtube_upload_file_delete($file) {
  // If the file is of type youtube.
  if ($file->filemime === 'youtube/video') {
    // And user has selected the automatic deletion checkbox.
    if (variable_get('media_youtube_upload_automatic_deletion', FALSE)) {
      // Delete the video from YouTube.
      $uri_parts = explode('/', $file->uri);
      $video_id = array_pop($uri_parts);
      $ytapi = new MediaYoutubeUploadYtapi();
      $ytapi->deleteVideo($video_id);
    }
  }
  // TODO: Remove references to a file that is in-use.
}

/**
 * Implements hook_file_update().
 */
function media_youtube_upload_file_update($file) {

  if ($file->type === 'youtube') {

    // Get the video id from file uri.
    $uri = $file->uri;
    $uri_parts = explode('/', $uri);
    $video_id = array_pop($uri_parts);

    // Get the field values in a correct key.
    $youtube_fields = field_info_instances('file', 'youtube');
    foreach ($youtube_fields as $field_name => $field) {
      $key = substr(strrchr($field_name, '_'), 1);
      if (isset($file->{"$field_name"}[LANGUAGE_NONE][0]['value'])) {
        $field_values[$key] = $file->{"$field_name"}[LANGUAGE_NONE][0]['value'];
      }
      elseif ($key === 'tags') {
        foreach ($file->{"$field_name"}[LANGUAGE_NONE] as $delta => $tid) {
          $term = taxonomy_term_load($tid['tid']);
          $field_values[$key][$delta] = $term->name;
        }
      }
    }
    // Update the values on YouTube.
    module_load_include('ytapi.inc', 'media_youtube_upload');
    $ytapi = new MediaYoutubeUploadYtapi();
    $ytapi->updateVideo($video_id, $field_values);
  }
}

/**
 * Implements hook_file_formatter_info().
 */
function media_youtube_upload_file_formatter_info() {

  $formatters['media_youtube_upload_image'] = array(
    'label' => t('YouTube Upload Preview Image'),
    'file types' => array('youtube'),
    'default settings' => array(
      'image_style' => '',
    ),
    'view callback' => 'media_youtube_upload_file_formatter_image_view',
    'settings callback' => 'media_youtube_upload_file_formatter_image_settings',
    'mime types' => array('youtube/video'),
  );

  return $formatters;
}

/**
 * Implements hook_file_formatter_info_alter().
 */
function media_youtube_upload_file_formatter_info_alter(&$info) {
  $info['media_youtube_video']['file types'][] = 'youtube';
  $info['media_youtube_video']['mime types'][] = 'youtube/video';
}

/**
 * Implements hook_file_formatter_FORMATTER_view().
 */
function media_youtube_upload_file_formatter_image_view($file, $display, $langcode) {
  $field_language = field_language('file', $file, 'field_file_youtube_thumb');
  $image_style = $display['settings']['image_style'];
  $error = '';
  if (!isset($file->field_file_youtube_thumb[$field_language][0]['fid'])) {
    // Get the video id from file uri.
    $uri_parts = explode('/', $file->uri);
    $video_id = array_pop($uri_parts);

    // Get the (max sized) thumbnail image from YouTube.
    module_load_include('ytapi.inc', 'media_youtube_upload');
    $ytapi = new mediaYoutubeUploadYtapi();
    $imginfo = $ytapi->getTitleThumbs($video_id);

    // Set an error text if we failed to get the thumbnail image.
    if (!empty($imginfo['error'])) {
      $error = $imginfo['error'];
    }
    // If the field of our thumb is empty, proceed.
    else {
      $video_title = $imginfo['title'];

      $remote_path = $imginfo['default_thumb'];
      $scheme     = file_default_scheme();
      $directory  = $scheme . '://youtube_thumbs';

      // Replace the existing one.
      if (file_prepare_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS)) {
        $result = drupal_http_request($remote_path);
        if (!empty($result->error) && $result->code != 404) {
          $error = t('Error @code getting the thumb: @error', array('@code' => $result->code, '@error' => $result->error));
        }

        // If the image is the default placeholder, reject it.
        elseif (md5($result->data) == 'e2ddfee11ae7edcae257da47f3a78a70') {
          $error = t('No thumbnail image generated yet by YouTube');
        }
        else {
          // Create the thumb.
          $code   = floor($result->code / 100) * 100;
          $types  = array('image/jpeg', 'image/png', 'image/gif');
          if (isset($result->data) && $result->data && $code != 500 && in_array($result->headers['content-type'], $types)) {
            $image = file_save_data($result->data, $directory . '/' . $video_id . '.jpg', FILE_EXISTS_REPLACE);
            $file->field_file_youtube_thumb[$field_language][0] = (array) $image;
            field_attach_update('file', $file);
          }
        }
      }
    }
  }
  else {
    $image = file_load($file->field_file_youtube_thumb[$field_language][0]['fid']);
  }
  if(!empty($error)) {
    drupal_set_message(t('An error occurred while getting the thumnail: !error', array('!error' => $error)), 'error');
  }
  else {
    $element = array(
      '#theme' => 'image_style',
      '#style_name' => $image_style,
      '#path' => $image->uri,
      '#alt' => isset($file->override['attributes']['alt']) ? $file->override['attributes']['alt'] : $file->filename,
    );
    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_youtube_upload_file_formatter_image_settings($form, &$form_state, $settings) {
  $element = array();

  $element['image_style'] = array(
    '#title' => t('Image style'),
    '#type' => 'select',
    '#options' => image_style_options(FALSE),
    '#default_value' => $settings['image_style'],
    '#empty_option' => t('None (original image)'),
  );

  return $element;
}

/**
 * Implements hook_file_default_displays().
 */
function media_youtube_upload_file_default_displays() {
  $file_displays = array();

  $file_display = new stdClass();
  $file_display->api_version = 1;
  $file_display->name = 'youtube__preview__media_youtube_upload_image';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'image_style' => 'media_thumbnail',
  );
  $file_displays['youtube__preview__media_youtube_upload_image'] = $file_display;

  return $file_displays;
}
