<?php

/**
 * @file
 * Default rules configurations for Commerce Urbano.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_urbano_default_rules_configuration() {
  $rules = array();

  // When an order is first paid in full we need to create the shipment in
  // Urbano.
  $rule = rules_reaction_rule();

  $rule->label = t('Urbano: Create shipment');
  $rule->active = TRUE;

  $rule
    ->event('commerce_payment_order_paid_in_full')
    ->condition('commerce_shipping_compare_shipping_service', array(
      'commerce_order:select' => 'commerce_order',
      'service' => 'urbano_home_delivery',
    ))
    ->action('commerce_urbano_set_shipment', array(
      'commerce_order:select' => 'commerce_order',
    ));

  $rules['commerce_urbano_create_shipment'] = $rule;

  return $rules;
}
