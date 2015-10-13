<?php

/**
 * @file
 * Contains theming functions and preprocesses.
 */

/**
 * Implements template_preprocess_image().
 */
function simplecrop_preprocess_image(&$variables) {

  // Make sure that the current image was processed with image style and no
  // unexpected issues with image path.
  if (!empty($variables['style_name']) && !empty($variables['path'])) {

    // We care only about images which were processed by simplecrop effect.
    $image_styles = simplecrop_imagestyles_with_crop_effect();
    $current_image_style = $variables['style_name'];
    if (empty($image_styles[$current_image_style])) {
      return;
    }

    // Parse image path.
    $parsed_url = parse_url($variables['path']);

    // Get a path without query string and "/styles/[style-name]/" part.
    $match = '/styles/' . $current_image_style . '/';
    $path = substr($parsed_url['path'], strrpos($parsed_url['path'], $match) + drupal_strlen($match));

    // Build the image URI.
    $path = explode('/', $path);
    $scheme = array_shift($path);
    $target = implode('/', $path);
    $image_uri = $scheme . '://' . urldecode($target);

    // Load information about crop for the current image.
    $crop = simplecrop_crop_load($image_uri);
    if (!empty($crop)) {

      // If image has a crop, then add unique token to the path string. When
      // new crop data will be here the has will change that will force browser
      // to reload an image.
      $hash = md5($crop->data['x'] . '|' . $crop->data['y'] . '|' . $crop->data['x2'] . '|' . $crop->data['y2']);
      $variables['path'] .= !empty($parsed_url['query']) ? '&' : '?';
      $variables['path'] .= 'sc=' . $hash;
    }
  }
}
