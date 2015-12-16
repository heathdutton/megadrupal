<?php

/**
 * @file
 * Admin configuration settings for Geofield Proximity Polygon.
 */

/**
 * Menu callback for admin settings.
 */
function geofield_proximity_polygon_admin_configure() {

  $form['gpp_advanced'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Advanced options'),
  );
  $form['gpp_advanced']['gpp_debug'] = array(
    '#type' => 'textfield',
    '#title' => t('Detail execution progress with status messages'),
    '#default_value' => variable_get('gpp_debug'),
    '#description' => t('Enter a comma-separated list of names of users that should see status messages coming from this module, e.g., for debugging purposes. Use <strong>anon</strong> for the anonymous user.'),
  );
  return system_settings_form($form);
}
