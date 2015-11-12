<?php

/**
 * @file
 * Definition of variables for Variable API module.
 */

/**
 * Implements hook_variable_info().
 */
function snoobi_variable_info($options) {
  $variables['snoobi_username'] = array(
    'type' => 'string',
    'title' => t('Snoobi Username', array(), $options),
    'default' => 'corporatename_fi',
    'description' => t('Your username for snoobi.com. Example: corporatename_fi'),
    'required' => TRUE,
    'group' => 'snoobi',
    'localize' => TRUE,
  );

  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function snoobi_variable_group_info() {
  $groups['snoobi'] = array(
    'title' => t('Snoobi'),
    'description' => t('Configure tracking behavior to get insights into your website traffic and marketing effectiveness.'),
    'access' => 'administer snoobi',
    'path' => array('admin/config/system/snoobi'),
  );

  return $groups;
}

