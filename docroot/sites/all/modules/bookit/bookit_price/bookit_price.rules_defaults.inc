<?php

/**
 * Implements hook_default_rules_configuration().
 */
function bookit_price_default_rules_configuration() {
  $rules = array();

  // Add booking price calculation rule.
  $rule = rules_reaction_rule();

  $rule->label = t('Calculate booking price');
  $rule->tags = array('Booking');
  $rule->active = TRUE;
  $rule->weight = -100;

  $rule
    ->event('commerce_product_calculate_sell_price')
    ->condition('entity_is_of_bundle', array(
      'entity:select' => 'commerce-line-item',
      'type' => 'commerce_line_item',
      'bundle' => array('bookit'),
    ))
    ->action('bookit_price_calculate_amount', array(
      'commerce_line_item:select' => 'commerce-line-item',
    ));

  $rules['bookit_price_calculate_amount'] = $rule;

  return $rules;
}
