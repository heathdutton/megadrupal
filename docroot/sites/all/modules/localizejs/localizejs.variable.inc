<?php

/**
 * @file
 * Definition of variables for Variable API module.
 */

/**
 * Implements hook_variable_info().
 */
function localizejs_variable_info($options) {
  $variables['localizejs_account'] = array(
    'type' => 'string',
    'title' => t('Project Key', array(), $options),
    'default' => '',
    'description' => t('Put here your localize project key. Create your project on https://localizejs.com.'),
    'required' => TRUE,
    'group' => 'localizejs',
    'localize' => TRUE,
    'multidomain' => TRUE,
    'validate callback' => 'localizejs_validate_localizejs_account',
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function localizejs_variable_group_info() {
  $groups['localizejs'] = array(
    'title' => t('Localizejs'),
    'description' => t('Configure your project key to integrate Localize js into your Drupal site.'),
    'access' => 'administer localizejs',
    'path' => array('admin/config/system/localizejs'),
  );

  return $groups;
}

/**
 * Validate localizejs key variable.
 */
function localizejs_validate_localizejs_account($variable) {
    
}
