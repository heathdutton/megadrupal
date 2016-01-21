<?php

/**
 * @file
 * Lightgallery theme functions.
 */

/**
 * Returns HTML for an Lightgallery image field formatter.
 *
 */
function theme_lightgallery_image_formatter($variables) {
  $item_list = array();
  $items = $variables['items'];
  $settings = $variables['display_settings'];

  foreach ($items as $item) {
    foreach ($settings['image_styles'] as $image_style => $label) {
      // Fetch path for content.
      if ($style_name = $settings['image_styles'][$image_style]) {
        // Create url for image style.
        $paths[$image_style] = file_create_url(image_style_url($style_name,
                $item['uri']));
      }
      else {
        // Create url for original image.
        $paths[$image_style] = file_create_url($item['uri']);
      }
    }
    // Store paths in array.
    $item_list[] = $paths;
  }

  // Render light gallery item list.
  $unique_id = uniqid();
  $lightgallery_item_list = array(
    '#theme' => 'lightgallery_item_list',
    '#items' => $item_list,
    '#unque_id' => $unique_id,
  );

  // Build options array.
  $options = array(
    'thumbnails' => $settings['lightgallery_use_thumbs'],
  );

  // Add libraries and settings.
  $option_set = _lightgallery_build_option_set($options);
  lightgallery_add($unique_id, $option_set);

  // Render the complete light gallery markyp.
  return drupal_render($lightgallery_item_list);
}
