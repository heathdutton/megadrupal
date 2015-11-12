<?php

/**
 * Implements hook_menu().
 * 
 */
function commerce_swipehq_menu() {
  $items['commerce/swipehq/test'] = array(
    'title' => t('SwipeHQ Test'),
    'page callback' => array('commerce_swipehq_test'),
    'access callback' => TRUE,
  );

  return $items;
}

/**
 * Callback for SwipeHQ Test
 */
function commerce_swipehq_test() {
  //need to manually configure $settings or $order_id
  $settings = array(
    'api_credentials' => array(
      'merchant_id' => '',
      'api_key' => '',
      'payment_page_url' => '',
      'api_url' => '',
      'supported_currencies' => '',
    ),
  );
  $order_id = 0;

  //here the debug code
  $order = commerce_order_load($order_id);
  $swipehq = new CommerceSwipeHq($settings, $order);
  $swipehq->getAcceptedCurrencies();
  $swipehq->getTransactionIdentifier();
  var_dump($swipehq->getTransactionIdentifier());
}

/**
 * Implements hook_commerce_payment_method_info()
 */
function commerce_swipehq_commerce_payment_method_info() {
  return array(
    'commerce_payment_swipehq' => array(
      'title' => t('Swipe HQ Payment Gateway'),
      'short_title' => t('SwipeHQ'),
      'display_title' => t('Swipe HQ Payment'),
      'description' => t('Provides integration with the Swipe HQ payment gateway.'),
      'offsite' => TRUE,
      'offsite_autoredirect' => TRUE,
      'active' => TRUE,
    ),
  );
}

/**
 * Payment method callback: settings form
 */
function commerce_payment_swipehq_settings_form($settings = NULL) {
  // Merge default settings into the stored settings array.
  $settings = (array) $settings + array(
    'api_credentials' => array(
      'merchant_id' => '',
      'api_key' => '',
      'payment_page_url' => '',
      'api_url' => '',
      'supported_currencies' => '',
    ),
    'checkout_button' => t('Submit Order'),
  );

  $form['api_credentials'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Crendentials'),
    '#collapsible' => FALSE
  );

  $form['api_credentials']['merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID'),
    '#default_value' => $settings['api_credentials']['merchant_id'],
    '#required' => TRUE,
  );
  $form['api_credentials']['api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => $settings['api_credentials']['api_key'],
    '#required' => TRUE,
  );

  $form['api_credentials']['payment_page_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Payment Page URL'),
    '#default_value' => $settings['api_credentials']['payment_page_url'],
    '#required' => TRUE,
  );
  $form['api_credentials']['api_url'] = array(
    '#type' => 'textfield',
    '#title' => t('API URL'),
    '#default_value' => $settings['api_credentials']['api_url'],
    '#required' => TRUE,
  );
  $form['api_credentials']['supported_currencies'] = array(
    '#type' => 'textfield',
    '#title' => t('Supported Currencies'),
    '#description' => t('Separated by commas.'),
    '#default_value' => $settings['api_credentials']['supported_currencies'],
    '#required' => TRUE,
  );

  $form['checkout_button'] = array(
    '#type' => 'textfield',
    '#title' => t('Checkout Button Text'),
    '#default_value' => $settings['checkout_button'],
    '#required' => TRUE,
  );

  return $form;
}

/**
 * Payment method callback: submit_form_submit
 */
function commerce_payment_swipehq_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  if (empty($payment_method['settings']['api_credentials']['api_url']) || empty($payment_method['settings']['api_credentials']['merchant_id']) || empty($payment_method['settings']['api_credentials']['payment_page_url']) || empty($payment_method['settings']['api_credentials']['api_key'])) {
    drupal_set_message(t('Payment gateway is not configured for use.'), 'error');
    return FALSE;
  }

  //Create a new transaction with no remote ID
  $transaction = commerce_payment_transaction_new('commerce_payment_swipehq', $order->order_id);
  $transaction->amount = $charge['amount'];
  $transaction->currency_code = $charge['currency_code'];
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
  $transaction->instance_id = $payment_method['instance_id'];
  commerce_payment_transaction_save($transaction);

  return TRUE;
}

/**
 * Payment method callback: redirect_form
 */
function commerce_payment_swipehq_redirect_form($form, &$form_state, $order, $payment_method) {
  $transactions = commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id));
  if (count($transactions)) {
    $transaction = end($transactions);
    //Get transaction identifier from Swipe HQ API, update to transaction
    $obj = new CommerceSwipeHq($payment_method['settings'], $order);
    $transaction->remote_id = $obj->getTransactionIdentifier();

    commerce_payment_transaction_save($transaction);

    $form['#action'] = $payment_method['settings']['api_credentials']['payment_page_url'];

    $form['identifier_id'] = array(
      '#type' => 'hidden',
      '#value' => $transaction->remote_id
    );
    $form['checkout'] = array(
      '#type' => 'hidden',
      '#value' => 'true'
    );

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => $payment_method['settings']['checkout_button'],
    );
    $form['actions']['cancel'] = array(
      '#markup' => l(t('Cancel'), url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE))),
    );

    return $form;
  }
}

/**
 * Payment method callback: redirect_form_submit
 */
function commerce_payment_swipehq_redirect_form_submit($order, $payment_method) {
  $transactions = commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id));
  if (count($transactions)) {
    $transaction = end($transactions);
    $commerce_swipehq = new CommerceSwipeHq($payment_method['settings'], $order);
    $commerce_swipehq->setTransactionIdentifier($transaction->remote_id);
    $verification = $commerce_swipehq->getVerification();
    if ($verification) {
      $transaction->remote_id = $commerce_swipehq->getTransactionID();
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      commerce_payment_transaction_save($transaction);
      return TRUE;
    } else {
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      commerce_payment_transaction_save($transaction);
    }
  }

  drupal_set_message(t('There was a problem processing your transaction.'), 'error');
  return FALSE;
}
