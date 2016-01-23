<?php

/*
 * @file
 * pg_rbkmoney.pages.inc
 * Page callbacks for RBK money module
 */

/**
 * Page callback for RBK money server response
 */
function pg_rbkmoney_done_payment() {

  // Set page header
  drupal_add_http_header('Content-type', 'text/html; charset=iso-8859-1');

  // Get data rbk money server
  $txnid              = $GLOBALS['_POST']['orderId'];
  $eshopId            = $GLOBALS['_POST']['eshopId'];
  $paymentId          = $GLOBALS['_POST']['paymentId'];
  $eshopAccount       = $GLOBALS['_POST']['eshopAccount'];
  $serviceName        = $GLOBALS['_POST']['serviceName'];
  $recipientAmount    = $GLOBALS['_POST']['recipientAmount'];
  $recipientCurrency  = $GLOBALS['_POST']['recipientCurrency'];
  $paymentStatus      = $GLOBALS['_POST']['paymentStatus'];
  $userName           = $GLOBALS['_POST']['userName'];
  $userEmail          = $GLOBALS['_POST']['userEmail'];
  $paymentData        = $GLOBALS['_POST']['paymentData'];
  $hash               = $GLOBALS['_POST']['hash'];

  // Get secred code
  $secretKey = variable_get('pg_rbkmoney_secretkey', '');
  $tocheck = md5($eshopId . '::' . $txnid . '::' . $serviceName . '::' . $eshopAccount . '::' . $recipientAmount . '::' . $recipientCurrency . '::' . $paymentStatus . '::' . $userName . '::' . $userEmail . '::' . $paymentData . '::' . $secretKey);

  // Load transaction
  $transaction = pgapi_transaction_load($txnid);

  // Build default transaction status
  $description = t('Payment from %User %Email was accepted. PaymentId: !paymentId', array('%User' => $userName, '%Email' => $userEmail, '!paymentId' => $paymentId));
  $status ='completed';

  // Check recieved data
  if ($recipientAmount != $recipientAmount) {
    $description = t('This number does not match the original price');
    $status = 'failed';
  }
  if ($hash != $tocheck) {
    $description = t('Security check failed');
    $status = 'denied';
  }
  if ($paymentStatus == 5) {
    // Save transaction
    $transaction->description = $description;
    $transaction->status = pgapi_get_status_id($status);
    pgapi_transaction_save($transaction);
  }
  echo 'OK';
}