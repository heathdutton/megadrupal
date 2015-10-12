<?php
/**
 * @file
 * Theme functions.
 */

/**
 * Returns HTML for a cmp field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: Associative array of image data, which may include "uri", "alt",
 *     "width", "height", "title" and "attributes".
 *   - image_style: An optional image style.
 *   - path: An array containing the link 'path' and link 'options'.
 *
 * @return string $output
 *  HTML formatted output, or empty string.
 */
function theme_cmp_default_formatter($variables) {
  $item = $variables['item'];
  $output = '';

  // If the image is exists locally, or a default image is defined.
  if (isset($item['uri'])) {
    $image = array(
      'path' => check_plain($item['uri']),
    );

    if (isset($item['alt'])) {
      $image['alt'] = check_plain($item['alt']);
    }

    if (isset($item['attributes'])) {
      $image['attributes'] = $item['attributes'];
    }

    if (isset($item['width']) && isset($item['height'])) {
      if ((int) $item['width'] > 0) {
        $image['width'] = (int) $item['width'];
      }

      if ((int) $item['height'] > 0) {
        $image['height'] = (int) $item['height'];
      }
    }

    // Do not output an empty 'title' attribute.
    if (isset($item['title']) && drupal_strlen($item['title']) > 0) {
      $image['title'] = check_plain($item['title']);
    }

    if ($variables['image_style']) {
      $image['style_name'] = $variables['image_style'];
      $output = theme('image_style', $image);
    }
    else {
      $output = theme('image', $image);
    }
  }

  // The link path and link options are both optional, but for the options to be
  // processed, the link path must at least be an empty string.
  if (!empty($output) && isset($variables['path']['path'])) {
    $path = $variables['path']['path'];
    $options = isset($variables['path']['options']) ? $variables['path']['options'] : array();
    // When displaying an image inside a link, the html option must be TRUE.
    $options['html'] = TRUE;
    $output = l($output, $path, $options);
  }

  return $output;
}
