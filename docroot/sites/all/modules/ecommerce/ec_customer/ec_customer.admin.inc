<?php

/**
 * @file
 * Administration functions for ec_customer module.
 */

/**
 * This page allows the customer types to be altered
 */
function ec_customer_ctypes_form($form, &$form_state) {
  $form = array();
  $types = ec_customer_get_types();

  uasort($types, 'ec_sort');

  $form['ec_customer_ctypes'] = array(
    '#tree' => TRUE,
  );
  foreach ($types as $ctype => $info) {
    $form['ec_customer_ctypes'][$ctype]['name'] = array(
      '#markup' => isset($info->settings_path) ? l($info->name, $info->settings_path) : $info->name,
    );
    $form['ec_customer_ctypes'][$ctype]['type'] = array(
      '#markup' => $info->type,
    );
    $form['ec_customer_ctypes'][$ctype]['description'] = array(
      '#markup' => $info->description,
    );
    $form['ec_customer_ctypes'][$ctype]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $info->weight,
      '#attributes' => array('class' => array('ctype-weight')),
    );
  }

  $form = system_settings_form($form);
  unset($form['#theme']);

  return $form;
}

/**
 * Customer Global Settings
 */
function ec_customer_ctypes_settings($form, &$form_state) {
  $form = array();

  $form['ec_customer_billing_address'] = array(
    '#type' => 'select',
    '#title' => t('Billing address'),
    '#default_value' => variable_get('ec_customer_billing_address', 1),
    '#options' => array(
      0 => t('Only with Shippable'),
      1 => t('Always'),
    ),
    '#description' => t('Change the behaviour during checkout to determine when creating invoices/transaction if the billing address will be collected. This may also impact on which payment gateways are available as the terms and conditions of some gateways require a billing address.'),
  );
  return system_settings_form($form);
}


