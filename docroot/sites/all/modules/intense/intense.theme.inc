<?php
/**
 * @file
 * Hook and preprocess functions for intense images.
 */

/**
 * Returns HTML for an intense image field formatter.
 *
 * @param $variables
 *   An associative array containing:
 *   - item: Associative array of image data, which may include "uri", "alt",
 *     "width", "height", "title" and "attributes".
 *   - item_attributes: An optional associative array of html attributes to be
 *     placed in the img tag.
 *   - settings: An array containing: image_style, intense_style, caption.
 *   - url: The intense image URL.
 *
 * @ingroup themeable
 */
function theme_intense($variables) {
  foreach (array('item', 'item_attributes', 'settings', 'url') as $key) {
    $variables[$key] = isset($variables['element']["#$key"]) ? $variables['element']["#$key"] : array();
  }

  $item = $variables['item'];
  $settings = $variables['settings'];
  $image = array(
    'path' => $item['uri'],
  );

  if (array_key_exists('alt', $item)) {
    $image['alt'] = $item['alt'];
  }

  $variables['item_attributes']['class'][] = 'intense';
  $variables['item_attributes']['data-image'] = $variables['url'];
  $image['attributes'] = $variables['item_attributes'];

  if (isset($item['width']) && isset($item['height'])) {
    $image['width'] = $item['width'];
    $image['height'] = $item['height'];
  }

  // Do not output an empty 'title' attribute.
  if (isset($item['title']) && drupal_strlen($item['title']) > 0) {
    $image['title'] = $item['title'];
  }

  if ($settings['image_style']) {
    $image['style_name'] = $settings['image_style'];
    $build = theme('image_style', $image);
  }
  else {
    $build = theme('image', $image);
  }

  return $build;
}
