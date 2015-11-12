<?php
/**
 * @file
 * Functions for Simple LDAP admin interface.
 */

/**
 * LDAP server configuration form.
 */
function simple_ldap_admin() {
  $form = array();

  $form['server'] = array(
    '#type' => 'fieldset',
    '#title' => t('LDAP Server Connection Information'),
    '#collapsible' => FALSE,
  );

  $form['server']['simple_ldap_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Host'),
    '#required' => TRUE,
    '#default_value' => variable_get('simple_ldap_host', ''),
    '#description' => t('To use SSL, prepend the host with "ldaps://".'),
  );

  $form['server']['simple_ldap_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port'),
    '#default_value' => variable_get('simple_ldap_port', '389'),
    '#required' => TRUE,
  );

  $form['server']['simple_ldap_starttls'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use startTLS'),
    '#default_value' => variable_get('simple_ldap_starttls', FALSE),
  );

  $form['server']['simple_ldap_readonly'] = array(
    '#type' => 'checkbox',
    '#title' => t('Read-only'),
    '#default_value' => variable_get('simple_ldap_readonly', FALSE),
  );

  $form['directory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Directory Information'),
    '#collapsible' => FALSE,
  );

  $form['directory']['simple_ldap_binddn'] = array(
    '#type' => 'textfield',
    '#title' => t('Bind DN'),
    '#default_value' => variable_get('simple_ldap_binddn', ''),
    '#description' => t('Leave this blank to bind anonymously'),
  );

  $form['directory']['simple_ldap_bindpw'] = array(
    '#type' => 'password',
    '#title' => t('Bind password'),
    '#description' => t('Leave this blank to bind anonymously'),
    '#attributes' => array('value' => array(variable_get('simple_ldap_bindpw', ''))),
  );

  $form['directory']['simple_ldap_basedn'] = array(
    '#type' => 'textfield',
    '#title' => t('Base DN'),
    '#default_value' => variable_get('simple_ldap_basedn', ''),
    '#description' => t('Leave this blank to attempt to detect the base DN.'),
  );

  $form['directory']['simple_ldap_pagesize'] = array(
    '#type' => 'textfield',
    '#title' => t('Search result page size'),
    '#default_value' => variable_get('simple_ldap_pagesize', ''),
    '#description' => t('Leave this blank to disable paged queries.'),
  );

  // Disable the option if paged queries are not supported.
  if (!function_exists('ldap_control_paged_result_response') && !function_exists('ldap_control_paged_result')) {
    $form['directory']['simple_ldap_pagesize']['#disabled'] = TRUE;
    $form['directory']['simple_ldap_pagesize']['#default_value'] = '';
    $form['directory']['simple_ldap_pagesize']['#description'] = t('Paged queries are not supported by this PHP installation.') . ' ' . t('Support was added in PHP version 5.4.');
  }

  // Advanced settings.
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['advanced']['simple_ldap_opt_referrals'] = array(
    '#type' => 'checkbox',
    '#title' => t('Follow LDAP referrals'),
    '#default_value' => variable_get('simple_ldap_opt_referrals', TRUE),
  );

  $form['advanced']['simple_ldap_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable watchdog debug logging.'),
    '#default_value' => variable_get('simple_ldap_debug', FALSE),
  );

  return system_settings_form($form);
}
