<?php

/**
 * @file
 * Configuration form for the Cookie Lock module
 */

/**
 * Page callback: Cookie Lock settings
 *
 * @see cookielock_menu()
 */
function cookielock_settings_page() {
  $form = drupal_get_form('cookielock_settings_form');
  return render($form);
}

/**
 * Cookie Lock settings form
 */
function cookielock_settings_form($form, &$form_state) {
  $form = array(
    'cookielock_islocked' => array(
      '#type' => 'checkbox',
      '#title' => t('Lock site'),
      '#default_value' => variable_get('cookielock_islocked', FALSE),
      '#description' => t('If checked, users must enter valid credentials to view site.'),
    ),
    'cookielock_username' => array(
      '#type' => 'textfield',
      '#title' => t('Username'),
      '#size' => 25,
      '#maxlength' => 25,
      '#default_value' => variable_get('cookielock_username', ''),
      '#description' => '<span style="color: red;">Changing this will force all users to reauthenticate.</span>',
    ),
    'cookielock_password' => array(
      '#type' => 'textfield',
      '#title' => t('Password'),
      '#size' => 25,
      '#maxlength' => 25,
      '#description' => t('<span style="color: red;">Changing this will force all users to reauthenticate.</span><br/>Leave blank to keep current password.'),
    ),
    'cookielock_unrestricted_ips' => array(
      '#type' => 'textarea',
      '#title' => t('Unrestricted IP addresses'),
      '#cols' => 40,
      '#default_value' => variable_get('cookielock_unrestricted_ips', ''),
      '#description' => t('Enter one IP address per line. Traffic from these IP addresses will be allowed without any restrictions. This is useful for cookieless API testing.'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ),
  );

  return $form;
}

/**
 * Form validate callback for the cookielock settings form
 */
function cookielock_settings_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['cookielock_islocked']) && $form_state['values']['cookielock_islocked'] == 1) {
    if (empty($form_state['values']['cookielock_username']) && variable_get('cookielock_username', '') == '') {
      form_set_error('cookielock_username', t('There isn\'t a username yet, so please set one.'));
    }
    if (empty($form_state['values']['cookielock_password']) && variable_get('cookielock_password', '') == '') {
      form_set_error('cookielock_password', t('There isn\'t a password yet, so please set one.'));
    }
  }

  if (!empty($form_state['values']['cookielock_unrestricted_ips'])) {
    $ips = explode(PHP_EOL, $form_state['values']['cookielock_unrestricted_ips']);
    foreach ($ips as $ip) {
      if (!filter_var(trim($ip), FILTER_VALIDATE_IP)) {
        if (trim($ip) != '') {
          form_set_error('cookielock_unrestricted_ips', t('IP address !ip is not valid.', array('!ip' => $ip)));
        }
        else {
          form_set_error('cookielock_unrestricted_ips', t('Unrestricted IP addresses cannot contain any blank lines.'));
        }
      }
    }
  }
}

/**
 * Form submit callback for the cookielock settings form
 */
function cookielock_settings_form_submit($form, &$form_state) {
  variable_set('cookielock_islocked', $form_state['values']['cookielock_islocked']);

  variable_set('cookielock_unrestricted_ips', $form_state['values']['cookielock_unrestricted_ips']);

  if (!empty($form_state['values']['cookielock_islocked']) && $form_state['values']['cookielock_islocked'] == 1) {
    // TODO should we do anything here?
  }

  // Only update the values if they are populated
  if (!empty($form_state['values']['cookielock_username'])) {
    variable_set('cookielock_username', $form_state['values']['cookielock_username']);
  }

  if (!empty($form_state['values']['cookielock_password'])) {
    variable_set('cookielock_password', md5($form_state['values']['cookielock_password']));
  }
}
