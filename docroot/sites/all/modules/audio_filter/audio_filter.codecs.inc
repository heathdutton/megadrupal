<?php

/**
 * @file
 * This file contains all codecs provided by Audio Filter.
 */

/**
 * Implements hook_audio_filter_codec_info().
 */
function audio_filter_audio_filter_codec_info() {
  $codecs = array();

  $codecs['soundcloud'] = array(
    'name' => t('SoundCloud'),
    'sample_url' => 'https://soundcloud.com/skinny-jean/03-pulverem',
    'callback' => 'audio_filter_soundcloud',
    'html5_callback' => 'audio_filter_soundcloud_html5',
    'regexp' => array(
      '@soundcloud\.com/([0-9A-Za-z\@\&\$_-]+)/([0-9A-Za-z\@\&\$_-]+)/([0-9A-Za-z\@\&\$_-]+)@i',
      '@soundcloud\.com/([0-9A-Za-z\@\&\$_-]+)/([0-9A-Za-z\@\&\$_-]+)@i',
      '@soundcloud\.com/([0-9A-Za-z\@\&\$_-]+)@i',
    ),
    'ratio' => FALSE,
    'control_bar_height' => 25,
  );

  return $codecs;
}

/**
 * Callback for SoundCloud codec.
 *
 * @see audio_filter_codec_info()
 */
function audio_filter_soundcloud($audio) {
    return audio_filter_soundcloud_html5($audio);
}

/**
 * HTML5 callback for SoundCloud codec.
 *
 * @see audio_filter_codec_info()
 */
function audio_filter_soundcloud_html5($audio) {
  $matches = $audio['codec']['matches'];
  $audio_url = 'http://' . $matches[0];
  $lookup = 'http://api.soundcloud.com/resolve.json?url=' . urlencode($audio_url) . '&client_id=' . AUDIO_FILTER_CLIENT_ID_SOUNDCLOUD;
  $response = drupal_http_request($lookup);
  if (!isset($response->error)) {
    $data = drupal_json_decode($response->data);
    switch ($data['kind']) {
      default:
      case 'track':
        $audio['source'] = 'http://w.soundcloud.com/player/?url=http%3A%2F%2Fapi.soundcloud.com%2Ftracks%2F' . $data['id'];
        break;
      case 'user':
        $audio['source'] = 'http://w.soundcloud.com/player/?url=http%3A%2F%2Fapi.soundcloud.com%2Fusers%2F' . $data['id'];
        $audio['height'] = '450';
        break;
    }
  }
  return audio_filter_iframe($audio);
}
