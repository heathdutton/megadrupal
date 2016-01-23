<?php
/**
 * @file
 * Provides rules integration for commerce_paypal_ipn_rules_integration
 */

/**
 * Implementation of hook_rules_event_info().
 */
function commerce_paypal_ipn_rules_integration_rules_event_info() {
  $events = array();

  $events['commerce_paypal_ipn_rules_integration_paypal_wps_redirect'] = array(
    'label' => t('Before user redirect to paypal for checkout'),
    'group' => t('Commerce Paypal'),
    'variables' => array(
      'order' => array(
        'label' => t('Order'),
        'type' => 'commerce_order',
      ),
      'form' => array(
        'label' => t('Form'),
        'type' => 'form',
      ),
    ),
  );

  $events['commerce_paypal_ipn_rules_integration_ipn_process'] = array(
    'label' => t('Process Paypal IPN'),
    'group' => t('Commerce Paypal'),
    'variables' => array(
      'order' => array(
        'label' => t('Order'),
        'type' => 'commerce_order',
      ),
      'payment_method' => array(
        'label' => t('Payment method'),
        'type' => 'text',
      ),
      'txn_type' => array(
        'label' => t('Transaction type'),
        'type' => 'text',
      ),
      'user' => array(
        'label' => t('User'),
        'type' => 'user',
      ),
      'ipn_id' => array(
        'label' => t('IPN ID'),
        'type' => 'integer',
      ),
      'txn_id' => array(
        'label' => t('Transaction ID'),
        'type' => 'text',
      ),
      'receiver_email' => array(
        'label' => t('Receiver email'),
        'type' => 'text',
      ),
      'payer_email' => array(
        'label' => t('Payer email'),
        'type' => 'text',
      ),
      'payment_status' => array(
        'label' => t('Payment status'),
        'type' => 'text',
      ),
      'test_ipn' => array(
        'label' => t('Sandbox test IPN'),
        'type' => 'boolean',
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_action_info
 */
function commerce_paypal_ipn_rules_integration_rules_action_info() {
  $actions = array();

  $actions['commerce_paypal_ipn_rules_integration_add_subscription_info'] = array(
    'label' => t('Add subscription information'),
    'group' => t('Commerce Paypal'),

    'parameter' => array(
      'form' => array(
        'type' => 'form',
        'label' => t('The form')
      ),
      'item_name' => array(
        'type' => 'text',
        'label' => t('Item name')
      ),
      'a3' => array(
        'type' => 'decimal',
        'label' => t('Amount')
      ),
      'p3' => array(
        'type' => 'integer',
        'label' => t('Interval amount')
      ),
      't3' => array(
        'type' => 'text',
        'label' => t('Interval period')
      ),
    ),
  );

  $actions['commerce_paypal_ipn_rules_integration_clone_order'] = array(
    'label' => t('Clone order'),
    'group' => t('Commerce Paypal'),

    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Original order')
      ),
    ),
    'provides' => array(
      'new_order' => array(
        'type' => 'commerce_order',
        'label' => t('New cloned order'),
      ),
    ),
  );

  return $actions;
}

/**
 * Implements hook_rules_data_info().
 */
function commerce_paypal_ipn_rules_integration_rules_data_info() {
  return array(
    'form' => array(
      'label' => t('Form'),
      'group' => 'Forms',
    ),
  );
}