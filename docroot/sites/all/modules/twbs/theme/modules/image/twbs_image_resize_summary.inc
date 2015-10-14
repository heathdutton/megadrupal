<?php

/**
 * Returns HTML for a summary of an image resize effect.
 *
 * @param $variables
 *   An associative array containing:
 *   - data: The current configuration for this resize effect.
 *
 * @ingroup themeable
 */
function twbs_image_resize_summary($variables) {
  $data = $variables['data'];

  if ($data['width'] && $data['height']) {
    return check_plain($data['width']) . 'x' . check_plain($data['height']);
  }
  else {
    return ($data['width']) ? t('width @width', array('@width' => $data['width'])) : t('height @height', array('@height' => $data['height']));
  }
}
