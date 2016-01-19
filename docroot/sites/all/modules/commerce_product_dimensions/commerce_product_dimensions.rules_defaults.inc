<?php

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_product_dimensions_default_rules_configuration() {
  $rule = rules_reaction_rule();
  $rule->label = 'Price dependant on product dimensions';
  $rule->active = TRUE;
  $rule->status = ENTITY_FIXED;
  $rule->event('commerce_product_calculate_sell_price')
    ->condition('text_matches', array(
      'text:select' => 'commerce-line-item:type',
      'match' => 'product_with_dimensions_',
      'operation' => 'starts',
    ))
    ->action('commerce_product_dimensions_get_factor', array(
      'commerce_line_item:select' => 'commerce_line_item',
      'provide' => array(
        'dimension_factor' => array('dimension_factor' => 'Multiplication Factor'),
        'dimension_base_price' => array('dimension_base_price' => 'Adjusted Base Price'),
      ),
    ))
    ->action('commerce_line_item_unit_price', array(
      'commerce_line_item:select' => 'commerce_line_item',
      'amount:select' => 'dimension-base-price',
      'component_name' => 'base_price',
      'round_mode' => 1.
    ))
    ->action('commerce_line_item_unit_price_multiply', array(
      'commerce_line_item:select' => 'commerce_line_item',
      'amount:select' => 'dimension-factor',
      'component_name' => 'base_price',
      'round_mode' => 1.
    ));

  $configs['commerce_product_dimensions_calculate_base_price'] = $rule;
  return $configs;
}
