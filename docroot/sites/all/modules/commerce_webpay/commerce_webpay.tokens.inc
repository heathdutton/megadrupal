<?php

/**
 * @file
 * Builds placeholder replacement tokens for order-related data.
 */


/**
 * Implements hook_token_info_alter().
 */
function commerce_webpay_token_info_alter(&$data) {
  $data['tokens']['commerce-order']['voucher-webpay'] = array(
    'name' => t('Voucher'),
    'description' => t('Webpay voucher') ,
  );
}


/**
 * Implements hook_tokens().
 */
function commerce_webpay_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'commerce-order' && !empty($data['commerce-order'])) {
    $order = $data['commerce-order'];
    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Simple key values on the order.
        case 'voucher-webpay':
          if ($order->data['payment_method'] == 'webpay|commerce_payment_webpay') {
            $webpay_data_table = commerce_webpay_prepare_voucher($order);

            $replacements[$original] = $webpay_data_table;
          }
          else {
            $replacements[$original] = '';
          }
          break;
      }
    }
  }

  return $replacements;
}
