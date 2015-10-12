<?php
/**
 * @file
 * commerce_eu_vat_rc.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_eu_vat_rc_default_rules_configuration() {

  $rule = rules_and(array(
    'commerce_order' => array(
      'label' => t('Commerce Order'),
      'type' => 'commerce_order',
    ),
  ));
  $rule->label = t('Reverse Charge - Order has valid VAT number');

  $rule
    ->condition('entity_has_field', array('entity:select' => 'commerce-order', 'field' => 'commerce_customer_eu_vat_rc'))
    ->condition('entity_has_field', array('entity:select' => 'commerce-order:commerce-customer-eu-vat-rc', 'field' => 'commerce_vat_number'))
    ->condition(rules_condition('data_is_empty', array('data:select' => 'commerce-order:commerce-customer-eu-vat-rc:commerce-vat-number'))->negate());

  $configs['commerce_eu_vat_rc_order_has_vat_number'] = $rule;

  return $configs;
}
