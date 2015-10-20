<?php
/**
 * @file
 * XC NCIP provider authentication functions
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_xc_auth_methods().
 */
function xc_ncip_provider_xc_auth_methods() {
  $methods['ncip_provider'] = array(
    'name' => t('NCIP server authentication'),
    'description' => t('Authentication with NCIP server using either username and password or LDAP username and password'),
    'credentials' => 'user_pass',
  );

  return $methods;
}

/**
 * Implementation of xc_auth_method_hook_form()
 */
function xc_auth_method_ncip_provider_form(&$form_state) {
  $ncip_provider_options = array();
  $ncip_providers = xc_ncip_provider_get_all();
  foreach ($ncip_providers as $ncip_provider) {
    if ($ncip_provider->properties['auth']['allow_authentication']) {
      $ncip_provider_options[$ncip_provider->ncip_provider_id] = $ncip_provider->name;
    }
  }

  $form['ncip_provider_auth_type'] = array(
    '#type' => 'radios',
    '#title' => t('NCIP server authentication type'),
    '#description' => t('Whether NCIP authenticating by default or LDAP username and password'),
    '#options' => array(
      'default' => t('Default'),
      'ldap' => t('LDAP'),
    ),
    '#default_value' => 'default',
    '#required' => TRUE,
  );

  $form['ncip_provider_id'] = array(
    '#type' => 'select',
    '#title' => t('NCIP server'),
    '#options' => $ncip_provider_options,
    '#required' => TRUE,
  );

  return $form;
}

function xc_auth_method_ncip_provider_validate($form, &$from_state) { }

function xc_auth_method_ncip_provider_submit($form, &$from_state) { }

function xc_auth_method_login_ncip_provider_validate($form, &$from_state) { }

function xc_auth_method_login_ncip_provider_submit($form, &$from_state) { }
