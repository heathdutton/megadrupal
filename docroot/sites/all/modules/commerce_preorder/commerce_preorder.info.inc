<?php

/**
 * @file
 * Provides entity metadata.
 */

/**
 * Implements hook_entity_property_info_alter()
 */
function commerce_preorder_entity_property_info_alter(&$info) {
  $price_property_info = commerce_price_field_data_property_info();

  // Commerce Order
  if (isset($info['commerce_order']) && isset($info['commerce_order']['properties'])) {
    $order_properties = &$info['commerce_order']['properties'];

    $order_properties['preOrderCheckoutURL'] = array(
      'label' => t('Pending pre-order checkout url'),
      'type' => 'uri',
      'description' => t('Provides a url to checkout with an order that is in the state "cart".'),
      'getter callback' => 'commerce_preorder_order_property_get',
      'computed' => TRUE,
    );

    $order_properties['preOrderLineItems'] = array(
      'label' => t('Pre-order line items'),
      'type' => 'list<commerce_line_item>',
      'description' => t('The pre-ordered line items on an order.'),
      'getter callback' => 'commerce_preorder_order_property_get',
      'computed' => TRUE,
    );

    $order_properties['preOrderTotal'] = array(
      'label' => t('Pre-order total'),
      'type' => 'commerce_price',
      'description' => t('The sum of the order total\'s preorder components.'),
      'getter callback' => 'commerce_preorder_order_property_get',
      'property info' => $price_property_info,
      'computed' => TRUE,
    );
  }

  // Commerce line item
  if (isset($info['commerce_line_item']) && isset($info['commerce_line_item']['properties'])) {
    $line_properties = &$info['commerce_line_item']['properties'];

    $line_properties['isPreOrdered'] = array(
      'label' => t('Is pre-ordered'),
      'type' => 'boolean',
      'description' => t('TRUE if the line item has preorder price components.'),
      'getter callback' => 'commerce_preorder_line_item_property_get',
      'computed' => TRUE,
    );

    $line_properties['preOrderUnitPrice'] = array(
      'label' => t('Pre-order unit price'),
      'type' => 'commerce_price',
      'description' => t('The sum of the line item unit price\'s preorder components.'),
      'getter callback' => 'commerce_preorder_line_item_property_get',
      'property info' => $price_property_info,
      'computed' => TRUE,
    );

    $line_properties['preOrderTotal'] = array(
      'label' => t('Pre-order total'),
      'type' => 'commerce_price',
      'description' => t('The sum of the line item total\'s preorder components.'),
      'getter callback' => 'commerce_preorder_line_item_property_get',
      'property info' => $price_property_info,
      'computed' => TRUE,
    );
  }
}
