<?php

/**
 * @file theme-settings.php
 * A file specifying custom settings for the Whiteboard theme.
 */

/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   array An array of saved settings for this theme.
 * @return
 *   array A form array.
 */
function whiteboard_settings($saved_settings) {
  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the template.php file.
   */
  $defaults = array(
    'header_image' => 1,
  );

  // Merge the saved variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  // Create the form widgets using Forms API
  $form['header_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Header image'),
    '#default_value' => $settings['header_image'],
  );

  // Return the additional form widgets
  return $form;
}
