<?php

/*
 * @file
 * pg_roboxchange.pages.inc
 * Page callbacks for robokassa module.
 */

/**
 * Page callback for RoboxChange server response.
 */
function pg_roboxchange_done_payment() {
  // Get roboxchange server response.
  $txnid  = $_POST['InvId'];
  $amount = $_POST['OutSum'];
  $hash   = drupal_strtoupper($_POST['SignatureValue']);
  $mps2   = variable_get('pg_roboxchange_mps2', '');
  $crc    = drupal_strtoupper(md5($amount . ':' . $txnid . ':' . $mps2));

  // Load transaction.
  $transaction = pgapi_transaction_load($txnid);

  // Set success transaction params.
  $description = t('Payment was accepted.');
  $status = 'completed';
  $answer = "OK" . $txnid;

  // Check transaction.
  if ($transaction->extra['amount'] != $amount) {
    $description = t('This number does not match the original price');
    $status = 'denied';
    $answer = 'ERR';
  }
  if ($crc != $hash) {
    $description = t('Security check failed');
    $status = 'denied';
    $answer = 'ERR';
  }

  // Save transaction.
  $transaction->description = $description;
  $transaction->status = pgapi_get_status_id($status);
  pgapi_transaction_save($transaction);

  echo $answer;
  exit();
}

/**
 * Page callback for success/fail roboxchange server response.
 *
 * @param $type
 *   String values:
 *   - success
 *   - fail
 */
function pg_roboxchange_payment_end($type) {
  $txnid        = $_POST['InvId'];
  $transaction  = pgapi_transaction_load($txnid);
  $url          = pgapi_get_redirect_url($transaction);
  drupal_goto($url);
}
