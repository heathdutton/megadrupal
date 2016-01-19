<?php

/**
 * @file
 * Manage KeyClient payment operation.
 */

/**
 * Callback function for payment completed.
 */
function commerce_keyclient_complete() {
  $response  = drupal_get_query_parameters(NULL, array('q'), '');

  // Get order id from url. Strip it from test string if necessary.
  $order_id = commerce_keyclient_getorderid($response['codTrans']);

  if ($response['esito'] == 'OK') {
    watchdog('commerce_keyclient', 'Order %order complete. redirect to complete page', array('%order' => $order_id), WATCHDOG_NOTICE);

    // Redirect to checkout complete page.
    drupal_goto('checkout/' . $order_id . '/complete');
  }

  if ($response['esito'] == 'KO') {
    watchdog('commerce_keyclient', 'Order %order KO. redirect to review page', array('%order' => $order_id), WATCHDOG_NOTICE);

    // Get remaining checkout attempts.
    $remain_attempts = commerce_keyclient_getremaintransactionattempt($order_id);

    // If there are attempts left, notify and redirect to review page.
    if ($remain_attempts > 0) {
      drupal_set_message(
        t('We are sorry but payment gateway refused the transaction. You have %remain_attempts attempts left', array('%remain_attempts' => $remain_attempts)),
        'warning'
      );

      // Redirect to checkout cancelled page.
      drupal_goto('checkout/' . $order_id . '/review');
    }
    else {

      // User has attempeted to checkout for 3 times. PG will not process any request anymore.
      // Puts order on checkout, the user may change gateway, and notify user.
      drupal_set_message(
        t('We are sorry but payment gateway refused the transaction for %max_attempts times.<br/> You could try to repeat the checkout process from the beginning, changing your payment method. For any question please contact us at %sitemail',
          array(
            '%sitemail' => variable_get('site_mail', ''),
            '%max_attempts' => COMMERCE_KEYCLIENT_MAX_ATTEMPTS)
          ),
      'error');

      // Load the order.
      $order = commerce_order_load($order_id);

      commerce_order_status_update($order, 'checkout_checkout', FALSE, TRUE, 'Transaction refused by Keyclient Payment Gateway');

      drupal_goto('checkout/' . $order_id . '/checkout');
    }
  }
}

/**
 * Callback function for payment aborted from user.
 */
function commerce_keyclient_cancel() {
  $response  = drupal_get_query_parameters(NULL, array('q'), '');

  if (!$response) {
    // We dont have any response from keyclient for whatever reason, this could
    // happen when trying to checkout an already precessed order id for now lets
    // just redirect to the front page and log this.
    watchdog('commerce_keyclient', 'response from keyclient is NULL. User redirected to frontpage', array(), WATCHDOG_ERROR);
    drupal_goto('<front>');

  }
  else {
    // Some parameter are missing from this response.
    $response['mac'] = NULL;
    $response['codAut'] = NULL;

    // Get order id from url. Strip it from test string if necessary.
    $order_id = commerce_keyclient_getorderid($response['codTrans']);

    // Load order.
    $order = commerce_order_load($order_id);

    if ($response['esito'] === 'ERRORE') {
      // User has attempeted to checkout for 3 times. PG will not process any request anymore.
      // Puts order on checkout, the user may change gateway, and notify user.
      drupal_set_message(
        t('We are sorry but payment gateway refused the transaction for %max_attempts times.<br/> You could try to repeat the checkout process from the beginning, changing your payment method. For any question please contact us at %sitemail',
          array(
            '%sitemail' => variable_get('site_mail', ''),
            '%max_attempts' => COMMERCE_KEYCLIENT_MAX_ATTEMPTS)
          ),
      'error');

      commerce_order_status_update($order, 'checkout_checkout', FALSE, TRUE, 'Transaction refused by Keyclient Payment Gateway');

      drupal_goto('checkout/' . $order_id . '/checkout');
    }

    if ($response['esito'] === 'ANNULLO') {
      commerce_payment_redirect_pane_previous_page($order);
      commerce_keyclient_transaction($order_id, $response, COMMERCE_PAYMENT_STATUS_PENDING);
      drupal_goto('checkout/' . $order_id . '/review');
    }
  }
}

/**
 * Callback function for server2server operation from KeyClient.
 */
function commerce_keyclient_server2server() {
  $response  = commerce_keyclient_process_response();

  if (!commerce_keycliente_server2server_validation($response)) {
    watchdog('commerce_keyclient', 'Error mac validation server2server. !response !session', array(
        '!response' => print_r($response, TRUE),
        '!session'  => print_r($_SESSION, TRUE),
      ), WATCHDOG_ERROR);
    drupal_access_denied();
    return;
  }

  // Get order id from url. Strip it from test string if necessary.
  $order_id = commerce_keyclient_getorderid($response['codTrans']);

  // Load the order.
  $order = commerce_order_load($order_id);

  if ($response['esito'] == 'OK') {
    $status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    // Move order forward.
    commerce_payment_redirect_pane_next_page($order);
  }

  if ($response['esito'] == 'KO') {
    // Move order backward.
    commerce_payment_redirect_pane_previous_page($order);
    $status = COMMERCE_PAYMENT_STATUS_FAILURE;
  }

  // Log the transaction attempt.
  commerce_keyclient_transaction($order_id, $response, $status);

  return '';
}


/**
 * Validate KeyClient server2server request.
 *
 * @param array $response
 *   KeyClient response
 *
 * @return bool
 *   Request is valid
 */
function commerce_keycliente_server2server_validation($response) {
  $settings = commerce_keyclient_getsettings();

  $mac  = 'codTrans=' . $response['codTrans'];
  $mac .= 'esito='    . $response['esito'];
  $mac .= 'importo='  . $response['importo'];
  $mac .= 'divisa='   . $response['divisa'];
  $mac .= 'data='     . $response['data'];
  $mac .= 'orario='   . $response['orario'];
  $mac .= 'codAut='   . $response['codAut'];
  $mac .= $settings['secret_string'];
  $mac = sha1($mac);

  return ($mac == $response['mac']);
}
