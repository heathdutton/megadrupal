<?php

/**
 * Implements hook_checkout_form().
 */
function commerce_realex_remote_dcc_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();

  // Don't display this checkout pane if the payment method isn't Realex
  // Payments Remote.
  if ($order->data['payment_method'] != 'commerce_realex_remote|commerce_payment_commerce_realex_remote') {
    return $pane_form;
  }

  // Only display DCC offer if (a) DCC is enabled and (b) we have a valid offer.
  $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
  if ($payment_method['settings']['dcc_provider'] != 'none' && !empty($order->data['commerce_realex_dcc_message'])) {
    $dcc_message = $order->data['commerce_realex_dcc_message'];

    $form['dcc'] = array(
      '#type' => 'fieldset',
      '#title' => t('Direct Currency Conversion'),
      '#collapsible' => FALSE,
      '#weight' => 10,
    );

    $pane_form['dcc_message'] = array(
      '#type' => 'markup',
      '#markup' => '<p>' . $dcc_message . '</p>',
      '#prefix' => '<div id="dcc-info">',
      '#suffix' => '</div>',
    );

    $pane_form['dcc_accept'] = array(
      '#type' => 'checkbox',
      '#title' => t('Yes - charge me in my own currency'),
      '#required' => FALSE,
    );
  }

  return $pane_form;
}

/**
 * Implements hook_checkout_form_submit().
 */
function commerce_realex_remote_dcc_pane_checkout_form_submit($form, &$form_state, $checkout_pane, $order) {
  if ($order->data['payment_method'] != 'commerce_realex_remote|commerce_payment_commerce_realex_remote') {
    return;
  }

return;
  // TODO: get $pane_values['credit_card']
  $payment_method = commerce_payment_method_instance_load($order->data['payment_method']);

  $start_date = NULL;
  if (!empty($pane_values['credit_card']['start_month'])) {
    $start_date = sprintf("%02d%02d", $pane_values['credit_card']['start_month'], substr($pane_values['credit_card']['start_year'], -2));
  }
  $expiry_date = sprintf("%02d%02d", $pane_values['credit_card']['exp_month'], substr($pane_values['credit_card']['exp_year'], -2));

  $request = array();
  $request['timestamp'] = strftime('%Y%m%d%H%M%S');
  mt_srand((double) microtime() * 1000000);

  $request['order_number'] = $order->order_number;
  $request['orderid'] = $request['order_number'] . $request['timestamp'];
  $request['merchantid'] = $payment_method['settings']['merchant_id'];
  $request['account'] = $payment_method['settings']['account_id'];
  $request['secret'] = $payment_method['settings']['shared_secret'];
  $request['auto_settle'] = $payment_method['settings']['auto_settle'];
  $request['dcc_provider'] = $payment_method['settings']['dcc_provider'];

  $request['chname'] = $pane_values['credit_card']['owner'];
  $request['cctype'] = _commerce_realex_get_card_type($pane_values['credit_card']['type'], $pane_values['credit_card']['number']);
  $request['ccnumber'] = $pane_values['credit_card']['number'];
  $request['cvn'] = $pane_values['credit_card']['code'];
  $request['issueno'] = isset($pane_values['credit_card']['issue']) ? $pane_values['credit_card']['issue'] : '';
  $request['start_date'] = $start_date;
  $request['expiry_date'] = $expiry_date;

  // Load the customer billing profile for the order.
  if (!empty($order->commerce_customer_billing)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $profile = $order_wrapper->commerce_customer_billing->value();
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();
    $request['billing']['customer_id'] = $profile->profile_id;
    if (isset($address['name_line'])) {
      $request['billing']['customer_name'] = check_plain($address['name_line']);
    }
    elseif (isset($address['first_name'])) {
      $request['billing']['customer_name'] = check_plain($address['first_name'] . ' ' .  $address['last_name']);
    }
    $request['billing']['street_one'] = isset($address['thoroughfare']) ? check_plain($address['thoroughfare']) : '';
    $request['billing']['postcode'] = isset($address['postal_code']) ? check_plain($address['postal_code']) : '';
    $request['billing']['city'] = isset($address['locality']) ? check_plain($address['locality']) : '';
    $request['billing']['region'] = isset($address['administrative_area']) ? check_plain($address['administrative_area']) : '';
    $request['billing']['country'] = isset($address['country']) ? check_plain($address['country']) : '';
  }

  // Load the customer shipping profile for the order.
  if (!empty($order->commerce_customer_shipping)) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $profile = $order_wrapper->commerce_customer_shipping->value();
    $profile_wrapper = entity_metadata_wrapper('commerce_customer_profile', $profile);
    $address = $profile_wrapper->commerce_customer_address->value();
    $request['shipping']['customer_id'] = $profile->profile_id;
    if (isset($address['name_line'])) {
      $request['shipping']['customer_name'] = check_plain($address['name_line']);
    }
    elseif (isset($address['first_name'])) {
      $request['shipping']['customer_name'] = check_plain($address['first_name'] . ' ' .  $address['last_name']);
    }
    $request['shipping']['street_one'] = isset($address['thoroughfare']) ? check_plain($address['thoroughfare']) : '';
    $request['shipping']['postcode'] = isset($address['postal_code']) ? check_plain($address['postal_code']) : '';
    $request['shipping']['city'] = isset($address['locality']) ? check_plain($address['locality']) : '';
    $request['shipping']['region'] = isset($address['administrative_area']) ? check_plain($address['administrative_area']) : '';
    $request['shipping']['country'] = isset($address['country']) ? check_plain($address['country']) : '';
  }
  $request['customer_id'] = isset($request['billing']['customer_id']) ? $request['billing']['customer_id'] : isset($request['shipping']['customer_id']) ? $request['shipping']['customer_id'] : 0;

  $request['amount'] = $charge['amount'];
  $request['currency'] = $charge['currency_code'];
  $request['ip_address'] = ip_address();

  // Get products purchased.
  $products = array();
  $comments = array();
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    $line_item = $line_item_wrapper->value();
    // If the line item is a product line item...
    if (in_array($line_item->type, commerce_product_line_item_types())) {
      $product = $line_item_wrapper->commerce_product->value();
      $products[] = $product->product_id;
      $comments[] = sprintf("%dx %s",$line_item->quantity, check_plain($product->title));
    }
  }
  $request['products'] = implode(',', $products);
  $request['comments'][0] = t('Products purchased:') . ' ' . implode(', ', $comments);
  $request['log_message'] = '';

  $pane_id = $checkout_pane['commerce_realex_remote_dcc_pane'];
  // Pane wasn't displayed as DCC offer unsuccessful.
  if (!isset($form_state['values'][$pane_id]['dcc_accept'])) {
    $request['dcc'] = FALSE;
  }
  // Customer does not accept the rate request.
  elseif (!$form_state['values'][$pane_id]['dcc_accept']) {
    $request['dcc'] = TRUE;
    $request['cardholdercurrency'] = $request['currency'];
    $request['cardholderamount'] = $request['amount'];
    $request['cardholderrate'] = 1;
  }
  // Customer accepts the rate request.
  else {
    $request['dcc'] = TRUE;
    $request['cardholdercurrency'] = $_SESSION['cardholdercurrency'];
    $request['cardholderamount'] = $_SESSION['cardholderamount'];
    $request['cardholderrate'] = $_SESSION['cardholderrate'];
  }

  // Build sha1 hash.
  $request['sha1hash'] = _commerce_realex_build_request_sha1hash($request);

  module_load_include('inc', 'commerce_realex', 'commerce_realex_remote');
  commerce_realex_remote_do_auth_request($payment_method, $request, $order);
}

