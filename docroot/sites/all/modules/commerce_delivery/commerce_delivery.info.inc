<?php

/**
 * @file
 * Provides metadata for the delivery entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_delivery_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_delivery properties.
  $properties = &$info['commerce_delivery']['properties'];

  $properties['delivery_id'] = array(
    'label' => t('Delivery ID', array(), array('context' => 'a drupal commerce delivery')),
    'description' => t('The internal numeric ID of the delivery.'),
    'type' => 'integer',
    'schema field' => 'delivery_id',
  );
  $properties['order_id'] = array(
    'label' => t('Order ID', array(), array('context' => 'a drupal commerce delivery')),
    'description' => t('The Order ID which the delivery belongs to.'),
    'setter callback' => 'entity_property_verbatim_set',
    'type' => 'integer',
    'required' => TRUE,
    'schema field' => 'order_id',
  );
//  not used, maybe in the future
//  $properties['view_url'] = array(
//    'label' => t('View URL'),
//    'description' => t('The URL a customer can visit to view the delivery.'),
//    'getter callback' => 'commerce_delivery_get_properties',
//    'type' => 'uri',
//  );
  $properties['admin_url'] = array(
    'label' => t('Admin URL'),
    'description' => t("The URL of the delivery's administrative view page."),
    'getter callback' => 'commerce_delivery_get_properties',
    'type' => 'uri',
  );
  $properties['edit_url'] = array(
    'label' => t('Edit URL'),
    'description' => t("The URL of the delivery's edit page."),
    'getter callback' => 'commerce_delivery_get_properties',
    'type' => 'uri',
  );
  $properties['status'] = array(
    'label' => t('Status'),
    'description' => t('The current status of the delivery.'),
    'type' => 'integer',
    'options list' => 'commerce_delivery_get_status',
    'setter callback' => 'entity_property_verbatim_set',
   'required' => TRUE,
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the delivery was created.'),
    'type' => 'date',
    'setter permission' => 'administer deliveries',
  );
  $properties['changed'] = array(
    'label' => t('Date changed'),
    'description' => t('The date the delivery was most recently updated.'),
    'type' => 'date',
    'setter permission' => 'administer deliveries',
  );

  return $info;
}

/**
 * Implements hook_entity_property_info_alter().
 * @see entity_metadata_entity_property_info_alter()
 */
function commerce_delivery_entity_property_info_alter(&$info) {
  // Add the delivery ref to the line item.
  $info['commerce_line_item']['properties']['delivery_ref'] = array(
    'label' => t("Delivery ID"),
    'type' => 'integer',
//    'validation callback' => 'entity_metadata_validate_integer_positive',
    'getter callback' => 'entity_metadata_field_property_get',
    'setter callback' => 'entity_metadata_field_property_set',
  );

  // Add the list of deliveries to the order.
  $info['commerce_order']['properties']['commerce_deliveries'] = array(
    'label' => t('Deliveries'),
    'description' => t('The deliveries on this order'),
    'getter callback' => 'commerce_order_get_property_delivery',
    'type' => 'list<commerce_delivery>',
  );
}


