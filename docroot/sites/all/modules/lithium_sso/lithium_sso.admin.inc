<?php

/**
 * @file
 * Provides the Lithium SSO module admin page.
 */

/**
 * Implements hook_form().
 */
function lithium_sso_admin_settings($form, &$form_state) {
  global $base_url;

  $form['global_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['global_fieldset']['lithium_sso_enable_sso'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('lithium_sso_enable_sso', 0),
    '#title' => '<strong>' . t('Enable Lithium SSO on this site.') . '</strong>',
    '#description' => t('Must be enabled to start generating authnitication tokens for %this_site.',
      array('%this_site' => $base_url)),
  );
  $form['global_fieldset']['lithium_sso_cookie_expire'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('lithium_sso_cookie_expire', ''),
    '#title' => t('Cookie expiry time'),
    '#description' => t('The number of days after which the cookie expires, If left blank the cookie will expire after 30 days.'),
    '#size' => 4,
    '#maxlength' => 4,
  );
  $form['client_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for the lithium client'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['client_fieldset']['lithium_sso_client_id'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('lithium_sso_client_id', ''),
    '#required' => TRUE,
    '#title' => t('Client ID'),
    '#description' => t('The client or community ID to create an SSO token for.'),
    '#size' => 14,
    '#maxlength' => 28,
  );
  $form['client_fieldset']['lithium_sso_client_domain'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('lithium_sso_client_domain', ''),
    '#required' => TRUE,
    '#title' => t('Client Domain'),
    '#description' => t('The domain name for which the cookie will be available, e.g. <em>.domain.com</em>'),
    '#size' => 40,
    '#maxlength' => 128,
  );
  $form['client_fieldset']['lithium_sso_sso_key'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('lithium_sso_sso_key', ''),
    '#required' => TRUE,
    '#title' => t('SSO key'),
    '#description' => t('The 128-bit or 256-bit secret key, represented in hexadecimal'),
    '#size' => 40,
  );

  return system_settings_form($form);
}
