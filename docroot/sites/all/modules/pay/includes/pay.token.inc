<?php

/**
 * @file
 * Pay module tokens support.
 */

/**
 * Implementation of hook_token_list().
 */
function pay_token_list($type = 'all') {

  // Payment form tokens.
  if ($type == 'pay_form' || $type == 'all') {
    $key = 'pay_form';
    $tokens[$key]['title']      = t('Form title');
    $tokens[$key]['pfid']       = t('Payment form ID');
    $tokens[$key]['handler']    = t('Payment form handler');
    $tokens[$key]['link']       = t('Link to form');
    $tokens[$key]['total_goal'] = t('Total goal amount');
    $tokens[$key]['min_amount'] = t('Minimum payment amount');
    $tokens[$key]['max_amount'] = t('Maximum payment amount');
  }

  // Payment transaction tokens.
  if ($type == 'pay_transaction' || $type == 'all') {
    $key = 'pay_transaction';
    $tokens[$key]['pxid']       = t('Transaction ID');
    $tokens[$key]['mail']       = t('Payment email address');
    $tokens[$key]['notes']      = t('Notes');
    $tokens[$key]['total']      = t('Total amount of transaction');
    $tokens[$key]['total_paid'] = t('Total received for transaction');
    $tokens[$key]['form-pfid']  = t('Payment form ID');
    $tokens[$key]['form-title'] = t('Payment form title');
    $tokens[$key]['form-link']  = t('Link to payment form');
  }

  return $tokens;
}

/**
 * Implementation of hook_token_values().
 */
function pay_token_values($type = 'all', $object = NULL) {
}
