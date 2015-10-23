<?php
/**
 * @file
 * commerce_cop_ccod.features.inc
 */

/**
 * Implements hook_commerce_custom_offline_payments().
 */
function commerce_cop_ccod_commerce_custom_offline_payments() {
  $items = array(
    'commerce_ccod' => array(
      'id' => 'commerce_ccod',
      'title' => 'Credit card on Delivery',
      'description' => 'This order is waiting for the payment by Credit card on delivery.',
      'information' => 'Pay by Credit card when you receive order products.',
      'format' => 'plain_text',
      'status' => 1,
      'checkout' => 1,
      'terminal' => 1,
      'fieldable' => 1,
    ),
  );
  return $items;
}
