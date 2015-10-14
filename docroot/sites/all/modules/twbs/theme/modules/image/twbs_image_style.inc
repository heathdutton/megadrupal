<?php

/**
 * Returns HTML for an image using a specific image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - style_name: The name of the style to be used to alter the original image.
 *   - path: The path of the image file relative to the Drupal files directory.
 *     This function does not work with images outside the files directory nor
 *     with remotely hosted images. This should be in a format such as
 *     'images/image.jpg', or using a stream wrapper such as
 *     'public://images/image.jpg'.
 *   - width: The width of the source image (if known).
 *   - height: The height of the source image (if known).
 *   - alt: The alternative text for text-based browsers.
 *   - title: The title text is displayed when the image is hovered in some
 *     popular browsers.
 *   - attributes: Associative array of attributes to be placed in the img tag.
 *
 * @ingroup themeable
 */
function twbs_image_style($variables) {
  // Determine the dimensions of the styled image.
  $dimensions = array(
    'width' => $variables['width'],
    'height' => $variables['height'],
  );

  image_style_transform_dimensions($variables['style_name'], $dimensions);

  $variables['width'] = $dimensions['width'];
  $variables['height'] = $dimensions['height'];

  // Determine the URL for the styled image.
  $variables['path'] = image_style_url($variables['style_name'], $variables['path']);
  return theme('image', $variables);
}
