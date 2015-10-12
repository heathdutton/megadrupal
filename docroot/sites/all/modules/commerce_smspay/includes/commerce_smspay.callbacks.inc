<?php

/**
 * @file
 * Callbacks definitions.
 */

/**
 * Successful payment callback.
 * @return boolean
 */
function commerce_smspay_callback_success() {
  $post = json_encode($_POST);

  if (!isset($_POST['reference'])) {
    watchdog('commerce_smspay', 'No reference present on POST: '. $post);
    return FALSE;
  }

  $remote_id = $_POST['reference'];

  $transaction = commerce_smspay_load_transaction_by_remote_id($remote_id);

  if ($transaction) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = $_POST['status'];
    commerce_payment_transaction_save($transaction);
    commerce_payment_commerce_payment_transaction_insert($transaction);
  } else {
    watchdog('commerce_smspay', t('Transaction with remote id :remote_id not found.', array(':remote_id' => $remote_id)));
    return FALSE;
  }

  return TRUE;
}

/**
 * Failed payment callback.
 * @return boolean
 */
function commerce_smspay_callback_failure() {
  $post = json_encode($_POST);

  if (!isset($_POST['reference'])) {
    watchdog('commerce_smspay', 'No reference present on POST: '. $post);
    return FALSE;
  }

  $remote_id = $_POST['reference'];

  $transaction = commerce_smspay_load_transaction_by_remote_id($remote_id);

  if ($transaction) {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $transaction->remote_status = $_POST['status'];
    $transaction->message = t('Number: +@number; Error: @message');
    $transaction->message_variables = array(
      '@number' => $_POST['phone'],
      '@message' => $_POST['cancelReason'],
    );
    commerce_payment_transaction_save($transaction);
  } else {
    watchdog('commerce_smspay', t('Transaction with remote id :remote_id not found.', array(':remote_id' => $remote_id)));
    return FALSE;
  }

  return TRUE;
}

/**
 * Loads a transaction by remote id.
 * 
 * @param string $remote_id
 * @return boolean
 */
function commerce_smspay_load_transaction_by_remote_id($remote_id) {
  $result = db_select('commerce_payment_transaction', 't')
    ->fields('t')
    ->condition('remote_id', $remote_id, '=')
    ->execute()
    ->fetchAssoc();

  if (isset($result['transaction_id'])) {
    $transaction = commerce_payment_transaction_load($result['transaction_id']);
    return $transaction;
  }

  return FALSE;
}