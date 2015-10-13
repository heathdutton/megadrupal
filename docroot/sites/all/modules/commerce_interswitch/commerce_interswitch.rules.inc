<?php
/**
 * @file
 * Rules Integration for Commerce Interswitch.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_interswitch_rules_action_info() {
  $actions = array();

  $actions['commerce_interswitch_add_interswitch_charges'] = array(
    'label' => t('Calculate and add Interswitch charge to order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to add charge to'),
      ),
    ),
    'group' => t('Commerce Interswitch'),
    'callbacks' => array(
      'execute' => 'commerce_interswitch_add_interswitch_charge',
    ),
  );

  $actions['commerce_interswitch_remove_interswitch_charges'] = array(
    'label' => t('Remove the Interswitch charge line item from an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to remove charge from'),
      ),
    ),
    'group' => t('Commerce Interswitch'),
    'callbacks' => array(
      'execute' => 'commerce_interswitch_remove_interswitch_charge',
    ),
  );

  return $actions;
}

/**
 * Rules action callback.
 *
 * Adds the Interswitch charge line item to an order.
 *
 * @param object $order
 *   The Commerce order.
 */
function commerce_interswitch_add_interswitch_charge($order) {
  $interswitch = commerce_payment_method_instance_load('interswitch|commerce_payment_interswitch');

  if (!$interswitch['settings']['enable_charges']) {
    return;
  }
  // Find and remove the Interswitch charge line item from the order.
  $interswitch_charge_line_item_id = '';
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if ('interswitch_charge' == $line_item_wrapper->type->value()) {
      $interswitch_charge_line_item_id = $line_item_wrapper->line_item_id->value();
      continue;
    }
  }
  if ($interswitch_charge_line_item_id) {
    commerce_line_item_delete($interswitch_charge_line_item_id);
  }

  // Add the Interswitch charge line item to the order.
  $interswitch_charge = commerce_interswitch_calculate_charge($order);
  if ($interswitch_charge) {
    $interswitch = commerce_payment_method_instance_load('interswitch|commerce_payment_interswitch');
    $interswitch_charge_line_item = commerce_line_item_new('interswitch_charge');
    $line_item_types = commerce_line_item_types();
    $interswitch_charge_line_item->line_item_label = $line_item_types['interswitch_charge']['name'];
    $interswitch_charge_line_item->order_id = $order->order_id;
    $interswitch_charge_line_item->quantity = 1;
    $currency_code = $interswitch['settings']['currency_code'];

    $unit_price = array(
      'amount' => $interswitch_charge,
      'currency_code' => $currency_code,
      'data' => array(
        'components' => array(
          array(
            'name' => 'interswitch_charge',
            'price' => array(
              'amount' => $interswitch_charge,
              'currency_code' => $currency_code,
              'data' => array(),
            ),
            'included' => TRUE,
          ),
        ),
      ),
    );
    $interswitch_charge_line_item->commerce_unit_price[LANGUAGE_NONE][] = $unit_price;

    commerce_line_item_save($interswitch_charge_line_item);

    $order_wrapper->commerce_line_items[] = $interswitch_charge_line_item;
    commerce_order_save($order);
  }
}

/**
 * Rules action callback.
 *
 * Removes the Interswitch charge line item from an order.
 *
 * @param object $order
 *   The Commerce order.
 */
function commerce_interswitch_remove_interswitch_charge($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if ('interswitch_charge' == $line_item_wrapper->type->value()) {
      commerce_line_item_delete($line_item_wrapper->line_item_id->value());
      commerce_order_save($order);
      return;
    }
  }
}
