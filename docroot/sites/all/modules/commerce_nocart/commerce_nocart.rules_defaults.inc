<?php
/**
 * @file
 * Default rule definitions for commerce_nocart module.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 *
 * Disable add-to-cart message rule for nocart-orders.
 */
function commerce_nocart_default_rules_configuration_alter(&$configs) {
  $rules_disabled_for_nocart_orders = array(
    'commerce_cart_add_to_cart_message',
    'commerce_cart_order_status_reset',
    // Disable the add-to-cart confirm dialog in Commerce Kickstart 7.x-2.x.
    'commerce_add_to_cart_confirmation_message',
  );
  foreach ($rules_disabled_for_nocart_orders as $rule_name) {
    if (isset($configs[$rule_name])) {
      $condition = rules_condition('data_is', array(
        'data:select' => 'commerce-order:commerce-nocart',
        'value' => '0',
      ));
      $configs[$rule_name]->condition($condition);
    }
  }
}
