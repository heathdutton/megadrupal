<?php

/**
 * @file
 * Admin functionality for the PostcodeAnywhere Address Field lookup service.
 */

/**
 * Form builder: PostcodeAnywhere address field lookup service configuration.
 */
function addressfield_lookup_postcodeanywhere_config_form() {
  $form['addressfield_lookup_postcodeanywhere_login'] = array(
    '#title' => t('Login'),
    '#type' => 'textfield',
    '#default_value' => variable_get('addressfield_lookup_postcodeanywhere_login', NULL),
    '#description' => t('The login associated with the Royal Mail license (not required for click licenses).'),
  );

  $form['addressfield_lookup_postcodeanywhere_license'] = array(
    '#title' => t('License'),
    '#type' => 'textfield',
    '#default_value' => variable_get('addressfield_lookup_postcodeanywhere_license', NULL),
    '#description' => t('API key to use to authenticate to Postcode Anywhere.'),
  );

  // Add a custom submit function.
  $form['#submit'][] = 'addressfield_lookup_postcodeanywhere_config_form_submit';

  return system_settings_form($form);
}

/**
 * Form submit handler: Redirect back to the address field lookup services page.
 */
function addressfield_lookup_postcodeanywhere_config_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/config/regional/addressfield-lookup';
}
