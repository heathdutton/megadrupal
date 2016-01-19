<?php

/**
 * @file
 * Default Rules for commerce_avatax.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_avatax_default_rules_configuration() {
  $rule = new RulesReactionRule();
  $rule->label = 'Calculate Sales Tax using AvaTax';
  $rule->active = TRUE;
  $rule->event('commerce_order_presave');
  $rule->condition(rules_condition('data_is_empty', array(
    'data:select' => 'commerce-order-unchanged',
  ))->negate());
  $rule->condition(rules_condition('data_is', array(
    'data:select' => 'commerce-order-unchanged:status',
    'op' => 'IN',
    'value' => array(
      'checkout_review' => 'checkout_review',
    ),
  ))->negate());
  $rule->condition(rules_condition('data_is', array(
    'data:select' => 'commerce-order:status',
    'op' => 'IN',
    'value' => array(
      'checkout_review' => 'checkout_review',
    ),
  )));
  $rule->action(rules_action('commerce_avatax_calculate_sales_tax', array(
    'order:select' => 'commerce-order',
  )));
  $rules['commerce_avatax_calculate_sales_tax'] = $rule;

  $rule = new RulesReactionRule();
  $rule->label = 'Delete AvaTax line items from order';
  $rule->active = TRUE;
  $rule->event('commerce_order_presave');
  $rule->condition(rules_condition('data_is', array(
    'data:select' => 'commerce-order:status',
    'op' => 'IN',
    'value' => array(
      'checkout_review' => 'cart',
    ),
  )));
  $rule->condition(rules_condition('data_is_empty', array(
    'data:select' => 'commerce-order-unchanged',
  ))->negate());
  $rule->condition(rules_condition('data_is', array(
    'data:select' => 'commerce-order-unchanged:commerce-order-total:amount',
    'op' => '==',
    'value' => 'commerce-order:commerce-order-total:amount',
  ))->negate());
  $rule->action(rules_action('commerce_avatax_delete_avatax_line_items', array(
    'order:select' => 'commerce-order',
  )));
  $rules['commerce_avatax_delete_avatax_line_items'] = $rule;

  return $rules;
}
