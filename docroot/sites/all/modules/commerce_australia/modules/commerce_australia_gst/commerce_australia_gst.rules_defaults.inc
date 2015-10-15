<?php

/**
 * @file
 * Defines default tax components and rules.
 */

/**
 * Implements hook_default_rules_configuration_alter().
 */
function commerce_australia_gst_default_rules_configuration_alter(&$configs) {
  if (isset($configs['commerce_tax_type_gst'])) {
    $configs['commerce_tax_type_gst']->condition(
    rules_condition('commerce_order_compare_address',
      array(
        'commerce_order' => 'commerce-line-item:order',
        'address_field' => 'commerce_customer_billing|commerce_customer_address',
        'address_component' => 'country',
        'value' => 'AU',
      ))); 
  }
}
