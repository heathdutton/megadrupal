<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers for the PMPAPI module.
 */

/**
 * Form constructor for the PMPAPI admin form.
 *
 * @see pmpapi_config_form_validate()
 * @see pmpapi_config_form_submit()
 *
 * @ingroup forms
 */
function pmpapi_config_form($form, &$form_state) {
  // Only run this if there are already credentials available.
  // Don't worry about calls to this function where there is a non-empty input
  // in $form_state, as this is a second call happening on submission. No need
  // to ping the API twice.
  if (variable_get('pmpapi_base_url') && variable_get('pmpapi_user_id') && variable_get('pmpapi_auth_client_id') && variable_get('pmpapi_auth_client_secret') && empty($form_state['input'])) {
    if (pmpapi_ping()) {
      $type = 'status';
      $message = t('Ping to API came back without errors.');
    }
    else {
      $type = 'warning';
      $message = t('Ping to API came back with errors.');
    }
    drupal_set_message($message, $type, FALSE);
  }

  $form['pmpapi_base_url'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Platform'),
    '#default_value' => variable_get('pmpapi_base_url'),
    '#options' => array(
      'https://api-sandbox.pmp.io'=> 'sandbox',
      'https://api.pmp.io'=> 'production',
    ),
  );

  $form['pmpapi_user_id'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('User/Org GUID'),
    '#default_value' => variable_get('pmpapi_user_id'),
  );

  $form['pmpapi_auth_client_id'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Authentication Client ID'),
    '#default_value' => variable_get('pmpapi_auth_client_id'),
  );
  
  $form['pmpapi_auth_client_secret'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Authentication Client Secret'),
    '#default_value' => variable_get('pmpapi_auth_client_secret'),
  );

  $custom_caches = pmpapi_get_custom_caches();
  if (variable_get('pmpapi_cache') && $custom_caches) {
    $form['pmpapi_cache_bin'] = array(
      '#type' => 'select',
      '#title' => t('Cache bin'),
      '#default_value' => variable_get('pmpapi_cache_bin', 'cache'),
      '#options' => drupal_map_assoc(array_merge(array('cache'), $custom_caches)),
    );
  }

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  return $form;
}

/**
 * Form validation handler for pmpapi_push_admin_config().
 */
function pmpapi_config_form_validate($form, &$form_state) {
  $base = $form_state['values']['pmpapi_base_url'];
  if (!valid_url($base, TRUE)) {
    form_set_error('pmpapi_base_url', t('Base URL must be a fully qualified URL.'));
  }
}

/**
 * Form submission handler for pmpapi_push_admin_config().
 */
function pmpapi_config_form_submit($form, &$form_state) {
  variable_set('pmpapi_base_url', trim($form_state['values']['pmpapi_base_url']));
  variable_set('pmpapi_user_id', trim($form_state['values']['pmpapi_user_id']));
  variable_set('pmpapi_auth_client_id', trim($form_state['values']['pmpapi_auth_client_id']));
  variable_set('pmpapi_auth_client_secret', trim($form_state['values']['pmpapi_auth_client_secret']));
  if (isset($form_state['values']['pmpapi_cache_bin'])) {
    variable_set('pmpapi_cache_bin', $form_state['values']['pmpapi_cache_bin']);
  }

  // If (PMP) cache isn't cleared, it can cause issues when creds change
  pmpapi_clear_pmp_cache();
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Scrapes $conf to determine all possible cache bins.
 *
 * @return array
 *   A list of all possible cache bins
 */
function pmpapi_get_custom_caches() {
  global $conf;
  $bins = array();
  foreach ($conf as $name => $value) {
    if (stripos($name, 'cache_class_') === 0) {
      $bins[] = str_ireplace('cache_class_', '', $name);
    }
  }
  return $bins;
}