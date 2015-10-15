<?php

/*
 * @file
 * pg_2co.pages.inc
 * Page callbacks for 2co module
 */

/**
 * Page callback. Recieve data after payment processed
 */
function pg_2co_done_payment() {
  // Recieve data from 2co server
  $txnid            = $_POST['item_number'];
  $payment_status   = check_plain($_POST['payment_status']);
  $payment_amount   = check_plain($_POST['mc_gross']);
  $payment_currency = check_plain($_POST['mc_currency']);
  $receiver_email   = check_plain($_POST['receiver_email']);
  $payer_email      = check_plain($_POST['payer_email']);
  $receiver_email   = check_plain($_POST['receiver_email']);

  // Set data for query
  $headers = array('Content-Type' => 'application/x-www-form-urlencoded');
  $post = $_POST;
  $post['cmd'] = '_notify-validate';
  unset($post['q']);
  $data = http_build_query($post, '', '&');
  $url = variable_get('pg_2co_validation_url', cPaypalValidationUrl);

  // Check recieved data on 2co server
  $request = drupal_http_request($url, array('headers' => $headers, 'method' => 'POST', 'data' => $data));

  // Get default settings
  $description = t('Payment has been received.');
  $status      = 'completed';
  $transaction = pgapi_transaction_load($txnid);
  $currency    = variable_get('pg_2co_currency_code', 'USD');
  $email       = variable_get('pg_2co_email', '');

  // Check data
  if ($request->data != 'VERIFIED') {
    $description = t('Verification has been failed');
    $status = 'denied';
  }
  if ($transaction->extra['amount'] != $payment_amount) {
    $description = t('This amount does not match with the original price');
    $status = 'denied';
  }
  if ($currency != $payment_currency) {
    $description = t('This currency does not match with the original currency');
    $status = 'denied';
  }
  if ($email != $receiver_email) {
    $description = t('This receiver email does not match with the original email');
    $status = 'denied';
  }

  // Save transaction
  $transaction->description = $description;
  $transaction->status      = pgapi_get_status_id($status);
  pgapi_transaction_save($transaction);
}