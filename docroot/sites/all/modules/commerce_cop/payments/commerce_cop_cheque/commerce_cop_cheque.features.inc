<?php
/**
 * @file
 * commerce_cop_cheque.features.inc
 */

/**
 * Implements hook_commerce_custom_offline_payments().
 */
function commerce_cop_cheque_commerce_custom_offline_payments() {
  $items = array(
    'commerce_cheque' => array(
      'id' => 'commerce_cheque',
      'title' => 'Cheque',
      'description' => 'Pay by cheque according to the instructions provided to the user.',
      'information' => 'Pay by cheque instructions.',
      'format' => 'plain_text',
      'status' => 1,
      'checkout' => 1,
      'terminal' => 1,
      'fieldable' => 1,
    ),
  );
  return $items;
}
