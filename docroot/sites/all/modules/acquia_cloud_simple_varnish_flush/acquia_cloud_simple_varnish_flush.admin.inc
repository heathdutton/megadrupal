<?php

/**
 * @file
 * Administration page callback for module.
 */
function acquia_cloud_simple_varnish_flush_settings($form, &$form_state) {
  $form['acquia_cloud_simple_varnish_flush_username'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Acquia Cloud API username'),
    '#default_value' => variable_get('acquia_cloud_simple_varnish_flush_username', NULL),
    '#description' => t('Visit https://docs.acquia.com/cloud/api/auth for your credentials'),
  );

  $form['acquia_cloud_simple_varnish_flush_password'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Acquia Cloud API key'),
    '#default_value' => variable_get('acquia_cloud_simple_varnish_flush_password', NULL),
    '#description' => t('Visit https://docs.acquia.com/cloud/api/auth for your credentials'),
  );

  return system_settings_form($form);
}
