<?php
/**
 * @file
 * Admin functions for azure_acs
 *
 */

/**
 * Form definitions for Windows Azure ACS main admin form
 *
 * @ingroup forms
 */
function azure_acs_admin($form, &$form_state) {
  $form = array();

  $form['azure_acs_namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace'),
    '#default_value' => variable_get('azure_acs_namespace', ''),
    '#size' => 60,
    '#description' => t('The fully qualified domain name for your ACS namespace (e.g. mynamespace.accesscontrol.windows.net)'),
  );

  $form['azure_acs_token_signing_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Token signing key'),
    '#default_value' => variable_get('azure_acs_token_signing_key', ''),
    '#size' => 60,
    '#description' => t('Signing key for relying party application. This key can be obtained from Windows Azure management portal.'),
  );

  $url = url('', array('absolute' => TRUE));
  $form['azure_acs_realm'] = array(
    '#type' => 'textfield',
    '#title' => t('Application realm'),
    '#default_value' => variable_get('azure_acs_realm', ''),
    '#size' => 60,
    '#description' => t('Application realm you entered when adding this site as a relying party application (optional, leave empty to default to %url).', array('%url' => $url)),
  );

  $form['azure_acs_create_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create users automatically'),
    '#default_value' => variable_get('azure_acs_create_users', TRUE),
    '#description' => t('Should new users be created and logged in automatically by the module or should the user be taken to the registration form with some prefilled values.'),
  );

  $form['azure_acs_link_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link users with multiple identity providers'),
    '#default_value' => variable_get('azure_acs_link_users', FALSE),
    '#description' => t('Should the module try and identify when an existing user has logged in with another identity provider. ' .
                        'If checked, an unidentified user will be logged in if they have the same email address as an existing user. ' .
                        'Otherwise, login will produce an error if a duplicate email address is found.'),
  );

  return system_settings_form($form);
}