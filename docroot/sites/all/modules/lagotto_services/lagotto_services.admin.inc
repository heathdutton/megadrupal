<?php
/**
 * @file
 * Administration settings page definition.
 */

/**
 * Menu callback; Displays the administration settings.
 */
function lagotto_services_admin_settings() {
  $form = array();
  $form['settings'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 0,
  );

  $form['serverinfo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Server setup'),
    '#group' => 'settings',
    '#weight' => 0,
  );

  $form['serverinfo']['lagotto_services_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Lagotto Server URL'),
    '#description' => t('The URL of the Lagotto Server (excluding /api/...).'),
    '#default_value' => variable_get('lagotto_services_url', LAGOTTO_SERVICES_DEFAULT_DOMAIN),
  );
  $form['serverinfo']['lagotto_services_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('User API Key'),
    '#description' => t('The API key to use to access the Lagotto Server.'),
    '#default_value' => variable_get('lagotto_services_apikey', LAGOTTO_SERVICES_DEFAULT_KEY),
  );
  return system_settings_form($form);
}
