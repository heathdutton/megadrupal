<?php

/**
 * @file
 * Rules defaults to restrict provision of Fat Zebra payment option to AUD only
 */

/**
 * Implements hook_default_rules_configuration_alter().
 */
function commerce_fatzebra_default_rules_configuration_alter(&$configs) {
  // This payment method is only available then currency is AUD.
  $method_id = 'commerce_fatzebra';
  $method_rule_name = 'commerce_payment_' . $method_id;
  if (isset($configs[$method_rule_name])) {
    $configs[$method_rule_name]->condition('data_is', array(
      'data:select' => 'commerce-order:type',
      'op' => '==',
      'value' => 'commerce_order',
    ));
    $configs[$method_rule_name]->condition('data_is', array(
      'data:select' => 'commerce-order:commerce-order-total:currency-code',
      'op' => '==',
      'value' => 'AUD',
    ));
  }
}
