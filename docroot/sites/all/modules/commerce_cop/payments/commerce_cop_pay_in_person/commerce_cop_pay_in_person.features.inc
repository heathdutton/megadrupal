<?php
/**
 * @file
 * commerce_cop_pay_in_person.features.inc
 */

/**
 * Implements hook_commerce_custom_offline_payments().
 */
function commerce_cop_pay_in_person_commerce_custom_offline_payments() {
  $items = array(
    'in_person' => array(
      'id' => 'in_person',
      'title' => 'Pay in person',
      'description' => 'Pay in person according to the instructions provided to the user.',
      'information' => 'Pay in person instructions.',
      'format' => 'plain_text',
      'status' => 1,
      'checkout' => 1,
      'terminal' => 1,
      'fieldable' => 1,
    ),
  );
  return $items;
}
