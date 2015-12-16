<?php

/**
 * Page callback: Bad Performance Administration Form
 */
function bad_performance_admin_settings($form, &$form_state) {
  $form['bad_performance_db_latency'] = array(
    '#type' => 'textfield',
    '#title' => t('Database latency'),
    '#default_value' => variable_get('bad_performance_db_latency', 0),
    '#size' => 4,
    '#maxlength' => 4,
    '#description' => t('Add database latency to all queries (in ms).  Max 1000 ms latency, let\'s not go crazy here.'),
    '#required' => TRUE,
  );

  $form['bad_performance_cpu_burn_multiplier'] = array(
    '#type' => 'textfield',
    '#title' => t('CPU Burn multiplier'),
    '#default_value' => variable_get('bad_performance_cpu_burn_multiplier', 0),
    '#size' => 3,
    '#maxlength' => 3,
    '#description' => t('Increase this to increase the amount of CPU load this module creates.  Setting this to "0" essentially disables this feature.  Try between 1 and 10.'),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Form validate for the settings form.
 */
function bad_performance_admin_settings_validate(&$form, &$form_state) {
  $dbl = $form_state['values']['bad_performance_db_latency'];
  if (!is_numeric($dbl) or $dbl < 0 or $dbl > 1000) {
    form_set_error('bad_performance_db_latency', t('Database latency must be a number between 0 and 1000 inclusively.'));
  }

  $bm = $form_state['values']['bad_performance_cpu_burn_multiplier'];
  if (!is_numeric($bm) or $bm < 0 or $bm > 1000) {
    form_set_error('bad_performance_cpu_burn_multiplier', t('CPU Burn multiplier must be a number between 0 and 1000 inclusively.'));
  }
}
