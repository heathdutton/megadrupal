<?php

/**
 * @file
 * Provides administrative forms for the ispconfig module.
 */

/**
 * Settings form for the ispconfig module.
 */
function ispconfig_settings_admin_form($form, &$form_state) {
  $protocol = array('http://' => t('http://'), 'https://' => t('https://'));

  // Get the settings
  $settings = variable_get('ispconfig_settings', array());

  $form = array();
  $form['#tree'] = TRUE;
  $form['ispconfig_settings'] = array();
  $form['ispconfig_settings']['protocol'] = array(
    '#type' => 'select',
    '#title' => t('Select Protocol'),
    '#options' => $protocol,
    '#default_value' => isset($settings['protocol']) ? $settings['protocol'] : 'http://',
    '#description' => t('Select http or https protocol.'),
    '#required' => TRUE,
  );
  $output = t('IPv4 Address or IPv6 Address  or Domain Name. eg:');
  $output .= '<b>192.168.12.20</b>' . t(' or ') . '<b>www.my-domain.com</b>';
  $output .= t(' or ') . '<b>2001:CDBA:0000:0000:0000:0000:3257:9652</b>.';
  $form['ispconfig_settings']['domain'] = array(
    '#type' => 'textfield',
    '#title' => t('Server Address'),
    '#default_value' => isset($settings['domain']) ? $settings['domain'] : '',
    '#description' => $output,
    '#required' => TRUE,
  );
  $form['ispconfig_settings']['port'] = array(
    '#type' => 'textfield',
    '#title' => t('Port Number'),
    '#default_value' => isset($settings['port']) ? $settings['port'] : '80',
    '#description' => t('Submit Port Number.'),
    '#required' => TRUE,
  );
  $form['ispconfig_settings']['connection_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Connection Timeout'),
    '#default_value' => isset($settings['connection_timeout']) ? $settings['connection_timeout'] : 0,
    '#description' => t('Connection timeout in seconds.'),
  );
  $form['ispconfig_settings']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('Username'),
    '#default_value' => isset($settings['username']) ? $settings['username'] : '',
    '#description' => t('ISPConfig Remote Username.'),
    '#required' => TRUE,
  );
  $form['ispconfig_settings']['password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => isset($settings['password']) ? $settings['password'] : '',
    '#description' => t('ISPConfig Remote Password.'),
    '#required' => TRUE,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );
  return $form;
}

/**
 * Validation for the ispconfig settings form.
 */
function ispconfig_settings_admin_form_validate($form, &$form_state) {
  // Defining Regular Expression for Domain, IPv4, IPv6 & Port.
  $domain_regx = '/^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]'
      . '|2[0-4][0-9]|[01]?[0-9][0-9]?)'
      . '|(([a-z0-9]+(-[a-z0-9-]+)*\.)+[a-z]{2,})'
      . '|([0-9a-fA-F]{1,4}(?:\:[0-9a-fA-F]{1,4}){7})'
      . '$/';
  $port_regx = '/^\d{1,5}$/';

  // Retrieving ISPConfig Value.
  $ispconfig_domain = $form_state['values']['ispconfig_settings']['domain'];
  $ispconfig_port = $form_state['values']['ispconfig_settings']['port'];
  $connection_timeout = $form_state['values']['ispconfig_settings']['connection_timeout'];

  // Check if Domain entered is a valid domain else set form error.
  if (!preg_match($domain_regx, $ispconfig_domain)) {
    form_set_error('ispconfig_settings][domain', t('Invalid Domain Name. Please enter valid domain name or ip address.'));
  }

  // Check if Port entered is a valid port else set form error.
  if (!is_numeric($ispconfig_port) || !(($ispconfig_port >= 0) && ($ispconfig_port <= 65535)) || !(preg_match($port_regx, $ispconfig_port))) {
    form_set_error('ispconfig_settings][port', t('Invalid Port Number. Please enter valid port number.'));
  }

  // Check if Negotiation Timeout field is valid.
  if ($connection_timeout != '') {
    if (!is_numeric($connection_timeout) || intval($connection_timeout) != $connection_timeout || $connection_timeout < 0) {
      form_set_error('ispconfig_settings][connection_timeout', t('Invalid value in Connection Timeout.'));
    }
  }
}

/**
 * Submit handler for ispconfig settings form.
 */
function ispconfig_settings_admin_form_submit($form, &$form_state) {
  // Rebuilding the form before submission.
  $form_state['rebuild'] = TRUE;

  $settings = $form_state['values']['ispconfig_settings'];
  $ipv6_regx = '/^[0-9a-fA-F]{1,4}(?:\:[0-9a-fA-F]{1,4}){7}$/';
  $soap_uri = $settings['protocol'] .
      ((preg_match($ipv6_regx, $settings['domain'])) ? ('[' . $settings['domain'] . ']') : $settings['domain']) .
      ((!empty($settings['port'])) ? ':' . $settings['port'] : '') .
      '/remote/';
  $soap_location = $soap_uri . 'index.php';

  $settings['location'] = $soap_location;
  $settings['uri'] = $soap_uri;

  // Saving ISPConfig settings to variables table in drupal database.
  variable_set('ispconfig_settings', $settings);

  // Notifying user of data has been saved.
  drupal_set_message(t('ISPConfig settings has been saved.'));
}