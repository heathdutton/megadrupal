<?php

/**
 * @file
 * Administrative page callbacks for the Facebook Pixel module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function facebook_pixel_admin_settings_form($form_state) {
  $form = array();

  $form['facebook_pixel_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Pixel ID'),
    '#required' => TRUE,
    '#default_value' => variable_get('facebook_pixel_id'),
  );

  return system_settings_form($form);
}
