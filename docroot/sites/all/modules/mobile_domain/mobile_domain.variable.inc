<?php
/**
 * @file
 * Switch mobile - Variable definition
 */

/**
 * Implements hook_variable_info().
 */
function mobile_domain_variable_info() {
  $variables['mobile_domain_type'] = array(
    'type' => 'select',
    'title' => t('Domain type'),
    'options callback' => 'mobile_domain_mode_list',
    'default' => MOBILE_DOMAIN_MODE_STANDARD,
    'group' => 'mobile_domain',
    'multidomain' => TRUE,
  );
  $variables['mobile_domain_linked'] = array(
    'type' => 'select',
    'title' => t('Linked domain'),
    'options callback' => '_mobile_domain_variable_list_domains',
    'default' =>  0,
    'description' => t('Domain to switch to'),
    'group' => 'mobile_domain',
    'multidomain' => TRUE,
  );
  return $variables;
}

/**
 * Implements hook_variable_group_info().
 */
function mobile_domain_variable_group_info() {
  $groups['mobile_domain'] = array(
    'title' => t('Mobile domain'),
  );
  return $groups;
}

function _mobile_domain_variable_list_domains() {
  return array('' => t('<none>')) + _mobile_domain_list_domains();
}
