<?php

/**
 * @file
 * Coupon rules integration file.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_coupon_rules_event_info() {
  $events = array();

  $events['commerce_coupon_validate'] = array(
    'label' => t('Validate a coupon'),
    'group' => t('Commerce Coupon'),
    'variables' => array(
      'coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('commerce coupon to validate')
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  $events['commerce_coupon_redeem'] = array(
    'label' => t('Redeem a coupon'),
    'group' => t('Commerce Coupon'),
    'variables' => array(
      'coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('commerce coupon to redeem')
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_coupon_rules_action_info() {
  $actions = array();

  $actions['commerce_coupon_action_is_valid_coupon'] = array(
    'label' => t('Set coupon as valid'),
    'parameter' => array(
      'override' => array(
        'type' => 'boolean',
        'label' => t('Override'),
        'description' => t('Normaly the validation result is concatenated by an AND operator. Here you can bypass this and set the value to true.'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_is_valid_coupon',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_is_valid_coupon',
    ),
  );

  $actions['commerce_coupon_action_is_invalid_coupon'] = array(
    'label' => t('Set coupon as invalid'),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_is_invalid_coupon',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_is_invalid_coupon',
    ),
  );

  $actions['commerce_coupon_action_create_coupon_line_item'] = array(
    'label' => t('Create coupon line item'),
    'parameter' => array(
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Commerce Coupon'),
      ),
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('commerce order'),
      ),
      'amount' => array(
        'type' => 'decimal',
        'label' => t('coupon amount'),
      ),
      'component_name' => array(
        'type' => 'text',
        'label' => t('Price component type'),
        'description' => t('Price components track changes to prices made during the price calculation process, and they are carried over from the unit price to the total price of a line item. When an order total is calculated, it combines all the components of every line item on the order. When the unit price is altered by this action, the selected type of price component will be added to its data array and reflected in the order total display when it is formatted with components showing. Defaults to base price, which displays as the order Subtotal.'),
        'options list' => 'commerce_price_component_titles',
        'default value' => 'base_price',
      ),
      'currency_code' => array(
        'type' => 'text',
        'label' => t('coupon amount currency code'),
      ),
    ),
    'provides' => array(
      'commerce_coupon_line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('commerce coupon line item'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_create_coupon_line_item',
    'callbacks' => array(
      'execute' => 'commerce_coupon_action_create_coupon_line_item',
    ),
  );

  $actions['commerce_coupon_action_get_coupons_for_order'] = array(
    'label' => t('Get coupons for order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
      ),
    ),
    'provides' => array(
      'order_coupons' => array(
        'type' => 'list<commerce_coupon>',
        'label' => t('Coupons attached to this order'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'base' => 'commerce_coupon_action_get_coupons_for_order',
  );

  $actions['commerce_coupon_action_get_coupon_uses'] = array(
    'label' => t('Get the redemption number of a coupon'),
    'parameter' => array(
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Commerce Coupon'),
      ),
    ),
    'group' => t('Commerce Coupon'),
    'provides' => array(
      'number_of_uses' => array(
        'type' => 'integer',
        'label' => t('number of uses'),
      ),
    ),
  );

  $actions['commerce_coupon_action_remove_coupon_from_order'] = array(
    'label' => t('Remove a coupon from an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce Order'),
      ),
      'commerce_coupon' => array(
        'type' => 'commerce_coupon',
        'label' => t('Commerce Coupon'),
      ),
    ),
    'group' => t('Commerce Coupon'),
  );

  $actions['commerce_coupon_action_remove_all_coupons_from_order'] = array(
    'label' => t('Remove all coupons from an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce Order'),
      ),
    ),
    'group' => t('Commerce Coupon'),
  );


  return $actions;
}

function commerce_coupon_action_is_valid_coupon($override = FALSE) {
  $validation_results = &drupal_static('commerce_coupon_action_validation_results');

  if (isset($validation_results) && $override == FALSE) {
    $validation_results = $validation_results && TRUE;
  }
  else {
    $validation_results[$commerce_coupon->coupon_id] = TRUE;
  }
}

function commerce_coupon_action_is_invalid_coupon() {
  $validation_results = &drupal_static('commerce_coupon_action_validation_results');
  $validation_results = FALSE;
}

function commerce_coupon_action_create_coupon_line_item($coupon, $commerce_order, $amount, $component_name, $currency_code) {
  if (!($commerce_order instanceof EntityMetadataWrapper)) {
    $commerce_order = entity_metadata_wrapper('commerce_order', $commerce_order);
  }

  $line_item = commerce_coupon_line_item_new($coupon, $commerce_order->order_id->raw());
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);

  // Set the unit price on the line item object.
  $line_item_wrapper->commerce_unit_price->amount = $amount * -1;
  $line_item_wrapper->commerce_unit_price->currency_code = $currency_code;

  // Add the base price to the components array.
  if ($coupon->is_active && !commerce_price_component_load($line_item_wrapper->commerce_unit_price->value(), $component_name)) {
    $line_item_wrapper->commerce_unit_price->data = commerce_price_component_add(
        $line_item_wrapper->commerce_unit_price->value(), $component_name, $line_item_wrapper->commerce_unit_price->value(), TRUE, FALSE
    );
  }

  commerce_line_item_save($line_item);

  $commerce_order->commerce_line_items[] = $line_item;

  return array('commerce_coupon_line_item' => $line_item);
}

/**
 * Action to get all uses for a coupon.
 */
function commerce_coupon_action_get_coupon_uses($coupon) {
  return array('number_of_uses' => commerce_coupon_get_number_of_uses($coupon->coupon_id));
}

/**
 * Action to remove a coupon from a given order.
 */
function commerce_coupon_action_remove_coupon_from_order($order, $coupon) {
  if (empty($coupon) || empty($order)) {
    return array();
  }
  commerce_coupon_remove_coupon_from_order($order, $coupon);
}

/**
 * Action to remove all coupons from a given order.
 */
function commerce_coupon_action_remove_all_coupons_from_order($order) {
  if (empty($order)) {
    return array();
  }
  commerce_coupon_remove_all_coupons_from_order($order);
}
