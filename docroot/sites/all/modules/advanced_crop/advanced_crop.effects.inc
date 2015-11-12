<?php
/**
 * @file
 * Image cropping effect hooks and callbacks.
 */

/**
 * Implements hook_image_effect_info().
 */
function advanced_crop_image_effect_info() {
  $effects = array(
    'advanced_crop_scale_and_crop_effect' => array(
      'label' => t('Advanced Cropper - Scale and crop'),
      'help' => t('Scale and crop based on data provided by <em>Advanced Image Cropper</em>.'),
      'effect callback' => 'advanced_crop_scale_and_crop_effect',
      'dimensions callback' => 'image_resize_dimensions',
      'form callback' => 'image_resize_form',
      'summary theme' => 'image_resize_summary',
    ),
  );

  return $effects;
}

/**
 * Image effect callback.
 *
 * @see advanced_crop_image_effect_info()
 */
function advanced_crop_scale_and_crop_effect(&$image, $data) {
  if ($crop_data = _advanced_crop_get_effect_crop_data($image, $data)) {
    // First crop the selected area.
    // image_crop_effect($image, array('width' => $crop_data['width'], 'height' => $crop_data['height'], 'anchor' => $crop_data['x'] . '-' . $crop_data['y']));
    image_crop($image, $crop_data['x'], $crop_data['y'], $crop_data['width'], $crop_data['height']);

    // Then scale it to the image style dimensions.
    // return image_scale_effect($image, array('width' => $data['width'], 'height' => $data['height'], 'upscale' => TRUE));
    return image_scale($image, $data['width'], $data['height'], TRUE);
  }
  elseif (module_exists('focal_point')) {
    return focal_point_scale_and_crop_effect($image, $data);
  }
  else {
    return image_scale_and_crop_effect($image, $data);
  }
}

/**
 * Compiles the necessary data for the image crop effect.
 *
 * @param object $image
 *   The image object provided to an effect callback.
 * @param array $data
 *   The data array provided to an effect callback.
 *
 * @return array|bool
 *   An array containing the following keys:
 *    - width
 *    - height
 *    - x
 *    - y
 *   If no image file can be located, FALSE is returned.
 */
function _advanced_crop_get_effect_crop_data($image, array $data) {
  $files = file_load_multiple(array(), array('uri' => $image->source));
  if (count($files)) {
    $file = reset($files);
    $crop_data = _advanced_crop_decode_crop_data($file->crop_data);
    if (!count($crop_data)) {
      return FALSE;
    }
    foreach ($crop_data as $cd) {
      if (round($cd['width'] / $cd['height'], 2) == round($data['width'] / $data['height'], 2)) {
        return $cd;
      }
    }
  }
  return FALSE;
}
