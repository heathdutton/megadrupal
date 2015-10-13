<?php

/**
 * @file
 * Rules integration for epay commerce.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function epay_commerce_rules_action_info() {
  $actions = array();

  $actions['epay_commerce_capture_remaining_amount'] = array(
    'label' => t('Capture the remaining amount of an ePay payment.'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('ePay Commerce'),
  );

  return $actions;
}

/**
 * Implements hook_rules_event_info().
 */
function epay_commerce_rules_event_info() {
  $items = array();

  $items['epay_commerce_captured_remaining_amount'] = array(
    'label' => t('Captured remaining amount'),
    'group' => t('ePay Commerce'),
    'variables' => array(
      'commerce_order' => array(
        'label' => t('Order'),
        'type' => 'commerce_order',
      ),
    ),
  );

  $items['epay_commerce_captured_remaining_amount_fail'] = array(
    'label' => t('Failed to captured remaining amount'),
    'group' => t('ePay Commerce'),
    'variables' => array(
      'commerce_order' => array(
        'label' => t('Order'),
        'type' => 'commerce_order',
      ),
      'txnid' => array(
        'label' => t('ePay transaction id'),
        'type' => 'text',
      ),
    ),
  );

  return $items;
}

/**
 * @}
 */

/**
 * Capture the remaining payment for an Drupal Commerce order.
 *
 * @param $order
 *    A Drupal Commerce order entity.
 */
function epay_commerce_capture_remaining_amount($order) {
  $commerce_transactions = commerce_payment_transaction_load_multiple(array(), array(
    'order_id' => $order->order_id,
    'payment_method' => 'epay_commerce',
    'status' => COMMERCE_PAYMENT_STATUS_SUCCESS,
  ));
  if (!$commerce_transactions) {
    return;
  }
  $commerce_transaction = array_shift($commerce_transactions);
  $payment_method = commerce_payment_method_instance_load($commerce_transaction->instance_id);
  $epay_transaction = epay_transaction_load($commerce_transaction->remote_id);
  if ($payment_method) {
    $params = array(
      'merchantnumber' => $payment_method['settings']['merchant'],
      'transactionid' => $epay_transaction['txnid'],
      'pwd' => $payment_method['settings']['remote_password'],
    );
    $epay_remote_transaction = epay_get_remote_transaction($params);
    if ($epay_remote_transaction) {
      $params['amount'] = $epay_remote_transaction->authamount - $epay_remote_transaction->capturedamount;
      $result = epay_capture_transaction($params);
      if ($result && $result['success']) {
        rules_invoke_event('epay_commerce_captured_remaining_amount', $order);
      }
      else {
        rules_invoke_all('epay_commerce_captured_remaining_amount_fail', $order, $epay_transaction['txnid']);
      }
    }
  }
}
