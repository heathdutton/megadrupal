<?php
/**
 * @file
 * Hooks provided by the Setup module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Inform Setup about Step types.
 *
 * @return array
 *  An array of Step types, keyed by the type name, contain the following keys:
 *  - form callback: Function that defines the Step form.
 *  - finish callback: Function that process the form data on completion.
 */
function hook_setup_info() {
  $types = array();

  $types['markup'] = array(
    'form callback' => 'system_setup_markup_form_callback',
  );

  $types['theme'] = array(
    'form callback' => 'system_setup_theme_form_callback',
    'finish callback' => 'system_setup_theme_finish_callback',
  );

  $types['theme_settings'] = array(
    'form callback' => 'system_setup_theme_settings_form_callback',
    'finish callback' => 'system_setup_theme_settings_finish_callback',
  );

  $types['variable_set'] = array(
    'form callback' => 'system_setup_variable_set_form_callback',
    'finish callback' => 'system_setup_variable_set_finish_callback',
  );

  return $types;
}

/**
 * Inform Setup about Setup styles.
 *
 * @return array
 *  An array of Setup styles, keyed by the style name, contains keys as per the
 *  FAPI #attached element.
 */
function hook_setup_styles() {
  $styles = array();

  // Drupal install.php inspired style.
  $styles['drupalsetup'] = array(
    'css' => array(
      drupal_get_path('module', 'setup') . '/styles/drupalsetup/drupalsetup.css',
    ),
  );

  // Empty style.
  $styles['none'] = array();

  return $styles;
}

/**
 * @} End of "addtogroup hooks".
 */
