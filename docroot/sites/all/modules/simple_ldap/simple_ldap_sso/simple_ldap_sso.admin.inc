<?php

/**
 * @file
 * Simple LDAP SSO Admin pages.
 */

/**
 * Admin form for Single Sign On.
 */
function simple_ldap_sso_admin() {
  $session_inc_path = drupal_get_path('module', 'simple_ldap_sso') . '/simple_ldap_sso.session.inc';
  $t_args = array('!path' => $session_inc_path);
  if (variable_get('session_inc') != $session_inc_path) {
    $form['session_inc'] = array(
      '#type' => 'textfield',
      '#title' => t('Drupal Session Include File'),
      '#description' => t('The file used to handle Drupal user sessions. This should be set to !path.', $t_args),
      '#default_value' => variable_get('session_inc'),
    );
  }

  $form['credentials'] = array(
    '#access' => variable_get('simple_ldap_readonly'),
    '#type' => 'fieldset',
    '#title' => t('Read/Write LDAP Credentials'),
    '#description' => t('Since the Simple LDAP module is currently in Read Only mode for this site, you must specify separate read/write credentials to connect to LDAP with.'),
  );
  $form['credentials']['simple_ldap_sso_binddn'] = array(
    '#type' => 'textfield',
    '#title' => t('Bind DN'),
    '#default_value' => variable_get('simple_ldap_sso_binddn'),
    '#required' => variable_get('simple_ldap_readonly'),
  );
  $form['credentials']['simple_ldap_sso_bindpw'] = array(
    '#type' => 'password',
    '#title' => t('Bind password'),
    '#default_value' => variable_get('simple_ldap_sso_bindpw'),
    '#required' => variable_get('simple_ldap_readonly'),
    // Set the value on the input so that it won't get wiped out. This will mean
    // that someone could view source and get the value, but if someone is
    // logged in as a user that can access this form, you've got worse problems.
    '#attributes' => array('value' => array(variable_get('simple_ldap_sso_bindpw', ''))),
  );

  $form['basic'] = array(
    '#type' => 'fieldset',
    '#title' => t('Basic Settings'),
  );
  $form['basic']['simple_ldap_sso_encryption_key'] = array(
    '#type' => 'password',
    '#title' => t('Shared Encryption Key'),
    '#description' => t('This is the encryption key used to encrypt and decrypt the cookie value used to assist the single-sign-on. It must be the same accross all sites.'),
    // Set the value on the input so that it won't get wiped out. This will mean
    // that someone could view source and get the value, but if someone is
    // logged in as a user that can access this form, you've got worse problems.
    '#attributes' => array('value' => array(variable_get('simple_ldap_sso_encryption_key', ''))),
  );

  $form['basic']['simple_ldap_sso_attribute_sid'] = array(
    '#type' => 'select',
    '#title' => t('LDAP Session ID Attribute'),
    '#options' => simple_ldap_sso_get_attribute_options(),
    '#default_value' => variable_get('simple_ldap_sso_attribute_sid'),
    '#required' => TRUE,
    '#description' => t('Specify the LDAP attribute that will store the session ID.'),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Advanced Settings'),
  );

  $options = SimpleLdap::hashes();
  unset($options['none']);
  $form['advanced']['simple_ldap_sso_hashing_algorithm'] = array(
    '#type' => 'select',
    '#title' => t('Session ID Hashing Algorithm'),
    '#description' => t('Choose the algorithm that will be used to hash the session ID stored on LDAP.'),
    '#options' => $options,
    '#default_value' => variable_get('simple_ldap_sso_hashing_algorithm', 'sha'),
  );

  $options = range(0, 20);
  $options[0] = t('Off. Not Recommended.');
  $form['advanced']['simple_ldap_sso_flood_limit'] = array(
    '#type' => 'select',
    '#title' => t('Failed SSO Limit'),
    '#description' => t('The limit of failed SSO attempts a user can make from a single IP.'),
    '#options' => $options,
    '#default_value' => variable_get('simple_ldap_sso_flood_limit', 3),
  );

  $form['advanced']['simple_ldap_sso_flood_window'] = array(
    '#type' => 'select',
    '#title' => t('Failed SSO Window'),
    '#description' => t('The window of time in which to enforce the above limit. Higher is safer. Lower is more tolerant.'),
    '#options' => array(
      60 => t('One minute'),
      120 => t('Two minutes'),
      300 => t('Five minutes'),
      600 => t('Ten minutes'),
      900 => t('Fifteen minutes'),
      1800 => t('Thirty minutes'),
      3600 => t('One hour'),
      7200 => t('Two hours'),
      1800 => t('Five hours'),
      86400 => t('One day'),
      604800 => t('One week'),
    ),
    '#default_value' => variable_get('simple_ldap_sso_flood_window', 3600),
  );

  return system_settings_form($form);
}

/**
 * Returns an array of LDAP attributes.
 */
function simple_ldap_sso_get_attribute_options() {
  // Initialize a Simple LDAP Server object. Used for dropdown options.
  $server = SimpleLdapServer::singleton();

  $objectclass = variable_get('simple_ldap_user_objectclass', array('inetorgperson'));

  $attributes = array();

  // Generate a list of attributes for the selected objectclass.
  foreach ($objectclass as $o) {
    $result = $server->schema->attributes($o, TRUE);
    foreach ($result as $attribute) {
      $attributes[strtolower($attribute)] = $attribute;
    }
  }
  asort($attributes);

  return $attributes;
}

/**
 * Element validate for the encryption salt variable.
 *
 * If the submitted value is empty and a previous value exists, it is preserved.
 */
function simple_ldap_sso_encryption_key_element_validate($element, &$form_state, $form) {
  if (empty($element['#value']) && $existing_salt = variable_get('simple_ldap_sso_encryption_key')) {
    form_set_value($element, $existing_salt, $form_state);
  }
}
