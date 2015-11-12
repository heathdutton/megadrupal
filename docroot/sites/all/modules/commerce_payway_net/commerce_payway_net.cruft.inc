<?php

/**
 * Moving things which *may* not be required here.
 *
 * Then try removing this include and see what breaks ;)
 */

/**
 * ORDER STATUS
 * updates the order status
 */
function commerce_payway_net_status_update($order) {
  // Load the order status object for the current order.
  $order_status = commerce_order_status_load($order->status);
  // dpm($order_status);
  if ($order_status['state'] == 'checkout' ) {
    $order = commerce_order_status_update($order, 'complete');
  }
}

/**
 * Implements hook_commerce_order_state_info().
 */
function commerce_payway_net_commerce_order_state_info() {
  $order_states = array();
  $order_states['finance'] = array(
    'name' => 'finance',
    'title' => t('Finance'),
    'description' => t('Orders in this state have been through checkout and need the payment verified by finance'),
    'weight' => 0,
    'default_status' => 'req_fin_confirm',
  );
  return $order_states;
}

/**
 * ORDER STATUS
 * Implements hook_commerce_order_status_info().
 */
function commerce_payway_net_commerce_order_status_info() {
  $order_statuses = array();
  $order_statuses['req_fin_confirm'] = array(
    'name' => 'req_fin_confirm',
    'title' => t('Requires Finance Confimation'),
    'state' => 'finance',
  );
  $order_statuses['has_fin_confirm'] = array(
    'name' => 'has_fin_confirm',
    'title' => t('Payment Confirmed By Finance'),
    'state' => 'finance',
  );
  return $order_statuses;
}

/*
function commerce_payway_net_redirect_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  dpm(__LINE__, __FUNCTION__);

}
*/

/**
 * Saves the details for the successful transaction
 */
/*
    function commerce_payway_net_transaction($payment_method, $order, $transaction_status) {
      dpm($order);
      dpm($payment_method);
      dpm($transaction_status);
      $query_data = commerce_nab_hosted_payment_callback_handler();

      $transaction = commerce_payment_transaction_new('commerce_payway_net', $order->order_id);
      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->remote_id = $query_data['commerce_nab_ret_bank_ref'];
      $transaction->amount = $query_data['commerce_nab_ret_bank_amt'];
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = $query_data['commerce_nab_ret_bank_num'];

      commerce_payment_transaction_save($transaction);
    }
*/

/**
 * URL RECEIVING THE DATA COMING BACK FROM WESTPAC
 * return callback - Implements hook_menu().
 */
/*
  function commerce_payway_net_menu() {
    $items['checkout/%/payment/return/%'] = array(
      'title' => t('Callback handler'),
      'page callback' => 'commerce_payway_net_callback_handler',
      'callback arguments' => array(1, 4),
      'type' => MENU_CALLBACK,
    );
  }
  */

/**
 * VALIDATES THE FORM SUBMITTED TO WESTPAC
 * Payment validate: this service doesn't provide any redirect for error or fail transactions
 * The module does nothing with these at this point. The will stay at checkout order state.
 */
/*
function commerce_payway_net_redirect_form_validate($order, $payment_method) {
  watchdog('commerce_payway_net', __FUNCTION__);
//  dpm(__LINE__, __FUNCTION__);
  module_load_include('inc', 'commerce_payway_net');
  $return_link_url = url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array(), array('absolute' => TRUE));

  // Throw an error if a long enough name was not provided.
  if (strlen($pane_values['phone']) < 6) {
    form_set_error(implode('][', array_merge($form_parents, array('name'))), t('You must enter a phone number 6 or more characters long.'));
    // Even though the form error is enough to stop the submission of the form,
    // it's not enough to stop it from a Commerce standpoint because of the
    // combined validation / submission going on per-pane in the checkout form.
    return FALSE;
  }

  commerce_nab_hosted_payment_callback_handler();
  commerce_nab_hosted_payment_status_update($order);
  commerce_nab_hosted_payment_transaction($payment_method, $order, COMMERCE_PAYMENT_STATUS_SUCCESS);
  commerce_payment_redirect_pane_next_page($order);
  return TRUE;
}
*/


/**
 * Creates a payway payment transaction for the specified charge amount.
 *
 * @param $payment_method
 *   The payment method instance object used to charge this payment.
 * @param $order
 *   The order object the payment applies to.
 * @param $charge
 *   An array indicating the amount and currency code to charge.
 * @param $name
 *   The name entered on the submission form.
 */
/*
function commerce_payway_net_transaction($payment_method, $order, $charge, $name) {
  error_log(__FUNCTION__);
  $transaction = commerce_payment_transaction_new('commerce_payway_net', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
  $transaction->message = 'Name: @name';
  $transaction->message_variables = array('@name' => $name);
  commerce_payment_transaction_save($transaction);
}
*/