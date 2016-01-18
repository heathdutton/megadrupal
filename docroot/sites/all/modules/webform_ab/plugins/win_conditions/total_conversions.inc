<?php

/**
 * @file
 * Provide a Webform A/B Test win condition: Total number of conversions
 */

// Plugin details
$plugin = array(
  'title' => t('Total Conversions'),
  'description' => t('This many conversions must be made by a webform before it is declared the winner.'),
  'config form' => 'webform_ab_total_conversions_config',
  'config form validation' => 'webform_ab_total_conversions_config_validate',
  'config summary' => 'webform_ab_total_conversions_config_summary',
  'check criteria' => 'webform_ab_total_conversions_check_win',
);


/**
 * Return the config options form for this win condition
 *
 * @param $current_config
 *   Array containing current config options (if set). Default values for form
 *   elements should come from here.
 */
function webform_ab_total_conversions_config($current_config) {
  $form = array();
  $form['conversions'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of Conversions'),
    '#description' => t(''),
    '#size' => 10,
    '#maxlength' => 255,
    '#default_value' => isset($current_config['conversions']) ? $current_config['conversions'] : '',
  );
  return $form;
}


/**
 * Validate the config for this plugin
 */
function webform_ab_total_conversions_config_validate($config, $field_name_prefix) {
  if (!ctype_digit($config['conversions']) || !($config['conversions'] > 0)) {
    form_set_error($field_name_prefix . 'conversions', t('Please enter an integer for Number of Conversions win condition.'));
  }
  return TRUE;
}


/**
 * Return a summary of the plugin config, for listing on the admin view of a
 * Webform A/B test node
 */
function webform_ab_total_conversions_config_summary($config) {
  return $config['conversions'];
}


/**
 * Check if the given Webform A/B Test node has a winner
 *
 * @param $config
 *   Array of config options for this plugin for the Webform A/B test being
 *   checked for a win. In this case, the array will contain 'conversions'
 * @param $webform_details
 *   Array of info about the webform to check.
 * @param $test_node
 *   Webform A/B Test node
 */
function webform_ab_total_conversions_check_win($config, $webform_details, $test_node) {
  return ($webform_details['conversions'] >= $config['conversions']);
}
