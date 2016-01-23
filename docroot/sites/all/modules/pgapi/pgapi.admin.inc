<?php

/**
 * @file
 * Administative functions for PGAPI module
 */

/**
 * Menu callback to list sub-items.
 */
function pgapi_admin_menu_block_page() {
  $item = menu_get_item();
  module_load_include('inc', 'system', 'system.admin');
  if ($content = system_admin_menu_block($item)) {
    $output = theme('admin_block_content', array('content' => $content));
  }
  else {
    $output = t('You do not have any administrative items.');
  }
  return $output;
}

/**
 * Form-builder with common PGAPI settings.
 */
function pgapi_admin_settings($form, &$form_state) {

  $pgapi_gw = pgapi_get_enabled_payment_systems();
  $gateways = pgapi_get_gateway_options();

  if (empty($gateways)) {
    $form['gateways']['#markup'] = t('No payment gateways available.');
  }
  else {
    $form['gateways'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Payment gateways'),
      '#options' => $gateways,
      '#default_value' => $pgapi_gw,
    );
    // Add the buttons.
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Update status of payment gateways'),
    );
  }

  return $form;
}

/**
 * Submit function for PGAPI common settings form.
 */
function pgapi_admin_settings_submit($form, &$form_state) {
  $pgapi_gw = array_filter($form_state['values']['gateways']);

  // Get settings for domain module.
  if (module_exists('domain_conf')) {
    $current_domain = domain_get_domain();
    $pgapi_gw = domain_conf_variable_set($current_domain['domain_id'], 'pgapi_gw', $pgapi_gw);
  }
  else {
    // Save enabled gateways.
    variable_set('pgapi_gw', $pgapi_gw);
  }

  drupal_set_message(t('Status of payment gateway has been updated.'));
}
