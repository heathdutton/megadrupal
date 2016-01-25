<?php

/**
 * @file
 * Default rules configurations for Shipping.
 */


/**
 * Implements hook_default_rules_configuration().
 */
function commerce_rounding_default_rules_configuration() {
  $rules = array();

  // When an order's status is updating to "Shopping cart" from any other status,
  // its rounding line items will be deleted. This captures any cart contents
  // change via Add to Cart forms / the Shopping cart Views form. It also happens
  // when the customer cancels out of the checkout form.
  $rule = rules_reaction_rule();
  $rule->label = t('Delete rounding line items on shopping cart updates');
  $rule->active = TRUE;
  $rule->event('commerce_order_update')
    ->condition('data_is', array(
        'data:select' => 'commerce-order:status',
        'op' => '==',
        'value' => 'cart',
      ))
    ->condition(rules_condition('data_is', array(
        'data:select' => 'commerce-order-unchanged:status',
        'op' => '==',
        'value' => 'cart',
      ))->negate())
    ->action('commerce_rounding_delete_rounding_line_items', array(
        'commerce_order:select' => 'commerce-order',
      ));
  $rules['commerce_rounding_cart_update_delete'] = $rule;

  // When an order's status is updating to "Shopping cart" from any other status,
  // its rounding line item will be added.
  $rule = rules_reaction_rule();
  $rule->label = t('Order Total Rounding');
  $rule->active = TRUE;
  $rule->event('commerce_order_update')
    ->action('commerce_rounding_apply', array(
        'commerce_order:select' => 'commerce-order',
      ));
  $rules['commerce_rounding_order_total'] = $rule;
  
  return $rules;
}

/**
 * Returns an array of variables for use in rounding service components.
 */
function commerce_rounding_service_component_variables() {
  return array(
    'commerce_order' => array(
      'type' => 'commerce_order',
      'label' => t('Order'),
    ),
  );
}
