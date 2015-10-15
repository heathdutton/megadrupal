<?php

/**
 * @file
 * prettyPhoto youtube formatter hooks and callbacks.
 */

/**
 * Theme function for displaying the prettyPhoto youtube field.
 *
 * @param array $variables
 *   An array of youtube field's attributes, e.g. class sizes.
 *
 * @return string
 *   HTML output for displaying the youtube video.
 */
function theme_prettyphoto_formatters_youtube($variables) {
  $settings = $variables['settings'];

  // Query string changes based on setings.
  $query = array();

  if (!$settings['module']['suggest']) {
    $query['rel'] = '0';
  }

  if ($settings['module']['modest_branding']) {
    $query['modestbranding'] = '1';
  }

  if ($settings['module']['theme']) {
    $query['theme'] = 'light';
  }

  if ($settings['module']['color']) {
    $query['color'] = 'white';
  }

  if ($settings['module']['enable_js_api']) {
    global $base_url;
    $query['enablejsapi'] = '1';
    $query['origin'] = parse_url($base_url, PHP_URL_HOST);
  }

  if ($settings['module']['wmode']) {
    $query['wmode'] = 'opaque';
  }

  if ($settings['field']['autoplay']) {
    $query['autoplay'] = '1';
  }

  if ($settings['field']['showinfo']) {
    $query['showinfo'] = '0';
  }

  if ($settings['field']['controls']) {
    $query['controls'] = '0';
  }

  if ($settings['field']['autohide']) {
    $query['autohide'] = '1';
  }

  if ($settings['field']['iv_load_policy']) {
    $query['iv_load_policy'] = '3';
  }

  // Domain changes based on settings.
  $domain = ($settings['module']['privacy']) ? 'youtube-nocookie.com' : 'youtube.com';

  // Embed URL.
  $embed_url = $settings['module']['protocol'] . '://www.' . $domain . '/embed/' . $variables['video_id'];

  $files = variable_get('file_public_path', conf_path() . '/files');
  $thumb_dir = $settings['module']['thumb_dir'];
  $dest = $files . '/' . $thumb_dir . '/' . $variables['video_id'] . '.png';

  // Check to see if a thumbnail exists locally.
  if (!file_exists($dest)) {
    // Retrieve the image from YouTube.
    youtube_get_remote_image($variables['video_id']);
  }

  if ($settings['field']['tsize']) {
    $uri = 'public://' . $thumb_dir . '/' . $variables['video_id'] . '.png';
    $image = theme('image_style', array(
      'style_name' => $settings['field']['tsize'],
      'path' => $uri,
    ));
  }
  else {
    $uri = $files . '/' . $thumb_dir . '/' . $variables['video_id'] . '.png';
    $image = theme('image', array(
      'path' => $uri,
    ));
  }

  $query['iframe'] = 'true';
  $query['width'] = $settings['module']['dimensions']['width'];
  $query['height'] = $settings['module']['dimensions']['height'];

  $options = array(
    'attributes' => array(
      'rel' => array('prettyPhoto[' . $variables['field']['field_name'] . ']'),
    ),
    'html' => TRUE,
    'query' => $query,
  );

  return l($image, $embed_url, $options);
}
