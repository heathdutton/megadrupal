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
function commerce_coinbase_settings_form($settings = array()) {
  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_coinbase_default_settings();

  $form['links'] = array(
    '#type' => 'markup',
    '#markup' => '<p>' .
    t('For more information visit the <a href=@overview>Coinbase Bitcoin API overview</a> or signup now with <a href=@coinbase>Coinbase</a>.', array(
      '@overview' => 'https://coinbase.com/docs/api/overview',
      '@coinbase' => 'https://coinbase.com/?r=5242664f8fec8d2e4b000007&utm_campaign=user-referral&src=referral-link',
      ))
    . '</p>',
  );

  $form['api_key'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['api_key'],
    '#title' => t('Coinbase API key'),
    '#size' => 64,
    '#maxlength' => 70,
    '#description' => t('Coinbase API access key from https://www.coinbase.com/settings/api, something like ox5Em66zOyTrlYq.'),
    '#required' => TRUE,
  );
  $form['api_secret'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['api_secret'],
    '#title' => t('Coinbase API secret'),
    '#size' => 64,
    '#maxlength' => 70,
    '#description' => t('Coinbase API secret from https://www.coinbase.com/settings/api, something like b5yBxo5q7ePr6ty96ufr4eWn90Lpse.'),
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
  $form['type'] = array(
    '#type' => 'select',
    '#default_value' => $settings['type'],
    '#title' => t('Transaction type'),
    '#options' => array(
      'buy_now' => t('Buy now'),
      'donation' => t('Donation'),
      'subcription' => t('Subscription'),
    ),
    '#description' => t('Change the type of payment button for all transactions.'),
    '#required' => TRUE,
  );
  $form['style'] = array(
    '#type' => 'select',
    '#default_value' => $settings['style'],
    '#title' => t('Button style'),
    '#options' => array(
      'large' => t('Large'),
      'small' => t('Small'),
    ),
    '#description' => t('Change the payment button style for all transactions.'),
    '#required' => TRUE,
  );
  $form['text'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['text'],
    '#title' => t('Button text'),
    '#description' => t('Change the payment button text for all transactions.'),
    '#required' => FALSE,
  );
  $form['redirect_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Checkout redirect mode'),
    '#options' => array(
      'iframe' => t('Stay on this site using an iframe to embed the hosted checkout page'),
      'hosted' => t('Redirect to a checkout page hosted by Coinbase'),
    ),
    '#description' => t('Using the hosted page will redirect to a generically themed page hosted by Coinbase.'),
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
 *   An array containing payment_method_info hook values and user settings.
 */
function commerce_coinbase_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Get order to wrapper.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get order price.
  $amount = $wrapper->commerce_order_total->amount->value();

  if (empty($amount)) {
    watchdog('commerce_coinbase', 'Skipping payment on order @id for zero balance.', array('@id' => $order->order_id), WATCHDOG_INFO, l(t('view order'), 'admin/commerce/orders/' . $order->order_id));
    commerce_checkout_complete($order);
    drupal_goto(url('checkout/' . $order->order_id . '/complete'));
  }

  $order->data['coinbase'] = $pane_values;

  commerce_coinbase_transaction($payment_method, $order, $charge);
}

/**
 * Payment method callback: redirect form.
 *
 * For the hosted checkout page, this form automatically redirects to the
 * Coinbase hosted invoice page through an HTTP GET request. For the iframe,
 * this returns form values for displaying markup elements necessary to embed
 * the iframe and a submit button.
 *
 * @param array $form
 *   Probably an empty array when this gets executed.
 * @param array $form_state
 *   Form submission data including order node information and payment method
 *   information.
 * @param object $order
 *   An object of general order information.
 * @param array $payment_method
 *   An array containing payment_method_info hook values and user settings.
 */
function commerce_coinbase_redirect_form($form, &$form_state, $order, $payment_method) {
  // Get order to wrapper.
  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Get order price.
  $amount = $wrapper->commerce_order_total->amount->value();
  $order_total = $wrapper->commerce_order_total->value();
  $site_name = variable_get('site_name', 'Drupal');

  $decimal_amount = commerce_currency_amount_to_decimal($amount, $order_total['currency_code']);

  $transaction_info = db_select('commerce_payment_transaction', 'cpt')
    ->fields('cpt', array('transaction_id', 'remote_id', 'message', 'changed'))
    ->condition('cpt.payment_method', 'coinbase')
    ->condition('cpt.order_id', $order->order_id)
    ->orderBy('cpt.transaction_id', 'DESC')
    ->range(0, 1)
    ->addTag('commerce_coinbase_redirect_transaction')
    ->execute()
    ->fetchAssoc();
  if ($transaction_info && is_array($transaction_info)) {
    $transaction = commerce_payment_transaction_load($transaction_info['transaction_id']);
  }
  else {
    watchdog('commerce_coinbase', 'Failed to load a transaction for order @id.', array('@id' => $order->order_id));
  }

  libraries_load('coinbase-php');

  $key = $payment_method['settings']['api_key'];
  $secret = $payment_method['settings']['api_secret'];
  $coinbase_auth = new Coinbase_ApiKeyAuthentication($key, $secret);
  $coinbase = new Coinbase($coinbase_auth);

  $response = $coinbase->createButton(
    $site_name . " order #" . $order->order_id,
    $decimal_amount,
    $order_total['currency_code'],
    $transaction->transaction_id,
    array(
      'type' => $payment_method['settings']['type'],
      'style' => $payment_method['settings']['type'] . '_' . $payment_method['settings']['style'],
      'text' => $payment_method['settings']['text'],
      'include_email' => empty($order->mail) ? '' : $order->mail,
      'callback_url' => url('coinbase/callback/' . $order->order_id . '/' . $transaction->transaction_id . '/' . $transaction->data['secret'], array('absolute' => TRUE)),
      // Success and cancel for the hosted invoice to redirect back to Drupal.
      'success_url' => url('checkout/' . $order->order_id . '/complete', array('absolute' => TRUE)),
      'cancel_url' => url('checkout/' . $order->order_id . '/commerce_coinbase/cancel_payment', array('absolute' => TRUE)),
    )
  );

  // Update the order status to the payment redirect page.
  commerce_order_status_update($order, 'checkout_payment', FALSE, NULL, t('Customer clicked the button to pay with Bitcoin on the cart page.'));
  commerce_order_save($order);

  switch ($payment_method['settings']['redirect_mode']) {
    case 'hosted':
      // Redirect to Coinbase hosted invoice page.
      drupal_goto('https://coinbase.com/checkouts/' . $response->button->code);
      break;

    case 'iframe':
      // Display an invoice in an iframe on the confirm payment page.
      $form['#action'] = url('checkout/' . $order->order_id . '/complete', array('absolute' => TRUE));

      $coinbase_path = drupal_get_path('module', 'commerce_coinbase');
      drupal_add_js($coinbase_path . '/commerce_coinbase_redirect.js', array('cache' => FALSE));

      drupal_add_js(array('coinbaseCompleteCheckout' => $form['#action']), 'setting');

      $form['iframe'] = array(
        '#markup' => '<iframe seamless style="width:500px; height:160px; overflow:hidden; border:none; margin:auto; display:block;" scrolling="no" allowtransparency="true" frameborder="0" src="https://coinbase.com/inline_payments/' . $response->button->code . '"><a href="https://coinbase.com/checkouts/' . $response->button->code . '" target="_blank"><img alt="Pay With Bitcoin" src="https://coinbase.com/assets/buttons/buy_now_large.png"></a></iframe>',
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
        '#markup' => l(t('Go back'), "checkout/$order->order_id/commerce_coinbase/cancel_payment",
          array(
            'attributes' => array(
              'class' => array('checkout-back'),
              'id' => 'cancel-coinbase-payment',
            ),
          )
        ),
      );
      return $form;
  }
}
