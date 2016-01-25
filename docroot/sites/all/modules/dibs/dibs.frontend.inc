<?php
/**
 * @file
 * Front end functions for the DIBS module
 */

/**
 * Page with hidden form fields that redirects to the DIBS payment
 *
 * @param array $transaction
 *
 * @return string
 */
function dibs_frontend_redirect_page($transaction) {
  $transaction['payment_price'] = (float) $transaction['payment_price'];

  // Making some checks to check if the transaction is valid
  if (isset($transaction['api_module'], $transaction['api_delta'])) {
    $info = module_invoke($transaction['api_module'], 'dibsapi', 'info', $transaction['api_delta']);
    if (empty($info) || !is_array($info)) {
      drupal_set_message(t('Error redirecting to DIBS: api_module and/or api_delta is not valid!'), 'error');
      return '';
    }
  }
  elseif (!empty($transaction['api_module']) || isset($transaction['api_delta'])) {
    drupal_set_message(t('Error redirecting to DIBS: api_module or api_delta is missing!'), 'error');
    return '';
  }
  elseif ($transaction['payment_status'] > 0) {
    drupal_set_message(t('Error redirecting to DIBS: the payment was already made!'), 'error');
    return '';
  }

  // Loading settings
  $settings = dibs_load_settings($transaction['api_module'], $transaction['api_delta']);

  if (empty($settings)) {
    drupal_set_message(t('DIBS module is not configured for module %module, delta %delta.', array('%module' => $transaction['api_module'], '%delta' => $transaction['api_delta'])), 'error');
    return t('Redirect could not be performed due to missing configuration settings.');
  }
  $settings['auto_submit'] = 1;

  // Getting redirect form
  $form = drupal_get_form('dibs_frontend_redirect_form', $settings, $transaction);

  // We are forcing the accept-charset to be ISO-8859-1, else will the order and delivery
  // info sent to DIBS be shown with wrong characters in the payment window and in their
  // administration system.
  if ($settings['general']['type'] == 'pay') {
    $form['#attributes']['accept-charset'] = 'ISO-8859-1';
  }

  return theme('dibs_redirect_page', array('form' => $form, 'settings' => $settings, 'transaction' => $transaction));
}

/**
 * Form builder; Building the payment form and submits
 * it to DIBS with javascript
 *
 * @param array $form_state
 * @param array $settings
 * @param array $data
 *
 * @return array
 */
function dibs_frontend_redirect_form($form, &$form_state = array(), $settings, $data) {
  if (isset($data['payment_retry_count']) && $data['payment_retry_count']) {
    $data['payment_order_id'] .= '-' . $data['payment_retry_count'];
  }

  $split = !empty($data['params']['split_payment_price']) ? count($data['params']['split_payment_price']) : 1;
  $subunit = !empty($data['params']['disable_subunit_calc']) ? 1 : 100;

  // Creating form fields
  // Required fields
  $form['merchant'] = array(
    '#type' => 'hidden',
    '#value' => $settings['general']['merchant'],
  );
  // Handle split payments.
  if ($split == 1) {
    $form['amount'] = array(
      '#type' => 'hidden',
      '#value' => $data['payment_price'] * $subunit,
    );
  }
  else {
    $form['split'] = array(
      '#type' => 'hidden',
      '#value' => $split,
    );
    $count = 1;
    foreach ($data['params']['split_payment_price'] as $payment_price) {
      $form['amount' . $count] = array(
        '#type' => 'hidden',
        '#value' => $payment_price * $subunit,
      );
      $count += 1;
    }
  }
  $form['currency'] = array(
    '#type' => 'hidden',
    '#value' => !empty($data['payment_currency']) ? $data['payment_currency'] : $settings['general']['currency'],
  );
  $form['lang'] = array(
    '#type' => 'hidden',
    '#value' => $settings['general']['lang'],
  );
  $form['orderid'] = array(
    '#type' => 'hidden',
    '#value' => $data['payment_order_id'],
  );
  $form['uniqueoid'] = array(
    '#type' => 'hidden',
    '#value' => 1,
  );
  $form['accepturl'] = array(
    '#type' => 'hidden',
    '#value' => url($settings['callbacks']['accepturl'] . '/' . $data['api_hash'], array('absolute' => TRUE)),
  );
  $form['cancelurl'] = array(
    '#type' => 'hidden',
    '#value' => url($settings['callbacks']['cancelurl'] . '/' . $data['api_hash'], array('absolute' => TRUE)),
  );
  $form['callbackurl'] = array(
    '#type' => 'hidden',
    '#value' => url($settings['callbacks']['callbackok'] . '/' . $data['api_hash'], array('absolute' => TRUE)),
  );
  $form['windowtype'] = array(
    '#type' => 'hidden',
    '#value' => $settings['general']['type'],
  );
  $form['api_module'] = array(
    '#type' => 'hidden',
    '#value' => $data['api_module'],
  );
  $form['api_delta'] = array(
    '#type' => 'hidden',
    '#value' => $data['api_delta'],
  );

  // Optional fields
  if (!empty($settings['general']['test_mode'])) {
    $form['test'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
  }
  if (!empty($settings['general']['account'])) {
    $form['account'] = array(
      '#type' => 'hidden',
      '#value' => $settings['general']['account'],
    );
  }
  if (!empty($settings['advanced']['capturenow'])) {
    $form['capturenow'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
  }
  if (!empty($settings['advanced']['calcfee']) && $split == 1) {
    $form['calcfee'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
  }
  if (!empty($settings['general']['md5'])) {
    $md5_key = md5($settings['general']['md5_key2'] . md5($settings['general']['md5_key1'] . 'merchant=' . $settings['general']['merchant'] . '&orderid=' . $data['payment_order_id'] . '&currency=' . $form['currency']['#value'] . '&amount=' . $data['payment_price'] * 100));
    $form['md5key'] = array(
      '#type' => 'hidden',
      '#value' => $md5_key,
    );
  }

  // Payment Window fields
  if ($settings['general']['type'] == 'pay') {
    if (!empty($settings['paymentwindow']['color'])) {
      $form['color'] = array(
        '#type' => 'hidden',
        '#value' => $settings['paymentwindow']['color'],
      );
    }
    $form['#action'] = 'https://payment.architrade.com/payment/start.pml';
  }
  // Flex Window fields
  elseif ($settings['general']['type'] == 'flex') {
    if (!empty($settings['flexwindow']['color'])) {
      $form['color'] = array(
        '#type' => 'hidden',
        '#value' => $settings['flexwindow']['color'],
      );
    }
    if (!empty($settings['flexwindow']['decorator']) && ($settings['flexwindow']['decorator'] != 'custom')) {
      $form['decorator'] = array(
        '#type' => 'hidden',
        '#value' => $settings['flexwindow']['decorator'],
      );
    }
    else {
      $form['decorator'] = array(
        '#type' => 'hidden',
        '#value' => '',
      );
    }
    if (!empty($settings['flexwindow']['voucher'])) {
      $form['voucher'] = array(
        '#type' => 'hidden',
        '#value' => $settings['flexwindow']['voucher'] ? 'yes' : '',
      );
    }
    $form['#action'] = 'https://payment.architrade.com/paymentweb/start.action';
  }
  // The mobile payment window uses a newer API version. So we need to change
  // some of the key names used to make it work. Another change is the use of
  // HMAC instead of MD5 checksum.
  elseif ($settings['general']['type'] == 'mobile') {
    $form['#action'] = 'https://sat1.dibspayment.com/dibspaymentwindow/entrypoint';

    // Set the params that have a new key.
    $form['acceptreturnurl'] = $form['accepturl'];
    unset($form['accepturl']);
    $form['cancelreturnurl'] = $form['cancelurl'];
    unset($form['cancelurl']);
    if (!empty($form['calcfee'])) {
      $form['addfee'] = $form['calcfee'];
      unset($form['calcfee']);
    }
    $form['language'] = $form['lang'];
    unset($form['lang']);

    // Language has changed format a bit, fix this.
    if ($form['language']['#value'] == 'da') {
      $form['language']['#value'] .= '_DK';
    }
    if ($form['language']['#value'] == 'en') {
      $form['language']['#value'] .= '_UK';
    }
    if ($form['language']['#value'] == 'sv') {
      $form['language']['#value'] .= '_SE';
    }
    if ($form['language']['#value'] == 'nb') {
      $form['language']['#value'] .= '_NO';
    }
    if ($form['language']['#value'] == 'fi') {
      $form['language']['#value'] .= '_FIN';
    }

    // TODO this should be configurable, for starters, lets just add all as possible options.
    $form['paytype'] = array(
      '#type' => 'hidden',
      '#value' => 'MC,VISA,ELEC,AMEX,DK,V-DK',
    );

    // Unique order ids is a bit different.
    unset($form['uniqueoid']);
    $form['uniqueid'] = array(
      '#type' => 'hidden',
      '#value' => $form['orderid']['#value']
    );

    // MD5 key is never used in mobile payment window
    unset($form['md5key']);
    // Add HMAC key if enabled.
    if (!empty($settings['general']['md5']) && !empty($settings['general']['hmac_key'])) {
      $hmac_data = array();
      foreach ($form as $key => $value) {
        $test = array(
          'acceptreturnurl',
          'addfee',
          'amount',
          'callbackurl',
          'cancelreturnurl',
          'currency',
          'language',
          'merchant',
          'orderid',
          'paytype',
          'test',
        );
        if (in_array($key, $test)) {
          $hmac_data[$key] = $value['#value'];
        }
      }
      $form['MAC'] = array(
        '#type' => 'hidden',
        '#value' => dibs_generate_hmac_key($hmac_data, $settings['general']['hmac_key']),
      );
    }
  }

  // Order and payment info fields
  // If order_info_long contains data are we not sending order_info_short to DIBS.
  // The reason for this is a bug in the DIBS administration. If both are sent,
  // will only the short version show up in the administration.
  if (!empty($data['order_info_long']) && is_array($data['order_info_long'])) {
    $order_lines = dibs_create_order_line_fields($data['order_info_long']);
    $form = array_merge($form, $order_lines);
  }
  else if (!empty($data['order_info_short'])) {
    $form['ordertext'] = array(
      '#type' => 'hidden',
      '#value' => $data['order_info_short'],
    );
  }
  if (!empty($data['order_price_info']) && is_array($data['order_price_info'])) {
    $price_lines = dibs_create_price_line_fields($data['order_price_info']);
    $form = array_merge($form, $price_lines);
  }

  // Delivery line fields
  $delivery_lines = dibs_create_delivery_line_fields($data);
  $form = array_merge($form, $delivery_lines);

  // Additional form fields
  $form['submit'] = array('#type' => 'submit', '#value' => t('Continue to DIBS payment'));
  if (!empty($settings['auto_submit'])) {
    $form['submit']['#attached']['js'][] = drupal_get_path('module', 'dibs') . '/dibs.auto_submit.js';
  }

  // Executing the hook_dibsapi_form_alter
  $func = $data['api_module'] .'_dibsapi_form_alter';
  if (function_exists($func)) {
    $func($form, $data['api_delta']);
  }
  return $form;
}

/**
 * The cancel page where the user has the possibility to go back to the
 * payment again
 *
 * @param array $transaction
 *
 * @return string
 */
function dibs_frontend_cancel_page($transaction) {
  $transaction['payment_price'] = (float)$transaction['payment_price'];

  // Making some checks to check if the transaction is valid
  if (isset($data['api_module'], $data['api_delta'])) {
    $info = module_invoke($transaction['api_module'], 'dibsapi', 'info', $transaction['api_delta']);
    if (empty($info) || !is_array($info)) {
      drupal_set_message(t('Error redirecting to DIBS: api_module and/or api_delta is not valid!'), 'error');
      return '';
    }
  }
  elseif (!empty($data['api_module']) || isset($data['api_delta'])) {
    drupal_set_message(t('Error redirecting to DIBS: api_module or api_delta is missing!'), 'error');
    return '';
  }
  elseif ($transaction['payment_status'] > 0) {
    drupal_set_message(t('Error redirecting to DIBS: the payment has already made!'), 'error');
    return '';
  }

  // Invoking the module that implemented the payment
  $function = $transaction['api_module'] . '_dibsapi';
  if (function_exists($function)) {
    $function('transaction_cancel', $transaction['api_delta'], $transaction);
  }

  // Loading settings
  $settings = dibs_load_settings($transaction['api_module'], $transaction['api_delta']);

  // Getting redirect form
  $form = drupal_get_form('dibs_frontend_cancel_form', $settings, $transaction);

  return theme('dibs_cancel_page', array('form' => $form, 'settings' => $settings, 'transaction' => $transaction));
}

/**
 * Form builder; Building the cancel form
 *
 * @param array $form_state
 * @param array $settings
 * @param array $data
 *
 * @return array
 *
 * @ingroup forms
 * @see dibs_frontend_cancel_form_submit()
 */
function dibs_frontend_cancel_form($node, &$form_state = array(), $settings, $data) {
  $form['hash'] = array('#type' => 'hidden', '#value' => $data['api_hash']);
  $form['submit'] = array('#type' => 'submit', '#value' => t('Return to DIBS payment'));
  return $form;
}

/**
 * Process the cancel form
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup forms
 * @see dibs_frontend_cancel_form()
 */
function dibs_frontend_cancel_form_submit($form, &$form_state) {
  $hash = $form_state['values']['hash'];

  // Loading the transaction
  $data = dibs_transaction_load($hash, 'hash');
  $settings = dibs_load_settings($data['api_module'], $data['api_delta'], TRUE);

  // Make changes to order id if dictated by retry policy
  $retry = isset($settings['general']['retry_handling']) ? $settings['general']['retry_handling'] : 'new_order_id';
  switch ($retry) {
    case 'new_order_id':
      // Generating order id
      $order_id = dibs_get_next_order_id($data['api_module'], $data['api_delta']);
      // Updating transaction
      $updated = db_update('dibs_transactions')
        ->fields(array(
          'payment_order_id' => $order_id,
          ))
        ->condition('api_hash', $hash)
        ->execute();
      break;
    case 'add_retry_suffix':
      $suffixed = array(
        'api_hash' => $hash,
        'payment_retry_count' => $data['payment_retry_count']+1,
      );
      drupal_write_record('dibs_transactions', $suffixed, array('api_hash'));
  }

  // Redirecting to the payment redirect page
  drupal_goto('payment/dibs/redirect/' . $hash);

  exit;
}

/**
 * Utility function to check if the $_REQUEST array has the needed transact
 * keys to perform validation.
 *
 * @param $request
 *    The $_REQUEST array
 * @param $split
 *    The number of payments the order is split into.
 *
 * @return boolean if the request is valid.
 */
function dibs_frontend_validate_transact($request, $split = 1) {
  if ($split == 1) {
    // Mobile payments uses the transaction key.
    return !empty($request['transact']) || !empty($request['transaction']);
  }
  foreach (range(1, $split) as $key) {
    if (!isset($request['transact' . $key]) || !isset($request['transaction' . $key])) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * DIBS payment ok callback
 *
 * @param array $transaction
 */
function dibs_frontend_dibs_callback_ok($transaction) {
  $split = $transaction['split_payments'];
  if (dibs_frontend_validate_transact($_REQUEST, $split)) {
    $paytype = isset($_REQUEST['paytype']) ? $_REQUEST['paytype'] : '';
    if (!empty($_REQUEST['cardTypeName'])) {
      $paytype = $_REQUEST['cardTypeName'];
    }
    // No split payments
    if ($split == 1) {
      // In the old API transact key is used, in the new API transaction key
      // is used. We should be able to handle both.
      if (!empty($_REQUEST['transact'])) {
        $transact = $_REQUEST['transact'];
        $transact_key = 'transact='. $transact;
      }
      elseif (!empty($_REQUEST['transaction'])) {
        $transact = $_REQUEST['transaction'];
        $transact_key = 'transaction='. $transact;
      }
    }
    // Split paymebts
    else {
      $lookup_key = !isset($request['transact' . $key]) ? 'transact' : 'transaction';
      $transact = '';
      $transact_a = array();
      $transact_key = '';
      foreach (range(1, $split) as $key) {
        $transact_a[$key] = $_REQUEST[$lookup_key . $key];
        if (empty($transact_key)) {
          $transact_key .= $lookup_key . '1=' . $_REQUEST[$lookup_key . $key];
        }
        else {
          $transact_key .= '&' . $lookup_key . $key . '=' . $_REQUEST[$lookup_key . $key];
        }
      }
    }
    if (!empty($transaction)) {
      $settings = dibs_load_settings($transaction['api_module'], $transaction['api_delta']);
      $test = FALSE;
      // Test MD5 or HMAC depending on what we got.
      if (!empty($settings['general']['md5'])) {
        if (!empty($_REQUEST['authkey'])) {
          $amount = $transaction['payment_price'] * 100 ;
          if (isset($_REQUEST['fee'])) {
            $amount += $_REQUEST['fee'];
          }
          $md5_key = md5($settings['general']['md5_key2'] . md5($settings['general']['md5_key1'] . $transact_key .'&amount='. $amount .'&currency='. $transaction['payment_currency']));
          $test = $md5_key == $_REQUEST['authkey'];
        }
        elseif (!empty($_REQUEST['MAC'])) {
          $hmac_data = $_REQUEST;
          unset($hmac_data['MAC']);
          $test = dibs_generate_hmac_key($hmac_data, $settings['general']['hmac_key']) == $_REQUEST['MAC'];
        }
      }

      // We need to pass the test or not do the test at all.
      if ($test || empty($settings['general']['md5'])) {
        // Updating transaction record with payment transaction id
        $update = array(
          'payment_status' => 1,
          'payment_type' => $paytype,
        );
        if (isset($_REQUEST['fee'])) {
            $update['payment_fee'] = $_REQUEST['fee'];
          }
        if ($split == 1) {
          $update['payment_transaction_id'] = $transact;
        }
        $updated = db_update('dibs_transactions')
          ->fields($update)
          ->condition('tid', $transaction['tid'])
          ->execute();
        if ($split > 1) {
          foreach ($transact_a as $delta => $payment_transaction_id) {
            db_insert('dibs_split_payments')
              ->fields(array(
                'tid' => $transaction['tid'],
                'payment_transaction_id' => $payment_transaction_id,
                'delta' => $delta,
              ))
              ->execute();
          }
        }

        if ($updated) {
          watchdog('dibs', 'Transaction #@tid validated as OK.', array(
            '@tid' => $transaction['tid'],
          ), WATCHDOG_INFO);
        }

        // Reload the transaction.
        $transaction = dibs_transaction_load($transaction['tid']);

        // Invoking the module that implemented the payment
        $function = $transaction['api_module'] . '_dibsapi';
        if (function_exists($function)) {
          $function('transaction_callback', $transaction['api_delta'], $transaction);
        }
      }
      else {
        watchdog('dibs', "MD5 key didn't match for transaction, got @md5_in expected @md5_calc" , array(
          '@md5_in' => $authkey,
          '@md5_calc' => $md5_key,
        ), WATCHDOG_WARNING);
      }
    }
  }
  else {
    watchdog('dibs', 'Transaction not set in request data for OK callback, transaction #@tid.', array(
      '@tid' => $transaction['tid'],
    ), WATCHDOG_ERROR);
  }
}

/**
 * DIBS accept page
 *
 * @param array $transaction
 */
function dibs_frontend_accept_page($transaction) {
  // Returning an error if the transaction is emtpy
  if (empty($transaction)) {
    drupal_set_message(t('An error occured. The transaction key is not valid!'), 'error');
    return '';
  }

  // Unsetting the temporary session
  unset($_SESSION['dibs']);

  // Invoking the module that implemented the payment
  $function = $transaction['api_module'] . '_dibsapi';
  if (function_exists($function)) {
    $function('transaction_accept', $transaction['api_delta'], $transaction);
  }

  // Loading settings
  $settings = dibs_load_settings($transaction['api_module'], $transaction['api_delta']);

  // Returning the themed output
  return theme('dibs_accept_page', array('settings' => $settings, 'transaction' => $transaction));
}

