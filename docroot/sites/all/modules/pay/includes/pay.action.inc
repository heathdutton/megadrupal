<?php

/**
 * @file
 * Functions that support the Drupal actions/triggers system for payment
 * transactions and activities.
 */

/**
 * Implements hook_action_info().
 */
function pay_action_info() {
  $hooks = array();
  if (function_exists('pay_trigger_info')) {
    $hooks = pay_trigger_info();
    $hooks = $hooks['pay'];
  }
  $return = array(
    'pay_transaction_complete_action' => array(
      'type' => 'pay_transaction',
      'label' => t('Complete transaction'),
      'configurable' => FALSE,
      'triggers' => $hooks,
    ),
    'pay_transaction_cancel_action' => array(
      'type' => 'pay_transaction',
      'label' => t('Cancel transaction'),
      'configurable' => FALSE,
      'triggers' => $hooks,
    ),
    'pay_transaction_delete_action' => array(
      'type' => 'pay_transaction',
      'label' => t('Delete transaction'),
      'configurable' => FALSE,
      'triggers' => $hooks,
    ),
  );
  return $return;
}

/**
 * Implementation of a Drupal action.
 * Completes a pending or preauthorized transaction.
 */
function pay_transaction_complete_action(&$transaction, $context) {
  pay_transaction_do_action($transaction, 'complete', FALSE);
}

/**
 * Implementation of a Drupal action.
 * Cancels a pending or preauthorized transaction.
 */
function pay_transaction_cancel_action(&$transaction, $context) {
  pay_transaction_do_action($transaction, 'cancel', FALSE);
}

/**
 * Implementation of a Drupal action.
 * Deletes a transaction.
 */
function pay_transaction_delete_action(&$transaction, $context) {
  pay_transaction_do_action($transaction, 'delete', FALSE);
}

/**
 * Implements hook_views_bulk_operations_object_info().
 */
function pay_views_bulk_operations_object_info() {
  $info = array(
    'pay_transaction' => array(
      'type' => 'pay_transaction',
      'base_table' => 'pay_transaction',
      'load' => 'pay_transaction_load',
      'oid' => 'pxid',
      'title' => 'title',
      'hook' => 'pay',
    ),
  );
  return $info;
}
