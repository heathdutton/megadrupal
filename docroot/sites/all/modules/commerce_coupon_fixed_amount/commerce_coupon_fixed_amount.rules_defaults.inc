<?php

/**
 * @file
 * Coupon fixed amount default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_coupon_fixed_amount_default_rules_configuration() {
  $rules = array();
  // Create fixed amount rule
  $rule = rules_reaction_rule(array(), array(
    'commerce_coupon_line_item' => array(
      'type' => 'commerce_coupon_line_item',
      'label' => 'commerce coupon line item')
    )
  );

  $rule->label = t('Redeem a coupon with fixed amount');
  $rule->active = TRUE;

  $rule
    ->event('commerce_coupon_redeem')
    ->condition('data_is', array('data:select' => 'coupon:type', 'op' => '=', 'value' => 'commerce_coupon_fixed'))
    ->condition('entity_has_field', array('entity:select' => 'coupon', 'field' => 'commerce_coupon_fixed_amount'))
    ->condition('entity_has_field', array('entity:select' => 'commerce_order', 'field' => 'commerce_coupon_order_reference'))
    ->condition(rules_and()->condition('data_is_empty', array('data:select' => 'coupon:commerce-coupon-fixed-amount'))->negate() )
    ->condition('data_is', array('data:select' => 'coupon:commerce-coupon-fixed-amount:amount', 'op' => '>', 'value' => 0))
    ->condition('data_is', array('data:select' => 'coupon:is-active', 'op' => '=', 'value' => TRUE))
    ->action('list_add', array(
      'list:select' => 'commerce-order:commerce-coupon-order-reference',
      'item:select' => 'coupon',
      'unique' => 1,
    ))
    ->action('commerce_coupon_action_create_coupon_line_item', array(
      'commerce_coupon:select' => 'coupon',
      'commerce_order:select' => 'commerce-order',
      'amount:select' => 'coupon:commerce-coupon-fixed-amount:amount',
      'component_name:select' => 'coupon:price-component-name',
      'currency_code:select' => 'coupon:commerce-coupon-fixed-amount:currency-code'
    ));

  $rules['commerce_coupon_fixed_amount_calculate_amount'] = $rule;

  return $rules;
}
