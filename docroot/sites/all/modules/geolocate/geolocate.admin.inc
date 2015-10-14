<?php
/**
 * @file
 * admin page file containing the menu callback function.
 */

/**
 * Main settings form. 
 * @function - form elements for custom admin settings. 
 * returns an array of form elements to validate and submit functions. 
 */
function geolocate_admin_settings($form, &$form_state) {    

    $form['geolocate_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debugging on?'),
    '#description' => t('Toggle this on to turn on console logging. <em>You must have the Administer module to view this data.</em>'),
    '#default_value' => variable_get('geolocate_debug', 1),
  );

  return system_settings_form($form);
}
