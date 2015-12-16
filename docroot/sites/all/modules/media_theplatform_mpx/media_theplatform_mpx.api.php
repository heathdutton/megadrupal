<?php

/**
 * @file
 * API integration for the media_theplatform_mpx module.
 */

/**
 * Act on an mpx account being loaded.
 *
 * @param MpxAccount $account
 *   The mpx account that is being loaded.
 */
function hook_media_theplatform_mpx_account_load(MpxAccount $account) {
  // @todo Add example implementation.
}

/**
 * Act on an mpx account being inserted or updated.
 *
 * @param MpxAccount $account
 *   The mpx account that is being inserted or updated.
 */
function hook_media_theplatform_mpx_account_presave(MpxAccount $account) {
  // @todo Add example implementation.
}

/**
 * Respond to creation of a new mpx account.
 *
 * @param MpxAccount $account
 *   The mpx account that is being created.
 */
function hook_media_theplatform_mpx_account_insert(MpxAccount $account) {
  // @todo Add example implementation.
}

/**
 * Respond to updates to an mpx account.
 *
 * @param MpxAccount $account
 *   The mpx account that is being updated.
 */
function hook_media_theplatform_mpx_account_update(MpxAccount $account) {
  // @todo Add example implementation.
}

/**
 * Respond to mpx account deletion.
 *
 * @param MpxAccount $account
 *   The mpx account that is being deleted.
 */
function hook_media_theplatform_mpx_account_delete(MpxAccount $account) {
  // @todo Add example implementation.
}

/**
 * Alter video metadata prior to import.
 *
 * @param array $video_item
 *   The processed video data. When this hook is complete this variable will be
 *   passed into media_theplatform_mpx_import_video().
 * @param array $video
 *   The array of original data from the media API.
 * @param MpxAccount $account
 *   The account that corresponds to the video.
 */
function hook_media_theplatform_mpx_media_import_item_alter(array &$video_item, array $video, MpxAccount $account) {
  // @todo Add more examples of common alterations.

  // Do not import any videos that mention dogs in the title.
  if (preg_match('/\bdogs?\b/i', $video_item['title'])) {
    $video_item['ignore'] = TRUE;
  }
}

/**
 * Alter an mpx API HTTP request.
 *
 * @param string $url
 *   The URL of the request.
 * @param array $params
 *   An optional array of query parameters.
 * @param array $options
 *   An optional array of request options to pass to drupal_http_request().
 */
function hook_media_theplatform_mpx_api_request_alter(&$url, array &$params, array &$options) {
  // Add a category filter.
  if (strpos($url, '//data.media.theplatform.com/media/data/Media') !== FALSE) {
    $params['byCategories'] = 'Test category';
  }
}

/**
 * @todo Document this hook.
 * @todo Deprecate this hook.
 *
 * @param string $url
 * @param array $options
 */
function hook_media_theplatform_mpx_feed_request_alter(&$url, array &$options) {
  // Add a category filter.
  if (strpos($url, '//data.media.theplatform.com/media/data/Media') !== FALSE) {
    $url .= '&byCategories=' . rawurlencode('Test category');
  }
}

/**
 * @todo Document this hook.
 * @todo Deprecate this hook.
 *
 * @param string $file_type
 * @param object $file
 * @param $account
 */
function hook_media_theplatform_mpx_file_type($file_type, $file, $account) {
  // @todo Add example implementation.
}

/**
 * @todo Document this hook
 *
 * @param $op
 * @param $video
 * @param MpxAccount $account
 */
function hook_media_theplatform_mpx_import_media($op, $video, MpxAccount $account) {
  // @todo Add example implementation.
}

/**
 * Alter a video player iframe element.
 *
 * @param array $element
 *   The render API array containing the iframe element to be rendered.
 * @param array $variables
 *   The variables array from theme_media_theplatform_mpx_player_iframe().
 *
 * @see theme_media_theplatform_mpx_player_iframe()
 */
function hook_media_theplatform_mpx_player_iframe_alter(array &$element, array $variables) {
  // Enable auto-play for all videos.
  $element['#attributes']['src']['options']['query']['autoPlay'] = 'true';
}

/**
 * Respond to creation of a new mpx video.
 *
 * @param int $fid
 *   The file ID of the mpx video file entity.
 * @param array $video
 *   Imported metadata array from mpx.
 * @param MpxAccount $account
 *   The mpx account associated with the video.
 *
 * @see media_theplatform_mpx_insert_video()
 */
function hook_media_theplatform_mpx_insert_video($fid, array $video, MpxAccount $account) {
  if ($fid && $file = file_load($fid)) {
    $file->field_guid[LANGUAGE_NONE][0]['value'] = $video['guid'];
    file_save($file);
  }
}

/**
 * Respond to updates to an mpx video.
 *
 * @param int $fid
 *   The file ID of the mpx video file entity.
 * @param array $video
 *   Imported metadata array from mpx.
 * @param MpxAccount $account
 *   The mpx account associated with the video.
 *
 * @see media_theplatform_mpx_update_video()
 */
function hook_media_theplatform_mpx_update_video($fid, array $video, MpxAccount $account) {
  if ($fid && $file = file_load($fid)) {
    $file->field_guid[LANGUAGE_NONE][0]['value'] = $video['guid'];
    file_save($file);
  }
}

/**
 * @todo Document this hook.
 * @todo Deprecate this hook.
 *
 * @param $id
 * @param $op
 *
 * @see media_theplatform_mpx_set_mpx_video_inactive()
 */
function hook_media_theplatform_mpx_set_video_inactive($id, $op) {
  // @todo Add example implementation.
}

/**
 * Alter the image of an mpx video.
 *
 * @param string $uri
 *   The image URI.
 * @param object $file
 *   The mpx file object.
 */
function hook_media_theplatform_mpx_image_uri_alter(&$uri, $file) {
  if ($file->filemime === 'player/mpx') {
    // Use the default mpx player image in the curren theme.
    $uri = $GLOBALS['theme_path'] . '/images/mpx-player.jpg';
  }
}
