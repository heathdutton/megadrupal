<?php

/**
 * @file
 * Default rule configurations for Pre-order.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_preorder_default_rules_configuration() {
  $rules = array();

  // Add a reaction rule to generate pending orders on checkout completion.
  $rule = rules_reaction_rule();

  $rule->label = t('Generate pre-order pending orders on checkout completion');
  $rule->tags = array('Commerce Pre-order');
  $rule->active = TRUE;
  $rule->weight = 10; // trigger after others

  $rule
    ->event('commerce_checkout_complete')
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-order',
      'field' => 'commerce_preorder_order',
    ))
    ->condition('data_is_empty', array(
      'data:select' => 'commerce-order:commerce-preorder-order',
    ))
    ->action('commerce_preorder_generate_pending_orders', array(
      'commerce_order:select' => 'commerce-order',
      'order_state' => 'preordered',
    ));

  $rules['commerce_preorder_checkout_complete_generate'] = $rule;


  // Create a new product pricing rule for pre-ordered products.
  $rule = rules_reaction_rule();

  $rule->label = t('Calculate pre-order price');
  $rule->tags = array('Commerce Pre-order');
  $rule->active = TRUE;
  $rule->weight = -10;

  // Add the action to invoke every tax rate's component matching this type.
  $rule
    ->event('commerce_product_calculate_sell_price')
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item',
      'field' => 'commerce_product',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:commerce-product',
      'field' => 'commerce_preorder_status',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:commerce-product',
      'field' => 'commerce_preorder_price',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:order',
      'field' => 'commerce_preorder_order',
    ))
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:commerce-product:status',
    ))->negate())
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:commerce-product:commerce-preorder-status',
    ))->negate())
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:commerce-product:commerce-preorder-price:amount',
    ))->negate())
    ->condition(rules_or()
      ->condition('data_is_empty', array(
        'data:select' => 'commerce-line-item:order-id',
      ))
      ->condition('data_is_empty', array(
        'data:select' => 'commerce-line-item:order:commerce-preorder-order',
      ))
    )
    ->action('commerce_line_item_unit_price_amount', array(
      'commerce_line_item:select' => 'commerce-line-item',
      'amount:select' => 'commerce-line-item:commerce-product:commerce-preorder-price:amount',
      'component_name' => 'preorder',
      'round_mode' => '1',
    ));

  $rules['commerce_preorder_set_preorder_price'] = $rule;


  // Create a new product pricing rule for pending pre-ordered products.
/** @todo: still needed? pending amount now set in generate orders function *******/

  $rule = rules_reaction_rule();

  $rule->label = t('Calculate pending pre-order price');
  $rule->tags = array('Commerce Pre-order');
  $rule->active = TRUE;
  $rule->weight = -10;

  // Add the action to invoke every tax rate's component matching this type.
  $rule
    ->event('commerce_product_calculate_sell_price')
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item',
      'field' => 'commerce_product',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:commerce-product',
      'field' => 'commerce_preorder_status',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:commerce-product',
      'field' => 'commerce_preorder_price',
    ))
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:order',
      'field' => 'commerce_preorder_order',
    ))
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:commerce-product:status',
    ))->negate())
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:commerce-product:commerce-preorder-status',
    ))->negate())
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:commerce-product:commerce-preorder-price:amount',
    ))->negate())
    ->condition(rules_condition('data_is_empty', array(
      'data:select' => 'commerce-line-item:order:commerce-preorder-order',
    ))->negate())

    ->action('commerce_line_item_unit_price_subtract', array(
      'commerce_line_item:select' => 'commerce-line-item',
      'amount:select' => 'commerce-line-item:commerce-product:commerce-preorder-price:amount',
      'component_name' => 'base_price',
      'round_mode' => '1',
    ));

  $rules['commerce_preorder_set_pending_preorder_price'] = $rule;


  return $rules;
}
