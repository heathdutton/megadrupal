<?php

/**
 * @file
 * Theme and preprocess functions for Media: ShareStream.
 */

/**
 * Preprocess function for theme('media_sharestrean_video').
 */
function media_sharestream_preprocess_media_sharestream_video(&$variables) {

  $server = variable_get('media_sharestream_server', '');

  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['video_id'] = check_plain($parts['v']);

  // Make the file object available.
  $file_object = file_uri_to_object($variables['uri']);

  // Provide the file id.
  $variables['id'] = $file_object->fid;

  // Parse options and build the query string.
  $query = array();

  // Make css z-index work with flash object. Must be the first parameter.
  $query['wmode'] = 'opaque';

  // The autoPlay parameter expects a string of 'true' or 'false'.
  if ($variables['options']['autoplay']) {
    $query['autoPlay'] = 'true';
  }
  else {
    $query['autoPlay'] = 'false';
  }

  // Add some options as their own template variables.
  foreach (array('width', 'height') as $theme_var) {
    $variables[$theme_var] = $variables['options'][$theme_var];
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
  // @TODO: Find an easy/ not too expensive way to get the ShareStream description
  // to use for the alternative content.
  $variables['alternative_content'] = t('Video of @title', array('@title' => $variables['title']));

  // Build the iframe URL with options query string.
  $variables['url'] = url('//' . $server . '/ssdcms/i.do?u=' . $variables['video_id'], array('query' => $query, 'external' => TRUE));
}
