<?php

/**
 * @file
 * HiringThing admin pages.
 */

/**
 * Form callback for HiringThing admin settings.
 */
function hiringthing_admin_settings_form($form, &$form_state) {
  $form['hiringthing_site_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Enter your HiringThing account URL'),
    '#size' => '40',
    '#field_prefix' => 'http://',
    '#field_suffix' => '.hiringthing.com',
    '#default_value' => variable_get('hiringthing_site_url', ''),
  );

  if (module_exists('libraries')) {
    $form['hiringthing_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('API key'),
      '#default_value' => variable_get('hiringthing_api_key', ''),
    );
    $form['hiringthing_api_password'] = array(
      '#type' => 'textfield',
      '#title' => t('API password'),
      '#default_value' => variable_get('hiringthing_api_password', ''),
    );
  }

  return system_settings_form($form);
}

