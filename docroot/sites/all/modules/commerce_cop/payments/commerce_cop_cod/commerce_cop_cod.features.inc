<?php
/**
 * @file
 * commerce_cop_cod.features.inc
 */

/**
 * Implements hook_commerce_custom_offline_payments().
 */
function commerce_cop_cod_commerce_custom_offline_payments() {
  $items = array(
    'commerce_cod' => array(
      'id' => 'commerce_cod',
      'title' => 'Cash on Delivery',
      'description' => 'Collect the payment on products delivery.',
      'information' => 'Pay when you receive order products.',
      'format' => 'plain_text',
      'status' => 1,
      'checkout' => 1,
      'terminal' => 1,
      'fieldable' => 1,
    ),
  );
  return $items;
}
