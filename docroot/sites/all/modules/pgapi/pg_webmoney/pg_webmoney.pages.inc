<?php

/*
 * @file
 * pg_webmoney.pages.inc
 * Page callbacks for Webmoney module
 */

/**
 * Page callback for Webmoney server response
 */
function pg_webmoney_done_payment() {
  // Get response from wm server
  drupal_add_http_header('Content-type', 'text/html; charset=iso-8859-1');
  $txnid              = $_POST['LMI_PAYMENT_NO'];
  $purse              = $_POST['LMI_PAYEE_PURSE'];
  $price              = $_POST['LMI_PAYMENT_AMOUNT'];
  $LMI_MODE           = $_POST['LMI_MODE'];
  $LMI_SYS_INVS_NO    = $_POST['LMI_SYS_INVS_NO'];
  $LMI_SYS_TRANS_NO   = $_POST['LMI_SYS_TRANS_NO'];
  $LMI_SYS_TRANS_DATE = $_POST['LMI_SYS_TRANS_DATE'];
  $LMI_SECRET_KEY     = $_POST['LMI_SECRET_KEY'];
  $LMI_PAYER_PURSE    = $_POST['LMI_PAYER_PURSE'];
  $LMI_PAYER_WM       = $_POST['LMI_PAYER_WM'];
  $prerequest         = $_POST['LMI_PREREQUEST'];
  $hash               = $_POST['LMI_HASH'];
  $tocheck            = drupal_strtoupper(md5($purse . $price . $txnid . $LMI_MODE . $LMI_SYS_INVS_NO . $LMI_SYS_TRANS_NO . $LMI_SYS_TRANS_DATE . $LMI_SECRET_KEY . $LMI_PAYER_PURSE . $LMI_PAYER_WM));

  // Get variables
  $transaction  = pgapi_transaction_load($txnid);
  $wmtype       = 'WM' . drupal_substr($purse, 0, 1);
  $stored_price = round($transaction->extra['prices'][$wmtype], 2);
  $wallet       = db_query('SELECT * FROM {pg_webmoney_rates} WHERE type = :type ', array(':type' => $wmtype))->fetchObject();
  $result       = 'YES';

  // Check recieved data
  if ($stored_price != $price) {
    $result = t('!price number does not match the original price !or_price !txnid', array('!price' => $price, 'or_price' => $stored_price, '!txnid' => $txnid));
    $status = 'failed';
  }
  if ($wallet->purse != $purse) {
    $result = t('Your wallet number !number does not match the original !original', array('!number' => $purse, '!original' => $wallet->purse));
    $status = 'failed';
  }

  // If not test mode
  if (!$LMI_MODE) {
    if ($hash != $tocheck) {
      $result = t('Security check failed');
      $status = 'denied';
    }
  }

  // Save transaction
  if ($prerequest != 1) {
    if ($result == "YES") {
      $transaction->description  = t('Payment from WM %WM and Wallet %purse', array('%WM' => $LMI_PAYER_WM, '%purse' => $LMI_PAYER_PURSE));
      $transaction->status = pgapi_get_status_id('completed');
    }
    else {
      $transaction->description = $result;
      $transaction->status = pgapi_get_status_id($status);
    }
    pgapi_transaction_save($transaction);
  }
  echo $result;
}

/**
 * Page callback for success/failure webmoney server response
 */
function pg_webmoney_payment_end($type) {
  global $user;
  $txnid = $_POST['LMI_PAYMENT_NO'];
  $transaction = pgapi_transaction_load($txnid);
  $url = pgapi_get_redirect_url($transaction);
  drupal_goto($url);
}