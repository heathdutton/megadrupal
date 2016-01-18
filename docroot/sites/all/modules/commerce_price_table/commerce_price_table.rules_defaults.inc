<?php

/**
 * @file
 * Default rule configurations for Cart.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_price_table_default_rules_configuration() {
  $rules = array();

  // Add a reaction rule to update a shopping cart order's status to "Shopping
  // cart" when a product is added to or removed from the order.
  $rule = rules_reaction_rule();

  $rule->label = t('Override price with price table');
  $rule->active = TRUE;

  $rule
    ->event('commerce_product_calculate_sell_price')
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item',
      'field' => 'commerce_product',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:commerce-product',
      'field' => 'field_price_table',
    ))
    ->action('commerce_price_table_set_price', array(
      'commerce_line_item:select' => 'commerce-line-item',
      'quantity:select' => 'commerce-line-item:quantity',
      'price_table:select' => 'commerce-line-item:commerce-product:field-price-table',
      'component_name' => 'base_price',
    ));

  $rule->weight = -10;

  $rules['commerce_price_table_override_price'] = $rule;

  return $rules;
}
