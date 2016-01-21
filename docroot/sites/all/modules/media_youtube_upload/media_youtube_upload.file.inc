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

  if ($file->type === 'youtube' && $file->filemime === 'youtube/video') {

    // Get the video id from file uri.
    $uri = $file->uri;
    $uri_parts = explode('/', $uri);
    $video_id = array_pop($uri_parts);

    // Get the field values in a correct key.
    $youtube_fields = field_info_instances('file', 'youtube');
    $native_youtube_fields = variable_get('media_youtube_upload_youtube_fields', unserialize(MEDIA_YOUTUBE_UPLOAD_NATIVE_YOUTUBE_FIELDS));
    $file_language = isset($file->translations->original) ? $file->translations->original : entity_language('file', $file);
    foreach ($youtube_fields as $field_name => $field) {
      if (in_array($field_name, $native_youtube_fields)) {
        $key = substr(strrchr($field_name, '_'), 1);
        $items = field_get_items('file', $file, $field_name, $file_language);
        if (isset($items[0]['value']) && $key !== 'tags') {
          $field_values[$key] = $items[0]['value'];
        }
        elseif ($key === 'tags') {
          foreach ($items as $delta => $tid) {
            $term = taxonomy_term_load($tid['tid']);
            $field_values[$key][$delta] = $term->name;
          }
        }
      }
    }
    $translations = array();
    if (isset($file->translations)) {
      $translations['original'] = $file->translations->original;
      foreach($file->translations->data as $lang_code) {
        if ($lang_code['language'] != $file_language) {
          $title = field_get_items('file', $file, 'field_file_youtube_title', $lang_code['language']);
          $translations['translations'][$lang_code['language']]['title'] = $title[0]['value'];
          $description= field_get_items('file', $file, 'field_file_youtube_description', $lang_code['language']);
          $translations['translations'][$lang_code['language']]['description'] = $description[0]['value'];
        }
      }
    }
    // Update the values on YouTube.
    module_load_include('ytapi.inc', 'media_youtube_upload');
    $ytapi = new MediaYoutubeUploadYtapi();
    $ytapi->updateVideo($video_id, $field_values, $translations);
  }
}

/**
 * Implements hook_file_presave().
 */
function media_youtube_upload_file_presave($file) {
  // Need to always reset the filemime because the media_youtube module tries to
  // make it a "video/youtube" mimetype again because of the filepath.
  if ($file->type === 'youtube') {
    $file->filemime = isset($file->original->filemime) ? $file->original->filemime : $file->filemime;
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

  $formatters['media_youtube_upload_youtube_download_link'] = array(
    'label' => t('YouTube Upload Download Link'),
    'file types' => array('youtube'),
    'default settings' => array(
      'quality' => 'best',
      'mode' => 'direct',
      'format' => 'video/mp4',
      'type' => 'single_link',
    ),
    'view callback' => 'media_youtube_upload_file_formatter_youtube_download_link_view',
    'settings callback' => 'media_youtube_upload_file_formatter_youtube_download_link_settings',
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
function media_youtube_upload_file_formatter_youtube_download_link_view($file, $display, $langcode) {

  $scheme = file_uri_scheme($file->uri);

  if ($scheme == 'youtube') {
    // Get the video id from file uri.
    $uri_parts = explode('/', $file->uri);
    $video_id = array_pop($uri_parts);

    $element = array(
      '#theme' => 'media_youtube_upload_youtube_download_link',
      '#video_id' => $video_id,
      '#filename' => $file->filename,
      '#options' => array(),
    );

    // Fake a default for attributes so the ternary doesn't choke.
    $display['settings']['attributes'] = array();

    foreach (array('format', 'quality', 'mode', 'type') as $setting) {
      $element['#options'][$setting] = isset($file->override[$setting]) ? $file->override[$setting] : $display['settings'][$setting];
    }

    return $element;
  }
}

/**
 * Implements hook_file_formatter_FORMATTER_settings().
 */
function media_youtube_upload_file_formatter_youtube_download_link_settings($form, &$form_state, $settings) {
  $element = array();

  $element['type'] = array(
    '#title' => t('Type'),
    '#description' => t('Select your preferred way for downloading the video.'),
    '#type' => 'select',
    '#options' => array(
      'single_link' => t('Single Link'),
      'multiple_links' => t('Multiple Links'),
      // Not implemented yet.
      // 'select' => t('Selectbox'),
    ),
    '#default_value' => $settings['type'],
  );

  $element['format'] = array(
    '#title' => t('Format'),
    '#description' => t('Select your preferred video format to download. Only applicable to single link downloads.'),
    '#type' => 'select',
    '#options' => array(
      'video/mp4' => t('MP4'),
      'video/webm' => t('WEBM'),
      'video/x-flv' => t('FLV'),
    ),
    '#default_value' => $settings['format'],
  );

  // Not implemented yet.
//  $element['quality'] = array(
//    '#title' => t('Quality'),
//    '#description' => t('Select your preferred video quality to download. Only applicable to single link downloads'),
//    '#type' => 'select',
//    '#options' => array(
//      'hd720' => t('Best'),
//      'medium' => t('Medium'),
//      'small' => t('Low'),
//    ),
//    '#default_value' => $settings['quality'],
//  );

  $element['mode'] = array(
    '#title' => t('Download mode'),
    '#description' => t('Select your preferred video download mode.'),
    '#type' => 'select',
    '#options' => array(
      'direct' => t('Direct'),
      'proxy' => t('Proxy'),
      // Not implemented yet.
      // 'both' => t('Both'),
    ),
    '#default_value' => $settings['mode'],
  );

  return $element;
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
  $file_display->name = 'youtube__default__media_youtube_video';
  $file_display->weight = 0;
  $file_display->status = TRUE;
  $file_display->settings = array(
    'width' => '640',
    'height' => '360',
    'theme' => 'dark',
    'color' => 'red',
    'autohide' => '2',
    'autoplay' => 0,
    'loop' => 0,
    'showinfo' => 0,
    'modestbranding' => 0,
    'rel' => 1,
    'nocookie' => 0,
    'protocol_specify' => 0,
    'protocol' => 'https:',
    'enablejsapi' => 0,
    'origin' => '',
  );
  $file_displays['youtube__default__media_youtube_video'] = $file_display;

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
