<?php

/**
 * @file
 * File being loaded by the Commerce Payment Framework when needed.
 * Compatible with ShopBuilder.me eCommerce platform
 * Author Eweev S.A.R.L (www.eweev.com)
 * Sponsor YellowPay (www.yellowpay.co)
 */

/**
 * Returns the default settings for the Bank Audi payment method.
 */
function commerce_yellowpay_default_settings() {
  return array(
    'apikey' => '',
    'apisecret' => '',
    'watchdog' => FALSE,
    'show_payment_instructions' => FALSE,
    'payment_instructions_text' => t('Continue to the next step to complete your Bitcoin payment with YellowPay.'),
  );
}

/**
 * Payment method callback: settings form.
 * This form is displayed to the merchant to configure the payment method
 */
function commerce_yellowpay_settings_form($settings = array()) {
  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_yellowpay_default_settings();

  if (isset($_GET['api']) || empty($settings['apikey']) || empty($settings['apisecret'])) {
    $form['apikey'] = array(
      '#type' => 'password',
      '#title' => t('API key'),
      '#description' => t("Please enter your API Key provided by YellowPay"),
      '#default_value' => $settings['apikey'],
    );
    $form['apisecret'] = array(
      '#type' => 'password',
      '#title' => t('API Secret'),
      '#description' => t("Please enter your API Secret provided by YellowPay"),
      '#default_value' => $settings['apisecret'],
    );
  }
  else {
    $form['apikey'] = array(
      '#type' => 'value',
      '#value' => $settings['apikey'],
    );
    $form['apisecret'] = array(
      '#type' => 'value',
      '#value' => $settings['apisecret'],
    );
    $form['apichange'] = array(
      '#markup' => t('<a href="@changelink">Click to change your API key and Secret</a>', array('@changelink' => url(current_path(), array('absolute' => TRUE)) . '?api=change')),
    );
  }

  $form['show_payment_instructions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show a message on the checkout form when YellowPay is selected.'),
    '#default_value' => $settings['show_payment_instructions'],
  );
  $form['payment_instructions_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to display.'),
    '#default_value' => $settings['payment_instructions_text'],
  );
  $form['watchdog'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log full transaction information from the YellowPay response to the system log.'),
    '#default_value' => $settings['watchdog'],
  );

  return $form;
}

/**
 * Payment method callback: redirect form
 * The YellowPay payment method has been selected and the iFrame will be
 * displayed to proceed with a Bitcoin payment
 */
function commerce_yellowpay_redirect_form($form, &$form_state, $order, $payment_method) {
  if (!empty($payment_method['settings']['show_payment_instructions'])) {
    $instructions = t('Continue to the next step to complete your Bitcoin payment with YellowPay.');
    if (!empty($payment_method['settings']['payment_instructions_text'])) {
      $instructions = t('@text', array('@text' => $payment_method['settings']['payment_instructions_text']));
    }

    $form['yellowpay_information'] = array(
      '#markup' => '<span class="commerce-yellowpay-info">' . $instructions . '</span>',
    );
  }

  // Prepare a display settings array.
  $display = array(
    'label' => 'hidden',
    'type' => 'commerce_price_formatted_components',
    'settings' => array(
      'calculation' => FALSE,
    ),
  );

  // Render the order's order total field with the current display.
  $order_total = field_view_field('commerce_order', $order, 'commerce_order_total', $display);

  $form['order_total'] = array(
    '#type' => 'markup',
    '#markup' => '<div id="order_totals">' . render($order_total) . '</div>',
    '#attributes' => array('class' => array('commerce-order-handler-area-order-total')),
    '#attached' => array('css' => array(drupal_get_path('module', 'commerce_cart') . '/theme/commerce_cart.theme.css')),
  );

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $amount = $order_wrapper->commerce_order_total->amount->value();
  $currency_code = $order_wrapper->commerce_order_total->currency_code->value();

  $url = url(COMMERCE_YELLOWPAY_API_URL . 'invoice/');

  $nonce = round(microtime(TRUE) * 1000);
  $data = array(
    "base_price" => commerce_currency_amount_to_decimal($amount, $currency_code),
    "base_ccy" => $currency_code,
    "callback" => url(COMMERCE_YELLOWPAY_RETURN_URL, array('absolute' => TRUE)),
  );

  $body = json_encode($data);

  $headers = array(
    "Content-type" => 'application/json',
    "API-Key" => $payment_method['settings']['apikey'],
    "API-Nonce" => $nonce,
    "API-Sign" => hash_hmac("sha256", $nonce . $url . $body, $payment_method['settings']['apisecret'], FALSE),
  );

  $options = array(
    'headers' => $headers,
    'method' => 'POST',
    'data' => $body,
    'timeout' => 15,
  );

  $response = drupal_http_request($url, $options);

  // If we get a valid response with data.
  if ($response->code = 200 && $response->status_message == 'OK') {
    $response_data = json_decode($response->data);
    $form['yellowpay-invoice'] = array(
      '#markup' => '<iframe src="' . check_url($response_data->url) . '" style="width:393px; height:220px; overflow:hidden; border:none; margin:auto; display:block;"  scrolling="no" allowtransparency="true" frameborder="0"></iframe>',
    );

    $form['yellowpay-invoice']['#attached']['js'] = array(
      drupal_get_path('module', 'commerce_yellowpay') . '/yellowPay.js',
    );

    drupal_add_js(array(
      'yellowPay' => array(
        'return-url' => url(request_path() . '/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
      )), 'setting');

    $transaction = commerce_payment_transaction_new('yellowpay', $order->order_id);
    $transaction->instance_id = commerce_yellowpay_get_method_instance_id();
    $transaction->remote_id = $response_data->id;
    $transaction->amount = $amount;
    $transaction->currency_code = $currency_code;
    $transaction->remote_status = 'Pending';
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->payload[REQUEST_TIME] = $response_data;
    commerce_payment_transaction_save($transaction);
  }

  // Add a cancel link to return to the previous page.
  $form['actions']['cancel'] = array(
    '#markup' => l(t('Cancel'), request_path() . '/back/' . $order->data['payment_redirect_key'], array('attributes' => array('id' => 'cancel_yellowpay_payment'))),
    '#weight' => '1000',
  );
  // Added to avoid bootstrap themes throwing error on empty #type.
  $form['actions']['#type'] = '';

  return $form;
}
