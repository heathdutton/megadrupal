<?php

/**
 * @file   commerce_order_counter.tokens.inc
 * @author Augustin Delaporte <guss.tissier@gmail.com>
 *         António P. P. Almeida <appa@perusio.net>
 * @date   Wed Mar  6 19:07:29 2013
 *
 * @brief  Token integration for commerce_order_counter.
 *
 */

/**
 * Implements hook_token_info().
 */
function commerce_order_counter_token_info() {
  $info = array();

  $info['tokens']['site']['order-counter'] = array(
    'name' => t('Order counter'),
    'description' => t('Completed orders code or number.'),
  );

  return $info;
} // commerce_order_counter_token_info

/**
 * Implements hook_tokens().
 */
function commerce_order_counter_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $replacements = array();

  if ($type == 'site') {
    if (isset($tokens['order-counter'])) {
      $replacements[$tokens['order-counter']] = commerce_order_counter_order_get();
    }
  }
  return $replacements;
} // commerce_order_counter_tokens
