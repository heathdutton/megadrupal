<?php
/**
 * @file
 * Default Rules for the Commerce Stock Reserve module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function commerce_stock_reserve_default_rules_configuration() {

  $configs = array();

  // Reserve stock when the user has added a product to their cart.
  $rule = rules_reaction_rule();
  $rule->label = 'Reserve stock when a user adds a product to their cart';
  $rule->active = TRUE;
  $rule->tags = array('commerce_stock_reserve', 'stock_control');
  $rule->event('commerce_cart_product_add');
  $rule->condition('data_is', array(
    'data:select' => 'commerce-order:state',
    'op' => '==',
    'value' => 'cart',
  ));
  $rule->action('commerce_stock_reserve_reserve', array(
    'commerce_line_item:select' => 'commerce-line-item',
  ));
  $configs['commerce_stock_reserve_for_cart'] = $rule;

  // Reserve stock when the user has updated a product in their cart.
  $rule = rules_reaction_rule();
  $rule->label = 'Reserve stock when a user updates a product in their cart';
  $rule->active = TRUE;
  $rule->tags = array('commerce_stock_reserve', 'stock_control');
  $rule->event('commerce_line_item_update');
  $rule->condition('data_is', array(
      'data:select' => 'commerce-line-item:order:state',
      'op' => '==',
      'value' => 'cart',
    ));
  $rule->action('commerce_stock_reserve_reserve', array(
      'commerce_line_item:select' => 'commerce-line-item',
    ));
  $configs['commerce_stock_reserve_cart_update'] = $rule;

  // Release stock when the user has removed a product from their cart.
  $rule = rules_reaction_rule();
  $rule->label = 'Release reserved stock for cart';
  $rule->active = TRUE;
  $rule->tags = array('commerce_stock_reserve', 'stock_control');
  $rule->event('commerce_cart_product_remove');
  $rule->action('commerce_stock_reserve_release', array(
    'commerce_line_item:select' => 'commerce-line-item',
  ));
  $configs['commerce_stock_reserve_release_for_cart'] = $rule;

  // Release stock when the user completes checkout for an order.
  $rule = rules_reaction_rule();
  $rule->label = 'Release reserved stock when completing checkout';
  $rule->active = TRUE;
  $rule->tags = array('commerce_stock_reserve', 'stock_control');
  $rule->event('commerce_checkout_complete');
  $rule->action(
    rules_loop(array(
      'list:select' => 'commerce-order:commerce-line-items',
      'item:var' => 'line_item',
      'item:label' => t('Current line item'),
      'item:type' => 'commerce_line_item',
    ))
    ->action('commerce_stock_reserve_release', array(
      'commerce_line_item:select' => 'line-item',
    ))
  );
  $configs['commerce_stock_reserve_release_checkout'] = $rule;

  // Release reserved stock for canceled orders.
  $rule = rules_reaction_rule();
  $rule->label = 'Release reserved stock for canceled orders';
  $rule->active = TRUE;
  $rule->tags = array('commerce_stock_reserve', 'stock_control');
  $rule->event('commerce_order_update');
  // Check that the order is now in a 'canceled' state, when it was previously
  // in a 'cart' or 'checkout' state.
  $rule->condition('data_is', array(
      'data:select' => 'commerce-order:state',
      'value' => 'canceled',
    ))
    ->condition('data_is', array(
      'data:select' => 'commerce-order-unchanged:state',
      'op' => 'IN',
      'value' => array(
        'cart' => 'cart',
        'checkout' => 'checkout',
      ),
    ));
  // Release stock for the order's line items.
  $rule->action(
    rules_loop(array(
      'list:select' => 'commerce-order:commerce-line-items',
      'item:var' => 'line_item',
      'item:label' => t('Current line item'),
      'item:type' => 'commerce_line_item',
    ))
    ->action('commerce_stock_reserve_release', array(
      'commerce_line_item:select' => 'line-item',
    ))
  );
  $configs['commerce_stock_reserve_release_canceled'] = $rule;

  return $configs;
}


/**
 * Implements hook_default_rules_configuration().
 */
function commerce_stock_reserve_default_rules_configuration_alter(&$configs) {
  // The commerce_ssr (Commerce Simple Stock Rules) module defines default Rules
  // which validate stock when the user is adding a product to the cart or
  // beginning checkout. These need to be modified to account for users who have
  // already reserved some stock for that product. So here we add a condition
  // and action to both of these Rules, which validate according to the amount
  // of stock available to the user, i.e. the product's current stock PLUS what
  // the user has already reserved.
  if (isset($configs['rules_stock_validate_add_to_cart'])) {
    $rule = $configs['rules_stock_validate_add_to_cart'];
    $rule->condition(
        rules_condition('commerce_stock_reserve_is_enough_available', array(
          'commerce_product:select' => 'commerce-product',
          'stock_already_ordered:select' => 'stock-already-ordered',
        ))
        ->negate()
      )
      ->action('commerce_stock_add_to_cart_set_state', array(
        'stock_action' => '1',
        'message' => 'The maximum quantity available for <em>[commerce-product:title]</em> is [commerce-product:user-available-stock].',
        'approved_quantity:select' => 'approved-quantity',
      ));
    $rule->tags[] = 'commerce_stock_reserve';
  }
  if (isset($configs['rules_stock_validate_checkout'])) {
    $rule = $configs['rules_stock_validate_checkout'];
    $rule->condition(
        rules_condition('commerce_stock_reserve_is_enough_available', array(
          'commerce_product:select' => 'commerce-product',
          'stock_already_ordered:select' => 'stock-already-ordered',
        ))
        ->negate()
      )
      ->action('commerce_stock_checkout_state', array(
        'stock_action' => '1',
        'message' => 'The maximum quantity available for <em>[commerce-product:title]</em> is [commerce-product:user-available-stock].',
        'approved_quantity:select' => 'commerce-product:commerce-stock',
      ));
    $rule->tags[] = 'commerce_stock_reserve';
  }
}
