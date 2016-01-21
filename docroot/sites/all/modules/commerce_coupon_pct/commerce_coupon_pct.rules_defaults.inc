<?php

/**
 * @file
 * Percentage coupon default rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_coupon_pct_default_rules_configuration() {
  $rules = array();

  $rule = rules_reaction_rule(array(), array(
    'commerce_coupon_line_item' => array(
      'type' => 'commerce_coupon_line_item',
      'label' => 'commerce coupon line item')
      )
  );

  $rule = rules_reaction_rule();
  $rule->label = t('Calculate coupon with percentage amount');
  $rule->active = TRUE;

  $rule
    ->event('commerce_coupon_redeem')
    ->condition('data_is', array('data:select' => 'coupon:type', 'op' => '=', 'value' => 'commerce_coupon_pct'))
    ->condition('entity_has_field', array('entity:select' => 'coupon', 'field' => 'commerce_coupon_percent_amount'))
    ->condition(rules_and()->condition('data_is_empty', array('data:select' => 'coupon:commerce-coupon-percent-amount'))->negate())
    ->condition('entity_has_field', array('entity:select' => 'commerce_order', 'field' => 'commerce_coupon_order_reference'))
    ->condition('data_is', array('data:select' => 'coupon:commerce-coupon-percent-amount', 'op' => '>', 'value' => 1))
    ->condition('data_is', array('data:select' => 'coupon:is-active', 'op' => '=', 'value' => TRUE))
    ->action('list_add', array(
      'list:select' => 'commerce-order:commerce-coupon-order-reference',
      'item:select' => 'coupon',
      'unique' => 1,
    ));

  $rules['commerce_coupon_pct_percent_amount'] = $rule;

  $rule = rules_reaction_rule();
  $rule->label = t('Apply percentage coupons to product line item');
  $rule->active = TRUE;

  $rule
    ->event('commerce_product_calculate_sell_price')
    ->condition(rules_condition('data_is_empty', array(
          'data:select' => 'commerce-line-item:line-item-id',
        ))->negate())
    ->condition('entity_has_field', array(
      'entity:select' => 'commerce-line-item:order',
      'field' => 'commerce_coupon_order_reference',
      ));


  $loop = rules_loop(array(
    'list:select' => 'commerce-line-item:order:commerce-coupon-order-reference',
    'item:var' => 'list_coupon',
    'item:label' => t('Current coupon'),
      ))
      ->action('commerce_coupon_pct_apply_to_product_line_item', array(
        'line_item:select' => 'commerce-line-item',
        'coupon:select' => 'list-coupon',
        'component_name:select' => 'list-coupon:price-component-name',
        'round_mode' => COMMERCE_ROUND_HALF_UP,
      ));

  $rule->action($loop);

  $rules['commerce_coupon_pct_apply_pct_coupons_to_line_item'] = $rule;

  return $rules;
}
