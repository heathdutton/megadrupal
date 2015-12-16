<?php

/**
 * Banklink
 */

/**
 * Build Sampo redirect form.
 */
function ms_ep_sampo_redirect_form($form, &$form_state, MsOrder $order) {
  return ms_ep_banklink_redirect_form($form, $order, 'ms_ep_sampo');
}

/**
 * Build SEB redirect form.
 */
function ms_ep_seb_redirect_form($form, &$form_state, MsOrder $order) {
  return ms_ep_banklink_redirect_form($form, $order, 'ms_ep_seb');
}

/**
 * Build SWEDBANK redirect form.
 */
function ms_ep_swedbank_redirect_form($form, &$form_state, MsOrder $order) {
  return ms_ep_banklink_redirect_form($form, $order, 'ms_ep_swedbank');
}

/**
 * Create banklink redirect form.
 */
function ms_ep_banklink_redirect_form($form, MsOrder $order, $method) {
  $VK = array(
    'VK_SERVICE' => 1002,
    'VK_VERSION' => '008',
    'VK_SND_ID' => ms_core_variable_get($method . '_merchant_id', '', $order),
    'VK_STAMP' => $order->oid,
    'VK_AMOUNT' => number_format($order->amount, 2, '.', ''),
    'VK_CURR' => $order->currency,
    'VK_REF' => ms_ep_banklink_get_reference_number($order->oid),
    'VK_MSG' => ms_core_get_order_title($order, 70),
  );

  $VK['VK_MAC'] = ms_ep_banklink_create_signature($VK, ms_core_variable_get($method . '_merchant_private_key', '', $order),
    ms_core_variable_get($method . '_merchant_private_key_passphrase', '', $order));

  $VK['VK_LANG'] = 'EST';
  if ($method == 'ms_ep_swedbank') {
    $VK['VK_ENCODING'] = 'UTF-8';
  } else {
    $VK['VK_CHARSET'] = 'UTF-8'; // Seems that Pangalink uses this instead of VK_ENCODING
  }
  $VK['VK_RETURN'] = ms_core_get_checkout_return_url($order);
  $VK['VK_CANCEL'] = ms_core_get_checkout_cancel_url($order);

  $form['#action'] = ms_core_variable_get($method . '_banklink_url', '', $order);

  foreach ($VK as $key => $value) {
    $form[$key] = array(
      '#type' => 'hidden',
      '#value' => $VK[$key],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue to Payment Site'),
  );

  return $form;
}

/**
 * Create verify signature.
 */
function ms_ep_banklink_verify_signature($VK, $order) {
  $VK_MAC = $VK['VK_MAC'];
  $signature = base64_decode($VK_MAC);
  $data = NULL;
  switch ($VK['VK_SERVICE']) {
    case '1101':
      $data = ms_ep_banklink_str_pad($VK['VK_SERVICE']) .
        ms_ep_banklink_str_pad($VK['VK_VERSION']) .
        ms_ep_banklink_str_pad($VK['VK_SND_ID']) .
        ms_ep_banklink_str_pad($VK['VK_REC_ID']) .
        ms_ep_banklink_str_pad($VK['VK_STAMP']) .
        ms_ep_banklink_str_pad($VK['VK_T_NO']) .
        ms_ep_banklink_str_pad($VK['VK_AMOUNT']) .
        ms_ep_banklink_str_pad($VK['VK_CURR']) .
        ms_ep_banklink_str_pad($VK['VK_REC_ACC']) .
        ms_ep_banklink_str_pad($VK['VK_REC_NAME']) .
        ms_ep_banklink_str_pad($VK['VK_SND_ACC']) .
        ms_ep_banklink_str_pad($VK['VK_SND_NAME']) .
        ms_ep_banklink_str_pad($VK['VK_REF']) .
        ms_ep_banklink_str_pad($VK['VK_MSG']) .
        ms_ep_banklink_str_pad($VK['VK_T_DATE']);
      break;

    case '1901':
      $data = ms_ep_banklink_str_pad($VK['VK_SERVICE']) .
        ms_ep_banklink_str_pad($VK['VK_VERSION']) .
        ms_ep_banklink_str_pad($VK['VK_SND_ID']) .
        ms_ep_banklink_str_pad($VK['VK_REC_ID']) .
        ms_ep_banklink_str_pad($VK['VK_STAMP']) .
        ms_ep_banklink_str_pad($VK['VK_REF']) .
        ms_ep_banklink_str_pad($VK['VK_MSG']);
      break;
  }
  $pubkey = openssl_get_publickey(ms_core_variable_get($order->gateway . '_bank_public_key', '', $order));
  $out = @openssl_verify($data, $signature, $pubkey);
  @openssl_free_key($pubkey);
  return $out;
}

/**
 * Compose a Data.
 */
function ms_ep_banklink_compose_data(&$VK_a) {
  $data = '';
  foreach ($VK_a as $data_bit) {
    $data .= ms_ep_banklink_str_pad($data_bit);
  }
  return $data;
}

/**
 * Strpad.
 */
function ms_ep_banklink_str_pad($str = "") {
  return str_pad(mb_strlen($str, 'UTF-8'), 3, "0", STR_PAD_LEFT) . $str;
}

/**
 * Create banklink signature.
 */
function ms_ep_banklink_create_signature($VK, $merchant_private_key, $passphrase = '') {
  $data = ms_ep_banklink_compose_data($VK);
  $pkeyid = openssl_get_privatekey($merchant_private_key, $passphrase);
  openssl_sign($data, $signature, $pkeyid);
  $VK_MAC = base64_encode($signature);
  openssl_free_key($pkeyid);
  return $VK_MAC;
}

/**
 * Makes reference number by adding the necessary number of control
 */
function ms_ep_banklink_get_reference_number($str) {
  $weights = array(7, 3, 1, 7, 3, 1, 7, 3, 1, 7, 3, 1, 7, 3, 1, 7, 3, 1, 7, 3);
  $str_a = preg_split("//", $str, -1, PREG_SPLIT_NO_EMPTY);
  $sum = 0;
  $weights = array_reverse(array_slice($weights, 0, count($str_a)));
  foreach ($str_a as $index => $num) {
    $add = $num * $weights[$index];
    $sum += $add;
  }
  if (($sum % 10) != 0) {
    $jrk = (10 - ($sum % 10));
  } else {
    $jrk = 0;
  }
  return "$str$jrk";
}
