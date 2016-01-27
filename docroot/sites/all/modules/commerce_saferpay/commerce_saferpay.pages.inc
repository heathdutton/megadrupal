<?php

/**
 * @file
 * Saferpay menu items callbacks.
 */

function commerce_saferpay_ipn($payment_method, $order) {
  // @todo: This code is not yet tested.

  $data = $_POST['DATA'];
  $signature = $_POST['SIGNATURE'];

  while (drupal_strtolower(drupal_substr($data, 0, 14)) == '<idp msgtype=\\') {
    $data = stripslashes($data);
  }

  $data_xml = simplexml_load_string($data);

  if (!_commerce_saferpay_validate($data, $signature)) {
    return FALSE;
  }

  $result = _commerce_saferpay_result();
  commerce_saferpay_process_data($order, $payment_method, $data_xml, $result);
}
