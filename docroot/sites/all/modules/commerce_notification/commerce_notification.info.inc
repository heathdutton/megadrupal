<?php
/**
 * @file
 * Provides metadata for the commerce abandoned cart notification account and transaction entities.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_notification_entity_property_info() {
  $info = array();

  // Add meta-data about the commerce_abandoned_cart properties.
  $properties = &$info['commerce_notification']['properties'];

  $properties['id'] = array(
    'label' => t('Commerce Notification ID'),
    'description' => t('The internal numeric ID of the commerce abandoned cart.'),
    'type' => 'integer',
    'schema field' => 'id',
  );

  $properties['uid'] = array(
    'label' => t('User ID that created the order'),
    'description' => t('The internal numeric ID of the commerce abandoned cart.'),
    'type' => 'user',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'uid',
  );

  $properties['order_id'] = array(
    'type' => 'commerce_order',
    'label' => t('Order ID', array(), array('context' => 'a drupal commerce order')),
    'description' => t('The internal numeric ID of the order.'),
    'schema field' => 'order_id',
  );

  $properties['order_vid'] = array(
    'label' => t('User ID that created the order'),
    'description' => t('Order revision ID of the order revision that was current when this notification was sent.'),
    'type' => 'commerce_order',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'order_vid',
  );

  $properties['created'] = array(
    'label' => t('Created'),
    'description' => t('Time of creation.'),
    'type' => 'date',
    'getter callback' => 'entity_property_verbatim_get',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'created',
  );

  return $info;
}
