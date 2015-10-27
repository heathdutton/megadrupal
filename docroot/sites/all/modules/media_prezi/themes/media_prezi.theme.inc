<?php

/**
 * @file
 * Media_prezi/themes/media_prezi.theme.inc.
 *
 * Theme and preprocess functions for Media: Prezi.
 */

/**
 * Preprocess function for theme('media_prezi_presentation').
 */
function media_prezi_preprocess_media_prezi_presentation(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['presentation_id'] = $parts['p'];
  // Make the file object available.
  $file_object = file_uri_to_object($variables['uri']);

  // Do something useful with the overridden attributes from the file.
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
  $variables['width'] = $variables['options']['width'];
  $variables['height'] = $variables['options']['height'];

  // Build the iframe URL with options query string.
  $variables['url'] = url('http://prezi.com/embed/' . $variables['presentation_id'],
    array(
      'bgcolor' => 'ffffff',
      'lock_to_path' => $variables['options']['lock_to_path'],
      'autoplay' => 0,
      'autohide_ctrls' => 0,
      'features' => 'undefined',
      'disabled_features' => 'undefined',
    ));
}
