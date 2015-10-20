<?php

/**
 * @file
 * Provides metadata for the product entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_product_key_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_product_key properties.
  $properties = &$info['commerce_product_key']['properties'];

  $properties['product_key_id'] = array(
    'label' => t('Product key ID'),
    'description' => t('The internal numeric ID of the product key.'),
    'type' => 'integer',
  );
  $properties['key_owner_id'] = array(
    'label' => t("Owner user ID"),
    'type' => 'integer',
    'description' => t("The user ID of user who owns the product key."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer product keys',
    'clear' => array('key_owner_id'),
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['order_id'] = array(
    'label' => t('Order ID', array(), array('context' => 'a drupal commerce order')),
    'description' => t('The internal numeric ID of the order.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer product keys',
    'clear' => array('order'),
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['code'] = array(
    'label' => t('Code'),
    'description' => t('The human readable product Code.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type of the product key.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer product keys',
    'options list' => 'commerce_product_key_type_options_list',
    'required' => TRUE,
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['activated'] = array(
    'label' => t('Activated'),
    'description' => t('Status of the code if it has been activated.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['revoked'] = array(
    'label' => t('revoked'),
    'description' => t('Status of the code if it has been revoked.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'required' => TRUE,
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['edit_url'] = array(
    'label' => t('Edit URL'),
    'description' => t("The URL of the product key's edit page."),
    'getter callback' => 'commerce_product_key_get_properties',
    'type' => 'uri',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the product key was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer product keys',
  );
  $properties['changed'] = array(
    'label' => t('Date changed'),
    'description' => t('The date the product key was most recently updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'query callback' => 'entity_metadata_table_query',
    'setter permission' => 'administer product keys',
  );
  $properties['uid'] = array(
    'label' => t("Creator ID"),
    'type' => 'integer',
    'description' => t("The unique ID of the product key creator."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer product keys',
    'clear' => array('creator'),
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['creator'] = array(
    'label' => t("Creator"),
    'type' => 'user',
    'description' => t("The creator of the product key."),
    'getter callback' => 'commerce_product_key_get_properties',
    'setter callback' => 'commerce_product_key_set_properties',
    'setter permission' => 'administer product keys',
    'required' => TRUE,
    'clear' => array('uid'),
  );

  return $info;
}
