<?php
/**
 * @file
 * Provides example code for the module
 */

/**
 * Implments hook_preprocess_HOOK().
 *
 * Example method for excluding an image from being "unveiled".
 *
 * @param array $vars
 *   Variables array used by theme_image
 *
 * @see theme_image()
 *
 */
function unveil_example_preprocess_image($vars) {
  // Some business logic for exluding an image from unveil.
  if ($vars['style_name'] == 'excluded_imagestyle') {
    // Variable denoting that this image should be excluded from
    //   being unveiled, meaning it will simply be shown on page load.
    $vars['unveil_exclude'] = TRUE;
  }
}
