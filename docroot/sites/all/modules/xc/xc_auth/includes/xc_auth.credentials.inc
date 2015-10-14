<?php
/**
 * @file
 * XC Authentication credentials functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_xc_auth_credentials().
 */
function xc_auth_xc_auth_credentials() {
  $credentials['user_pass'] = array(
    'name' => t('Username and Password'),
    'description' => t('Username and password combination for authentication'),
    'credentials' => array(
      'user' => array(
        'type' => 'textfield',
        'title' => t('Username'), // "title" != "label"
        'label' => t('Username'),
        'weight' => 0,
        'required' => TRUE,
        'identity' => TRUE,
        'stored' => TRUE,
      ),
      'pass' => array(
        'type' => 'password',
        'title' => t('Password'), // "title" != "label"
        'label' => t('Password'),
        'weight' => 1,
        'required' => TRUE,
        'stored' => FALSE,
      ),
    ),
  );

  $credentials['user_pass_confirm'] = array(
    'name' => t('Username and Password with Confirmation'),
    'description' => t('Username and password combination for authentication, with password confirmation'),
    'credentials' => array(
      'user' => array(
        'type' => 'textfield',
        'title' => t('Username'), // "title" != "label"
        'label' => t('Username'),
        'description' => t(''),
        'weight' => 0,
        'required' => TRUE,
        'identity' => TRUE,
        'stored' => TRUE,
      ),
      'pass' => array(
        'type' => 'password_confirm',
        'title' => t('Password'), // "title" != "label"
        'label' => t('Password'),
        'description' => t(''),
        'weight' => 1,
        'required' => TRUE,
        'stored' => FALSE,
      ),
    ),
  );

  return $credentials;
}

/**
 * Implementation of xc_auth_identify_hook()
 */
function xc_auth_identify_user_pass($values) {
  return $values['user'];
}

/**
 * Implementation of xc_auth_reverse_identify_hook()
 */
function xc_auth_reverse_identify_user_pass($identity) {
  $values['user'] = $identity;
  return $values;
}

/**
 * Implementation of xc_auth_identify_hook()
 */
function xc_auth_identify_user_pass_confirm($values) {
  return $values['user'];
}

/**
 * Implementation of xc_auth_reverse_identify_hook()
 */
function xc_auth_reverse_identify_user_pass_confirm($identity) {
  $values['user'] = $identity;
  return $values;
}
