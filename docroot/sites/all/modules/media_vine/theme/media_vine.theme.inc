<?php

/**
 * @file
 * Theme and preprocess functions for Media: Vine.
 */

/**
 * Preprocess function for theme('media_vine_video').
 */
function media_vine_preprocess_media_vine_video(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['video_id'] = $parts['v'];
  $file_object = file_uri_to_object($variables['uri']);

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

  // Add template variables.
  $variables['title'] = check_plain($file_object->filename);
  $variables['alternative_content'] = '';

  $variables['embed_format'] = !empty($variables['options']['embed_format']) ? $variables['options']['embed_format'] : 'simple';

  $variables['autoplay_audio'] = !empty($variables['options']['autoplay_audio']) ? TRUE : FALSE;
  $query = $variables['autoplay_audio'] ? array('audio' => '1') : array();

  // Build the iframe URL with options query string.
  $variables['url'] = url('https://vine.co/v/' . $variables['video_id'] . '/embed/' . $variables['embed_format'], array(
    'query' => $query,
    'external' => TRUE,
  ));
}
