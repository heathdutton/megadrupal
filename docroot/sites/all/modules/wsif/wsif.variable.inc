<?php

/**
 * @file
 * Variable module integration.
 */

/**
 * Implements hook_variable_group_info().
 *
 * Create a variable group for each web service.
 */
function wsif_variable_group_info() {

  foreach (wsif_get_apis_info() as $api_name => $api_info) {
    $groups[$api_name] = array(
      'title' => $api_info['name'],
      'description' => $api_info['description'],
      'access' => 'administer site configuration',
    );
  }

  return $groups;
}

/**
 * Implements hook_variable_info().
 */
function wsif_variable_info($options) {
  $variable = array();

  foreach (wsif_get_apis_info() as $api_name => $api_info) {
    $variable["wsif-{$api_name}-unavailable"] = array(
      'title' => t('@api_name is unavailable', array('@api_name' => $api_info['name'])),
      'description' => t('Set to TRUE when the webservice is unavailable'),
      'type' => 'boolean',
      'default' => FALSE,
      'group' => $api_name,
    );

    $variable["wsif-{$api_name}-fail-count"] = array(
      'title' => t('@api_name fail count', array('@api_name' => $api_info['name'])),
      'description' => t('The number of failed accounts since the last cron run'),
      'type' => 'number',
      'default' => 0,
      'group' => $api_name,
    );

    $variable["wsif-{$api_name}-fail-cutoff"] = array(
      'title' => t('@api_name fail cutoff', array('@api_name' => $api_info['name'])),
      'description' => t('The number of failed connections to the webservice which can occur between cron runs before the webservice is switched off.'),
      'type' => 'number',
      'default' => 5,
      'group' => $api_name,
    );

    $variable["wsif-{$api_name}-fail-email"] = array(
      'title' => t('@api_name failure email', array('@api_name' => $api_info['name'])),
      'description' => t('The email address an email is sent to in the case of the webservice being cut off.'),
      'type' => 'mail_address',
      'default' => '',
      'group' => $api_name,
    );

    $variable["wsif-{$api_name}-kill"] = array(
      'title' => t('@api_name kill switch', array('@api_name' => $api_info['name'])),
      'description' => t('Set to TRUE to make the service unreachable, regardless of failure tracking.'),
      'type' => 'boolean',
      'default' => FALSE,
      'group' => $api_name,
    );
  }

  return $variable;
}
