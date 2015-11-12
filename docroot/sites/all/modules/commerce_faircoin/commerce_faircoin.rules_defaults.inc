<?php
/**
 * @file
 * Rules defaults to restrict provision of faircoin payment option to FAIR only.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 */
function commerce_faircoin_default_rules_configuration_alter(&$configs) {
  // This payment method is only available when currency is FAC.
  $method_id = 'commerce_faircoin';
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
      'value' => 'FAC',
    ));
  }
}
