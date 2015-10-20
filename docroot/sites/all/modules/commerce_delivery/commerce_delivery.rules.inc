<?php

/**
 * @file
 * Rules integration for deliveries.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_delivery_rules_event_info() {
  $events = array();

  $events['commerce_delivery_insert'] = array(
    'label' => t('After saving a new delivery'),
    'group' => t('Commerce Delivery'),
    'variables' => entity_rules_events_variables('commerce_delivery', t('Created delivery')),
  );
  $events['commerce_delivery_update'] = array(
    'label' => t('After updating an existing delivery'),
    'group' => t('Commerce Delivery'),
    'variables' => entity_rules_events_variables('commerce_delivery', t('Updated delivery'), TRUE),
  );
/*  $events['commerce_delivery_delete'] = array(
    'label' => t('After deleting a delivery'),
    'group' => t('Commerce Delivery'),
    'variables' => commerce_delivery_rules_event_variables(t('Deleted delivery')),
    'access callback' => 'commerce_delivery_rules_access',
  );
 */
  return $events;
}

/**
 * Returns a variables array for delivery events.
 *
 * @param $label
 *   The label for the primary delivery variable.
 * @param $unchanged
 *   Boolean indicating whether or not to include the unchanged order.
 * @param $skip_save
 *   Boolean indicating whether or not the primary delivery variable should skip
 *     saving after event execution.
 */
function commerce_delivery_rules_event_variables($label, $unchanged = FALSE, $skip_save = FALSE) {
  $variables = array(
    'delivery' => array(
      'type' => 'commerce_delivery',
      'label' => $label,
      'skip save' => $skip_save,
    ),
  );

  if ($unchanged) {
    $variables['delivery_unchanged'] = array(
      'type' => 'commerce_delivery',
      'label' => t('Unchanged delivery'),
      'skip save' => TRUE,
      'handler' => 'rules_events_entity_unchanged',
    );
  }

  return $variables;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_delivery_rules_condition_info() {
  $conditions = array();

  $conditions['commerce_delivery_order_has_shippable_item'] = array(
    'label' => t('Order has shippable items'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Delivery'),
  );
  $conditions['commerce_delivery_order_has_status'] = array(
    'label' => t('Order has status'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'status' => array(
        'type' => 'list<text>',
        'label' => t('Order status'),
        'options list' => 'commerce_order_status_options_list',
        'description' => 'Select status (multiple allowed).',
        'restriction' => 'input',
      ),
    ),
    'group' => t('Delivery'),
  );
  $conditions['commerce_delivery_item_is_shippable'] = array(
    'label' => t('Line item is shippable'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Delivery'),
  );
  $conditions['commerce_delivery_item_is_orphan'] = array(
    'label' => t('Line item is not in any delivery'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Delivery'),
  );
  $conditions['commerce_delivery_active_delivery_exists'] = array(
    'label' => t('Order has active deliveries'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Delivery'),
  );

  return $conditions;
}

/**
 * Rules condition callback:
 * checks to see if the given order has shippable items
 */
function commerce_delivery_order_has_shippable_item($order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $line_items = $order_wrapper->commerce_line_items->value();
  foreach ($line_items as $item) {
    if (commerce_delivery_item_is_shippable($item)) {
      return TRUE;
    };
  }
  return FALSE;
}

/**
 * Rules condition callback:
 * checks to see if the given order has a selected status
 */
function commerce_delivery_order_has_status($order, $status_list) {
  if (in_array($order->status, $status_list)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Rules condition callback:
 * checks to see if the given line item is shippable
 *
 * @param $line_item
 *
 * @return
 *   TRUE or FALSE
 */
function commerce_delivery_item_is_shippable($line_item) {
  $field_id = variable_get('commerce_delivery_shippable_field', '');
  $field_info = field_info_field_by_id($field_id);
  $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  if (isset($wrapper->commerce_product->$field_info['field_name'])) {
    return $wrapper->commerce_product->$field_info['field_name']->value();
  }
  return FALSE;
}

/**
 * Rules condition callback:
 * checks to see if the given item is in any delivery
 */
function commerce_delivery_item_is_orphan($line_item) {
  if ($line_item->delivery_ref->value() == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Rules condition callback:
 * checks to see if the given order has delivery with active status
 */
function commerce_delivery_active_delivery_exists($order) {
  // getting this order deliveries.
  $deliveries = commerce_delivery_get_deliveries($order->order_id, 'active');
  if (count($deliveries) > 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_delivery_rules_action_info() {
  $actions = array();

  $actions['commerce_delivery_assign_delivery'] = array(
    'label' => t('Assign line item to delivery'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Commerce Line Item'),
  );
  $actions['commerce_delivery_assign_orphans'] = array(
    'label' => t('Assign orphan line items to delivery'),
    'parameter' => array(
      'delivery' => array(
        'type' => 'commerce_delivery',
        'label' => t('Commerce Delivery'),
      ),
    ),
    'group' => t('Commerce Line Item'),
  );
  return $actions;
}

/**
 * Rules action callback:
 * assign line items to order default delivery.
 */
function commerce_delivery_assign_delivery($line_item) {
  // TODO: check if line_item has order_id set
  // getting this order deliveries.
  $deliveries = commerce_delivery_get_deliveries($line_item->order_id, 'active');
  // reloading the line item or saving gives error
  $item = current(entity_load('commerce_line_item', array($line_item->line_item_id)));
  $wrapper = entity_metadata_wrapper('commerce_line_item', $item);
  // setting the deliver ref
  $wrapper->delivery_ref = current($deliveries['commerce_delivery'])->delivery_id;
  commerce_line_item_save($item);
}

/**
 * Rules action callback:
 * assign orphan line items to order default delivery.
 */
function commerce_delivery_assign_orphans($delivery) {
  // getting shippable items from this order
  $order = commerce_order_load($delivery->order_id);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $items = $order_wrapper->commerce_line_items->value(); 
  foreach ($items as $line_item) {
    $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    // checking if it's orphan
    if (commerce_delivery_item_is_orphan($wrapper) && commerce_delivery_item_is_shippable($wrapper)) {
      $wrapper->delivery_ref = $delivery->delivery_id;
      commerce_line_item_save($line_item);
    }
  }
}

