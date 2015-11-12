<?php

/**
 * @file
 * Provide a Webform A/B Test win threshold: Minimum number of hits
 */

// Plugin details
$plugin = array(
  'title' => t('Minimum Hits'),
  'description' => t('All webforms must be visited by at least this many users before the win conditions are checked.'),
  'config form' => 'webform_ab_minimum_hits_config',
  'config form validation' => 'webform_ab_minimum_hits_config_validate',
  'config summary' => 'webform_ab_minimum_hits_config_summary',
  'check criteria' => 'webform_ab_minimum_hits_check_threshold',
);

/**
 * Return the config options form for this win threshold
 *
 * @param $current_config
 *   Array containing current config options (if set). Default values for form
 *   elements should come from here.
 */
function webform_ab_minimum_hits_config($current_config) {
  $form = array();
  $form['hits'] = array(
    '#type' => 'textfield',
    '#title' => t('Hits'),
    '#description' => t('The minimum number of hits.'),
    '#size' => 10,
    '#maxlength' => 255,
    '#default_value' => isset($current_config['hits']) ? $current_config['hits'] : '',
  );
  return $form;
}

/**
 * Validate the config for this plugin
 */
function webform_ab_minimum_hits_config_validate($config, $field_name_prefix) {
  if (!ctype_digit($config['hits']) || !($config['hits'] > 0)) {
    form_set_error($field_name_prefix . 'hits', t('Please enter an integer for Minimum Hits win threshold.'));
  }
}


/**
 * Return a summary of the plugin config, for listing on the admin view of a
 * Webform A/B test node
 */
function webform_ab_minimum_hits_config_summary($config) {
  return $config['hits'];
}


/**
 * Check if the given Webform A/B Test node has a winner
 *
 * @param $config
 *   Array of config options for this plugin for the Webform A/B test being
 *   checked for a win. In this case, the array will contain 'hits'
 * @param $webform_details
 *   Array of info about the webform to check.
 * @param $test_node
 *   Webform A/B Test node
 */
function webform_ab_minimum_hits_check_threshold($config, $webform_details, $test_node) {
  $hits = isset($webform_details['hits']) ? $webform_details['hits'] : 0;
  return ($hits >= $config['hits']);
}
