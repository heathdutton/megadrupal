<?php

/**
 * @file
 * Payment method callbacks.
 */

/**
 * Payment method callback: settings form.
 *
 * Settings form for Payment Method
 */
function commerce_dibs_payment_window_settings_form($settings = NULL) {
  $form = array();

  $settings = array_replace_recursive(_commerce_dibs_payment_window_get_settings_default(), $settings);

  $form['commerce_dibs_payment_window_merchant_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Merchant ID provided by %dibs', array('%dibs' => 'DIBS')),
    '#description' => t('A (usually) numeric ID provided by %dibs when you signed up with the Internet Services.', array('%dibs' => 'DIBS')),
    '#maxlength' => '128',
    '#size' => '32',
    '#required' => TRUE,
    '#default_value' => $settings['commerce_dibs_payment_window_merchant_id'],
  );

  // Payment Window.
  $form['commerce_dibs_payment_window'] = array(
    '#type' => 'fieldset',
    '#title' => t('Payment Window Settings'),
    '#description' => t('Settings for the Payment Window the customer gets sent to.'),
  );

  $form['commerce_dibs_payment_window']['commerce_dibs_payment_window_language'] = array(
    '#type' => 'select',
    '#title' => t('Language', array()),
    '#description' => t('Specify what language the customer should see on the %dpw. Note: %dibs only supports some languages.', array('%dpw' => 'DIBS Payment Window', '%dibs' => 'DIBS')),
    '#size' => '1',
    '#required' => TRUE,
    '#default_value' => $settings['commerce_dibs_payment_window']['commerce_dibs_payment_window_language'],
    '#options' => _commerce_dibs_payment_window_get_payment_window_languages(),
  );

  $form['commerce_dibs_payment_window']['commerce_dibs_payment_window_payment_window'] = array(
    '#type' => 'select',
    '#title' => t('Paymen window'),
    '#description' => t('Select which of the supported DIBS payment windows you want customers to be sent to.'),
    '#size' => '1',
    '#required' => TRUE,
    '#default_value' => $settings['commerce_dibs_payment_window']['commerce_dibs_payment_window_payment_window'],
    '#options' => _commerce_dibs_payment_window_get_payment_windows(),
  );

  $form['commerce_dibs_payment_window']['commerce_dibs_payment_window_payment_window_info'] = array(
    '#markup' => t('<strong>NOTE:</strong> Mobile Payment Window does not allow for Instant Capture. System will not try to capture if it is selected.'),
  );

  // API and security.
  $form['commerce_dibs_payment_window_api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API and security'),
    '#description' => t('Settings related to API access and security while communicating with DIBS.'),
  );

  $form['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_username'] = array(
    '#type' => 'textfield',
    '#title' => t('API username'),
    '#description' => t('API user that should perform the API calls to DIBS.'),
    '#maxlength' => '128',
    '#size' => '32',
    '#required' => TRUE,
    '#default_value' => $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_username'],
  );

  $form['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_password'] = array(
    '#type' => 'textfield',
    '#title' => t('API password'),
    '#description' => t('Password for the API user that should perform the API calls to DIBS.'),
    '#maxlength' => '128',
    '#size' => '32',
    '#required' => TRUE,
    '#default_value' => $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_password'],
  );

  $form['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac'] = array(
    '#type' => 'textfield',
    '#title' => t('HMAC key'),
    '#description' => t('Optional. If set, we will require operation to send and recieve (if they can) a HMAC key to prevent tampering of data.'),
    '#maxlength' => '128',
    '#size' => '128',
    '#default_value' => $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac'],
  );

  // Orders.
  $form['commerce_dibs_payment_window_orders'] = array(
    '#type' => 'fieldset',
    '#title' => t('Orders'),
  );

  $form['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_instant_capture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Instant capture'),
    '#description' => t("Do capture at the moment of payment registration. Only to be used when selling non-shippable products."),
    '#default_value' => $settings['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_instant_capture'],
  );

  $form['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_test_mode'] = array(
    '#type' => 'checkbox',
    '#title' => t('Test mode'),
    '#description' => t("Puts the system in test mode, and tells DIBS that transactions are tests. DO NOT USE LIGHTLY."),
    '#default_value' => $settings['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_test_mode'],
  );

  $form['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_paytypes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Payment types'),
    '#description' => t('Select what payment types you want the customer to have the ability to choose from at the DIBS payment window.'),
    '#default_value' => $settings['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_paytypes'],
    '#options' => _commerce_dibs_payment_window_get_paytypes(),
  );

  return $form;
}

/**
 * Payment method callback: submit form.
 *
 * Shown to user upon selecting this payment method.
 */
function commerce_dibs_payment_window_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  // Merge settings with defaults, this is needed in case no config has been
  // done.
  $settings = array_replace_recursive(_commerce_dibs_payment_window_get_settings_default(), $payment_method['settings']);

  // Merge in values from the order.
  if (!empty($order->data['commerce_dibs_payment_window'])) {
    $pane_values += $order->data['commerce_dibs_payment_window'];
  }

  // Issue: http://drupal.org/node/1705862
  // Fixed in aug 2012, we bypass it.
  $form['fix_undefined_index'] = array(
    '#type' => 'hidden',
    '#value' => 'fix_undefined_index',
  );

  return $form;
}

/**
 * Payment method callback: submit form submit.
 */
function commerce_dibs_payment_window_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  // Create our own array on the order.
  if (!isset($order->data['commerce_dibs_payment_window'])) {
    $order->data['commerce_dibs_payment_window'] = array();
  }

  // Merge selected values with the order.
  $order->data['commerce_dibs_payment_window'] += is_array($pane_values) ? $pane_values : array();
}

/**
 * Payment method callback; generation callback for the payment redirect form.
 */
function commerce_dibs_payment_window_redirect_form($form, &$form_state, $order, $payment_method) {
  try {
    // Merge settings with defaults.
    $settings = array_replace_recursive(_commerce_dibs_payment_window_get_settings_default(), $payment_method['settings']);

    if (empty($settings['commerce_dibs_payment_window_merchant_id']) ||
      empty($settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_username']) ||
      empty($settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_password'])) {
      throw new Exception('Missing configuration');
    }

    $charge = commerce_payment_order_balance($order);
    $transaction = commerce_payment_transaction_new($payment_method['method_id'], $order->order_id);
    $transaction->instance_id = $payment_method['instance_id'];
    $transaction->amount = $charge['amount'];
    $transaction->currency_code = $charge['currency_code'];
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_STATUS_UNDEFINED;
    commerce_payment_transaction_save($transaction);

    $order->data['commerce_dibs_payment_window']['current_transaction'] = $transaction->transaction_id;
    commerce_order_save($order);

    $data = _commerce_dibs_payment_window_create_data_for_window($order, $transaction, $settings);

    foreach ($data as $key => $value) {
      $form[$key] = array(
        '#type' => 'hidden',
        '#value' => $value,
      );
    }

    if (!($url = _commerce_dibs_payment_window_get_dibs_url($settings['commerce_dibs_payment_window']['commerce_dibs_payment_window_payment_window']))) {
      throw new Exception('Missing url');
    }

    // Add a submit button.
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Proceed to DIBS Payment Window'),
    );

    $form['#action'] = $url;
  }
  catch (Exception $e) {
    // If there is a transaction already - add failure status.
    if (!empty($transaction)) {
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->message = $e->getMessage();
      commerce_payment_transaction_save($transaction);
    }
    drupal_set_message(t('There is a problem with the DIBS Payment Window, please consult the site administrator.'), 'error');
    watchdog_exception('commerce_dibs_payment_window', $e);
    $form['#action'] = url('checkout/' . $order->order_id . '/payment/back/' . $order->data['payment_redirect_key']);
  }
  return $form;
}

/**
 * Payment method callback; validation callback for redirected payments.
 *
 * Called when user returns from DIBS.
 *
 * We have no guarantee from DIBS that this one is called. So we check for
 * correct status and orderId. DIBS is ordered to provide us another callback
 * that will process the acceptance of the order.
 *
 * TRUE: Checkout complete, FALSE: go back one step.
 *
 * @return bool
 *   TRUE if the payment was successful, FALSE otherwise.
 */
function commerce_dibs_payment_window_redirect_form_validate($order, $payment_method) {
  // Load response to variable.
  $response = $_POST;

  // Save data to watchdog.
  watchdog('commerce_dibs_payment_window', 'Customer returned from DIBS. <pre>Response: %pp</pre>', array('%pp' => print_r($response, TRUE)));

  try {
    if (empty($response['s_localTransactionID'])) {
      throw new Exception('Missing transaction ID.');
    }

    if (!($transaction = commerce_payment_transaction_load(intval($response['s_localTransactionID'])))) {
      throw new Exception('Unable to load transaction for the order.');
    }

    // Skip next steps if transaction has been processed by callback already.
    if (isset($response['captureStatus']) && $transaction->remote_status === COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_CAPTURED && $transaction->status != COMMERCE_DIBS_PAYMENT_WINDOW_STATUS_UNDEFINED) {
      return TRUE;
    }
    elseif (!isset($response['captureStatus']) && $transaction->remote_status === COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_AUTHORIZED && $transaction->status != COMMERCE_DIBS_PAYMENT_WINDOW_STATUS_UNDEFINED) {
      return TRUE;
    }

    // Load settings with default settings fallback.
    $settings = array_replace_recursive(_commerce_dibs_payment_window_get_settings_default(), $payment_method['settings']);

    // Check for MAC from DIBS.
    if (isset($response['MAC']) && _commerce_dibs_payment_window_should_add_mac_to_data($settings)) {
      $provided_mac = $response['MAC'];

      // Remove mac from response.
      unset($response['MAC']);
      $local_mac = _commerce_dibs_payment_window_create_hmac_from_data($response, $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac']);

      if ($local_mac !== $provided_mac) {
        throw new Exception(format_string(
          'Local MAC and provided MAC does not match for order: %order_id and transaction: %transaction_id. This might be a sign of tampering! ',
          array(
            '%order_id' => $order->order_id,
            '%transaction_id' => $transaction->transaction_id,
          )
        ));
      }
    }

    // Check in response his successful.
    $checks = array(
      's_localTransactionID' => $transaction->transaction_id,
      'orderId' => $order->order_id,
      'merchant' => $settings['commerce_dibs_payment_window_merchant_id'],
      'transaction' => NULL,
      'amount' => $transaction->amount,
    );
    _commerce_dibs_payment_window_validate_response($response, $checks);

    // Default statuses.
    $transaction->remote_id = check_plain($response['transaction']);
    $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_AUTHORIZED;
    $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;
    $transaction->log = $transaction->message = 'Transaction authorized';
    $transaction->revision = TRUE;

    // Check for instant capture. ERROR or DECLINED should throw an Exception.
    if (isset($response['captureStatus']) && in_array($response['captureStatus'], array('ERROR', 'DECLINED'))) {
      // Set error to screen.
      drupal_set_message(t('Transaction could not be finalized due to credit card capture error. Try once again or use another card.'), 'error');
      // Throw exception with proper message.
      throw new Exception(format_string(
        'DIBS Callback, failed to capture funds for order: %order_id and transaction: %transaction_id. Please consult your DIBS administration.',
        array(
          '%order_id' => $order->order_id,
          '%transaction_id' => $transaction->transaction_id,
        )
      ));
    }
    elseif (isset($response['captureStatus']) && $response['captureStatus'] == 'ACCEPTED') {
      $transaction->remote_status = COMMERCE_DIBS_PAYMENT_WINDOW_REMOTE_STATUS_CAPTURED;
      $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
      $transaction->message = 'Captured';
    }

    // Save response as a payload.
    $transaction->data['last_payload'] = $transaction->payload = $response;

    commerce_payment_transaction_save($transaction);
    return TRUE;
  }
  catch (Exception $e) {

    if (isset($transaction)) {
      $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
      $transaction->revision = TRUE;
      $transaction->log = $transaction->message = $e->getMessage();
      $transaction->data['last_payload'] = $transaction->payload = $_POST;
      commerce_payment_transaction_save($transaction);
    }

    watchdog_exception('commerce_dibs_payment_window', $e);
    return FALSE;
  }
}

/**
 * Gets settings defaults for a DIBS payment method.
 *
 * @return array
 *   Default settings for DIBS Payment Window.
 */
function _commerce_dibs_payment_window_get_settings_default() {

  // We don't support all the languages.
  $dibs_languages = _commerce_dibs_payment_window_get_supported_languages();

  $site_default_language = language_default('language');
  if (!in_array($site_default_language, array_keys($dibs_languages))) {
    $site_default_language = 'en';
  }

  $defaults = array(
    'commerce_dibs_payment_window_merchant_id' => '',
    'commerce_dibs_payment_window' => array(
      'commerce_dibs_payment_window_language' => $site_default_language,
      'commerce_dibs_payment_window_payment_window' => 'normal',
    ),
    'commerce_dibs_payment_window_api' => array(
      'commerce_dibs_payment_window_api_username' => '',
      'commerce_dibs_payment_window_api_password' => '',
      'commerce_dibs_payment_window_api_hmac' => '',
    ),
    'commerce_dibs_payment_window_orders' => array(
      'commerce_dibs_payment_window_orders_instant_capture' => FALSE,
      'commerce_dibs_payment_window_orders_test_mode' => TRUE,
      'commerce_dibs_payment_window_orders_paytypes' => array(),
    ),
  );

  return $defaults;
}

/**
 * Gets a list of the currently supported languages for the DIBS Payment Window.
 *
 * DIBS only supports a subset of all the languages Drupal is available in. We
 * put them here and give that to form elements that need them.
 *
 * @return array
 *   Array containing supported languages.
 */
function _commerce_dibs_payment_window_get_supported_languages() {
  $supported_languages = array(
    'en' => 'English (US)',
    'en-gb' => 'English (GB)',
    'da' => 'Danish',
    'sv' => 'Swedish',
    'nb' => 'Norwegian (Bokmål)',
  );

  return $supported_languages;
}

/**
 * Gets languages with Drupal titles for the payment settings.
 *
 * Since there is a difference in what languages a Drupal install can support
 * and what DIBS supports we need to cross check and then map titles from
 * drupal onto our supported languages.
 *
 * @return array
 *   Languages with correct titles.
 */
function _commerce_dibs_payment_window_get_payment_window_languages() {
  $supported_languages = _commerce_dibs_payment_window_get_supported_languages();
  $drupal_languages = language_list('language');

  $languages = array();

  foreach ($supported_languages as $code => $title) {
    $languages[$code] = isset($drupal_languages[$code]) ? $drupal_languages[$code]->name : $title;
  }

  return $languages;
}

/**
 * Gets currently supported payment windows.
 *
 * @return array
 *   List of supported Payment Windows
 */
function _commerce_dibs_payment_window_get_payment_windows() {
  $windows = array(
    'normal' => 'Payment Window',
    'mobile' => 'Mobile Payment Window',
  );

  return $windows;
}

/**
 * Gets supported paytypes for DIBS.
 *
 * @return array
 *   List of supported Paytypes.
 */
function _commerce_dibs_payment_window_get_paytypes() {
  $paytypes = array(
    'AMEX' => 'American Express',
    'DIN' => 'Diners Club',
    'DK' => 'Dankort',
    'ELEC' => 'VISA Electron',
    'FFK' => 'Forbrugsforeningen',
    'JCB' => 'JCB',
    'MC' => 'MasterCard',
    'MTRO' => 'Maestro',
    'V-DK' => 'VISA-Dankort',
    'VISA' => 'VISA',
  );

  return $paytypes;
}

/**
 * Create data needed to ship an order to the payment window.
 *
 * @param stdClass $order
 *   Order object to create data for.
 * @param array $settings
 *   Payment method of the order.
 * @param object $transaction
 *   Transaction to create data for.
 *
 * @return array
 *   An array with data to ship to DIBS.
 */
function _commerce_dibs_payment_window_create_data_for_window($order, $transaction, $settings, $textreply = FALSE) {

  $data = _commerce_dibs_payment_window_create_base_data_for_order($order, $transaction, $settings, $textreply);

  $cancel_url = sprintf('checkout/%s/payment/back/%s', $order->order_id, $order->data['payment_redirect_key']);
  $accept_url = sprintf('checkout/%s/payment/return/%s', $order->order_id, $order->data['payment_redirect_key']);
  $callback_url = sprintf('commerce-dibs-payment-window-callback/%s/%s/%s', $order->order_id, $transaction->transaction_id, $order->data['payment_redirect_key']);

  $data += array(
    'acceptReturnUrl' => url($accept_url, array('absolute' => TRUE)),
    'cancelReturnUrl' => url($cancel_url, array('absolute' => TRUE)),
    'callbackUrl' => url($callback_url, array('absolute' => TRUE)),
    'payType' => _commerce_dibs_payment_window_get_enabled_paytypes_formatted($settings),
    'language' => $settings['commerce_dibs_payment_window']['commerce_dibs_payment_window_language'],
  );

  if (_commerce_dibs_payment_window_payment_method_should_do_instant_capture($settings)) {
    $data['captureNow'] = '1';
  }

  if (_commerce_dibs_payment_window_payment_method_is_in_test_mode($settings)) {
    $data['test'] = '1';
  }

  if (_commerce_dibs_payment_window_should_add_mac_to_data($settings)) {
    $data['MAC'] = _commerce_dibs_payment_window_create_hmac_from_data($data, $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac']);
  }

  return $data;
}

/**
 * Checks if a payment method is in test mode.
 *
 * @param array $settings
 *   Payment method settings to check.
 *
 * @return bool
 *   TRUE if in test mode, FALSE otherwise.
 */
function _commerce_dibs_payment_window_payment_method_is_in_test_mode($settings) {
  return $settings['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_test_mode'] == 1;
}

/**
 * Gets a formatted list of Paytypes for DIBS.
 *
 * @param array $settings
 *   Payment method settings we should get paytypes for.
 *
 * @return string
 *   A string suited to send to DIBS (VISA;ELEC;MC).
 */
function _commerce_dibs_payment_window_get_enabled_paytypes_formatted($settings) {
  $enabled_paytypes = _commerce_dibs_payment_window_get_enabled_paytypes($settings);
  return implode(',', $enabled_paytypes);
}

/**
 * Get the enabled Paytypes for DIBS.
 *
 * @param array $settings
 *   Payment method settings we should get paytypes for.
 *
 * @return array
 *   List of enabled paytypes.
 */
function _commerce_dibs_payment_window_get_enabled_paytypes($settings) {
  $return = array();

  foreach ($settings['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_paytypes'] as $key => $value) {
    if ($value !== 0) {
      array_push($return, $key);
    }
  }

  return $return;
}

/**
 * Check if we should (and can) do Instant Capture.
 *
 * @param array $settings
 *   Payment method settings we should check.
 *
 * @return bool
 *   TRUE if we should (and can) do Instant Capture, FALSE otherwise.
 */
function _commerce_dibs_payment_window_payment_method_should_do_instant_capture($settings) {
  return _commerce_dibs_payment_window_payment_method_can_do_instant_capture($settings) &&
  $settings['commerce_dibs_payment_window_orders']['commerce_dibs_payment_window_orders_instant_capture'];
}

/**
 * Checks if a payment method can do Instant Capture.
 *
 * @param array $settings
 *   The payment method settings to check.
 *
 * @return bool
 *   TRUE if we should do Instant Capture, FALSE otherwise.
 */
function _commerce_dibs_payment_window_payment_method_can_do_instant_capture($settings) {
  return $settings['commerce_dibs_payment_window']['commerce_dibs_payment_window_payment_window'] == 'normal';
}

/**
 * Does API call, performs the parsed response as an array, or null on failure.
 *
 * @param array $data
 *   Data to send with the API call.
 * @param string $apicall
 *   Which API call to perform.
 * @param string $user
 *   API username to use.
 * @param string $pass
 *   API password to use.
 *
 * @return array
 *   Parsed response from the API call, or NULL on failure.
 */
function _commerce_dibs_payment_window_perform_api_call($data, $apicall = NULL, $user = '', $pass = '') {

  // Safe guard.
  if (!$apicall) {
    throw new Exception('DIBS Payment Window, API call must be specified.');
  }

  // Safe guard.
  if (!is_array($data)) {
    throw new Exception('DIBS Payment Window, $data needs to be array.');
  }

  if (empty($user) || empty($pass)) {
    throw new Exception('DIBS Payment Window, API user and password is required!');
  }

  $url = _commerce_dibs_payment_window_get_dibs_url($apicall);
  if (substr_count($url, '%s')) {
    // It's required with username and pass for this call.
    $url = sprintf($url, $user, $pass);
  }

  $response = _commerce_dibs_payment_window_perform_post($url, $data);

  $parsed_response = array();
  parse_str($response, $parsed_response);

  if (empty($parsed_response)) {
    watchdog('commerce_dibs_payment_window', 'DIBS returned a bad response.', array(), WATCHDOG_ERROR);
    return NULL;
  }

  return $parsed_response;
}


/**
 * Gets a list of URLs to various DIBS services.
 *
 * @return array
 *   List of URLS to various DIBS services.
 */
function _commerce_dibs_payment_window_get_dibs_urls() {
  $urls = array(
    'normal' => 'https://sat1.dibspayment.com/dibspaymentwindow/entrypoint',
    'mobile' => 'https://mopay.dibspayment.com',

    // API URLs.
    'capture' => 'https://payment.architrade.com/cgi-bin/capture.cgi',
    'refund' => 'https://%s:%s@payment.architrade.com/cgi-adm/refund.cgi',
    'cancel' => 'https://%s:%s@payment.architrade.com/cgi-adm/cancel.cgi',
  );

  return $urls;
}

/**
 * Gets the URL to a DIBS service.
 *
 * @param string $key
 *   Type of service we wish to get the URL for.
 *
 * @return string
 *   URL of the service.
 */
function _commerce_dibs_payment_window_get_dibs_url($key) {
  $urls = _commerce_dibs_payment_window_get_dibs_urls();

  return $urls[$key];
}

/**
 * Creates data needed for an API call to DIBS.
 *
 * @param object $order
 *   Order object.
 * @param object $transaction
 *   Transaction object.
 * @param array $settings
 *   Settings array.
 *
 * @return array
 *   Request parameters.
 */
function _commerce_dibs_payment_window_create_data_for_api($order, $transaction, $settings) {
  $data = _commerce_dibs_payment_window_create_base_data_for_order($order, $transaction, $settings, TRUE);

  $data['transact'] = $transaction->remote_id;

  if (_commerce_dibs_payment_window_should_add_mac_to_data($settings)) {
    $data['MAC'] = _commerce_dibs_payment_window_create_hmac_from_data($data, $settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac']);
  }

  return $data;
}

/**
 * Create a base data array for DIBS.
 *
 * @param object $order
 *   Order object to create data for.
 * @param object $transaction
 *   Transaction to create data for.
 * @param array $settings
 *   Payment method settings of the order.
 *
 * @return array
 *   An array with data to ship to DIBS.
 */
function _commerce_dibs_payment_window_create_base_data_for_order($order, $transaction, $settings, $textreply = FALSE) {

  $data = _commerce_dibs_payment_window_create_bare_data($settings, $textreply);

  $data += array(
    'orderId' => $order->order_id,
    'currency' => $transaction->currency_code,
    'amount' => $transaction->amount,
    's_localTransactionID' => $transaction->transaction_id,
  );

  return $data;
}

/**
 * Creates the bare minimum data for DIBS calls.
 */
function _commerce_dibs_payment_window_create_bare_data($settings = NULL, $textreply = FALSE) {
  if (!$settings) {
    throw new Exception('DIBS Payment Window, Failure in data passed to create_base_data_from_order, $settings is required.');
  }

  if (empty($settings['commerce_dibs_payment_window_merchant_id'])) {
    throw new Exception('Missing merchant ID.');
  }

  $data = array(
    'merchant' => $settings['commerce_dibs_payment_window_merchant_id'],
  );

  if ($textreply) {
    $data['textreply'] = 'yes';
  }

  return $data;
}

/**
 * Creates a HMAC SHA256 string from an array of data.
 *
 * @param array $data
 *   Values to create HMAC for.
 *
 * @return string
 *   HMAC SHA256 string.
 */
function _commerce_dibs_payment_window_create_hmac_from_data($data, $key = '') {
  $k = _commerce_dibs_payment_window_hextostr($key);

  $string = '';
  ksort($data);

  foreach ($data as $key => $value) {
    if (strlen($string) > 1) {
      $string .= "&";
    }
    $string .= $key . "=" . $value;
  }

  return hash_hmac("sha256", $string, $k);
}

/**
 * Formats an hex value to string.
 *
 * Note: This was copied from DIBS.
 *
 * @param mixed $hex
 *   Value to format.
 *
 * @return string
 *   Formatted value.
 */
function _commerce_dibs_payment_window_hextostr($hex) {
  $string = '';

  foreach (explode("\n", trim(chunk_split($hex, 2))) as $h) {
    $string .= chr(hexdec($h));
  }

  return $string;
}

/**
 * Checks if a response array contains what we can consider success from DIBS.
 *
 * @param array $response
 *   The response array from DIBS.
 * @param array $additional_checks
 *   Additional checks to perform.
 *
 * @throws \Exception
 */
function _commerce_dibs_payment_window_validate_response(&$response = NULL, $additional_checks = array()) {

  // If response is empty.
  if (empty($response)) {
    throw new Exception('Missing response');
  }

  // For refund we don't get this, so we fake it.
  if (!isset($response['status']) && $response['result'] == 0) {
    $response['status'] = 'ACCEPTED';
  }
  elseif ($response['status'] === 'ACCEPTED' && !isset($response['result'])) {
    $response['result'] = 0;
  }

  // If result doesn't match quit with error code and message.
  if ($response['result'] != 0) {
    throw new Exception(_commerce_dibs_payment_window_human_readable_error_message($response['result']), $response['result']);
  }

  if ($response['status'] !== 'ACCEPTED') {
    throw new Exception('Incorrect response status.');
  }

  // If no additional checks just return TRUE.
  if (empty($additional_checks)) {
    return;
  }

  // Perform additional checks.
  foreach ($additional_checks as $key => $value) {

    // Check if key exists.
    if (!array_key_exists($key, $response)) {
      throw new Exception(format_string(
        'Key @key does not exist.',
        array(
          '@key' => $key,
        )
      ));
    }

    // Value is not empty and response matches value.
    if (!empty($value) && $response[$key] !== $value) {
      throw new Exception(format_string(
        'Value @value for the key @key is incorrect.',
        array(
          '@value' => $value,
          '@key' => $key,
        )
      ));
    }
  }
}

/**
 * Returns an array of values (excluding status) we expect from a DIBS response.
 *
 * @param string $context
 *   The context we need to know expected values from.
 * @param string $status
 *   Status of the response.
 *
 * @return array
 *   Values we expect and can check if exists.
 */
function _commerce_dibs_payment_window_expected_values($context, $status = 'DECLINED') {
  $expected = array(
    'capture' => array(
      'ACCEPTED' => array('result', 'transact'),
      'DECLINED' => array('result', 'transact'),
    ),
    'cancel' => array(
      'ACCEPTED' => array('transact', 'cardtype'),
      'DECLINED' => array('reason'),
    ),
    'refund' => array(
      'ACCEPTED' => array(),
    ),
    'callback' => array(
      'ACCEPTED' => array(),
      'DECLINED' => array(),
    ),
  );

  return isset($expected[$context][$status]) ? $expected[$context][$status] : array();
}

/**
 * Checks if the parsed response has the expected values returned.
 *
 * @param array $response
 *   Parsed response.
 * @param array $expected
 *   Expected values. NULL by default.
 *
 * @throws \Exception
 */
function _commerce_dibs_payment_window_response_has_correct_values($response, $expected = NULL) {

  // Safe guard in case we get null passed in.
  if (!is_array($expected)) {
    throw new Exception('Response is not an array.');
  }

  // Check all keys and compare with expected values array.
  foreach ($expected as $key) {
    if (!array_key_exists($key, $response)) {
      throw new Exception(format_string('Missing @key in response.', array('@key' => $key)));
    }
  }
}

/**
 * Performs a HTTP POST.
 *
 * @param string $url
 *   URL to place the POST against.
 * @param array $data
 *   An array containing data to pass along.
 */
function _commerce_dibs_payment_window_perform_post($url, $data) {

  // DIBS expect us to send with & and not &amp;
  $data_query = http_build_query($data);

  $options = array(
    'method' => 'POST',
    'data' => $data_query,
    'timeout' => 15,
    'headers' => array('Content-Type' => 'application/x-www-form-urlencoded'),
  );
  $result = drupal_http_request($url, $options);

  // Handle HTTP error codes.
  if (isset($result->code) && (($result->code > 299) || ($result->code < 200))) {
    // Throw an exception with message and code to layer above.
    throw new Exception(isset($result->error) ? $result->error : 'Unexpected error', $result->code);
  }

  // No data but successful.
  if (empty($result->data)) {
    return FALSE;
  }

  return $result->data;
}

/**
 * Check if a commerce_payment_method has a HMAC key that we can use.
 *
 * Use this to generate a MAC key for DIBS.
 *
 * @param array $settings
 *   Payment method settings to check
 *
 * @return bool
 *   TRUE if we can generate a MAC key, FALSE otherwise.
 */
function _commerce_dibs_payment_window_should_add_mac_to_data($settings) {
  return strlen($settings['commerce_dibs_payment_window_api']['commerce_dibs_payment_window_api_hmac']) == 128;
}

/**
 * Gets a human readable error message from an result code from DIBS.
 *
 * @param int $result
 *   Result code from DIBS.
 * @param bool $auth
 *   Should we use error messages for Auth?
 *
 * @return string
 *   Error message for the result code.
 */
function _commerce_dibs_payment_window_human_readable_error_message($result, $auth = FALSE) {
  $msgs = $auth ?
    // Messages for auth API calls.
    array(
      '0' => 'Rejected by acquirer.',
      '1' => 'Communication problems.',
      '2' => 'Error in the parameters sent to the DIBS server. An additional parameter called "message" is returned, with a value that may help identifying the error.',
      '3' => 'Error at the acquirer.',
      '4' => 'Credit card expired.',
      '5' => 'Your shop does not support this credit card type, the credit card type could not be identified, or the credit card number was not modulus correct.',
      '6' => 'Instant capture failed.',
      '7' => 'The order number (orderid) is not unique.',
      '8' => 'There number of amount parameters does not correspond to the number given in the split parameter.',
      '9' => 'Control numbers (cvc) are missing.',
      '10' => 'The credit card does not comply with the credit card type.',
      '11' => 'Declined by DIBS Defender.',
      '20' => 'Cancelled by user at 3D Secure authentication step',
    ) :
    // Messages for the other API calls.
    array(
      '0' => 'Accepted',
      '1' => 'No response from acquirer.',
      '2' => 'Timeout',
      '3' => 'Credit card expired.',
      '4' => 'Rejected by acquirer.',
      '5' => 'Authorisation older than7 days.',
      '6' => 'Transaction status on the DIBS server does not allow capture.',
      '7' => 'Amount too high.',
      '8' => 'Error in the parameters sent to the DIBS server. An additional parameter called "message" is returned, with a value that may help identifying the error.',
      '9' => 'Order number (orderid) does not correspond to the authorisation order number.',
      '10' => 'Re-authorisation of the transaction was rejected.',
      '11' => 'Not able to communicate with the acquier.',
      '12' => 'Confirm request error',
      '14' => 'Capture is called for a transaction which is pending for batch - i.e. capture was already called',
      '15' => 'Capture was blocked by DIBS.',
    );

  if (array_key_exists($result, $msgs)) {
    return $msgs[$result];
  }

  return '';
}
