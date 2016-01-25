<?php

/**
 * @file uc_eway.rules.inc
 * This file contains the Rules integration hooks and functions.
 */

/**
 * Implements hook_rules_event_info().
 */
function uc_eway_rules_event_info() {
  $events['uc_eway_payment_processed'] = array(
    'label' => t('A payment gets successfully processed for an order'),
    'group' => t('eWay'),
    'variables' => array(
      'order' => array(
        'type' => 'uc_order',
        'label' => t('Order'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_default_rules_configuration().
 */
function uc_eway_default_rules_configuration() {
  // Set the order status to "Payment Received" when a payment is received
  // and the balance is less than or equal to 0.
  $rule = rules_reaction_rule();
  $rule ->label = t('Successful payment via eWay');
  $rule->active = variable_get('uc_eway_change_order_status', UC_EWAY_CHANGE_ORDER_STATUS_DEFAULT);
  $rule->event('uc_eway_payment_processed')
    ->condition(rules_condition('data_is', array('data:select' => 'order:order-status', 'value' => 'payment_received'))->negate())
    ->condition('uc_payment_condition_order_balance', array(
      'order:select' => 'order',
      'balance_comparison' => 'less_equal',
    ))
    ->action('uc_order_update_status', array(
      'order:select' => 'order',
      'order_status' => 'payment_received',
    ));
  $configs['uc_eway_payment_processed'] = $rule;

  return $configs;
}
