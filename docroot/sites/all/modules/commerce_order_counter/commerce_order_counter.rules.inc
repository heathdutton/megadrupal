<?php

/**
 * @file   commerce_order_counter.rules.inc
 * @author Augustin Delaporte <guss.tissier@gmail.com>
 *         António P. P. Almeida <appa@perusio.net>
 * @date   Wed Mar  6 18:08:38 2013
 *
 * @brief Rules integration. Provides the rules action that increments the
 *        order counter.
 *
 */

/**
 * Implements hook_rules_action_info().
 *
 * Create the action to update the order number with the order-counter token.
 *
 */
function commerce_order_counter_rules_action_info() {
  $actions = array();

  $actions['commerce_order_counter_update_order_number'] = array(
    'label' => t('Update the order number'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order to update'),
      ),
      'order_number' => array(
        'type' => 'text',
        'label' => t('Order number'),
        'description' => t('Select the order number that you want to use to update the completed order. It uses the [site:order-counter] by default. Keep that value unless you\'re sure of what you\'re doing...'),
        'default value' => '[site:order-counter]',
      ),
    ),
    'group' => t('Commerce (contrib)'),
    'callbacks' => array(
      'execute' => 'commerce_order_counter_rules_update_number',
    ),
  );

  return $actions;
} // commerce_order_counter_rules_action_info

/**
 * Updates the order number using the current order and taking in
 * consideration the increment method.
 *
 * @param int $order
 *   The order ID.
 * @param int $number
 *   The order number.
 * @return int
 *   The new order number.
 *
 */
function commerce_order_counter_rules_update_number($order, $number) {
  // Update the order number using the $number parameter. By default, it's set
  // to the [site:order-counter] token.
  commerce_order_counter_number_update(
    $order, commerce_order_counter_order_set($number), FALSE, NULL, t('Order number updated via Rules.')
  );
} // commerce_order_counter_rules_update_number
