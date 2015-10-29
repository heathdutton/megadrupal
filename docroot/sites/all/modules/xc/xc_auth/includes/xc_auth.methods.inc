<?php
/**
 * @file
 * XC Authentication method register
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * Implements hook_xc_auth_methods().
 */
function xc_auth_xc_auth_methods() {
  $methods = array();

//  LDAP INTEGRATION EXPERIMENTAL!!!
  //
//  if ($_xc_auth_ldap_enabled) {
//    $methods['ldap'] = array(
//      'name' => t('LDAP Authentication'),
//      'description' => t('User/password authentication via LDAP'),
//      'credentials' => 'user_pass'
//    );
//  }

//  $methods['drupal'] = array(
//    'name' => t('Drupal'),
//    'description' => t('User/password authentication via Drupal'),
//    'credentials' => 'user_pass'
//  );

  return $methods;
}

