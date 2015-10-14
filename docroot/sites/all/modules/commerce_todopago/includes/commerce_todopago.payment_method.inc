<?php

/**
 * @file
 * Implements Todo Pago payment callbacks for use with Drupal Commerce.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_todopago_settings_form($settings = array()) {
  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_todopago_default_settings();

  $form['merchant'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant code'),
    '#description' => t('The code of the company in Todo Pago\'s system.'),
    '#default_value' => $settings['merchant'],
    '#required' => TRUE,
  );
  $form['currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Default currency'),
    '#description' => t('Transactions in other currencies will be converted to this currency, so multi-currency sites must be configured to use appropriate conversion rates.'),
    '#options' => commerce_todopago_currencies(),
    '#default_value' => $settings['currency_code'],
  );
  $form['allow_supported_currencies'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow transactions to use any currency in the options list above.'),
    '#description' => t('Transactions in unsupported currencies will still be converted into the default currency.'),
    '#default_value' => $settings['allow_supported_currencies'],
  );
  $form['authorization'] = array(
    '#type' => 'textfield',
    '#title' => t('Authorization code'),
    '#description' => t('The authorizarion code for the company in Todo Pago\'s system.'),
    '#default_value' => $settings['authorization'],
    '#required' => TRUE,
  );
  $form['security'] = array(
    '#type' => 'textfield',
    '#title' => t('Security code'),
    '#description' => t('The security code for the company in Todo Pago\'s system.'),
    '#default_value' => $settings['security'],
    '#required' => TRUE,
  );
  $form['category'] = array(
    '#type' => 'select',
    '#title' => t('Commerce category'),
    '#description' => t('The category of the company\'s bussiness in Todo Pago\'s system.'),
    '#options' => array(
      'retail' => t('Retail'),
      'travel' => t('Travel'),
      'services' => t('Services'),
      'digital_goods' => t('Digital goods'),
      'ticketing' => t('Ticketing'),
    ),
    '#default_value' => $settings['category'],
    '#required' => TRUE,
  );
  $form['mode'] = array(
    '#type' => 'radios',
    '#title' => t('Mode'),
    '#options' => array(
      'test' => ('Development'),
      'prod' => ('Production'),
    ),
    'test' => array('#description' => 'Use for testing the integration.'),
    'prod' => array('#description' => 'Use for processing real transactions.'),
    '#default_value' => $settings['mode'],
  );

  return $form;
}

/**
 * Returns the default settings for the Todo Pago's payment method.
 */
function commerce_todopago_default_settings() {
  $default_currency = commerce_default_currency();

  return array(
    'merchant' => '',
    'currency_code' => in_array($default_currency, array_keys(commerce_todopago_currencies())) ? $default_currency : 'ARS',
    'allow_supported_currencies' => FALSE,
    'authorization' => '',
    'security' => '',
    'category' => 'retail',
    'mode' => 'test',
  );
}

/**
 * Payment method callback: adds a message to the submission form if enabled in
 * the payment method settings.
 */
function commerce_todopago_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  $image_title = t('(Continue with checkout to complete payment via Todo Pago.)');
  $image_variables = array(
    'path' => drupal_get_path('module', 'commerce_todopago') . '/images/payment-methods.jpg',
    'title' => $image_title,
    'alt' => $image_title,
    'attributes' => array(
      'class' => array('commerce-todopago-icon'),
    ),
  );
  $form['todopago_information'] = array(
    '#markup' => theme('image', $image_variables),
  );

  return $form;
}

/**
 * Payment method callback: redirect form, a wrapper around the module's general
 *   use function for building a Todo Pago's form.
 */
function commerce_todopago_redirect_form($form, &$form_state, $order, $payment_method) {
  // Return an error if the enabling action's settings haven't been configured.
  if (
    (empty($payment_method['settings']['merchant'])) ||
    (empty($payment_method['settings']['authorization'])) ||
    (empty($payment_method['settings']['security']))
  ) {
    drupal_set_message(t('Todo Pago is not configured for use.'), 'error');
    return array();
  }

  $payment_method['settings']['instance_id'] = $payment_method['instance_id'];

  return commerce_todopago_order_form($form, $form_state, $order, $payment_method['settings']);
}

/**
 * Builds a Todo Pago's form from an order object.
 *
 * @param $order
 *   The fully loaded order being paid for.
 * @param $settings
 *   An array of settings used to build out the form, including:
 *   - server: which server to use, either sandbox or live
 *   - merchant: the merchant code of the company the payment submits to
 *   - currency_code: the currency code to use for this payment if the
 *     total for the order is in a non supported currency
 *   - authorization: the authorization code for the company
 *   - security: the security code for the company
 *   - instance_id: the instance_id of the payment method
 *
 * @return
 *   A renderable form array.
 */
function commerce_todopago_order_form($form, &$form_state, $order, $settings) {
  $form = array();

  if (($library = libraries_load('todopago')) && !empty($library['loaded'])) {
    try {
      $todopago = new TodoPagoConnector($settings);

      $form = $todopago->create_order_form($order);
    }
    catch (Exception $e) {
      commerce_payment_redirect_pane_previous_page($order);

      watchdog('commerce_todopago', 'There was an error with the payment for Order Nr. @order_number: %error', array('@order_number' => $order->order_number, '%error' => $e->getMessage()), WATCHDOG_NOTICE);

      drupal_set_message(t('There was a problem connecting with Todo Pago. Try again later.'), 'error');

      $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Try again'),
      );
    }
  }

  return $form;
}

/**
 * Payment method callback; cancellation callback for the redirected payments.
 */
function commerce_todopago_redirect_form_back($order, $payment_method) {
  // This callback shouldn't we necessary
  // Todo Pago does not have a Cancel button
}

/**
 * Payment method callback; validation callback for redirected payments.
 */
function commerce_todopago_redirect_form_validate($order, $payment_method) {
  $return = FALSE;

  $hash = arg(5);
  $remote_id = commerce_todopago_remote_id($order, $hash);
  if (
    (!empty($_GET['Answer'])) &&
    (!empty($hash)) &&
    ($transaction = commerce_todopago_payment_transaction_load($remote_id)) &&
    ($order->order_number == $transaction->order_id)
  ) {
    $transaction->data['answer_key'] = $_GET['Answer'];
    commerce_payment_transaction_save($transaction);

    if (($library = libraries_load('todopago')) && !empty($library['loaded'])) {
      $todopago = new TodoPagoConnector($payment_method['settings']);

      try {
        $todopago->update_status($transaction);
      }
      catch (Exception $e) {
        watchdog('commerce_todopago', 'There was an error fetching the status of the Transaction Nr. @transaction_number of the payment for Order Nr. @order_number: %error', array('@transaction_number' => $transaction->transaction_id, '@order_number' => $order->order_number, '%error' => $e->getMessage()), WATCHDOG_NOTICE);

        drupal_set_message(t('There was an error getting the payment status from Todo Pago. Please contact an administrator before trying again.'), 'error');
      }

      switch ($transaction->status) {
        case COMMERCE_PAYMENT_STATUS_SUCCESS:
          $return = TRUE;
          break;
        case COMMERCE_PAYMENT_STATUS_FAILURE:
          $return = FALSE;
          break;
        case COMMERCE_PAYMENT_STATUS_PENDING:
          // We let the customer continue with the checkout,
          // but without a payment transaction for the order
          $return = TRUE;
          break;
      }
    }
  }

  return $return;
}

/**
 * Payment method callback; submission callback for redirected payments.
 */
function commerce_todopago_redirect_form_submit($order, $payment_method) {
  $hash = arg(5);
  $remote_id = commerce_todopago_remote_id($order, $hash);
  if (
    (!empty($hash)) &&
    ($transaction = commerce_todopago_payment_transaction_load($remote_id)) &&
    (!empty($transaction->data['answer_key'])) &&
    ($order->order_number == $transaction->order_id)
  ) {
    if ($transaction->status == COMMERCE_PAYMENT_STATUS_PENDING) {
      drupal_set_message(t('We are waiting for the confirmation of your payment. If you think it should be confirmed by now, please contact an administrator.'), 'warning');
    }
  }
}

