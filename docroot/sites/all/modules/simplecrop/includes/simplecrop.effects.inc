<?php

/**
 * @file
 * Contains definition and implementation of new image style effects.
 */

/**
 * Implements hook_image_effect_info().
 */
function simplecrop_image_effect_info() {

  $effects['simplecrop'] = array(
    'label' => t('Apply SimpleCrop'),
    'help' => t('Crop image to selected area.'),
    'effect callback' => 'simplecrop_crop_effect_callback',
  );

  return $effects;
}

/**
 * Crops image to a selected area.
 */
function simplecrop_crop_effect_callback(&$image) {

  // Try to find a crop for this image.
  $crop = simplecrop_crop_load($image->source);

  // If crop doesn't exists then nothing to do here.
  if (empty($crop) || empty($crop->data)) {
    return TRUE;
  }

  // Calculate a crop resolution based on given coordinates.
  $width  = abs($crop->data['x'] - $crop->data['x2']);
  $height = abs($crop->data['y'] - $crop->data['y2']);

  // If width or height is 0, then we don't need to crop
  // anything, so just return success status.
  if (!$width || !$height) {
    return TRUE;
  }

  // Try to crop image.
  if (!image_crop($image, $crop->data['x'], $crop->data['y'], $width, $height)) {
    watchdog('image', 'Image crop failed using the %toolkit toolkit on %path (%mimetype, %dimensions)', array('%toolkit' => $image->toolkit, '%path' => $image->source, '%mimetype' => $image->info['mime_type'], '%dimensions' => $image->info['width'] . 'x' . $image->info['height']), WATCHDOG_ERROR);
    return FALSE;
  }

  return TRUE;
}
