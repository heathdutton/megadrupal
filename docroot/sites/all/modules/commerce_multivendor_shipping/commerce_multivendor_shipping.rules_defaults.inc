<?php

/**
 * @file
 * Default rules configurations for Multivendor shipping.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_multivendor_shipping_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule();

  $rule->label = t('Apply shipping rate before order is saved');
  $rule->active = TRUE;

  $rule
    ->event('commerce_order_presave')
    ->condition('data_is', array(
      'data:select' => 'commerce-order:status',
      'op' => '==',
      'value' => 'cart',
    ))
    ->action('commerce_multivendor_shipping_rate_apply', array(
      'commerce_order:select' => 'commerce-order',
    ));

  $rules['commerce_multivendor_shipping_cart_update'] = $rule;

  return $rules;
}
