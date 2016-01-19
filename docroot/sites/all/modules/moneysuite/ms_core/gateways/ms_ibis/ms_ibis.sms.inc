<?php

/**
 * @file
 * First Data LV redirected payment service SMS processing functions.
 */

/**
 * SMS payment process function.
 */
function ms_ibis_process_sms(MsOrder $order) {
  global $language;

  $ecomm_client_url = (variable_get('ms_ibis_server', 'test') == 'test') ? variable_get('client_url_test', '') : variable_get('client_url_live', '');
  $ecomm_server_url = (variable_get('ms_ibis_server', 'test') == 'test') ? variable_get('server_url_test', '') : variable_get('server_url_live', '');

  $amount = $order->amount * 100;
  $ip = ip_address();
  // Use localhost IP address to make testing procedure for blakclist IP.
  // $ip = '192.168.1.2'.
  $currency = variable_get('ms_ibis_currency', 'EUR');
  $description = urlencode(htmlspecialchars(t('Order #!order payment', array('!order' => check_plain($order->order_number))), ENT_QUOTES));
  $lang = $language->language;
  $ibis_order_id = $order->oid;

  $merchant = new Merchant($ecomm_server_url, variable_get('ms_ibis_cert_path', ''), variable_get('ms_ibis_cert_pass', ''), 1);

  $resp = $merchant->startSMSTrans($amount, $currency, $ip, $description, $lang);

  if (substr($resp, 0, 14) == "TRANSACTION_ID") {
    $trans_id = substr($resp, 16, 28);
    $url = $ecomm_client_url . '?trans_id=' . urlencode($trans_id) . '&ibis_order_id=' . $ibis_order_id;
    db_insert('ms_ibis_transaction')
      ->fields(array(
        'trans_id' => $trans_id,
        'amount' => $amount,
        'currency' => $currency,
        'client_ip_addr' => $ip,
        'order_id' => $ibis_order_id,
        'description' => t('Order #!order payment', array('!order' => check_plain($order->order_number))),
        'language' => $lang,
        't_date' => REQUEST_TIME,
        'response' => $resp,
      ))
      ->execute();

    drupal_goto($url);
  } else {
    ms_core_log('ms_ibis', 'SMS payment make failed: !result', array('!result' => $resp));
    db_insert('ms_ibis_error')
      ->fields(array(
        'error_time' => REQUEST_TIME,
        'action' => 'startSMStrans',
        'response' => $resp,
      ))
      ->execute();
    drupal_set_message(check_plain(t('Technical error occurred! Please contact merchant!')));
    drupal_goto();
  }
}
