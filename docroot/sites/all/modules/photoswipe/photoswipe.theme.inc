<?php

/**
 * @file
 * Photoswipe theme functions.
 */

/**
 * Returns HTML for an Photoswipe image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: An array of image data.
 *   - image_style: An optional image style.
 *   - path: An array containing the link 'path' and link 'options'.
 *
 * @ingroup themeable
 */
function theme_photoswipe_image_formatter($variables) {
  $item = $variables['item'];
  $node = $variables['node'];
  $field = $variables['field'];
  $settings = $variables['display_settings'];
  $alt = !empty($item['alt']) ? $item['alt'] : '';
  $title = !empty($item['title']) ? $item['title'] : '';

  if (empty($alt) && !empty($item['field_file_image_alt_text'][LANGUAGE_NONE])) {
    $alt = $item['field_file_image_alt_text'][LANGUAGE_NONE][0]['value'];
  }
  if (empty($title) && !empty($item['field_file_image_title_text'][LANGUAGE_NONE])) {
    $title = $item['field_file_image_title_text'][LANGUAGE_NONE][0]['value'];
  }
  $image = array(
    'path' => $item['uri'],
    'alt' => $alt,
    'title' => $title,
    'style_name' => $settings['photoswipe_node_style'],
  );

  $dimensions = array();
  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $dimensions['width'] = $item['width'];
    $image['height'] = $dimensions['height'] = $item['height'];
  }

  if ($style_name = $settings['photoswipe_image_style']) {
    $path = image_style_url($style_name, $image['path']);
    $dimensions = array(
      'width' => $item['width'],
      'height' => $item['height'],
    );
    image_style_transform_dimensions($style_name, $dimensions);
  }
  else {
    $path = file_create_url($image['path']);
  }

  return theme('photoswipe_imagefield', array('image' => $image, 'path' => $path, 'dimensions' => $dimensions['width'] . 'x' . $dimensions['height']));
}


/**
 * Returns HTML for an image using a specific Photoswipe image style.
 *
 * @param $variables
 *   An associative array containing:
 *   - image: image item as array.
 *   - path: The path of the image that should be displayed in the Photoswipe.
 *
 * @ingroup themeable
 */
function theme_photoswipe_imagefield($variables) {
  $class = array('photoswipe');
  if (!empty($variables['image']['style_name'])) {
    $image = theme('image_style', $variables['image']);
  }
  else {
    $image = theme('image', $variables['image']);
  }

  $options = array(
    'html' => TRUE,
    'attributes' => array(
      'class' => implode(' ', $class),
      'data-size' => $variables['dimensions'],
      'data-overlay-title' => $variables['image']['alt']
    )
  );

  return l($image, $variables['path'], $options);
}
