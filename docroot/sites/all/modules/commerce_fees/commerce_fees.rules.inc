<?php

/**
 * @file
 * Rules integration for the Commerce Fees module.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_fees_rules_event_info() {
  $items = array();

  $items['commerce_fees_order'] = array(
    'label' => t('Apply a fee to a given order'),
    'group' => t('Commerce fees'),
    'variables' => entity_rules_events_variables('commerce_order', t('Order', array(), array('context' => 'a drupal commerce order'))),
    'access callback' => 'commerce_order_rules_access',
  );

  return $items;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_fees_rules_action_info() {
  $actions = array();

  $actions['commerce_fees_apply'] = array(
    'label' => t('Apply a fee to an order'),
    'parameter' => array(
      'entity' => array(
        'label' => t('Entity'),
        'type' => 'entity',
        'wrapped' => TRUE,
      ),
      'fee' => array(
        'type' => 'text',
        'label' => t('Fee'),
        'description' => t('The fee to add.'),
        'options list' => 'commerce_fees_get_types',
      ),
      'amount' => array(
        'type' => 'decimal',
        'label' => t('Amount'),
        'description' => t('Specify a numeric amount to add to the unit price.'),
      ),
      'currency_code' => array(
        'type' => 'text',
        'label' => t('Currency'),
        'options list' => 'commerce_currency_get_code',
      ),
    ),
    'group' => t('Commerce Fees'),
  );

  // Add support for base rates including tax.
  if (module_exists('commerce_tax')) {
    $actions['commerce_fees_apply']['parameter']['include_tax'] = array(
      'type' => 'text',
      'label' => t('Include tax in this rate'),
      'description' => t('Saving a fee tax inclusive will bypass later calculations for the specified tax.'),
      'options list' => 'commerce_fees_get_tax_rate_options',
    );
  }

  return $actions;
}

/**
 * Action: Apply a custom fee to an order.
 */
function commerce_fees_apply(EntityDrupalWrapper $order_wrapper, $fee, $amount, $currency_code, $include_tax = NULL) {
  $order = $order_wrapper->value();
  $price_component_type = $fee;

  // Built the unit_price.
  $base_rate = array(
    'amount' => $amount,
    'currency_code' => $currency_code,
    'data' => array(),
  );

  // If the data array specifies an included tax, include it now.
  if (module_exists('commerce_tax') && !empty($include_tax) && $tax_rate = commerce_tax_rate_load($include_tax)) {
    // Reverse apply the tax.
    $tax_amount = $base_rate['amount'] - ($base_rate['amount'] / (1 + $tax_rate['rate']));
    $tax_amount = commerce_tax_rate_round_amount($tax_rate, $tax_amount);

    // Add a base price to the data array.
    $component = array(
      'amount' => $base_rate['amount'] - $tax_amount,
      'currency_code' => $base_rate['currency_code'],
      'data' => array(),
    );

    $base_rate['data'] = commerce_price_component_add($base_rate, $price_component_type, $component, TRUE, FALSE);

    // Add the tax to the data array.
    $component['amount'] = $tax_amount;
    $component['data']['tax_rate'] = $tax_rate;

    $base_rate['data'] = commerce_price_component_add($base_rate, $tax_rate['price_component'], $component, TRUE);
  }
  else {
    $base_rate['data'] = commerce_price_component_add($base_rate, $price_component_type, $base_rate, TRUE, FALSE);
  }

  // Delete any existing custom fee line items with the same name from
  // the order.
  commerce_fees_delete_fee_line_item($order, $fee, TRUE);

  // Create a new custom fee line item with the amount from the form.
  $line_item = commerce_fees_line_item_new($fee, $base_rate, $order->order_id);

  // Save and add the line item to the order.
  commerce_fees_add_fee_line_item($line_item, $order, TRUE);
}
