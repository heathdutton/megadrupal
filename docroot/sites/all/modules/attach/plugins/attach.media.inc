<?php
/**
 * Embed external media.
 *
 * Syntax:
 *   [embed_media|url=...|...]
 *
 * Examples:
 *   [embed_media|url=http://www.youtube.com/watch?v=9bZkp7q19f0|width=300]
 */
function attach_media_process($params) {
  if (!file_exists('sites/all/libraries/MediaWrapper/MediaWrapper/MediaWrapper.php')) {
    return;
  }

  include_once 'sites/all/libraries/MediaWrapper/MediaWrapper/MediaWrapper.php';
  $wrapper = MediaWrapper::getInstance()->getWrapper($params['url']);
  return $wrapper->player($params);
}

