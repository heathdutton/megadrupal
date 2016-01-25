<?php
/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_variable_info().
 */
function webducateapi_variable_info($options) {
  $variables = array();

  $variables['webducateapi_settings'] = array(
    'name' => 'webducateapi_settings',
    'title' => 'API settings',
    'type' => 'array',
    'default' => array(
      'url' => 'http://portal.webducate.com.au/services/xmlrpc',
      'key' => NULL,
      'domain' => NULL,
      'username' => NULL,
      'password' => NULL,
    ),
    'group' => 'webducateapi',
    'module' => 'webducateapi',
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function webducateapi_variable_group_info() {
  $groups = array();

  $groups['webducateapi'] = array(
    'title' => 'WebducateAPI',
    'description' => '',
    'path' => 'admin/config/services/webducate',
    'access' => 'access administration menu',
  );

  return $groups;
}
