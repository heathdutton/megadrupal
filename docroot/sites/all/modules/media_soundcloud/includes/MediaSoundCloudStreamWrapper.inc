<?php

/**
 *  @file
 *  Extends the MediaReadOnlyStreamWrapper class to handle SoundCloud audio.
 */

/**
 *  Create an instance like this:
 *  $soundcloud = new MediaSoundCloudStreamWrapper('soundcloud://u/[user-name]/a/[audio-code]');
 */
class MediaSoundCloudStreamWrapper extends MediaReadOnlyStreamWrapper {
  protected $base_url = 'http://soundcloud.com/';
  protected $parameters = array('u','a','g','s');

  static function getMimeType($uri, $mapping = NULL) {
    return 'audio/soundcloud';
  }

  function interpolateUrl() {
    $url = "";

    // User.
    if (isset($this->parameters['u'])) {
      $url = $this->base_url . check_plain($this->parameters['u']);
    }

    // Group.
    if (isset($this->parameters['g'])) {
      $url = $this->base_url . 'groups/' . check_plain($this->parameters['g']);
    }

    // Single song.
    if (isset($this->parameters['u']) && isset($this->parameters['a'])) {
      $url = $this->base_url . check_plain($this->parameters['u']) . '/' . check_plain($this->parameters['a']);
    }

    // Audio sets.
    if (isset($this->parameters['u']) && isset($this->parameters['s'])) {
      $url = $this->base_url . check_plain($this->parameters['u']) . '/sets/' . check_plain($this->parameters['s']);
    }

    return $url;
  }

  function getOriginalThumbnailPath() {
    $parts = $this->get_parameters();
    $uri = '';

    // User.
    if (isset($parts['u'])) {
      $uri = 'soundcloud://u/' . check_plain($parts['u']);;
    }

    // Group.
    if (isset($parts['g'])) {
      $uri = 'soundcloud://g/' . check_plain($parts['g']);
    }

    // Single song.
    if (isset($parts['u']) && isset($parts['a'])) {
      $uri = 'soundcloud://u/' . check_plain($parts['u']) . '/a/' . check_plain($parts['a']);
    }

    // Audio sets.
    if (isset($parts['u']) && isset($parts['s'])) {
      $uri = 'soundcloud://u/' . check_plain($parts['u']) . '/s/' . check_plain($parts['s']);
    }

    $uri = file_stream_wrapper_uri_normalize($uri);
    $external_url = file_create_url($uri);
    $oembed_url = url('http://soundcloud.com/oembed', array('query' => array('url' => $external_url)));
    $response = drupal_http_request($oembed_url);

    if (!isset($response->error)) {
      $data = new SimpleXMLElement($response->data);
      return $data->{'thumbnail-url'};
    }
    else {
      throw new Exception("Error Processing Request. (Error: {$response->code}, {$response->error})");
      return;
    }
  }

  function getLocalThumbnailPath() {
    $parts = $this->get_parameters();
    $local_path = '';

    // User.
    if (isset($parts['u'])) {
      $local_path = 'public://media-soundcloud/u/' . check_plain($parts['u']) . '.jpg';
    }

    // Group.
    if (isset($parts['g'])) {
      $local_path = 'public://media-soundcloud/g/' . check_plain($parts['g']) . '.jpg';
    }

    // Single song.
    if (isset($parts['u']) && isset($parts['a'])) {
      $local_path = 'public://media-soundcloud/u/' . check_plain($parts['u']) . '/a/'.check_plain($parts['a']).'.jpg';
    }

    // Audio sets.
    if (isset($parts['u']) && isset($parts['s'])) {
      $local_path = 'public://media-soundcloud/u/' . check_plain($parts['u']) . '/s/'.check_plain($parts['s']).'.jpg';
    }

    if (!file_exists($local_path)) {
      $dirname = drupal_dirname($local_path);
      file_prepare_directory($dirname, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
      $response = drupal_http_request($this->getOriginalThumbnailPath());

      if (!isset($response->error)) {
        file_unmanaged_save_data($response->data, $local_path, TRUE);
      }
      else {
        @copy($this->getOriginalThumbnailPath(), $local_path);
      }
    }

    return $local_path;
  }
}
