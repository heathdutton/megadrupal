<?php

/**
 * @file
 * i18n integration for Multistep Nodeform.
 */

/**
 * Implements hook_i18n_string_info().
 */
function msnf_i18n_string_info() {
  $groups['msnf_step'] = array(
    'title' => t('Form steps'),
    'description' => t('Translatable form steps: title and description.'),
    'format' => FALSE,
    'list' => TRUE,
  );
  return $groups;
}

/**
 * Implements hook_i18n_string_list().
 *
 * @param <string> $group
 *   Text group name.
 */
function msnf_i18n_string_list($group) {
  $strings = array();
  if ($group == 'msnf_step') {
    // Get a list of all step definitions across all entity types and bundles.
    $step_data = msnf_info_steps('node');
    foreach ($step_data as $bundle => $steps) {
      foreach ($steps as $step_name => $step) {
        $strings['msnf_step'][$step->step_name][$bundle]['label'] = filter_xss($step->label);
        $strings['msnf_step'][$step->step_name][$bundle]['description'] = filter_xss($step->format_settings['instance_settings']['description']);
        $strings['msnf_step'][$step->step_name][$bundle]['button_previous'] = filter_xss($step->format_settings['instance_settings']['buttons']['previous']);
        $strings['msnf_step'][$step->step_name][$bundle]['button_next'] = filter_xss($step->format_settings['instance_settings']['buttons']['next']);
        $strings['msnf_step'][$step->step_name][$bundle]['button_skip'] = filter_xss($step->format_settings['instance_settings']['buttons']['skip']);
      }
    }
  }
  return $strings;
}
