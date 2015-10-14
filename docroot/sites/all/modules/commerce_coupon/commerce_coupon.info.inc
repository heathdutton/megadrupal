<?php

/**
 * @file
 * Provides metadata for the coupon entity.
 */

/**
 * Implements hook_entity_property_info() of entity module.
 * @see http://drupal.org/node/1021466
 */
function commerce_coupon_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_coupon properties.
  $properties = &$info['commerce_coupon']['properties'];

  $properties['coupon_id'] = array(
    'type' => 'integer',
    'label' => t('Coupon ID', array(), array('context' => 'a drupal commerce coupon')),
    'description' => t('The internal numeric ID of the coupon.'),
    'schema field' => 'coupon_id',
  );
  $properties['is_active'] = array(
    'label' => t('Is active'),
    'description' => t('Boolean indicating whether the coupon is active or disabled.'),
    'type' => 'boolean',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'is_active',
  );
  $properties['price_component_name'] = array(
    'label' => t('Price component name'),
    'description' => t('Identifies the component name for each coupon'),
    'type' => 'token',
    'getter callback' => 'commerce_coupon_get_properties',
    'schema field' => 'price_component_name',
  );
  $properties['type'] = array(
    'type' => 'commerce_coupon_type',
    'label' => t('Type'),
    'description' => t('The human readable name of the coupon type.'),
    'setter callback' => 'entity_property_verbatim_set',
    'options list' => 'commerce_coupon_type_options_list',
    'required' => FALSE,
    'schema field' => 'type',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the coupon was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the coupon was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'schema field' => 'changed',
  );
  $properties['times_used'] = array(
    'type' => 'token',
    'label' => t('Times used'),
    'description' => t('Number of times that the coupon has been used.'),
    'getter callback' => 'commerce_coupon_get_properties',
  );

  // Add metadata anpit coupon type properties.
  $properties = &$info['commerce_coupon_type']['properties'];

  $properties['type'] = array(
    'type' => 'token',
    'label' => t('Coupon type'),
    'description' => t('The machine name of the coupon type.'),
    'schema field' => 'type',
  );

  $properties['label'] = array(
    'type' => 'text',
    'label' => t('Coupon type label'),
    'description' => t('The human readable name of the coupon type.'),
    'schema field' => 'label',
  );

  $properties['status'] = array(
    'type' => 'boolean',
    'label' => t('Coupon type status'),
    'description' => t('Status of the coupon type.'),
    'schema field' => 'status',
  );

  return $info;
}

/**
 * Implements hook_entity_property_info_alter() on top of the Product module.
 */
function commerce_coupon_entity_property_info_alter(&$info) {
  if (!empty($info['commerce_coupon']['bundles'])) {
    $properties = array();

    foreach ($info['commerce_coupon']['bundles'] as $bundle => $bundle_info) {
      $bundle_info += array('properties' => array());
      $properties += $bundle_info['properties'];
    }

    $info['commerce_coupon']['properties']['commerce_coupon_code'] = $properties['commerce_coupon_code'];
    $info['commerce_coupon']['properties']['commerce_coupon_number_of_uses'] = $properties['commerce_coupon_number_of_uses'];
  }
}
