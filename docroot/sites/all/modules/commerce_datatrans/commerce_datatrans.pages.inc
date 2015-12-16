<?php

/**
 * @file
 * Datatrans menu items callbacks.
 */

/**
 * Custom menu callback: Datatrans payment processing.
 *
 * @param $order_id           Commerce order ID.
 * @param $payment_method_id  Commerce payment method ID.
 * @param $payment_key        URL key used to validate request.
 */
function commerce_datatrans_payment_processing($order_id, $payment_method_id, $payment_key) {
  $order     = commerce_order_load($order_id);
  $datatrans = $_POST;

  // First check if the request comes from Datatrans.
  if (isset($datatrans['status']) && $order->data['payment_redirect_key'] == $payment_key) {
    switch ($datatrans['status']) {
      case 'cancel' :
        // Go back to the checkout page we came from.
        drupal_goto(commerce_checkout_order_uri($order));
        break;
      case 'success':
        // Get payment method.
        $payment_method = commerce_payment_method_instance_load($payment_method_id);
        $payment_method['instance_id'] = $payment_method_id;

        if (commerce_datatrans_redirect_form_validate($order, $payment_method)) {
          // Transaction save.
          _commerce_datatrans_transaction_save($payment_method, $order, $datatrans, COMMERCE_PAYMENT_STATUS_SUCCESS);

          // Send the customer onto the next checkout page.
          commerce_datarans_redirect_pane_next_page($order, t('Customer successfully submitted payment at the payment gateway.'));
          drupal_goto(commerce_checkout_order_uri($order));
        }
        break;
      default:
        $error_msg = _commerce_datatrans_map_error_code($datatrans['errorCode']);
        watchdog('commerce_datatrans', $error_msg, $datatrans, WATCHDOG_ERROR);
        drupal_set_message($error_msg, 'error');
        break;
    }
  }
  else {
    drupal_set_message(t('Direct access is not permitted.'), 'error');
  }

  drupal_goto();
}
