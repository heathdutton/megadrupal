<?php

/**
 *
 */
function gp_configuration_form($form, &$form_state) {
  $form['readme'] = array(
    '#markup' => l(t('View README.txt'), drupal_get_path('module', 'gp') . '/README.txt')
  );
  $form['gp_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Places API Key'),
    '#default_value' => gp_api_key(),
    '#size' => 48,
    '#description' => t('Your Google Places API Key.'),
    '#required' => TRUE,
  );
  $form['gp_cache_expiration'] = array(
    '#type' => 'select',
    '#title' => t('Cache results'),
    '#default_value' => gp_cache_expiration(),
    '#options' => array(
      0 => 'Disabled',
      3600 => '1 hour',
      10800 => '3 hours',
      21600 => '6 hours',
      43200 => '12 hours',
      86400 => '1 day',
      259200 => '3 days',
      604800 => '7 days',
      1296000 => '15 days',
      2592000 => '30 days'
    ),
    '#description' => t('How long to cache a Place result from Google.')
  );
  $form['gp_cache_queue_worker_time'] = array(
    '#type' => 'textfield',
    '#title' => t('Queue worker time'),
    '#size' => 8,
    '#field_suffix' => t('seconds'),
    '#default_value' => gp_cache_queue_worker_time(),
    '#description' => t('How many seconds to work when rebuilding the Place cache from Google.')
  );
  return system_settings_form($form);
}

/**
 *
 */
function gp_configuration_form_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['gp_cache_queue_worker_time'])) {
    form_set_error('gp_cache_queue_worker_time', t('You must enter an integer value.'));
  }
}

