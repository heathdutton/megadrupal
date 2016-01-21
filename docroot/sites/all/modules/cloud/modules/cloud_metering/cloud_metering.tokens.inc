<?php

/**
 * @file
 * Supports cloud_meterings tokens
 */

/**
 * Implements hook_token_info(). 
 */
function cloud_metering_token_info() {
  $info['types']['cloud_metering_instance'] = array(
    'name' => t('Cloud Metering Instance Tokens'),
    'description' => t('Tokens related to a specific instance'),
    'needs-data' => 'instance',
  );
  $info['types']['cloud_metering_group'] = array(
    'name' => t('Cloud Metering Group Tokens'),
    'description' => t('Tokens for group threshold'),
    'needs-data' => 'group',
  );
  $info['types']['cloud_metering_user'] = array(
    'name' => t('Cloud Metering User Tokens'),
    'description' => t('Tokens for user threshold'),
    'needs-data' => 'cloud_user',
  );
  
  // instance tokens
  $info['tokens']['cloud_metering_instance']['username'] = array(
    'name' => t('Username'),
    'description' => t('Username'),
  );
  $info['tokens']['cloud_metering_instance']['instance-id'] = array(
    'name' => t('Instance Id'),
    'description' => t('The instance id'),
  );
  $info['tokens']['cloud_metering_instance']['instance-name'] = array(
    'name' => t('Instance Name'),
    'description' => t('The name of the instance'),
  );
  $info['tokens']['cloud_metering_instance']['terminate-url'] = array(
    'name' => t('Terminate Url'),
    'description' => t('Terminate Url'),
  );
  
  // group budget, cost, and group_threshold
  $info['tokens']['cloud_metering_group']['username'] = array(
    'name' => t('Username'),
    'description' => t('Username'),
  );
  $info['tokens']['cloud_metering_group']['group_budget'] = array(
    'name' => t('Group Budget'),
    'description' => t('Group Budget'),
  );
  $info['tokens']['cloud_metering_group']['group_cost'] = array(
    'name' => t('Group Cost'),
    'description' => t('Group Cost'),
  );
  $info['tokens']['cloud_metering_group']['group_threshold'] = array(
    'name' => t('Group Threshold'),
    'description' => t('Group Threshold'),
  );
  
  // user threshold
  $info['tokens']['cloud_metering_user']['username'] = array(
    'name' => t('Username'),
    'description' => t('Username'),
  );
  $info['tokens']['cloud_metering_user']['user_cost'] = array(
    'name' => t('User Cost'),
    'description' => t('User Cost'),
  );
  $info['tokens']['cloud_metering_user']['user_budget'] = array(
    'name' => t('User Budget'),
    'description' => t('User Budget'),
  );
  // cost, user_budget
  return $info;
}

/**
 * Implements hook_tokens().
 */
function cloud_metering_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $url_options = array('absolute' => TRUE);
    if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);
  $replacements = array();
  
  switch ($type) {
    case 'cloud_metering_instance':
      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'username':
            $replacements[$original] = $sanitize ? check_plain($data['user']->name) : $data['user']->name;
            break;
          case 'instance-id':
            $replacements[$original] = $sanitize ? check_plain($data['instance_id']) : $data['instance_id'];
            break;
          case 'instance-name':
            $replacements[$original] = $sanitize ? check_plain($data['instance_name']) : $data['instance_name'];
            break;
          case 'terminate-url':
            $replacements[$original] = url("clouds/${date['cloud_context']}/instances/terminate", array('query' => array('instance_id' => $data['instance_id'])));
            break;
        }
      }
      break;
    case 'cloud_metering_group':
      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'username':
            $replacements[$original] = $sanitize ? check_plain($data['user']->name) : $data['user']->name;
            break;
          case 'group_budget':
            $replacements[$original] = $sanitize ? check_plain($data['group_budget']) : $data['group_budget'];
            break;
          case 'group_cost':
            $replacements[$original] = $sanitize ? check_plain($data['group_cost']) : $data['group_cost'];
            break;
          case 'group_threshold':
            $replacements[$original] = $sanitize ? check_plain($data['group_threshold']) : $data['group_threshold'];
            break;
        } 
      }
      break;
    case 'cloud_metering_user':
      foreach ($tokens as $name => $original) {
        switch ($name) {
          case 'username':
            $replacements[$original] = $sanitize ? check_plain($data['user']->name) : $data['user']->name;
            break;
          case 'user_cost':
            $replacements[$original] = $sanitize ? check_plain($data['user_cost']) : $data['user_cost'];
            break;
          case 'user_budget':
            $replacements[$original] = $sanitize ? check_plain($data['user_budget']) : $data['user_budget'];
            break;
        }
      }
      break;
  }
  return $replacements;
}
