<?php
/**
 * @file
 * Banklink special functions.
 */

/**
 * Create banklink redirect form.
 */
function banklink_expand_payment_form(&$form, $settings, $payment) {
  $VK = array(
    'VK_SERVICE' => 1002,
    'VK_VERSION' => '008',
    'VK_SND_ID' => $settings['merchant_id'],
    'VK_STAMP' => $payment['order_id'],
    'VK_AMOUNT' => number_format($payment['amount'], 2, '.', ''),
    'VK_CURR' => $payment['currency'],
    'VK_REF' => banklink_get_reference_number($payment['order_id']),
    'VK_MSG' => $payment['message'],
  );

  $form['#action'] = $settings['banklink_url'];

  foreach ($VK as $key => $value) {
    $form[$key] = array(
      '#type' => 'hidden',
      '#value' => $VK[$key],
    );
  }
  $form['VK_MAC'] = array(
    '#type' => 'hidden',
    '#value' => banklink_create_signature($VK, $settings['merchant_private_key'],
       $settings['merchant_private_key_passphrase']),
  );
  $form['VK_LANG'] = array(
    '#type' => 'hidden',
    '#value' => 'EST',
  );
  $form['VK_RETURN'] = array(
    '#type' => 'hidden',
    '#value' => $settings['return'],
  );
  $form['VK_CANCEL'] = array(
    '#type' => 'hidden',
    '#value' => $settings['cancel_return'],
  );
}
/**
 * Create verify signature.
 */
function banklink_verify_signature($VK_a, $settings) {
  $VK_MAC = $VK_a['VK_MAC'];
  $signature = base64_decode($VK_MAC);
  switch ($VK_a['VK_SERVICE']) {
    case '1101':
      $data = banklink_str_pad($VK_a['VK_SERVICE']) .
      banklink_str_pad($VK_a['VK_VERSION']) .
      banklink_str_pad($VK_a['VK_SND_ID'])  .
      banklink_str_pad($VK_a['VK_REC_ID']) .
      banklink_str_pad($VK_a['VK_STAMP']) .
      banklink_str_pad($VK_a['VK_T_NO']) .
      banklink_str_pad($VK_a['VK_AMOUNT']) .
      banklink_str_pad($VK_a['VK_CURR']) .
      banklink_str_pad($VK_a['VK_REC_ACC']) .
      banklink_str_pad($VK_a['VK_REC_NAME']) .
      banklink_str_pad($VK_a['VK_SND_ACC']) .
      banklink_str_pad($VK_a['VK_SND_NAME']) .
      banklink_str_pad($VK_a['VK_REF']) .
      banklink_str_pad($VK_a['VK_MSG']) .
      banklink_str_pad($VK_a['VK_T_DATE']);
      break;

    case '1901':
      $data = banklink_str_pad($VK_a['VK_SERVICE']) .
      banklink_str_pad($VK_a['VK_VERSION']) .
      banklink_str_pad($VK_a['VK_SND_ID']) .
      banklink_str_pad($VK_a['VK_REC_ID']) .
      banklink_str_pad($VK_a['VK_STAMP']) .
      banklink_str_pad($VK_a['VK_REF']) .
      banklink_str_pad($VK_a['VK_MSG']);
      break;
  }
  $pubkey = openssl_get_publickey($settings['bank_public_key']);
  $out = @openssl_verify($data, $signature, $pubkey);
  @openssl_free_key($pubkey);
  return $out;
}

/**
 * Compose a Data.
 */
function banklink_compose_data(&$VK_a) {
  $data = '';
  foreach ($VK_a as $data_bit) {
    $data .= banklink_str_pad($data_bit);
  }
  return $data;
}
/**
 * Strpad.
 */
function banklink_str_pad($str = "") {
  return str_pad(strlen($str), 3, "0", STR_PAD_LEFT) . $str;
}

/**
 * Create banklink signature.
 */
function banklink_create_signature($VK, $merchant_private_key, $passphrase = '') {
  $data = banklink_compose_data($VK);
  $pkeyid = openssl_get_privatekey($merchant_private_key, $passphrase);
  openssl_sign($data, $signature, $pkeyid);
  $VK_MAC = base64_encode($signature);
  openssl_free_key($pkeyid);
  return $VK_MAC;
}

/**
 * Makes reference number by adding the necessary number of control
 */
function banklink_get_reference_number($str) {
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
  }
  else {
    $jrk = 0;
  }
  return "$str$jrk";
}
