<?php

/**
 * @file
 * Defines the administration UI for ideal connector.
 */

/**
 * Form constructor for adding an ideal configuration.
 *
 * @see ideal_advanced_admin_add_form_submit()
 *
 * @ingroup forms
 */
function ideal_advanced_admin_add_form($form, &$form_state) {
  $form_state['ideal_config'] = new IdealAdvancedConfigHandler();
  $form = ideal_advanced_admin_form_fields($form_state['ideal_config']->defaultConfiguration(), $form_state);

  form_load_include($form_state, 'inc', 'ideal_advanced', 'form/ideal_advanced_validate_form');

  return $form;
}

/**
 * Form submission handler for ideal_advanced_admin_add_form().
 */
function ideal_advanced_admin_add_form_submit($form, &$form_state) {
  $form_state['ideal_config']->configurationAdd($form_state['values']);
  $form_state['redirect'] = 'admin/config/services/ideal';
}

/**
 * Form constructor for editing an ideal configuration.
 *
 * @see ideal_advanced_admin_edit_form_submit()
 *
 * @ingroup forms
 */
function ideal_advanced_admin_edit_form($form, &$form_state) {
  $form_state['ideal_config'] = new IdealAdvancedConfigHandler();
  $settings = $form_state['ideal_config']->getSettings($form_state['build_info']['args'][0]);
  $form = ideal_advanced_admin_form_fields($settings, $form_state);

  form_load_include($form_state, 'inc', 'ideal_advanced', 'form/ideal_advanced_validate_form');

  return $form;
}

/**
 * Form submission handler for ideal_advanced_admin_edit_form().
 */
function ideal_advanced_admin_edit_form_submit($form, &$form_state) {
  $form_state['ideal_config']->configurationEdit($form_state['values'], $form_state['build_info']['args'][0]);
  $form_state['redirect'] = 'admin/config/services/ideal';
}

/**
 * Form constructor for deleting an ideal configuration.
 *
 * @see ideal_advanced_admin_delete_form_submit()
 *
 * @ingroup forms
 */
function ideal_advanced_admin_delete_form($form, &$form_state) {
  $question = t('Are you sure you want to delete this iDEAL configuration?');
  $return_path = 'admin/config/services/ideal';

  return confirm_form($form, $question, $return_path);
}

/**
 * Form submission handler for ideal_advanced_admin_delete_form().
 */
function ideal_advanced_admin_delete_form_submit($form, &$form_state) {
  entity_delete('ideal_advanced_config', $form_state['build_info']['args'][0]);
  $form_state['redirect'] = 'admin/config/services/ideal';
}

/**
 * Defaults form fields for the ideal advanced admin ui.
 *
 * @param array $settings
 *   Object with the ideal configuration settings.
 *
 * @return array
 *   Drupal form fields.
 *
 * @see ideal_advanced_admin_add_form()
 * @see ideal_advanced_admin_edit_form()
 */
function ideal_advanced_admin_form_fields(array $settings) {
  $form = array();

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $settings['title'],
    '#description' => '',
    '#required' => TRUE,
  );
  $form['payment_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['module_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Module settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['payment_settings']['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#default_value' => $settings['merchant_id'],
    '#description' => '',
    '#required' => TRUE,
    '#element_validate' => array('ideal_advanced_validate_merchant_id'),
  );
  $form['payment_settings']['sub_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Sub ID'),
    '#default_value' => $settings['sub_id'],
    '#description' => '',
    '#element_validate' => array('ideal_advanced_validate_sub_id'),
  );
  $form['payment_settings']['private_key_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key path'),
    '#default_value' => $settings['private_key_path'],
    '#description' => t('File path relative to the private folder. For example: ideal/priv.key'),
    '#required' => TRUE,
    '#element_validate' => array('ideal_advanced_validate_private_key_path'),
  );
  $form['payment_settings']['private_key_password'] = array(
    '#type' => 'textfield',
    '#title' => t('Private key password'),
    '#default_value' => $settings['private_key_password'],
    '#description' => t('The password to use the private key'),
    '#required' => TRUE,
  );
  $form['payment_settings']['private_certificate_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Private certificate path'),
    '#default_value' => $settings['private_certificate_path'],
    '#description' => t('File path relative to the private folder. For example:  ideal/cert-256.cer'),
    '#required' => TRUE,
    '#element_validate' => array('ideal_advanced_validate_private_certificate_path'),
  );
  $form['payment_settings']['ideal_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('iDEAL server URL'),
    '#default_value' => $settings['ideal_server_url'],
    '#description' => t('The URL of the acquirer iDEAL Server'),
    '#required' => TRUE,
    '#element_validate' => array('ideal_advanced_validate_ideal_server_url'),
  );
  $form['payment_settings']['public_certificate_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Public iDEAL certificate path'),
    '#default_value' => $settings['public_certificate_path'],
    '#description' => t('File path relative to the private folder. For example: ideal/ideal_v3.cer'),
    '#required' => TRUE,
    '#element_validate' => array('ideal_advanced_validate_public_certificate_path'),
  );
  $form['payment_settings']['expiration_period'] = array(
    '#type' => 'select',
    '#title' => t('Expiration period'),
    '#default_value' => $settings['expiration_period'],
    '#description' => t('The acquirer expiration period'),
    '#required' => TRUE,
    '#options' => array(
      10 => t('10 minutes'),
      15 => t('15 minutes'),
      20 => t('20 minutes'),
      30 => t('30 minutes'),
      45 => t('45 minutes'),
      60 => t('1 hour'),
      120 => t('2 hours'),
    ),
  );
  $form['module_settings']['debug_mode'] = array(
    '#type' => 'select',
    '#title' => t('Debug mode'),
    '#default_value' => $settings['debug_mode'],
    '#description' => t('Show all exceptions from the iDeal connector library'),
    '#required' => FALSE,
    '#options' => array(
      1 => t('Off'),
      0 => t('On'),
    ),
  );
  $form['module_settings']['icon_path'] = array(
    '#type' => 'textfield',
    '#title' => t('iDEAL icon path'),
    '#default_value' => $settings['icon_path'],
    '#description' => t('The full path to the iDEAL icon. For example: sites/default/files/ideal.gif'),
    '#required' => FALSE,
    '#element_validate' => array('ideal_advanced_validate_icon_path'),
  );

  $form['#theme'][] = 'system_settings_form';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}
