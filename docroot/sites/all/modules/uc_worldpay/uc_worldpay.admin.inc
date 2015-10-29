<?php

/**
 * @file
 * WorldPay payment method administration.
 */

/**
 * Overview of currently configured WorldPay payment methods.
 */
function uc_worldpay_configure_methods() {
  $methods = variable_get('uc_worldpay_configured_payment_methods', array('default' => t('Default')));

  if (count($methods) > 0) {
    $rows = array();
    foreach ($methods as $machine_name => $method) {
      $rows[] = array(
        'id' => $machine_name,
        'title' => $method,
        'actions' => l(t('edit'), 'admin/store/settings/worldpay/'. $machine_name .'/edit') .' | '. l(t('delete'), 'admin/store/settings/worldpay/'. $machine_name .'/delete'),
      );
    }
    $headers = array('ID', 'Title', 'Edit');
    $output = theme('table', array('header' => $headers, 'rows' => $rows));
  }
  else {
    $output = '<p>'. t('There are currently no configured WorldPay payment methods.') .'</p>';
  }
  $output .= '<p>'. l(t('Add new payment method'), 'admin/store/settings/worldpay/add') .'</p>';
  return $output;
}

/**
 * Callback to add a new payment method.
 */
function uc_worldpay_add_payment_method_form($form, $form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment method title'),
    '#required' => TRUE,
  );
  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#required' => TRUE,
    '#maxlength' => 20,
    '#size' => 20,
    '#machine_name' => array(
      'exists' => 'uc_worldpay_machine_name_exists',
      'source' => array('title'),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save payment method'),
  );
  return $form;
}

/**
 * Submit handler for adding a new payment method.
 */
function uc_worldpay_add_payment_method_form_submit(&$form, &$form_state) {
  $methods = variable_get('uc_worldpay_configured_payment_methods', array('default' => t('Default')));
  $methods[$form_state['values']['machine_name']] = $form_state['values']['title'];
  variable_set('uc_worldpay_configured_payment_methods', $methods);

  drupal_set_message(t('Payment method %method_name created.', array('%method_name' => $form_state['values']['title'])));
  drupal_goto('admin/store/settings/worldpay');
}

/**
 * Callback to check if the machine name is already in use for payment method.
 */
function uc_worldpay_machine_name_exists($arg) {
  $methods = variable_get('uc_worldpay_configured_payment_methods', array('default' => t('Default')));
  if (in_array($arg, array_keys($methods)) || $arg == 'add') return TRUE;
  return FALSE;
}

/**
 * Form for editing existing payment methods.
 */
function uc_worldpay_edit_payment_method_form($form, $form_state, $method_id) {
  $methods = variable_get('uc_worldpay_configured_payment_methods', array('default' => t('Default')));

  $method = $methods[$method_id];

  $form['method_id'] = array(
    '#type' => 'value',
    '#value' => $method_id,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment method title'),
    '#required' => TRUE,
    '#default_value' => $method,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update payment method'),
  );
  return $form;
}

/**
 * Submit handler for editing payment method.
 */
function uc_worldpay_edit_payment_method_form_submit(&$form, &$form_state) {
  $methods = variable_get('uc_worldpay_configured_payment_methods', array('default' => t('Default')));
  $methods[$form_state['values']['method_id']] = $form_state['values']['title'];
  variable_set('uc_worldpay_configured_payment_methods', $methods);

  drupal_set_message(t('Payment method %method_name updated.', array('%method_name' => $form_state['values']['title'])));
  drupal_goto('admin/store/settings/worldpay');
}

/**
 * Form for deleteing existing payment methods.
 */
function uc_worldpay_delete_payment_method_form($form, $form_state, $method_id) {
  $form['method_id'] = array(
    '#type' => 'value',
    '#value' => $method_id,
  );

  $form['markup'] = array(
    '#type' => 'markup',
    '#markup' => '<p>'. t('Are you sure you want to delete this payment method? This cannot be undone.') .'</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete payment method'),
  );
  return $form;
}

/**
 * Submit handler for editing payment method.
 */
function uc_worldpay_delete_payment_method_form_submit(&$form, &$form_state) {
  $method = $form_state['values']['method_id'];

  $methods = variable_get('uc_worldpay_configured_payment_methods', array('default' => t('Default')));
  $title = $methods[$method];
  unset($methods[$method]);
  variable_set('uc_worldpay_configured_payment_methods', $methods);

  // Delete all the other variables
  $variables = array(
    'uc_worldpay_'. $method .'_sid',
    'uc_worldpay_'. $method .'_merchant_code',
    'uc_worldpay_'. $method .'_debug',
    'uc_worldpay_'. $method .'_checkout_button',
    'uc_worldpay_'. $method .'_payment_methods',
    'uc_worldpay_'. $method .'_test',
    'uc_worldpay_'. $method .'_test_result',
    'uc_worldpay_'. $method .'_md5',
    'uc_worldpay_'. $method .'_callbackPW',
    'uc_worldpay_'. $method .'_callbackURL',
    'uc_worldpay_'. $method .'_desc',
    'uc_worldpay_'. $method .'_cancel_order',
    'uc_worldpay_'. $method .'_edit_contact',
    'uc_worldpay_'. $method .'_show_contact',
    'uc_worldpay_'. $method .'_lang',
    'uc_worldpay_'. $method .'_test_url',
    'uc_worldpay_'. $method .'_live_url',
  );
  foreach ($variables as $variable) {
    variable_del($variable);
  }

  drupal_set_message(t('Payment method %method_name deleted.', array('%method_name' => $title)));
  drupal_goto('admin/store/settings/worldpay');
}