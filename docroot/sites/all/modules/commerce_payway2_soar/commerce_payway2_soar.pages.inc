<?php

/**
 * @file
 * Manage 'PayWay 2 SOAR payment operation.
 */

/**
 * Callback function for payment completed.
 */
function commerce_payway2_soar_complete_ok() {
  //commerce_payway2_soar_check_server_session();
  $settings = commerce_payway2_soar_getsettings();
  $response = drupal_get_query_parameters(NULL, array('q'), '');

  // Get order id from url. Strip it from test string if necessary.
  $order_id = str_replace($settings['order_prefix'], '', $response['OID']);

  commerce_payway2_soar_transaction($order_id, $response, COMMERCE_PAYMENT_STATUS_SUCCESS);
  
  // Transaction OK.
  if ($response['STA'] == 'S') {
    $order = commerce_order_load($order_id);
    $order = commerce_order_status_update($order, 'checkout_complete');
    commerce_checkout_complete($order);
    
    $_SESSION['commerce_cart_orders'][] = $order_id;
    watchdog('commerce_payway2_soar', 'Order %order complete. redirect to complete page', array('%order' => $order_id), WATCHDOG_NOTICE);
    drupal_goto('checkout/' . $order_id . '/complete', array('CODAUTO' => $response['CODAUTO']));
  }
  
  drupal_goto('checkout/soar/failed/' . $order_id);
}

/**
 * Callback function for payment completed.
 */
function commerce_payway2_soar_complete_ko() {
  //commerce_payway2_soar_check_server_session();
  $settings = commerce_payway2_soar_getsettings();
  $response = drupal_get_query_parameters(NULL, array('q'), '');

  // Get order id from url. Strip it from test string if necessary.
  $order_id = str_replace($settings['order_prefix'], "", $response['OID']);
  
  commerce_payway2_soar_transaction($order_id, $response, COMMERCE_PAYMENT_STATUS_FAILURE);

  // Transaction KO.
  if ($response['STA'] == 'N') {
    watchdog('commerce_payway2_soar', 'Order %order KO. redirect to failed page', array('%order' => $order_id), WATCHDOG_NOTICE);
  }

  // Transaction CANCELLED.
  if ($response['STA'] == 'A') {
    watchdog('commerce_payway2_soar', 'Order %order CANCELLED. redirect to cancelled page', array('%order' => $order_id), WATCHDOG_NOTICE);
  }

  drupal_goto('checkout/soar/failed/' . $order_id);
}

/**
 * Callback function for PayWay2 cancelled operation.
 */
function commerce_payway2_soar_failed($order) {
  global $user;

  if ($order->uid == $user->uid) {
    $output = array(
      'first' => array(
        '#markup' => t('We are sorry but you order has been cancelled because our payment gateway refused the transaction or transaction cancelled by user.<br/> You could try to repeat the checkout process from the beginning. For any question please contact us at %sitemail', array('%sitemail' => variable_get('site_mail', ''))),
      ),
    );

    return $output;
  }
  else {
    drupal_access_denied();
  }
}

/**
 * Check the remote ip.
 */
function commerce_payway2_soar_check_server_session() {
  $remote_address = ip_address();

  if ($remote_address == COMMERCE_PAYWAY2_SOAR_PAYMENT_IP) {
    return TRUE;
  }

  drupal_goto('<front>');
}
