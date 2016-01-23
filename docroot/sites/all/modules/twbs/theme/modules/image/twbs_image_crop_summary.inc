<?php

/**
 * Returns HTML for a summary of an image crop effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this crop effect.
 *
 * @ingroup themeable
 */
function twbs_image_crop_summary($variables) {
  return theme('image_resize_summary', $variables);
}
