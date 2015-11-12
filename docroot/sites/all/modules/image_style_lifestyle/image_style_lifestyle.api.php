<?php

/**
 * @file
 * Hooks provided by the Image Style Lifestyle module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Allows you to change what function is called to detect a render.
 *
 * @param string $callback
 *   The current function callback.
 *
 * @return string
 *   The new function callback.
 */
function hook_image_style_lifestyle_is_render_alter($callback) {
  $new_callback = "_image_style_lifestyle_average_colour_is_near_white";
  return $new_callback;
}

/**
 * @} End of "addtogroup hooks".
 */
