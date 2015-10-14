<?php

/**
 * @file
 * Rules integration for multivendor shipping.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_multivendor_shipping_rules_action_info() {
  $actions = array();

  $actions['commerce_multivendor_shipping_rate_apply'] = array(
    'label' => t('Apply shipping rate to an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce Multivendor Shipping'),
  );

  return $actions;
}

/**
 * Action: Apply a shipping rate to an order.
 */
function commerce_multivendor_shipping_rate_apply($order) {

  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $entityreference_fields = commerce_info_fields('entityreference', 'commerce_product');
  $vendors = array();

  foreach ($order_wrapper->commerce_line_items as $delta => $liw) {
    if (in_array($liw->type->value(), commerce_product_line_item_types())) {
      foreach ($entityreference_fields as $field_name => $info) {
        $type = $liw->commerce_product->type->value();
        $types = array_flip($info['bundles']['commerce_product']);
        if ($info['settings']['target_type'] == 'commerce_multivendor_shipping_vendor' && isset($types[$type])) {
          $vendor = $liw->commerce_product->$field_name->value();
          if (isset($vendor)) {
            if (empty($vendors[$vendor->id])) {
              $vendors[$vendor->id] = $vendor;
              $vendors[$vendor->id]->quantity = $liw->quantity->value();
            }
            else {
              $vendors[$vendor->id]->quantity += $liw->quantity->value();
            }
          }
        }
      }
    }
  }

  $shipping_amount = 0;
  foreach ($vendors as $id => $vendor) {
    // Calculate the rate for vendor and add to total shipping costs.
    if ($vendor->quantity > 0) {
      $shipping_amount += commerce_multivendor_shipping_rate_calculate($vendor);
    }
  }

  // Add or update the line item with the shipping cost.
  commerce_multivendor_shipping_service_add_line_item($order, $shipping_amount);

}

/**
 * @}
 */
