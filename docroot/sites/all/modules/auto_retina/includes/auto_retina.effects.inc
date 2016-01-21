<?php
/**
 * @file
 * Provides alterations to effects for retina processing.
 *
 * @ingroup auto_retina
 */

/**
 * Implements hook_auto_retina_effect_alter().
 */
function auto_retina_auto_retina_effect_alter(&$effect, $retina_info, $context) {
  list(,$original) = $context;
  $original_info   = image_get_info($original);

  if (!empty($effect['data']['width'])) {
    $width        = &$effect['data']['width'];
    $onex_width   = $width;
    $native_width = $original_info['width'];

    if (!empty($effect['data']['height'])) {
      auto_retina_multiply_width_maintain_aspect_ratio($retina_info['multiplier'], $width, $effect['data']['height']);
    }
    else {
      $width *= $retina_info['multiplier'];
    }

    // Detect and note when the retina image is not of the highest quality.
    if ($native_width < $width && variable_get('auto_retina_log', AUTO_RETINA_LOG)) {
      $percentage = intval($native_width * 100 / $width);
      watchdog('auto_retina', 'Poor retina quality of @percentage%.  To fix this, you must upload a new version of %file at least @width pixels wide.', array(
        '%file' => $original,
        '@width' => $width,
        '@percentage' => $percentage,
      ), WATCHDOG_WARNING);
    }
  }
}
