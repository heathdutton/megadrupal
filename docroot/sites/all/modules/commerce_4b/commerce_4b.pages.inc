<?php

/**
 * Order details to reply to 4b payment.
 */
function commerce_4b_checkout_details($payment_method = NULL, $debug_response = array()) {
  if (empty($debug_response)) {
    $response = $_REQUEST; 
  } else {
    $response = $debug_response;
  }
  
  $config = _commerce_4b_get_rule_config();

  // If the order ID is invalid, log a message and exit.
  if (!is_numeric($response['order'])) {
    watchdog('commerce_4b', 'Invalid Order ID @order. !response', array('@order' => $response['order'], '!response' => '<pre>'. print_r($response, TRUE) .'<pre>'), WATCHDOG_ERROR);
    return FALSE;
  }
  
  // If the store ID is invalid, log a message and exit.
  if ($response['store'] != $config['commerce_4b_store']) {
    watchdog('commerce_4b', 'Invalid Store ID @store. !response', array('@store' => $response['store'], '!response' => '<pre>'. print_r($response, TRUE) .'<pre>'), WATCHDOG_ERROR);
    return FALSE;
  }
  
  $order = commerce_order_load($response['order']);
  
  // If the order ID is not found, log a message and exit.
  if (!$order) {
    watchdog('commerce_4b', 'Order ID @order not found. !response', array('@order' => $response['order'], '!response' => '<pre>'. print_r($response, TRUE) .'<pre>'), WATCHDOG_ERROR);
    return FALSE;
  }
  
  $wrapper = entity_metadata_wrapper('commerce_order', $order);  
  $amount = $wrapper->commerce_order_total->amount->value();
  $currency = $config['commerce_4b_currency']; // @todo Shouldn't we take the currency from the store?
  
  // Only inform to the platform one item and total price.
  $items = 1;
  
  $shop = $config['commerce_4b_store'];

  $ret = 'M' . $currency . $amount . "\n";
  $ret .= $items . "\n";
  $ret .= '1' . "\n";    // Reference number.
  $ret .= $shop . "\n";  // Description.
  $ret .= '1' . "\n";    // Quantity.
  $ret .= $amount . "\n"; // Price.
    
  // @todo only output when in debug mode
  watchdog('commerce_4b', 'Sent order data to 4B: !ret', array('!ret' => '<pre>' . check_plain(print_r($ret, TRUE)) . '</pre>'), WATCHDOG_DEBUG);
  print $ret;
}

/**
 * Payment method callback: checkout form submission.
 */
function commerce_4b_redirect_page($debug_response = array()) {
  if (empty($debug_response)) {
    $response = $_REQUEST; 
  } else {
    $response = $debug_response;
  }
  
  // @todo not always the MAC will be facilitated, but if it has been, we need to do something with it

  $origin = ip_address();
  
  $config = _commerce_4b_get_rule_config();
  
  // Load the order
  $order = commerce_order_load(intval($response['pszPurchorderNum']));
  if (!$order) {
    return FALSE;
  }
  
  // Make an IP check when in production
  if (!$config['commerce_4b_use_testing'] && $origin != $config['commerce_4b_production_ip']) {
    watchdog('commerce_4b', 'Invalid origin IP address: @ip_address. !response', array('@ip_address' => $origin, '!response' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>'), WATCHDOG_ERROR); 
    return FALSE;
  }
  
  commerce_4b_transaction($order, $response);  
  watchdog('commerce_4b', 'Response processed for order number @order_number. !response', array('@order_number' => $order->order_number, '!response' => '<pre>' . check_plain(print_r($response, TRUE)) . '</pre>'), WATCHDOG_INFO);  
}

/**
 * Payment method callback: return from 4B.
 */
function commerce_4b_return($debug_response = array()) {
  if (empty($debug_response)) {
    $response = $_REQUEST; 
  } else {
    $response = $debug_response;
  }
  
  // Load the order
  $order = commerce_order_load(intval($response['pszPurchorderNum']));
  if (!$order) {
    return FALSE;
  }
  
  switch ($response['result']) {
    case 0:
      commerce_payment_redirect_pane_next_page($order);
      drupal_goto('checkout/' . $order->order_id . '/payment/return');      
      break;
    
    case 2:
    default:
      commerce_payment_redirect_pane_previous_page($order);
      drupal_goto('checkout/' . $order->order_id . '/payment/back');
      break;
  }
}