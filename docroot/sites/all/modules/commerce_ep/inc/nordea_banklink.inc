<?php
/**
 * @file
 * Special functions for Nordea payment method.
 */

/**
 * Build Nordea payment form.
 */
function banklink_expand_nordea_payment_form(&$form, $settings, $payment) {
  $VK = array(
    'SOLOPMT_VERSION' => '0003',
    'SOLOPMT_STAMP' => $payment['order_id'],
    'SOLOPMT_RCV_ID' => $settings['merchant_id'],
    // 3 = English 4 = Estonian 6 = Latvian 7 = Lithuanian.
    'SOLOPMT_LANGUAGE' => 4,
    'SOLOPMT_AMOUNT' => number_format($payment['amount'], 2, '.', ''),
    'SOLOPMT_REF' => nordea_banklink_get_reference_number($payment['order_id']),
    'SOLOPMT_DATE' => 'EXPRESS',
    'SOLOPMT_MSG' => $payment['message'],
    'SOLOPMT_KEYVERS' => '0001',
    'SOLOPMT_CUR' => $payment['currency'],
    'SOLOPMT_CONFIRM' => 'YES',
    'SOLOPMT_REJECT' => $settings['cancel_return'],
    'SOLOPMT_CANCEL' => $settings['cancel_return'],
    'SOLOPMT_RETURN' =>  $settings['return'],
  );

  $form['#action'] = $settings['banklink_url'];
  foreach ($VK as $key => $value) {
    $form[$key] = array(
      '#type' => 'hidden',
      '#value' => $VK[$key],
    );
  }
  $form['SOLOPMT_MAC'] = array(
    '#type' => 'hidden',
    '#value' => banklink_create_nordea_signature($VK, $settings),
  );
}
/**
 * Create Nordea signature.
 */
function banklink_create_nordea_signature($VK, $settings) {
  if (isset($settings['merchant_mac'])) {
    return strtoupper(call_user_func($settings['hash'],
      "{$VK['SOLOPMT_VERSION']}&{$VK['SOLOPMT_STAMP']}&{$VK['SOLOPMT_RCV_ID']}&{$VK['SOLOPMT_AMOUNT']}&{$VK['SOLOPMT_REF']}&{$VK['SOLOPMT_DATE']}&{$VK['SOLOPMT_CUR']}&{$settings['merchant_mac']}&"));
  }
}
/**
 * Create Nordea return singature.
 */
function banklink_create_nordea_return_signature($VK, $settings) {
  return strtoupper(call_user_func($settings['hash'],
    "{$VK['SOLOPMT_RETURN_VERSION']}&{$VK['SOLOPMT_RETURN_STAMP']}&{$VK['SOLOPMT_RETURN_REF']}&{$VK['SOLOPMT_RETURN_PAID']}&{$settings['merchant_mac']}&"));
}
/**
 * Build Nordea payment form.
 */
function banklink_verify_nordea_signature($VK_a, $settings) {
  return $VK_a['SOLOPMT_RETURN_MAC'] == banklink_create_nordea_return_signature($VK_a, $settings);
}
/**
 * Makes reference number by adding the necessary number of control.
 */
function nordea_banklink_get_reference_number($str) {
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
