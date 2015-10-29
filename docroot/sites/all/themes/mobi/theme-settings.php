<?php

/**
 * Implementation of THEMEHOOK_settings() function.
 *
 * @param $saved_settings
 *   array An array of saved settings for this theme.
 * @return
 *   array A form array.
 */
function phptemplate_settings($saved_settings) {
  /*
   * The default values for the theme variables. Make sure $defaults exactly
   * matches the $defaults in the template.php file.
   */
  $defaults = array(
    'mobi_header_background_color' => '#eef7fc',
    'mobi_left_background_color' => '#ffffff',
    'mobi_content_background_color' => '#ffffff',
    'mobi_right_background_color' => '#ffffff',
    'mobi_footer_background_color' => '#eef7fc'
  );

  // Merge the saved variables and their default values
  $settings = array_merge($defaults, $saved_settings);

  $form['background_colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Background colors'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );
  $form['background_colors']['mobi_header_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Header'),
    '#default_value' => $settings['mobi_header_background_color']
  );
  $form['background_colors']['mobi_left_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Left sidebar'),
    '#default_value' => $settings['mobi_left_background_color']
  );
  $form['background_colors']['mobi_content_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Content'),
    '#default_value' => $settings['mobi_content_background_color']
  );
  $form['background_colors']['mobi_right_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Right sidebar'),
    '#default_value' => $settings['mobi_right_background_color']
  );
  $form['background_colors']['mobi_footer_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Footer'),
    '#default_value' => $settings['mobi_footer_background_color']
  );

  // Return the additional form widgets
  return $form;
}
