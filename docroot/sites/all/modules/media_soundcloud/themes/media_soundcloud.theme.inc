<?php

/**
 * @file media_soundcloud/themes/media_soundcloud.theme.inc
 *
 * Theme and preprocess functions for Media: SoundCloud.
 */

/**
 * Preprocess function for theme('media_soundcloud_audio').
 */
function media_soundcloud_preprocess_media_soundcloud_audio(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();

  $url = "";

  // User.
  if (isset($parts['u'])) {
    $url = '/u/' . check_plain($parts['u']);
  }

  // Group.
  if (isset($parts['g'])) {
    $url = '/g/' . check_plain($parts['g']);
  }

  // Single song.
  if (isset($parts['u']) && isset($parts['a'])) {
    $url = '/u/' . check_plain($parts['u']) . '/a/'.check_plain($parts['a']);
  }

  // Audio sets.
  if (isset($parts['u']) && isset($parts['s'])) {
    $url = '/u/' . check_plain($parts['u']) . '/s/'.check_plain($parts['s']);
  }

  $variables['audio_id'] = $url;

  $uri = file_stream_wrapper_uri_normalize('soundcloud://' . $url);
  $external_url = file_create_url($uri);

  // Make the file object available.
  $file_object = file_uri_to_object($variables['uri']);

  // Parse options and build the query string. Only add the option to the query
  // array if the option value is not default. Be careful, depending on the
  // wording in media_soundcloud.formatters.inc, TRUE may be query=0.
  // @see https://developers.soundcloud.com/docs/widget
  $query = array();

  $query['url'] = $external_url;

  // These queries default to 0. If the option is true, set value to 1.
  foreach (array('auto_play', 'visual') as $option) {
    if ($variables['options'][$option]) {
      $query[$option] = TRUE;
    }
  }
  // Add a query player ID and identical html ID if js API is set.
  if ($variables['options']['api']) {
    $query['player_id'] = drupal_html_id('media-soundcloud-' . $variables['audio_id']);
    $variables['api_id_attribute'] = 'id="' . $query['player_id'] . '" ';
  }
  else {
    $variables['api_id_attribute'] = NULL;
  }

  // These queries default to 1. If the option is false, set value to 0.
  foreach (array('show_artwork', 'show_comments') as $option) {
    if (!$variables['options'][$option]) {
      $query[$option] = FALSE;
    }
  }

  // Strings.
  if (isset($variables['options']['color'])) {
    $query['color'] = str_replace('#', '', $variables['options']['color']);
  }

  // Non-query options.
  if ($variables['options']['protocol_specify']) {
    $protocol = $variables['options']['protocol'];
  }
  else {
    $protocol = '//';
  }

  // Add some options as their own template variables.
  foreach (array('width', 'height') as $themevar) {
    $variables[$themevar] = $variables['options'][$themevar];
  }

  // Do something useful with the overridden attributes from the file
  // object. We ignore alt and style for now.
  if (isset($variables['options']['attributes']['class'])) {
    if (is_array($variables['options']['attributes']['class'])) {
      $variables['classes_array'] = array_merge($variables['classes_array'], $variables['options']['attributes']['class']);
    }
    else {
      // Provide nominal support for Media 1.x.
      $variables['classes_array'][] = $variables['options']['attributes']['class'];
    }
  }

  // Add template variables for accessibility.
  $variables['title'] = check_plain($file_object->filename);
  // @TODO: Find an easy/ not too expensive way to get the SoundCloud description
  // to use for the alternative content.
  $variables['alternative_content'] = t('Audio titled @title', array('@title' => $variables['title']));

  // Build the iframe URL with options query string.
  $variables['url'] = url($protocol . 'w.soundcloud.com/player/', array('query' => $query, 'external' => TRUE));
}
