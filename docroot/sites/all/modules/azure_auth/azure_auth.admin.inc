<?php
/**
 * @file
 * Implements the module's settings form and related callbacks.
 */

/**
 * Settings form.
 */
function azure_auth_settings($form, $form_state) {
  $form = array();

  $form['azure_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Windows Azure Settings'),
    '#description' => t("Once you've created a namespace and created the appropriate identity providers and relying party application in the ACS management portal, enter the relying party application details here."),
  );
  $form['azure_settings']['azure_auth_namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace'),
    '#default_value' => variable_get('azure_auth_namespace'),
    '#required' => TRUE,
  );
  $form['azure_settings']['azure_auth_realm'] = array(
    '#type' => 'textfield',
    '#title' => t('Realm'),
    '#default_value' => variable_get('azure_auth_realm'),
    '#required' => TRUE,
  );
  $form['azure_settings']['azure_auth_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Token Signing Key'),
    '#default_value' => variable_get('azure_auth_key'),
    '#required' => TRUE,
  );
  $form['azure_settings']['azure_auth_oldkey'] = array(
    '#type' => 'textfield',
    '#title' => t('Old Token Signing Key'),
    '#default_value' => variable_get('azure_auth_oldkey'),
    '#description' => t("Leave blank if you don't have one."),
  );

  $form['ui_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('UI Settings'),
    '#description' => t("Enabled the dedicated block will allow users to log in using Windows Azure, while altering the user login form places Windows Azure and the normal user login next to each other."),
    'block' => array(
      '#type' => 'fieldset',
      '#title' => t('Dedicated Block'),
    ),
    'alter' => array(
      '#type' => 'fieldset',
      '#title' => t('User Login Form'),
    ),
  );
  $form['ui_settings']['block']['azure_auth_provide_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide Block'),
    '#default_value' => variable_get('azure_auth_provide_block', FALSE),
    '#description' => t("Provides a dedicated block (separate from the normal user login block) for logging in via Windows Azure."),
  );
  $form['ui_settings']['alter']['azure_auth_alter_login_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Alter User Login Block'),
    '#default_value' => variable_get('azure_auth_alter_login_form', FALSE),
    '#description' => t("Adds a link at the bottom of the default user login form (block and page) to log in via Windows Azure."),
  );
  $form['ui_settings']['alter']['azure_auth_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link Text'),
    '#default_value' => variable_get('azure_auth_link_text', 'Sign in using Windows Azure'),
    '#required' => TRUE,
  );

  $form['#validate'][] = 'azure_auth_settings_validate';
  $form['#submit'][] = 'azure_auth_settings_submit';

  return system_settings_form($form);
}

/**
 * Form validate callback.
 */
function azure_auth_settings_validate($form, &$form_state) {
  if (form_get_errors()) {
    /*
     * Skip our attempt to validate the namespace and realm if previous errors
     * were encountered.
     */
    return;
  }
  $identity_providers = _azure_auth_get_identity_providers($form_state['values']['azure_auth_namespace'], $form_state['values']['azure_auth_realm']);
  if (!$identity_providers) {
    form_set_error('azure', 'Invalid settings detected.');
  }
}

/**
 * Form submit callback.
 */
function azure_auth_settings_submit($form, &$form_state) {
  variable_set('azure_auth_settings_valid', TRUE);
}
