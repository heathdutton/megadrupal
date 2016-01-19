<?php
/**
 * @theme.inc
 *
 * Preprocess and theming functions.
 */

/**
 * Implementation of hook_preprocess.
 */

function template_preprocess_commerce_eps_transfer_initiator_details(&$vars) {
  $order = $vars['order']; //commerce order object
  $settings = $vars['settings']; // configuration settings, e. g. of payment method

  // get total amount of invoice
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $amount = $wrapper->commerce_order_total->amount->value();
  $order_currency_code = $wrapper->commerce_order_total->currency_code->value();
  $amount = commerce_eps_price_amount(commerce_currency_convert($amount, $order_currency_code, COMMERCE_EPS_CURRENCY_CODE_DEFAULT), COMMERCE_EPS_CURRENCY_CODE_DEFAULT);

  // include article details
  $articles = array();
  foreach ($order->commerce_line_items['und'] as $line_item_id) {
    $line_item = commerce_line_item_load($line_item_id);
    $product = commerce_product_load($line_item->commerce_product['und'][0]['product_id']);
    $price = $product->commerce_price['und'][0]['amount'];
    $article['name'] = $product->title;
    $article['count'] = intval($line_item->quantity);
    $article['price'] = commerce_eps_price_amount(commerce_currency_convert($price, $order_currency_code, COMMERCE_EPS_CURRENCY_CODE_DEFAULT), COMMERCE_EPS_CURRENCY_CODE_DEFAULT);
    $articles[] = $article;
  }

  // fill placeholders with settings
  $vars += array(
    'date' => date('Y-m-d'), // today's date
    'reference_id' => $order->order_id, // for trader's investigation
    'bfi_bic_id' => $settings['bfi_bic_id'], // BIC
    'ben_name_address' => $settings['ben_name_address'],
    'ben_account_id' => $settings['ben_account_id'], // IBAN
    'remittance_id' => $order->order_id, // reference for trader/customer
    'currency' => COMMERCE_EPS_CURRENCY_CODE_DEFAULT,
    'amount' => $amount,
    'confirmation_url' => url('commerce_eps/confirm/' . $order->data['payment_redirect_key'], array('absolute' => TRUE)),
    'transaction_ok_url' => url('commerce_eps/success', array('absolute' => TRUE)),
    'transaction_nok_url' => url('commerce_eps/error', array('absolute' => TRUE)),
    'articles' => $articles,
    'user_id' => $settings['user_id'], // login id
  );

  // calculate md5 hash with secret pin for authentification
  $hash = $settings['pin'] . $vars['date'] . $vars['reference_id'] . $vars['ben_account_id'] .
    $vars['remittance_id'] . $vars['amount'] . $vars['currency'] . $vars['user_id'];
  $vars['md5_hash'] = md5(utf8_encode($hash));
}
