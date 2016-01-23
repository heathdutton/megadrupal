<?php

/**
 * @file
 * First Data redirected payment service pages.
 */

/**
 * Payment complete function.
 */
function ms_ibis_complete() {
  $ecomm_server_url = (variable_get('ms_ibis_server', 'test') == 'test') ? variable_get('server_url_test', '') : variable_get('server_url_live', '');

  $order_id = $_REQUEST['ibis_order_id'];
  $trans_id = $_REQUEST['trans_id'];

  $now = REQUEST_TIME;

  $merchant = new Merchant($ecomm_server_url, variable_get('ms_ibis_cert_path', ''), variable_get('ms_ibis_cert_pass', ''), 1);

  $resp = $merchant->getTransResult(urlencode($trans_id), ip_address());

  $failed = FALSE;
  if (substr($resp, 8, 8) == 'DECLINED') {
    $failed = TRUE;
  } elseif (substr($resp, 8, 6) == 'FAILED') {
    $failed = TRUE;
  }

  if (strstr($resp, 'RESULT:')) {

    // $resp example RESULT: OK RESULT_CODE: 000 3DSECURE: NOTPARTICIPATED
    // RRN: 915300393049 APPROVAL_CODE: 705368 CARD_NUMBER: 4***********9913.
    if (strstr($resp, 'RESULT:')) {
      $result = explode('RESULT: ', $resp);
      $result = preg_split('/\r\n|\r|\n/', $result[1]);
      $result = $result[0];
    } else {
      $result = '';
    }

    if (strstr($resp, 'RESULT_CODE:')) {
      $result_code = explode('RESULT_CODE: ', $resp);
      $result_code = preg_split('/\r\n|\r|\n/', $result_code[1]);
      $result_code = $result_code[0];
    } else {
      $result_code = '';
    }

    if (strstr($resp, '3DSECURE:')) {
      $result_3dsecure = explode('3DSECURE: ', $resp);
      $result_3dsecure = preg_split('/\r\n|\r|\n/', $result_3dsecure[1]);
      $result_3dsecure = $result_3dsecure[0];
    } else {
      $result_3dsecure = '';
    }

    if (strstr($resp, 'CARD_NUMBER:')) {
      $card_number = explode('CARD_NUMBER: ', $resp);
      $card_number = preg_split('/\r\n|\r|\n/', $card_number[1]);
      $card_number = $card_number[0];
    } else {
      $card_number = '';
    }

    db_update('ms_ibis_transaction')
      ->fields(array(
        'order_id' => $order_id,
        'result' => $result,
        'result_code' => $result_code,
        'result_3dsecure' => $result_3dsecure,
        'card_number' => $card_number,
        'response' => $resp,
      ))
      ->condition('trans_id', $trans_id)
      ->execute();
  } else {
    db_insert('ms_ibis_error')
      ->fields(array(
        'error_time' => $now,
        'action' => 'ReturnOkURL',
        'response' => $resp,
      ))
      ->execute();
  }

  $payment = ms_core_new_payment($order_id, 'ms_ibis', 'cart');
  $order = ms_core_order_load($order_id);

  if ($failed == TRUE) {
    drupal_set_message(t('Payment failed. Check card number and expiration date.'), 'error');

    $payment->type = 'failed';

    ms_core_log_error('ms_ibis', 'Payment failed: !result', array('!result' => $resp));

    ms_core_enter_payment($payment);

    // Go back to checkout.
    $order->status = 'checkout';
    $order->save();
    ms_core_set_order_session($order);
    drupal_goto('ms/checkout');
  } else {
    ms_core_log('ms_ibis', 'Receiving new order notification for order !order_id.', array('!order_id' => $order->order_number));
    ms_core_log('ms_ibis', 'Payment received: !result', array('!result' => $resp));

    $payment->amount = $order->amount;
    $payment->currency = $order->currency;

    ms_core_enter_payment($payment);

    drupal_set_message(t('Thank you for your payment!'));

    ms_core_set_order_session($order);

    drupal_goto('ms/thankyou');
  }
}

/**
 * Payment failed function.
 */
function ms_ibis_failed() {
  $ecomm_server_url = (variable_get('ms_ibis_server', 'test') == 'test') ? variable_get('server_url_test', '') : variable_get('server_url_live', '');

  $trans_id = $_REQUEST['trans_id'];
  $error_msg = $_REQUEST['error'];
  $now = REQUEST_TIME;

  $result = db_query("SELECT client_ip_addr FROM {ms_ibis_transaction} WHERE `trans_id` = :`trans_id`", array(':`trans_id`' => $trans_id));

  $client_ip_addr = $result->fetchField('client_ip_addr');

  $merchant = new Merchant($ecomm_server_url, variable_get('ms_ibis_cert_path', ''), variable_get('ms_ibis_cert_pass', ''), 1);

  $resp = $merchant->getTransResult(urlencode($trans_id), $client_ip_addr);
  $resp = $error_msg . ' + ' . $resp;
  db_insert('ms_ibis_error')
    ->fields(array(
      'error_time' => $now,
      'action' => 'ReturnFailURL',
      'response' => $resp,
    ))
    ->execute();
  drupal_set_message(check_plain(t('Technical error occurred! Please contact merchant! !message'), array('!message' => $error_msg)));
  drupal_goto();
}

/**
 * Refund payment to customer.
 */
function ms_ibis_refund(MsPayment $payment) {
  $ecomm_server_url = (variable_get('ms_ibis_server', 'test') == 'test') ? variable_get('server_url_test', '') : variable_get('server_url_live', '');

  $id = $_REQUEST['order_id'];
  $trans_id = urlencode($_REQUEST['trans_id']);
  $amount = $_REQUEST['amount'] * 100;
  $now = REQUEST_TIME;

  if ($amount == '0') {
    drupal_set_message(t('Amount invalid. <a href=\"javascript:history.go(-1)\"><< Back </a>'));
  }
  $merchant = new Merchant($ecomm_server_url, variable_get('ms_ibis_cert_path', ''), variable_get('ms_ibis_cert_pass', ''), 1);
  $resp = $merchant->reverse($trans_id, $amount);

  if (substr($resp, 8, 2) == "OK" || substr($resp, 8, 8) == "REVERSED") {

    if (strstr($resp, 'RESULT:')) {
      $result = explode('RESULT: ', $resp);
      $result = preg_split('/\r\n|\r|\n/', $result[1]);
      $result = $result[0];
    } else {
      $result = '';
    }

    if (strstr($resp, 'RESULT_CODE:')) {
      $result_code = explode('RESULT_CODE: ', $resp);
      $result_code = preg_split('/\r\n|\r|\n/', $result_code[1]);
      $result_code = $result_code[0];
    } else {
      $result_code = '';
    }

    db_update('ms_ibis_transaction')
      ->fields(array(
        'reversal_amount' => $amount,
        'result_code' => $result_code,
        'result' => $result,
        'response' => $resp,
      ))
      ->condition('trans_id', $_REQUEST['trans_id'])
      ->execute();
    ms_core_log('ms_ibis', 'Payment reversed: !result', array('!result' => $resp));
    drupal_set_message(t('Payment reversed.'));
    drupal_goto('admin/store/orders/' . $id);

  } else {
    ms_core_log('ms_ibis', 'Payment reverse failed: !result', array('!result' => $resp . $trans_id));
    drupal_set_message(t('Payment reverse failed.'));
    drupal_goto('admin/store/orders/' . $id);
    db_insert('ms_ibis_error')
      ->fields(array(
        'error_time' => $now,
        'action' => 'reverse',
        'response' => $resp,
      ))
      ->execute();
  }
}
