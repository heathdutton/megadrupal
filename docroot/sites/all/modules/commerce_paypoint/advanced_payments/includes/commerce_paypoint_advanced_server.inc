<?php

/**
 * @file
 * commerce_paypoint_advanced_server.inc
 * Functions for Server integration method.
 */

/**
 * Implements hook_redirect_form().
 */
function commerce_paypoint_advanced_redirect_form($form, &$form_state, $order, $payment_method) {

  // Return an error if the enabling action's settings haven't been configured.
  if (empty($payment_method['settings'])) {
    drupal_set_message(t('This payment method must be configured by an administrator before it can be used.'), 'error');
    return array();
  }

  // Add Javascript to allow iFrame to work.
  //drupal_add_js(drupal_get_path('module', 'commerce_paypoint_advanced') . '/js/commerce_paypoint_advanced.js');

  // Wrap the order for easier access to data.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  $total = commerce_line_items_total($wrapper->commerce_line_items);

  // Add tax if we have sales tax in the order.
  $total['amount'] = $wrapper->commerce_order_total->amount->value();
  $total['currency_code'] = $total['currency_code'] ?: 'GBP';

  // Load customer profile.
  $profile = commerce_customer_profile_load($order->commerce_customer_billing[LANGUAGE_NONE][0]['profile_id']);

  // Get user billing address.
  $billing_address = $profile->commerce_customer_address[LANGUAGE_NONE][0];

  // Get user delivery address.
  $delivery_address = NULL;

  if (isset($order->commerce_customer_shipping)) {
    $delivery_profile = commerce_customer_profile_load($order->commerce_customer_shipping[LANGUAGE_NONE][0]['profile_id']);
    $delivery_address = $delivery_profile->commerce_customer_address[LANGUAGE_NONE][0];
  }
//var_dump($profile, $billing_address); exit;

  $settings = $payment_method['settings'];
  //var_dump(__FUNCTION__); exit;
  // $data = _commerce_paypoint_advanced_encrypted_order($settings, $order, $total, $billing_address, $delivery_address, SAGEPAY_SERVER);

  module_load_include('inc', 'commerce_paypoint_advanced', 'commerce_paypoint_advanced.class');
  $hosted = $settings['connectivity']['request_type'] == 'hosted';
  $url = commerce_paypoint_advanced_server_url($settings['testing']['test_status']);
  $auth = $settings['merchant'];

  // Initialize and send the PayPoint request.
  $instId = 5301067; // @fixme
  $returnUrl = 'checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'];
  $notifyUrl = url('commerce_paypoint_advanced/todo', array('absolute' => TRUE));
  $email = user_load($order->uid)->mail;
  $address = array(
    'line1'         => $billing_address['thoroughfare'],
    'line2'         => $billing_address['premise'],
    'line3'         => $billing_address['sub_premise'],
    'line4'         => $billing_address['organisation_name'],
    'city'          => $billing_address['locality'],
    'region'        => $billing_address['administrative_area'],
    'postcode'      => $billing_address['postal_code'],
    'countryCode'   => $billing_address['country'],
  );
  $telephone = '';
  $ip_address = ip_address();
  $ppo = new PPOAdvancedPayment($url, $auth['username'], $auth['password'], $instId, $hosted);
  $ppo->setSessionReturnUrl($returnUrl);
  $ppo->setSessionNotificationUrl($notifyUrl);
  $ppo->setCustomerDetails($billing_address['name_line'], $address, $telephone, $email, $ip_address);
  $ppo->setTransactionMerchantReference('your_reference_here'); // @fixme
  $ppo->setTransactionMoneyFixed($total['amount'], $total['currency_code']);
  $ppo->setCustomerNew();
  $ppo->setCustomerMerchantId('1111111111112'); // @fixme
  $response = array();
  try {
    // Call PayPoint to get payment form that can then be included in an iFrame.
    //var_dump($order); exit;
    $response = $ppo->sendProcessPayment();
  }
  catch (Exception $e) {
    watchdog_exception('commerce_paypoint_advanced', $e);
  }
  $response = $ppo->getLastResponse();
  $status = $ppo->getLastStatus();
  $code = $ppo->getLastCode();
  $data = $ppo->getLastData();
  if ($settings['connectivity']['debug'] || variable_get('commerce_paypoint_debug', FALSE)) {
    watchdog('commerce_paypoint_advanced', 'PayPoint response:\n<br>%data', array('%data' => json_encode($ppo->getLastResponse())), WATCHDOG_DEBUG);
  }

  // Create a new payment transaction and setup the amount.
  $transaction = commerce_payment_transaction_new('commerce_paypoint_advanced', $order->order_id);
  $transaction->amount = $total['amount'];
  $transaction->currency_code = $total['currency_code'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $transaction->instance_id = $instId;
  $transaction->remote_id = $data->sessionId;
  //$transaction->payload = $response;
  //$transaction->payload['VendorTxCode'] = $data['VendorTxCode'];

  // Add the user id to the transaction.
  //$transaction->payload['merchantCustomerId'] = $order->uid;

  // Determine the next action depending on the Status Code returned.
  if (empty($code)) {
    //$transaction->payload['vps_status'] = 'TIMEOUT';
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->message = 'No valid response from PayPoint';
    commerce_payment_transaction_save($transaction);
    watchdog('commerce_paypoint_advanced', 'No Status code received in PayPoint callback.', array(), WATCHDOG_ERROR);
  }

  $transaction_status = explode(" ", $status);

  switch ($status) {
    case 'SUCCESS':
      $transaction->remote_status = $status;
      commerce_payment_transaction_save($transaction);

      $iframe_enabled = (bool)$settings['integration_type']['use_iframe'];

      if ($iframe_enabled) {
        // Build the iFrame for the next step.
        $form['#action'] = $data->redirectUrl;
        $form['#method'] = 'GET';
        $form['#attributes']['target'] = 'iframe_paypoint';

        $form['submit'] = array(
          '#type'  => 'submit',
          '#value' => t('Proceed to PayPoint'),
        );
        $form['iframe_paypoint'] = array(
          '#markup' => '<iframe style="width: 600px; height:500px" scrolling="no" frameborder="0" name="iframe_paypoint" id="iframe_paypoint" src="' . url('commerce_paypoint/paypoint_waiting_page') . '"></iframe>',
        );

        return $form;
      }
      else {
        header("Location: " . $data->redirectUrl);
      }

      exit;

    case 'FAILED':
      $transaction->remote_status = $status;
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'FAIL Response from PayPoint';
      commerce_payment_transaction_save($transaction);
      drupal_goto('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key']);
      break;

    default:
      $transaction->remote_status = $status;
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = 'Unknown or invalid response from PayPoint';
      commerce_payment_transaction_save($transaction);
      watchdog('commerce_paypoint_advanced', 'Unrecognised Status response from PayPoint for order %order_id (%response_code)', array('%order_id' => $order->order_id, '%response_code' => $transaction_status[0]), WATCHDOG_ERROR);
      drupal_goto('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key']);
      break;
  }
}

/**
 * Outputs a temporary page before the PayPoint form is submitted.
 *
 * Displayed when there is a load delay or if the user has Javascript disabled.
 */
function commerce_paypoint_advanced_waiting_page() {
  print ('<html><head><title></title></head><body><p>');
  print t('Please wait to be redirected to the payment page.');
  print ('</p></body></html>');
}

