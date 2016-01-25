<?php

/**
 * @file
 * Rules integration for Dibs API module.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_dibs_rules_action_info() {
  $actions = array();

  $actions['commerce_dibs_capture_order'] = array(
    'label' => t('Capture payments on commerce order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to make the capture on'),
      ),
    ),
    'group' => t('Commerce Order'),
    'callbacks' => array(
      'execute' => 'commerce_dibs_capture_order',
    ),
  );

  return $actions;
}

/**
 * Rules action: Remove all shipping line items on an order.
 */
function commerce_dibs_capture_order($order) {
  $commerce_transactions = commerce_payment_transaction_load_multiple(FALSE, array('order_id' => $order->order_id));
  foreach ($commerce_transactions as $commerce_transaction) {
    // Only try to capture payments that are auth'ed and have a dibs id.
    if (
        $commerce_transaction->payment_method == 'commerce_dibs' &&
        $commerce_transaction->status == COMMERCE_CREDIT_AUTH_ONLY &&
        !empty($commerce_transaction->payload['dibs_id'])
    ) {
      $status = dibs_capture_transaction('commerce_dibs', 'commerce_dibs', $commerce_transaction->payload['dibs_id']);
      if ($status['status']) {
        $commerce_transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
        commerce_payment_transaction_save($commerce_transaction);
      }
    }
  }
  module_invoke_all('commerce_dibs_order_captured', $order, $status);
}
