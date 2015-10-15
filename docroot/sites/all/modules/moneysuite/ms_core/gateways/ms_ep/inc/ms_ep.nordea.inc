<?php

/**
 * Nordea
 */

/**
 * Build Nordea redirect form.
 */
function ms_ep_nordea_redirect_form($form, &$form_state, MsOrder $order) {
  $order->gateway = 'ms_ep_nordea';
  $order->save();

  $VK = array(
    'SOLOPMT_VERSION' => '0003',
    'SOLOPMT_STAMP' => $order->oid,
    'SOLOPMT_RCV_ID' => ms_core_variable_get('ms_ep_nordea_merchant_id', '', $order),
    // 3 = English 4 = Estonian 6 = Latvian 7 = Lithuanian.
    'SOLOPMT_LANGUAGE' => 4,
    'SOLOPMT_AMOUNT' => number_format($order->amount, 2, '.', ''),
    'SOLOPMT_REF' => ms_ep_nordea_banklink_get_reference_number($order->oid),
    'SOLOPMT_DATE' => 'EXPRESS',
    'SOLOPMT_MSG' => $order->title,
    'SOLOPMT_KEYVERS' => '0001',
    'SOLOPMT_CUR' => $order->currency,
    'SOLOPMT_CONFIRM' => 'YES',
    'SOLOPMT_REJECT' => ms_core_get_checkout_cancel_url($order),
    'SOLOPMT_CANCEL' => ms_core_get_checkout_cancel_url($order),
    'SOLOPMT_RETURN' => ms_core_get_checkout_return_url($order),
  );

  $form['#action'] = ms_core_variable_get('ms_ep_nordea_banklink_url', '', $order);
  foreach ($VK as $key => $value) {
    $form[$key] = array(
      '#type' => 'hidden',
      '#value' => $VK[$key],
    );
  }
  $form['SOLOPMT_MAC'] = array(
    '#type' => 'hidden',
    '#value' => ms_ep_banklink_create_nordea_signature($VK, $order),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue to Payment Site'),
  );

  return $form;
}

/**
 * Create Nordea signature.
 */
function ms_ep_banklink_create_nordea_signature($VK, $order) {
  if (ms_core_variable_get('ms_ep_nordea_merchant_mac', '', $order)) {
    $data = $VK["SOLOPMT_VERSION"] . "&" .
        $VK["SOLOPMT_STAMP"] . "&" .
        $VK["SOLOPMT_RCV_ID"] . "&" .
        $VK["SOLOPMT_AMOUNT"] . "&" .
        $VK["SOLOPMT_REF"] . "&" .
        $VK["SOLOPMT_DATE"] . "&" .
        $VK["SOLOPMT_CUR"] . "&" .
        ms_core_variable_get('ms_ep_nordea_merchant_mac', '', $order) . "&";

    return strtoupper(call_user_func(ms_core_variable_get('ms_ep_nordea_hash', '', $order), $data));
  }
}
/**
 * Create Nordea return singature.
 */
function ms_ep_banklink_create_nordea_return_signature($VK, $order) {
  $merchant_mac = ms_core_variable_get('ms_ep_nordea_merchant_mac', '', $order);
  return strtoupper(call_user_func(ms_core_variable_get('ms_ep_nordea_hash', '', $order),
    "{$VK['SOLOPMT_RETURN_VERSION']}&{$VK['SOLOPMT_RETURN_STAMP']}&
      {$VK['SOLOPMT_RETURN_REF']}&{$VK['SOLOPMT_RETURN_PAID']}&
        {$merchant_mac}&"));
}
/**
 * Build Nordea payment form.
 */
function ms_ep_banklink_verify_nordea_signature($VK, $order) {
  return $VK['SOLOPMT_RETURN_MAC'] == ms_ep_banklink_create_nordea_return_signature($VK, $order);
}
/**
 * Makes reference number by adding the necessary number of control.
 */
function ms_ep_nordea_banklink_get_reference_number($str) {
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
