<?php

/**
 * @file
 * A set of configuration and customisable functions for the site banner module.
 */

/**
 * Function defining the color code and readable name for the background color.
 *
 * Including a code example for ease of configuration, not commented code
 *
 * @code
 * @ignore production_code $background_colors = array(
 *                           '#000000' => t('Black'),
 *                           '#ff0000' => t('Red'),
 *                           '#00ff00' => t('Green'),
 *                           '#0000ff' => t('Blue'),
 *                           '#ffffff' => t('White'),
 * @ignore production_code );
 * @endcode
 *
 * Always needs to map a 6 digit hexadecimal (hex) color code to a name
 * displayed on the module's admin page.
 *
 * @return array
 *   array mapping hex colors to readable names
 */
function site_banner_get_background_colors() {
  $background_colors = array(
    '#000000' => t('Black'),
    '#ff0000' => t('Red'),
    '#00ff00' => t('Green'),
    '#0000ff' => t('Blue'),
    '#ffffff' => t('White'),
  );
  return $background_colors;
}

/**
 * Function defining the color code and readable name for the text color.
 *
 * Including a code example for ease of configuration, not commented code
 *
 * @code
 * @ignore production_code $text_colors = array(
 *                           '#000000' => t('Black'),
 *                           '#ffffff' => t('White'),
 * @ignore production_code );
 * @code
 *
 * Always needs to map a 6 digit hexadecimal (hex) color code to a name
 * displayed on the module's admin page.
 *
 * @return array
 *   array mapping hex colors to readable names
 */
function site_banner_get_text_colors() {
  $text_colors = array(
    '#000000' => t('Black'),
    '#ffffff' => t('White'),
  );
  return $text_colors;
}
