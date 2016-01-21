<?php

/**
 * @file
 * Rules integration.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_surcharge_rules_event_info() {
  $items = array();

  $items['commerce_surcharge_order'] = array(
    'label' => t('Credit card is chosen for an order'),
    'group' => t('commerce surcharge'),
    'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order'))),
    'access callback' => 'commerce_order_rules_access',
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_surcharge_rules_action_info() {
  $actions = array();

  $actions['commerce_surcharge_apply'] = array(
    'label' => t('Apply credit card surcharge to an order'),
    'parameter' => array(
      'entity' => array(
        'label' => t('Entity'),
        'type' => 'entity',
        'wrapped' => TRUE,
      ),
      'currency_code' => array(
        'type' => 'text',
        'label' => t('Currency'),
        'options list' => 'commerce_currency_get_code',
      ),
    ),
    'group' => t('commerce surcharge'),
  );

  return $actions;
}

/**
 * Action: Apply a custom fee to an order.
 */
function commerce_surcharge_apply(EntityDrupalWrapper $order_wrapper, $currency_code) {
  // Delete surcharge line item at first.
  $order = $order_wrapper->value();
  $order_price = $order_wrapper->commerce_order_total->amount->value();
  module_load_include('inc', 'commerce_surcharge', 'includes/commerce_surcharge.func');
  _commerce_surcharge_line_item_delete($order, COMMERCE_SURCHARGE_COMPONENT_TYPE, TRUE);

  // Add surcharge rate.
  // If current card is empty, return.
  $current_card = variable_get('commerce_surcharge_current_card', NULL);
  if (is_null($current_card) || variable_get('commerce_surcharge_enabled') == 0) {
    return;
  }

  // If current card is not enabled, return.
  if (variable_get('commerce_surcharge_card_' . $current_card) !== 1) {
    return;
  }

  $surcharge_base = variable_get('commerce_surcharge_base');
  $surcharge_card_rate = variable_get('commerce_surcharge_card_' . $current_card . '_rate');
  $surcharge_rate = $surcharge_base + $surcharge_card_rate;

  // Set surcharge price.
  $surcharge_mode = variable_get('commerce_surcharge_mode', 1);
  switch($surcharge_mode) {
    case 1:
      $surcharge_price = $order_price * $surcharge_rate;
      break;
    default:
      $surcharge_price = $surcharge_rate * 100;
  }

  // Return if surcharge price is invalid.
  if ($surcharge_price == 0) {
    return;
  }

  // Built the unit_price.
  $unit_price = array(
    //'amount' => $amount,
    'amount' => $surcharge_price,
    'currency_code' => $currency_code,
    'data' => array(),
  );

  // @see commerce_price_component_add().
  $unit_price['data'] = commerce_price_component_add($unit_price, COMMERCE_SURCHARGE_COMPONENT_TYPE, $unit_price, TRUE, FALSE);
  // Create a new custom fee line item with the amount from the form.
  $line_item = _commerce_surcharge_line_item_prepare(COMMERCE_SURCHARGE_COMPONENT_TYPE, $unit_price, $order->order_id);
  // Save and add the line item to the order.
  _commerce_surcharge_line_item_add($line_item, $order, TRUE);
}
