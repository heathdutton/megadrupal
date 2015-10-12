<?php

/*
 * @file
 * Default coupon rules.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_coupon_default_rules_configuration() {
  $rules = array();

  // Add validation rule component to check the number of uses of a coupon
  $rule = rule(array(
    'commerce_coupon' => array(
      'type' => 'commerce_coupon',
      'label' => t('Coupon'),
    ),
    'number_of_redemptions' => array(
      'type' => 'integer',
      'label' => t('Number Of Redemptions'),
    ),
      ));
  $rule->label = t('Coupon Validation: Set the coupon as invalid if number of uses is reached');
  $rule
    ->condition('entity_has_field', array('entity:select' => 'commerce_coupon', 'field' => 'commerce_coupon_number_of_uses'))
    ->condition(rules_and()->condition(
          rules_or()->condition('data_is', array('data:select' => 'commerce_coupon:commerce-coupon-number-of-uses', 'op' => '<', 'value:select' => 'number-of-redemptions'))
          ->condition('data_is', array('data:select' => 'commerce_coupon:commerce-coupon-number-of-uses', 'value:select' => 'number-of-redemptions'))
        )
  );
  $rule->action('drupal_message', array(
    'message' => t('Sorry, the maximum number of redemptions for this coupon has been reached.'),
    'type' => 'error',
  ));
  $rule->action('commerce_coupon_action_is_invalid_coupon', array());
  $rules['commerce_coupon_validate_uses_of_coupon_component'] = $rule;

  // Reaction on the validation event for check uses of coupons:
  $rule = rules_reaction_rule();
  $rule->label = t('Coupon Validation: Check the number of redemptions');
  $rule->active = TRUE;

  $rule
    ->event('commerce_coupon_validate')
    ->condition('entity_has_field', array('entity:select' => 'coupon', 'field' => 'commerce_coupon_number_of_uses'))
    ->action('commerce_coupon_action_get_coupon_uses', array('commerce_coupon:select' => 'coupon'))
    ->action('component_commerce_coupon_validate_uses_of_coupon_component', array(
      'commerce_coupon:select' => 'coupon',
      'number_of_redemptions:select' => 'number-of-uses'
    ));
  $rules['commerce_coupon_validate_uses_of_coupon'] = $rule;

  // When removing a product from the cart, trigger deleting the coupon.
  $rule = rules_reaction_rule();
  $rule->label = t('Remove coupons from empty carts');
  $rule->active = TRUE;

  $rule
    ->event('commerce_cart_product_remove')
    ->condition('commerce_order_compare_total_product_quantity', array('entity:select' => 'commerce_order', 'operator' => '=', 'value' => 0))
    ->action('commerce_coupon_action_remove_all_coupons_from_order', array('commerce_order:select' => 'commerce_order'));

  $rules['commerce_coupon_remove_coupons_from_empty_carts'] = $rule;

  return $rules;
}
