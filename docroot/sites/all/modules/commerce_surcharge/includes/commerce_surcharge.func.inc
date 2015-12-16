<?php

/**
 * @file
 * Commerce surcharge helper function.
 */

/**
 * @param $type
 * @param $unit_price
 * @param $order_id
 * @param array $data
 * @return bool
 */
function _commerce_surcharge_line_item_prepare($type, $unit_price, $order_id, $data = array()) {
  // Create the new line item.
  $line_item = entity_create('commerce_line_item', array(
    'type' => $type,
    'order_id' => $order_id,
    'quantity' => 1,
    'data' => $data,
  ));
  // Populates a custom fee line item with the specified values.
  _commerce_surcharge_line_item_populate($line_item, $type, $unit_price);
  // Return the line item.
  return $line_item;
}

/**
 * @param $line_item
 * @param $order
 */
function _commerce_surcharge_line_item_add($line_item, $order) {
  if (empty($order)) {
    return FALSE;
  }
  commerce_line_item_save($line_item);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_wrapper->commerce_line_items[] = $line_item;
  commerce_order_save($order);
  commerce_order_calculate_total($order);
}

function _commerce_surcharge_line_item_populate($line_item, $type, $unit_price) {
  // Use the label to store the display title of the custom fee.
  $line_item->line_item_label = t('Surcharge');

  // Populates a custom fee line item with the specified values.
  $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
  $line_item_wrapper->commerce_unit_price = $unit_price;

  // Ensure a quantity of 1.
  $line_item->quantity = 1;
}

/**
 * @param $order
 * @param $type
 * @param bool $skip_save
 */
function _commerce_surcharge_line_item_delete($order, $type, $skip_save = FALSE) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  // When deleting more than one line item, metadata_wrapper will give problems
  // if deleting while looping through the line items. So first remove from
  // order and then delete the line items.
  $line_item_ids = array();

  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    if ($line_item_wrapper->getBundle() == $type) {
      $price = $line_item_wrapper->commerce_unit_price->value();
      foreach ($price['data']['components'] as $component) {
        if ($line_item_wrapper->getBundle() == $type) {
          // Store its ID for later deletion and remove the reference from the
          // line item reference field.
          $line_item_ids[] = $line_item_wrapper->line_item_id->value();
          $order_wrapper->commerce_line_items->offsetUnset($delta);
        }
      }
    }
  }

  // If we found any fee type line items...
  if (!empty($line_item_ids)) {
    // First save the order to update the line item reference field value.
    if (!$skip_save) {
      commerce_order_save($order);
    }

    // Then delete the line items.
    commerce_line_item_delete_multiple($line_item_ids);
  }
}
