<?php
/**
 * @file
 * Theme functions for RoyalSlider.
 */

/**
* Implements hook_preprocess_HOOK().
*
* Preprocess variables for the RoyalSlider template.
*/
function royalslider_preprocess_royalslider(&$variables) {
  // Load optionset.
  if ($optionset = royalslider_optionset_load($variables['optionset'])) {
    $variables['settings'] = $optionset->options;

    // Add wrapper attributes.
    $variables['attributes_array']['class'][] = 'royalSlider';
    $variables['attributes_array']['id'] = $variables['royalslider_id'];

    // Setup skin & fallback to default.
    $skin = !empty($variables['skin']) ? $variables['skin'] : $optionset->skin;
    $skins = royalslider_skins();

    $skin_class = isset($skins[$skin]['class'])
      ? $skins[$skin]['class']
      : $skins['default']['class'];

    $variables['attributes_array']['class'][] = $skin_class;

    // @TODO figure out if we can calculate the largest image size and use it
    // as a base for automatically setting the slider size.

    // Now prepare items for rendering.
    $items_processed = array();

    foreach ($variables['items'] as $item) {
      $items_processed[] = array(
        '#theme' => 'royalslider_item',
        '#royalslider_id' => $variables['royalslider_id'],
        '#optionset' => $optionset,
        '#item' => $item,
      );
    }

    $variables['items_processed'] = $items_processed;

    if (!empty($items_processed)) {
      // Add resources.
      royalslider_add($variables['royalslider_id'], $optionset, $skin);

      // We're done!
      return;
    }
  }
}

/**
 * Implements hook_preprocess_HOOK().
 *
 * Preprocess variables for the RoyalSlider Item template.
 */
function royalslider_preprocess_royalslider_item(&$variables) {
  // Only set slider dimensions once.
  static $slider_size_added = FALSE;

  $optionset = $variables['optionset'];
  $item = $variables['item'];

  // Tag is defined by optionset, not individual item.
  $variables['tag'] = $optionset->options['usePreloader'] ? 'a' : 'img';


  $add_classes = array('rsImg');
  $variables['classes_array'] = array_unique(array_merge($variables['classes_array'], $add_classes));
  $attributes = array('class' => $variables['classes_array']);

  if (!empty($item['alt'])) {
    $attributes['alt'] = $item['alt'];
  }

  if (!empty($item['title'])) {
    $attributes['title'] = $item['title'];
    $variables['caption'] = $item['title'];
  }
  
  if (array_key_exists('row', $item)) {
    $variables['row'] = $item['row'];
  }
  // Handle image and slider dimensions.
  $manual_width = !empty($optionset->options['imgWidth']);
  $manual_height = !empty($optionset->options['imgHeight']);

  if ($optionset->options['drupalAutoSetImageDimensions']
    || $manual_width
    || $manual_height
  ) {
    // Grab image dimensions.
    $dimensions = array(
      'width' => $item['width'],
      'height' => $item['height'],
    );

    // Resize if we're using an image preset.
    // @TODO: This should be skipped when item is part of a views_slideshow.
    if (!empty($optionset->imagestyle_normal)) {
      image_style_transform_dimensions($optionset->imagestyle_normal, $dimensions);
    }

    // Automatic image dimensions.
    if ($optionset->options['drupalAutoSetImageDimensions']) {
      $attributes['data-rsw'] = $dimensions['width'];
      $attributes['data-rsh'] = $dimensions['height'];
    }
    // Hard-coded image dimensions.
    else if ($manual_width || $manual_height) {
      if ($manual_width) {
        $attributes['data-rsw'] = $optionset->options['imgWidth'];
      }
      if ($manual_height) {
        $attributes['data-rsh'] = $optionset->options['imgHeight'];
      }
    }
  }

  // Handle full-screen images.
  if ($optionset->options['fullscreen']['enabled']) {
    $image_style = $optionset->imagestyle_fullscreen;

    $attributes['data-rsBigImg'] = !empty($image_style)
      ? _royalslider_image_style($image_style, $item['uri'])
      : file_create_url($item['uri']);
  }

  // Handle thumbnail images.
  if ($optionset->options['controlNavigation'] === 'thumbnails') {
    $image_style = $optionset->imagestyle_thumbnail;

    $attributes['data-rsTmb'] = !empty($image_style)
      ? _royalslider_image_style($image_style, $item['uri'])
      : file_create_url($item['uri']);
  }

  // Set main image url.
  $image_style = $optionset->imagestyle_normal;
  $variables['url'] = !empty($image_style)
    ? _royalslider_image_style($image_style, $item['uri'])
    : file_create_url($item['uri']);

  if (isset($item['path'])) {
    $variables['path'] = $item['path'];
  }

  // Set slider dimensions.
  if ($variables['optionset']->options['autoScaleSlider']) {
    // Only add them once.
    if (!$slider_size_added) {
      drupal_add_js(array(
        'royalslider' => array(
          'instances' => array(
            $variables['royalslider_id'] => array(
              'slider_height' => $dimensions['height'],
              'slider_width' => $dimensions['width'],
            ),
          ),
        ),
      ), 'setting');
    }
  }

  // Finally, set attributes.
  $variables['attributes_array'] = $attributes;
}

/**
 * Remote Stream Wrapper-aware helper function to apply Drupal image styles
 * to image URIs.
 *
 * @param string
 *   The image style we want to apply.
 * @param string
 *   The image's URI.
 *
 * @return string
 *   The image's path.
 */
function _royalslider_image_style($image_style, $uri) {
  // Compatibility with Remote Stream Wrapper.
  static $remote_stream_wrapper;

  if (!isset($remote_stream_wrapper)) {
    $remote_stream_wrapper = module_exists('remote_stream_wrapper');
  }

  if ($remote_stream_wrapper) {
    $path = file_is_scheme_remote(file_uri_scheme($uri))
      ? remote_stream_wrapper_image_style_path($image_style, $uri)
      : image_style_url($image_style, $uri);
  }
  else {
    $path = image_style_url($image_style, $uri);
  }

  return $path;
}
