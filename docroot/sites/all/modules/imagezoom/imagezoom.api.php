<?php

/**
 * @file
 * Hook documentation for the Image Zoom module.
 */

/**
 * Change settings for the elevatezoom plugin.
 *
 * For all available settings, see http://www.elevateweb.co.uk/image-zoom/configuration
 *
 * @param array $settings
 *   A array of settings keyed by the setting name.
 *
 * @param array $context
 *   An array containing information about the field and instance being rendered.
 */
function hook_imagezoom_settings_alter(&$settings, $context) {
  // Change the border color to red
  $settings['borderColour'] = '#f00';
}
