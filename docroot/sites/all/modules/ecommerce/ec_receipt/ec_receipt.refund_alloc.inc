<?php

/**
 * @file
 * Implement refund type allocation.
 */

/**
 * Implements hook_alloc_load().
 */
function ec_receipt_refund_alloc_load($erid) {
  return $erid;
}

/**
 * Implements hook_alloc_get_currency().
 */
function ec_receipt_refund_alloc_get_currency($erid) {
  if ($receipt = ec_receipt_load($erid)) {
    return $receipt->currency;
  }
}

/**
 * Implements hook_alloc_get_total().
 */
function ec_receipt_refund_alloc_get_total($erid) {
  if ($receipt = ec_receipt_load($erid)) {
    return $receipt->amount;
  }
}

/**
 * Implements hook_alloc_allocation().
 */
function ec_receipt_refund_alloc_allocation($erid, $balance) {
  if ($receipt = ec_receipt_load($erid)) {
    $receipt->status = RECEIPT_STATUS_REFUND_PENDING;
    ec_receipt_save($receipt);
    watchdog('ec_receipt', 'Receipt !erid refunded to customer.', array('!erid' => $erid));
  }
}
