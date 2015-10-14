<?php
/**
 * @file
 * Stream wrapper fro Brightcove videos.
 */

class MediaBrightcoveStreamWrapper extends MediaReadOnlyStreamWrapper {
  public function getTarget() {
    return FALSE;
  }

  public static function getMimeType($uri, $mapping = NULL) {
    return 'video/brightcove';
  }

  public function interpolateUrl() {
    // Load the default brightcove player.
    $player = brightcove_default_player();
    // Parse the video id.
    $video_id = $this->getVideoID();
    if (!is_null($video_id) && !is_null($player)) {
      if ($player) {
        $path = brightcove_get_viewer_url();

        return url($path, array(
            'query' => array(
              'playerID' => $player->player_id,
              'playerKey' => $player->player_key,
              'isVid' => 'true',
              'isUI' => 'true',
              '@videoPlayer' => $video_id,
            ),
          ));
      }
      else {
        watchdog('brightcove', 'Video Player is missing.', array(), WATCHDOG_ERROR);
      }
    }
    return NULL;
  }

  public function getVideoID() {
    if ($url = parse_url($this->uri)) {
      if ($url['scheme'] == 'brightcove' && is_numeric($url['host'])) {
        return $url['host'];
      }
    }

    return NULL;
  }

  public function setUri($uri) {
    $this->uri = $uri;
  }

  function getOriginalThumbnailPath() {
    $video = brightcove_video_load($this->getVideoID());

    if (!empty($video->videoStillURL)) {
      return $video->videoStillURL;
    }

    if (!empty($video->thumbnailURL)) {
      return $video->thumbnailURL;
    }

    return FALSE;
  }

  /**
   * Get the local thumbnail path of the bc video.
   *
   * @return string
   *  The local path of the thumbnail image.
   */
  function getLocalThumbnailPath() {
    $video_id = $this->getVideoID();
    $thumbnail_path = 'public://media-brightcove/' . $video_id . '.jpg';

    // Return with the video thumbnail path if the file exists.
    if (file_exists($thumbnail_path)) {
      return $thumbnail_path;
    }

    // Prepare the directory for the thumbnails.
    $thumbnail_directory_name = drupal_dirname($thumbnail_path);
    file_prepare_directory($thumbnail_directory_name, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

    $original_path = $this->getOriginalThumbnailPath();
    // Try to download the OriginalThumbnail.
    if (!empty($original_path)) {
      $response = drupal_http_request($original_path);
      if (!empty($response->data)) {
        file_unmanaged_save_data($response->data, $thumbnail_path, FILE_EXISTS_ERROR);
        return $thumbnail_path;
      }
    }

    // If there was no thumbnail obtained, use a default icon.
    $default_path = 'public://media-brightcove/default.jpg';
    if (!file_exists($default_path)) {
      // Save a copy from the default image under files folder.
      $bc_default_thumbnail = variable_get('brightcove_default_thumbnail', brightcove_get_default_image());
      file_unmanaged_copy($bc_default_thumbnail, $default_path, FILE_EXISTS_REPLACE);
    }

    return $default_path;
  }
}
