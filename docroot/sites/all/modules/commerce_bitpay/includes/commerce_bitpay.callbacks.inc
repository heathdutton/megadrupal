<?php

/**
 * @file
 * Callbacks used by hook_commerce_payment_method_info().
 */

/**
 * Global, required API configuration form.
 *
 * @return array
 *   Returns form elements for the payment method’s settings form included as
 *   part of the payment method’s enabling action in Rules.
 */
function commerce_bitpay_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_bitpay_default_settings();

  $form['apiKey'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['apiKey'],
    '#title' => t('BitPay API key'),
    '#size' => 45,
    '#maxlength' => 130,
    '#description' => t('BitPay API access key. Something like aW4x5kLr4mer4fovDJLGTMXSATkf81DLKcm349ajd12'),
    '#required' => TRUE,
  );
  $form['checkout_display'] = array(
    '#type' => 'radios',
    '#title' => t('Payment display'),
    '#options' => array(
      'text' => t('Bitcoin text only'),
      'icon' => t('Bitcoin icon only'),
      'both' => t('Both text and icon'),
    ),
    '#default_value' => $settings['checkout_display'],
    '#description' => t('When selecting a payment option, select the indicator next to the radio buttons for payment options.'),
  );
  $form['redirectURL'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['redirectURL'],
    '#title' => t('Redirect URL'),
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('URL for a return link that is displayed on the receipt to return the
shopper back to your website after a successful purchase. Leave blank to use the default "checkout complete" page.'),
    '#required' => FALSE,
  );
  $form['notificationEmail'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['notificationEmail'],
    '#title' => t('Notification email'),
    '#size' => 60,
    '#maxlength' => 254,
    '#description' => t('The email where invoice update notifications should be sent. Leave blank to use default settings defined in your BitPay account.'),
    '#required' => FALSE,
  );
  $form['transactionSpeed'] = array(
    '#type' => 'select',
    '#default_value' => $settings['transactionSpeed'],
    '#empty_option' => t('- BitPay account default -'),
    '#empty_value' => '',
    '#title' => t('Transaction speed'),
    '#description' => t('Default: use the Risk/Speed in your BitPay !account_settings; High: confirmed immediately; Medium: confirmed after 1 block confirmation (~10 minutes); Low: confirmed after 6 block confirmations (~1 hour). Orders are posted to your BitPay account summary after 6 confirmations regardless of this setting.', array('!account_settings' => l(t('account settings'), 'https://bitpay.com/order-settings'))),
    '#options' => array(
      'high' => t('High'),
      'medium' => t('Medium'),
      'low' => t('Low'),
    ),
    '#required' => FALSE,
  );
  $form['fullNotifications'] = array(
    '#type' => 'select',
    '#title' => t('Full notifications'),
    '#options' => array('true' => t('Enabled'), 'false' => t('Disabled')),
    '#default_value' => $settings['fullNotifications'],
    '#description' => t('Full notifications send an update on every status change, otherwise notifications are sent only when a transaction is confirmed according to the transaction speed.'),
  );
  $form['redirect_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Checkout redirect mode'),
    '#options' => array(
      'iframe' => t('Stay on this site using an iframe to embed the hosted checkout page'),
      'post' => t('Redirect to a checkout page hosted by BitPay'),
    ),
    '#description' => t('Using the hosted page will use a generically themed page, hosted by BitPay which can display supplemental invoice details for the buyer. Using the iframe keeps the buyer on this site, but will have reduced functionality for displaying invoice metadata.'),
    '#default_value' => $settings['redirect_mode'],
  );

  return $form;
}

/**
 * Payment method callback: submit form submission.
 *
 * Processes payment as necessary using data inputted via the payment details
 * form elements on the form, resulting in the creation of a payment
 * transaction.
 *
 * @param array $payment_method
 *   An array containing BitPay info hook values and user settings.
 */
function commerce_bitpay_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Get order to wrapper.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get order price.
  $amount = $wrapper->commerce_order_total->amount->value();

  // @link https://drupal.org/node/2136989 $0 invoice throws an error @endlink
  if (empty($amount)) {
    watchdog('commerce_bitpay', 'Skipping payment on order @id for zero balance.', array('@id' => $order->order_id), WATCHDOG_INFO, l(t('view order'), 'admin/commerce/orders/' . $order->order_id));
    commerce_checkout_complete($order);
    drupal_goto(_commerce_bitpay_redirecturl($payment_method, $order));
  }

  $order->data['bitpay'] = $pane_values;

  commerce_bitpay_transaction($payment_method, $order, $charge);
}

/**
 * Payment method callback: redirect form.
 *
 * For the hosted checkout page, this form automatically redirects to the BitPay
 * hosted invoice page through an HTTP GET request. For the iframe, this returns
 * form values for displaying markup elements necessary to embed the iframe
 * and a submit button.
 *
 * @param array $form
 *   Probably an empty array when this gets executed.
 * @param array $form_state
 *   Form submission data including order node information and payment method
 *   information.
 * @param object $order
 *   An object of general order information.
 * @param array $payment_method
 *   An array containing BitPay info hook values and user settings.
 */
function commerce_bitpay_redirect_form($form, &$form_state, $order, $payment_method) {
  // Get order to wrapper.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get order price.
  $amount = $wrapper->commerce_order_total->amount->value();
  $order_total = $wrapper->commerce_order_total->value();
  $billing_profile = $wrapper->commerce_customer_billing->value();
  $shipping_profile = $wrapper->commerce_customer_shipping->value();
  $shipping_services = commerce_shipping_services();

  $decimal_amount = commerce_currency_amount_to_decimal($amount, $order_total['currency_code']);

  global $base_url;
  if (stripos($base_url, 'https://') !== 0) {
    watchdog('commerce_bitpay', 'Site base_url does not use HTTPS. Transactions will not automatically update from BitPay.', array(), WATCHDOG_CRITICAL);
  }

  $options = array();

  $options['apiKey'] = $payment_method['settings']['apiKey'];
  $options['fullNotifications'] = $payment_method['settings']['fullNotifications'];
  $options['notificationEmail'] = $payment_method['settings']['notificationEmail'];
  $options['redirectURL'] = _commerce_bitpay_redirecturl($payment_method, $order);

  // If the customer has a billing address, include it on the invoice.
  if (!empty($billing_profile->commerce_customer_address)) {
    list (, $billing_address) = each($billing_profile->commerce_customer_address);
    if (!empty($billing_address)) {
      // Not trimming to 100 characters will cause invoice generation to fail.
      $options['buyerName'] = empty($billing_address[0]['name_line']) ? '' : substr($billing_address[0]['name_line'], 0, 100);
      $options['buyerAddress1'] = empty($billing_address[0]['thoroughfare']) ? '' : substr($billing_address[0]['thoroughfare'], 0, 100);
      $options['buyerAddress2'] = empty($billing_address[0]['premise']) ? '' : substr($billing_address[0]['premise'], 0, 100);
      $options['buyerCity'] = empty($billing_address[0]['locality']) ? '' : substr($billing_address[0]['locality'], 0, 100);
      $options['buyerState'] = empty($billing_address[0]['administrative_area']) ? '' : substr($billing_address[0]['administrative_area'], 0, 100);
      $options['buyerZip'] = empty($billing_address[0]['postal_code']) ? '' : substr($billing_address[0]['postal_code'], 0, 100);
      /*
      API: buyerCountry appears to be not used in invoices.
      Sending a value for buyerCountry >100 characters has no error, either.
      */
      $options['buyerEmail'] = empty($order->mail) ? '' : substr($order->mail, 0, 100);
      // API: buyerPhone is not implemented since Kickstart has no user phone.
    }
  }

  /*
  Guess if it is a physical item by checking for shipping attached to the
  order listed in the array of priced items.
  */
  if (!empty($shipping_profile->commerce_customer_address)) {
    foreach ($order_total['data']['components'] as $component) {
      foreach ($shipping_services as $service) {
        if (isset($service['price_component']) && $service['price_component'] == $component['name']) {
          $options['physical'] = 'true';
          break 2;
        }
      }
    }
  }

  /*
  There is a 100-char limit on descriptions;
  going over the limit causes the invoice to be rejected.
  */
  $item_description = '';
  // Get the products in the order.
  foreach ($order->commerce_line_items as $commerce_line_items) {
    foreach ($commerce_line_items as $line_item) {
      $line_item_description = commerce_line_item_load($line_item['line_item_id']);
      if ($line_item_description->type == 'product') {
        if (!empty($line_item_description->commerce_product['und'][0]['product_id'])) {
          // Load the product title and SKU.
          $product = commerce_product_load($line_item_description->commerce_product['und'][0]['product_id']);
          // Extra trims remove trailing zeros and . if no zeros remain.
          $item_description = trim(trim(trim($line_item_description->quantity, '0'), '.') . 'x ' . $product->title . ' - ' . $product->sku);
        }
        else {
          // Just send the SKU.
          $item_description = trim(trim(trim($line_item_description->quantity, '0'), '.') . 'x ' . $line_item_description->line_item_label);
        }
        if (count($commerce_line_items) > 1) {
          $description_ending = t(', etc.');
        }
        else {
          $description_ending = '';
        }

        // Only describe the first item to BitPay.
        break 2;
      }
    }
  }
  if ((strlen($item_description) + strlen($description_ending)) > 100) {
    $item_description = substr($item_description, 0, (97 - strlen($description_ending))) . $description_ending . '...';
  }
  else {
    $item_description .= $description_ending;
  }
  $options['itemDesc'] = $item_description;

  /*
  @todo
  Use the currency defined for each item or let something else in Commerce
  handle price standardizing before it gets to this point?
  */
  $options['currency'] = $order_total['currency_code'];

  // Get transaction base information.
  $transaction_info = db_select('commerce_payment_transaction', 'cpt')
    ->fields('cpt', array('transaction_id', 'remote_id', 'message', 'changed'))
    ->condition('cpt.payment_method', 'bitpay')
    ->condition('cpt.order_id', $order->order_id)
    ->range(0, 1)
    ->addTag('commerce_bitpay_redirect_transaction')
    ->execute()
    ->fetchAssoc();
  if ($transaction_info && is_array($transaction_info)) {
    $transaction = commerce_payment_transaction_load($transaction_info['transaction_id']);
  }
  else {
    watchdog('commerce_bitpay', 'Failed to load a transaction for order @id.', array('@id' => $order->order_id));
  }

  $options['notificationURL'] = $base_url . '/bitpay/ipn/' . $order->order_id . '/' . $transaction->transaction_id;
  watchdog('commerce_bitpay', 'Using @url for notificationURL.', array('@url' => $options['notificationURL']), WATCHDOG_DEBUG);

  libraries_load('php-client');

  /*
  900 seconds (15 minutes) is how long it takes for an invoice to expire.
  We need to rate-limit the generation of new invoices because otherwise
  clicking back on the BitPay invoice page will refresh to the redirect form
  with a completely new invoice id. Since BitPay only allows 100 invoices
  per hour, a malicious user could DoS the Bitcoin payment option with
  repeated, rapid requests. We rate-limit by re-using existing remote_ids.
  */
  if (isset($transaction_info['remote_id']) && strlen($transaction_info['remote_id']) >= 4 && strpos($transaction_info['message'], 'https://bitpay.com/invoice?id=') !== FALSE && (REQUEST_TIME - $transaction_info['changed'] < 875)) {
    $invoice['id'] = $transaction_info['remote_id'];
    $invoice['url'] = $transaction_info['message'];

    drupal_set_message(t('Your payment must be acknowledged by the blockchain before you can complete the checkout.'), 'warning');
    watchdog('commerce_bitpay', 'Re-used existing invoice for order @order.', array('@order' => $order->order_id), WATCHDOG_DEBUG, l(t('invoice'), $invoice['url']));
  }
  else {
    /*
    While this section could do the call to commerce_payment_transaction_new(),
    it's the commerce_payment_transaction_save() that creates the transaction_id
    that's needed to send to bpCreateInvoice() so it can be included as part of
    the IPN. All the _new() function does is return an initialized object, it
    does not create an actual transaction with ID.

    Since this code waits until after the invoice is created to do the _save(),
    the _new() with a _save() should be called somewhere early enough to have
    a transaction ID by now. That setup is being handled by the
    submit_form_submit() callback, which gets called before redirect_form().
    */

    // bpCreateInvoice() is a function in the BitPay php-client library.
    $invoice = bpCreateInvoice($order->order_id, $decimal_amount, $transaction->transaction_id, $options);

    if (!isset($invoice['id']) || !isset($invoice['url']) || strlen($invoice['id']) < 4 || strpos($invoice['url'], 'https://bitpay.com/invoice?id=') === FALSE) {
      if (isset($invoice['error'])) {
        // Also available: $invoice['error']['type']
        drupal_set_message(t('Invoice creation failed: @message', array('@message' => $invoice['error']['message'])), 'error');
      }
      else {
        drupal_set_message(t('Invoice creation failed. Payment cannot proceed at this time.'), 'error');
      }
      watchdog('commerce_bitpay', 'Invoice creation failure: @serial', array('@serial' => serialize($invoice)), WATCHDOG_ERROR);
      return;
    }

    // Invoice ID is a hash.
    $transaction->remote_id = $invoice['id'];
    // Invoice ID is a complete url for hosted payment, including the above ID.
    $transaction->message = $invoice['url'];
    // Expected value: new. Others: paid, confirmed, complete, expired, invalid.
    $transaction->remote_status = $invoice['status'];
    commerce_payment_transaction_save($transaction);
  }

  // Marks order to system.
  $order->data['bitpay'] = $invoice['url'];

  // Update the order status to the payment redirect page.
  commerce_order_status_update($order, 'checkout_payment', FALSE, NULL, t('Customer clicked the button to pay with Bitcoin on the cart page.'));
  commerce_order_save($order);

  switch ($payment_method['settings']['redirect_mode']) {
    case 'post':
      // Redirect to BitPay hosted invoice page.
      drupal_goto($invoice['url']);
      break;

    case 'iframe':
      // Display an invoice in an iframe on the confirm payment page.
      $form['#action'] = url('checkout/' . $order->order_id . '/complete');
      $form['iframe'] = array(
        '#markup' => '<iframe seamless style="width:500px; height:150px; overflow:hidden; border:none; margin:auto; display:block;" scrolling="no" allowtransparency="true" frameborder="0" src="' . $invoice['url'] . '&view=iframe">Your browser does not support IFRAMEs. Please click <a href="' . $invoice['url'] . '" target="_blank">HERE</a> to make payment.</iframe>',
      );

      $form['buttons'] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('checkout-buttons')),
      );
      $form['buttons']['continue'] = array(
        '#type' => 'button',
        '#value' => t('Complete checkout'),
        '#attributes' => array('class' => array('checkout-continue')),
        '#suffix' => '<span class="checkout-processing element-invisible"></span>',
      );

      $button_operator = '<span class="button-operator">' . t('or') . '</span>';
      $form['buttons']['back'] = array(
        '#prefix' => $button_operator,
        '#markup' => l(t('Go back'), "checkout/$order->order_id/commerce_bitpay/cancel_payment",
          array(
            'attributes' => array(
              'class' => array('checkout-back'),
              'id' => 'cancel-bitpay-payment',
            ),
          )
        ),
      );
      return $form;
  }
}

/**
 * Central place to figure out a redirect URL after completed checkout.
 *
 * @return string
 *   A URL to redirect users to after successful checkout.
 */
function _commerce_bitpay_redirecturl($payment_method, $order) {
  global $base_url;
  if ($payment_method['settings']['redirectURL'] == '') {
    return $base_url . '/checkout/' . $order->order_id . '/complete';
  }
  return $payment_method['settings']['redirectURL'];
}
