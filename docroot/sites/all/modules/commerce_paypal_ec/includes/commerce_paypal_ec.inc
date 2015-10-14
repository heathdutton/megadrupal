<?php

/**
 * Payment method callback: settings form.
 */
function commerce_paypal_ec_settings_form($settings = NULL) {
  $form = array();
  // Merge default settings into the stored settings array.
  $default_currency = variable_get('commerce_default_currency', 'USD');
  // Options to autofill the PayPal address parameter
  $address_options = drupal_map_assoc(array('None', 'Billing', 'Shipping'));

  $settings = (array) $settings + array(
    'autofill_address' => 'Billing',
    'business' => '',
    'cart_items_list' => 0,
    'confirm_shipping' => 0,
    'currency_code' => in_array($default_currency, array_keys(commerce_paypal_ec_currencies())) ? $default_currency : 'USD',
    'ipn_logging' => 'notification',
    'landing_page' => 'Billing',
    'language' => 'US',
    'page_style' => 'paypal',
    'payment_action' => 'sale',
    'server' => 'sandbox',
    'solution_type' => 'Sole',
  );

  $form['api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal API Username'),
    '#description' => t('The PayPal API Username.'),
    '#default_value' => $settings['api_username'],
    '#required' => TRUE,
  );
  $form['api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal API Password'),
    '#description' => t('The PayPal API Password.'),
    '#default_value' => $settings['api_password'],
    '#required' => TRUE,
  );
  $form['api_signature'] = array(
    '#type' => 'textfield',
    '#title' => t('PayPal API Signature'),
    '#description' => t('The PayPal API Signature.'),
    '#default_value' => $settings['api_signature'],
    '#required' => TRUE,
  );
  $form['currency_code'] = array(
    '#type' => 'select',
    '#title' => t('Currency code'),
    '#description' => t('Transactions can only be processed in one of the listed currencies.'),
    '#options' => commerce_paypal_ec_currencies(),
    '#default_value' => $settings['currency_code'],
  );
  $form['language'] = array(
    '#type' => 'select',
    '#title' => t('PayPal login page language'),
    '#options' => commerce_paypal_ec_countries_locale(),
    '#default_value' => $settings['language'],
  );
  $form['payment_action'] = array(
    '#type' => 'radios',
    '#title' => t('Payment action'),
    '#options' => array(
      'sale' => t('Sale - authorize and capture the funds at the time the payment is processed'),
      'authorization' => t('Authorization - reserve funds on the card to be captured later through your PayPal account'),
    ),
    '#default_value' => $settings['payment_action'],
  );
  $form['solution_type'] = array(
    '#type' => 'radios',
    '#title' => t('Solution type'),
    '#options' => array(
      'Sole' => ('Sole - Buyer does not need to create a PayPal'),
      'Mark' => ('Mark - Buyer must have a PayPal account to check out)'),
    ),
    '#default_value' => $settings['solution_type'],
  );
  $form['landing_page'] = array(
    '#type' => 'radios',
    '#title' => t('Landing page'),
    '#options' => array(
      'Billing' => ('Put focus in the no PayPal account form (credit card) in the PayPal payment page.'),
      'Login' => ('Put focus in the PayPal Login form in the PayPal payment page.'),
    ),
    '#default_value' => $settings['landing_page'],
  );
  $form['autofill_address'] = array(
    '#type' => 'select',
    '#options' => $address_options,
    '#title' => t('Autofill the PayPal Payment form address with the order address. Note that if you use Shipping address you must have activated the commerce_shipping to work correctly. Billing address is by default captured so no additionally steps required.'),
    '#default_value' => $settings['autofill_address'],
  );
  $form['cart_items_list'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send the shopping cart item list to PayPal to display in the summary of PayPal payment page.'),
    '#default_value' => $settings['cart_items_list'],
  );
  $form['confirm_shipping'] = array(
    '#type' => 'checkbox',
    '#title' => t('Confirmed shipping address is mandatory for the PayPal payment.'),
    '#default_value' => $settings['confirm_shipping'],
  );
  $form['page_style'] = array(
    '#type' => 'textfield',
    '#title' => t('Name of your custom page style.'),
    '#description' => t('In you PayPal account you can create a custom page style to add a custom logo, header and background colors to the PayPal payment page, here you can define the name of your page style to give a customized experience to your clients.'),
    '#default_value' => $settings['page_style'],
    '#required' => TRUE,
  );
  $form['server'] = array(
    '#type' => 'radios',
    '#title' => t('PayPal server'),
    '#options' => array(
      'sandbox' => ('Sandbox - use for testing, requires a PayPal Sandbox account'),
      'live' => ('Live - use for processing real transactions'),
    ),
    '#default_value' => $settings['server'],
  );
  $form['ipn_logging'] = array(
    '#type' => 'radios',
    '#title' => t('IPN logging'),
    '#options' => array(
      'notification' => t('Log notifications during IPN validation and processing.'),
      'full_ipn' => t('Log notifications with the full IPN during validation and processing (used for debugging).'),
    ),
    '#default_value' => $settings['ipn_logging'],
  );

  return $form;
}

/**
 * Payment method callback: adds a message and CSS to the submission form.
 */
function commerce_paypal_ec_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form['paypal_ec_information'] = array(
    '#markup' => '<span class="commerce-paypal-ec-info">' . t('(Continue with checkout to complete payment via PayPal.)') . '</span>',
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'commerce_paypal_ec') . '/theme/commerce_paypal_ec.css'),
    ),
  );

  return $form;
}

/**
 * Payment method callback: redirect form, a wrapper around the module's general
 *   use function for building a EC form.
 */
function commerce_paypal_ec_redirect_form($form, &$form_state, $order, $payment_method) {
  // Return an error if the enabling action's settings haven't been configured.
  $api_params = array('api_username', 'api_password', 'api_signature');
  foreach ($api_params as $param_name) {
    if (empty($payment_method['settings'][$param_name])) {
      drupal_set_message(t('PayPal EC is not configured for use. No PayPal !param_name address has been specified.', array('!param_name' => $param_name)), 'error');
      return array();
    }
  }

  $settings = array(
    // Return to the previous page when payment is canceled
    'cancel_return' => url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    // Return to the payment redirect page for processing successful payments
    'return' => url('checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    'notify_url' => commerce_paypal_ipn_url($payment_method['instance_id']),
  );

  return commerce_paypal_ec_order_form($form, $form_state, $order, $payment_method['settings'] + $settings);
}

/**
 * Builds a PayPal Express Checkout form from an order object.
 *
 * @param $order
 *   The fully loaded order being paid for.
 * @param $settings
 *   An array of settings used to build out the form, including:
 *   - server: which server to use, either sandbox or live
 *   - business: the PayPal e-mail address the payment submits to
 *   - cancel_return: the URL PayPal should send the user to on cancellation
 *   - return: the URL PayPal should send the user to on successful payment
 *   - currency_code: the PayPal currency code to use for this payment if the
 *     total for the order is in a non-PayPal supported currency
 *   - language: the PayPal language code to use on the payment form
 *   - payment_action: the PayPal payment action to use: sale, authorization,
 *     or order
 *   - payment_method: optionally the name of the Commerce payment method to
 *     include in the IPN notify_url
 *
 * @return
 *   A renderable form array.
 */
function commerce_paypal_ec_order_form($form, &$form_state, $order, $settings) {
  global $language;
  $settings += array('payment_method' => '');
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $currency_code = _commerce_paypal_ec_lookup_currency_code($order, $settings);
  $order_total = $order_wrapper->commerce_order_total->value();

  // calculate the order total base price
  $order_total_base_price = commerce_price_component_total($order_total, 'base_price');
  $order_base_amount = $order_total_base_price['amount'];
  // order price including all price components like tax
  $order_amount = $order_wrapper->commerce_order_total->amount->value();

  // look which lang we should use in the PayPal page
  if ($settings['language'] == 'multilingual') {
    // in case of a multilingual site use the current used lang
    $current_lang = $language->language;
    $paypal_locale = commerce_paypal_ec_locale();
    // use the correct localization country code and if not found use US english
    $locale_code = isset($paypal_locale[$current_lang]) ? $paypal_locale[$current_lang] : 'en_US';
  }
  else {
    // no multilingual so use the configured lang
    $locale_code = $settings['language'];
  }

  $request_params = array(
    // Determines whether or not the PayPal pages should display the shipping address
    'ADDROVERRIDE' => 1,
    // Indicates you require the buyer’s shipping PayPal address be a confirmed address
    'REQCONFIRMSHIPPING' => $settings['confirm_shipping'],
    // Determines where or not PayPal displays shipping address fields on the PayPal pages
    'NOSHIPPING' => 1,
    // Sum of cost of all items in this order without shipping and tax.
    'PAYMENTREQUEST_0_ITEMAMT' => commerce_paypal_ec_format_amount($order_base_amount, $currency_code),
    // Total cost of the transaction to the buyer. If shipping cost and tax
    // charges are known, include them in this value
    'PAYMENTREQUEST_0_AMT' => commerce_paypal_ec_format_amount($order_amount, $currency_code),
    // Your own invoice or tracking number
    'PAYMENTREQUEST_0_INVNUM' => commerce_paypal_ipn_invoice($order),
    // URL to which the buyer’s browser is returned after pay
    'RETURNURL' => $settings['return'],
    // URL to which the buyer is returned if the buyer does not approve the payment
    'CANCELURL' => $settings['cancel_return'],
    'PAYMENTREQUEST_0_NOTIFYURL' => $settings['notify_url'],
    // The three-character currency code for the transaction
    'PAYMENTREQUEST_0_CURRENCYCODE' => $currency_code,
    // General description of the purchase
    'PAYMENTREQUEST_0_DESC' => t('Order @order_number at @store', array('@order_number' => $order->order_number, '@store' => variable_get('site_name', url('<front>', array('absolute' => TRUE))))),
    // How you want to obtain payment. (Authorization, Order, Sale)
    'PAYMENTREQUEST_0_PAYMENTACTION' => $settings['payment_action'],
    // Enables the buyer to enter a note to the merchant on the PayPal page during checkout.
    //'ALLOWNOTE' => 1,
    // Locale of pages displayed by PayPal during Express Checkout
    'LOCALECODE' => $locale_code,
    // Name of the Custom Payment Page Style for payment pages associated
    'PAGESTYLE' => $settings['page_style'],
    // Type of checkout flow. (Sole = Buyer does not need to create a PayPal
    // account ~ Mark = Buyer must have a PayPal account to check out)
    'SOLUTIONTYPE' => $settings['solution_type'],
    // Type of PayPal page to display. (Billing = Non-PayPal account ~ Login = PayPal account login)
    'LANDINGPAGE' => $settings['landing_page'],
    // A label that overrides the business name in the PayPal account
    'BRANDNAME' => variable_get('site_name', 'Undefined'),
    // Enables display of “Estimated Total” instead of “Total”
    'TOTALTYPE' => 'Estimated Total',
  );

  // process the address autofill setting
  $address_autofill_params = _commerce_paypal_ec_process_address_autofill($settings, $order_wrapper);
  // merge params to main params array
  $request_params = $request_params + $address_autofill_params;

  // if set, we add all the cart items to display in the PayPal Payement Page
  if ($settings['cart_items_list']) {
    $order_items = array();
    $item_no = 0;

    foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
      // If the line item is not a product then skip it, we don't want to include
      // taxes or coupons here
      if (!in_array($line_item_wrapper->type->value(), commerce_product_line_item_types())) {
        continue;
      }

      $title = $line_item_wrapper->commerce_product->title->value();
      // if no defined title then take the SKU
      if (empty($title)) {
        $title = $line_item_wrapper->line_item_label->value();
      }

      // calculate item total
      $item_price = $line_item_wrapper->commerce_unit_price->value();

      $new_item = array(
        // Item name
        'L_PAYMENTREQUEST_0_NAME' . $item_no => $title,
        // Cost of item.
        'L_PAYMENTREQUEST_0_AMT'  . $item_no => commerce_paypal_ec_format_amount($item_price['amount'], $item_price['currency_code']),
        // Item quantity.
        'L_PAYMENTREQUEST_0_QTY' . $item_no => (int) $line_item_wrapper->quantity->value(),
      );

      // calculate line item tax amount
      $line_item_tax_amount = 0;
      if (module_exists('commerce_tax')) {
        // calculate item tax amount
        $line_item_tax_amount = commerce_tax_total_amount($item_price['data']['components'], false, $item_price['currency_code']);
        // Tax amount
        $new_item['L_PAYMENTREQUEST_0_TAXAMT' . $item_no] = commerce_paypal_ec_format_amount($line_item_tax_amount, $currency_code);
      }

      $order_items = $order_items + $new_item;

      $item_no++;
    }

    // add order_items to the request_params
    $request_params = $request_params + $order_items;
  }

  // if commerce_shipping is enabled an order could have applied different
  // shipping services so look it and sum to have the shipping total
  $order_shipping_amount = 0;
  if (module_exists('commerce_shipping')) {
    $shipping_price_component_types = array_keys(commerce_shipping_commerce_price_component_type_info());
    // sum all the order total shipping price components
    foreach ($shipping_price_component_types as $shipping_price_component_type) {
      $price_component_type_total = commerce_price_component_total($order_total, $shipping_price_component_type);
      $order_shipping_amount += $price_component_type_total['amount'];
    }
    // Total shipping costs for this order.
    $request_params['PAYMENTREQUEST_0_SHIPPINGAMT'] = commerce_paypal_ec_format_amount($order_shipping_amount, $currency_code);
  }

  // if enabled, display the tax amount in the PayPal Payment Page
  if (module_exists('commerce_tax')) {
    $order_tax_amount = commerce_tax_total_amount($order_total['data']['components'], false, $currency_code);
    // Tax amount
    $request_params['PAYMENTREQUEST_0_TAXAMT'] = commerce_paypal_ec_format_amount($order_tax_amount, $currency_code);
  }

  $response_data = commerce_paypal_ec_api_call("SetExpressCheckout", $request_params, $settings);

  // handle error response
  if (isset($response_data['ACK']) && $response_data['ACK'] != 'Success') {
    drupal_set_message(t('A problem was found connecting to PayPal. Please try again later.'), 'error');
    watchdog('commerce_paypal_ec', 'The PayPal EC API call failed. !response_data', array('!response_data' => print_r($response_data, TRUE)), WATCHDOG_ERROR);
    return array();
  }

  // handle no token response
  if (empty($response_data['TOKEN'])) {
    drupal_set_message(t('A problem was found getting a Token session at PayPal. Please try again later.'), 'error');
    watchdog('commerce_paypal_ec', 'The PayPal EC API call not provided a Token. !response_data', array('!response_data' => print_r($response_data, TRUE)), WATCHDOG_ERROR);
    return array();
  }

  if (!empty($response_data['TOKEN'])) {
    $form['#action'] = commerce_paypal_ec_server_url($settings['server']) . $response_data['TOKEN'];
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Proceed to PayPal'),
  );

  return $form;
}

/**
 * Helper function that process the address autofill settings. Determine which address
 * (billing, shipping or none) should be passed to the PayPal payment page address fiels.
 *
 * @param array $settings
 *   The payment settings
 * @param object $order_wrapper
 *   A wrapped order entity
 * @return $shipping_address_params
 *   An array with the extra params that should be passed to PayPal EC request.
 */
function _commerce_paypal_ec_process_address_autofill($settings, $order_wrapper) {
  // Select the address we should pass to PayPal request
  if ($settings['autofill_address'] == 'Shipping') {
    if(module_exists('commerce_shipping')) {
      $order_address = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
    }
    else {
      // commerce_shipping is required to get the commerce_customer_shipping, if no exists then log
      // the problem. The address won't be passed.
      watchdog('commerce_paypal_ec', 'The autofill address is configured to use Shipping address but commerce_shipping module is not installed. The address will be empty.', array(), WATCHDOG_DEBUG);
    }
  }
  else if ($settings['autofill_address'] == 'Billing') {
    $order_address = $order_wrapper->commerce_customer_billing->commerce_customer_address->value();
  }

  // select the ship to name
  if (!empty($order_address['name_line'])) {
    $order_shiptoname = $order_address['name_line'];
  }
  else if (!empty($order_address['first_name']) && !empty($order_address['last_name'])) {
    $order_shiptoname = $order_address['first_name'] . ' ' . $order_address['last_name'];
  }
  else {
    $order_shiptoname = t('Undefined');
  }

  // if set, add the shipping address to the request
  if (isset($order_address)) {
    $address_autofill_params = array(
      'PAYMENTREQUEST_0_SHIPTONAME' => $order_shiptoname,
      'PAYMENTREQUEST_0_SHIPTOSTREET' => $order_address['thoroughfare'],
      'PAYMENTREQUEST_0_SHIPTOSTREET2' => $order_address['premise'],
      'PAYMENTREQUEST_0_SHIPTOCITY' => $order_address['locality'],
      // If PayPal don't receive the administrative_area then don't
      // set correctly the country to the value we are passing so
      // in case of no administrative_area we set the same as locality
      'PAYMENTREQUEST_0_SHIPTOSTATE' => !empty($order_address['administrative_area']) ? $order_address['administrative_area'] : $order_address['locality'],
      'PAYMENTREQUEST_0_SHIPTOCOUNTRYCODE' => $order_address['country'],
      'PAYMENTREQUEST_0_SHIPTOZIP' => $order_address['postal_code'],
      'PAYMENTREQUEST_0_SHIPTOPHONENUM' => '',
      'EMAIL' => $order_wrapper->mail->value(),
    );
  }

  return isset($address_autofill_params) ? $address_autofill_params : array();
}

/**
 * Payment method callback: redirect form submit
 */
function commerce_paypal_ec_redirect_form_submit($order, $payment_method) {
  if (!$_GET['token'] || !$_GET['PayerID']) {
    drupal_set_message(t('No token or PayerID argument was received from PayPal return URL.', 'error'));
    return;
  }

  $token = $_GET['token'];
  $payer_id = $_GET['PayerID'];
  $request_params = array(
    'TOKEN' => $token,
  );

  // call GetExpressCheckoutDetails to review and execute the payment
  $settings = $payment_method['settings'];
  $response = commerce_paypal_ec_api_call("GetExpressCheckoutDetails", $request_params, $settings);

  // Exit when we don't get a sucess request
  if (isset($response['ACK']) && $response['ACK'] != 'Success') {
    drupal_set_message(t('The Payment at PayPal could not be successful. Try again later.'), 'error');
    commerce_payment_redirect_pane_previous_page($order);
    watchdog('commerce_paypal_ec', 'The PayPal EC API call failed in GetExpressCheckoutDetails. !response_data', array('!response_data' => print_r($response, TRUE)), WATCHDOG_ERROR);
    return;
  }

  // Do the payment
  $request_params = array(
    'TOKEN' => $response['TOKEN'],
    'PAYERID' => $response['PAYERID'],
    'PAYMENTREQUEST_0_AMT' => $response['PAYMENTREQUEST_0_AMT'],
    'PAYMENTREQUEST_0_PAYMENTACTION' => $settings['payment_action'],
    'PAYMENTREQUEST_0_CURRENCYCODE' => $response['PAYMENTREQUEST_0_CURRENCYCODE'],
  );

  // Make the call to PayPal to finalize payment
  $do_response = commerce_paypal_ec_api_call("DoExpressCheckoutPayment", $request_params, $settings);

  // Exit when we don't get a sucess request
  if (isset($do_response['ACK']) && $do_response['ACK'] != 'Success') {
    drupal_set_message(t('The Payment at PayPal could not be successful. Try again later.'), 'error');
    commerce_payment_redirect_pane_previous_page($order);
    $debug_data = array('!request_data' => print_r($request_params, TRUE), '!response_data' => print_r($do_response, TRUE));
    watchdog('commerce_paypal_ec', 'The PayPal EC API call failed in DoExpressCheckoutPayment. !request_data \n\n !response_data', $debug_data, WATCHDOG_ERROR);
    return;
  }

  // add order_id to the response array
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $do_response['order_id'] = $order_wrapper->order_id->value();

  // convert response to IPN format and store in paypal_ipn
  $ipn = commerce_paypal_ec_response_to_ipn($do_response);

  if (!empty($ipn)) {
    // Process the response. Instead of use the IPN request we use the DoExpressCheckoutPayment
    // response and we can have the Payment info instantly
    commerce_paypal_ec_paypal_ipn_process($order, $payment_method, $ipn);
    // save the IPN after commerce_paypal_ec_paypal_ipn_process to have the transaction_id
    commerce_paypal_ipn_save($ipn);
  }
}
