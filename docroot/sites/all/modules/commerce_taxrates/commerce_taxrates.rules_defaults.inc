<?php

/**
 * @file
 * Default Rules for commerce_taxrates.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_taxrates_default_rules_configuration() {
  $rule = new RulesReactionRule();
  $rule->label = 'Calculate sales tax using TaxRates';
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
  $rule->action(rules_action('commerce_taxrates_calculate_sales_tax', array(
    'order:select' => 'commerce-order',
  )));
  $rules['commerce_taxrates_calculate_sales_tax'] = $rule;

  $rule = new RulesReactionRule();
  $rule->label = 'Delete TaxRate line items from order';
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
  $rule->action(rules_action('commerce_taxrates_delete_taxrates_line_items', array(
    'order:select' => 'commerce-order',
  )));
  $rules['commerce_taxrates_delete_line_items'] = $rule;

  return $rules;
}
