<?php

/**
 * Implements hook_rules_action_info().
 */
function commerce_rounding_rules_action_info() {
  $actions = array();

  $actions['commerce_rounding_apply'] = array(
    'label' => t('Apply rounding to an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
        'save' => TRUE,
      ),
    ),
    'group' => t('Commerce Rounding'),
  );

  $actions['commerce_rounding_delete_rounding_line_items'] = array(
    'label' => t('Delete all rounding line items from an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce Rounding'),
  );

  return $actions;
}

/**
 * Checks an order for the existence of a rounding line item.
 *
 * @param $order
 *   The order to check for a rounding line item.
 * @param $service
 *   The machine-name of a particular rounding service to search for; if '-any-'
 *   the condition returns TRUE for any found rounding line item.
 */
function commerce_rounding_rules_line_item_exists($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // Loop over all the line items on the order.
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    // If the current line item is a rounding line item return TRUE.
    if ($line_item_wrapper->type->value() == 'rounding') {
      return TRUE;
    }
  }

  return FALSE;
}

/**
 * Action: Apply a rounding rate to an order.
 */
function commerce_rounding_apply($order) {
  // Delete any existing rounding line items from the order.
  commerce_rounding_delete_rounding_line_items($order, TRUE);

  // Save and add the line item to the order.
  commerce_rounding_add_rounding_line_item($order, TRUE);
}

/**
 * @}
 */
