<?php
/**
 * @file
 * Defines the API functions provided by the auto_retina module.
 *
 * @ingroup auto_retina
 * @{
 */

/**
 * Implements hook_auto_retina_create_derivative_alter().
 *
 * Allow modules to modify ANY IMAGE STYLE before the derivative is created.
 *
 * @param  array &$style
 *   An additional element is added:
 *   - auto_retina
 *     - suffix string The suffix used e.g. '@2x'
 *     - multiplier int|float The numeric part of the suffix for math, e.g. 2
 *
 * @see image_style_create_derivative().
 */
function hook_auto_retina_create_derivative_alter(&$style, &$source, &$destination) {
  
}

/**
 * Implements hook_auto_retina_effect_EFFECT_NAME_alter().
 *
 * Allow modules to alter a specific effect when processing a retina derivative.
 */
function hook_auto_retina_effect_EFFECT_NAME_alter(&$effect, $context) {
  
}

/**
 * Implements hook_auto_retina_effect_alter().
 *
 * Allow modules to alter a effects when processing a retina derivative.
 */
function hook_auto_retina_effect_alter(&$effect, $retina_info, $context) {
  if (!empty($effect['data']['width']) && !empty($effect['data']['height'])) {
    auto_retina_multiply_width_maintain_aspect_ratio($retina_info['multiplier'], $effect['data']['width'], $effect['data']['height']);
  }
  elseif (!empty($effect['data']['width'])) {
    $effect['data']['width'] *= $retina_info['multiplier'];
  }
}