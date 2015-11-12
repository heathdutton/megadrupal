<?php

/**
 * @file
 * Provides Entity metadata integration.
 */
 
/**
 * Implements hook_entity_property_info().
 */
function datastore_system_entity_property_info() {
  $info = array();
  
  $properties = &$info['site']['properties'];
  
  $properties['datastore_system_node_count'] = array(
    'label' => t("Absolute Node Count"),
    'description' => t("Retrieves absolute node count"),
    'getter callback' => 'datastore_system_node_count',
    'type' => 'datastore_set',
    'property info' => datastore_system_datastore_set_info(),
  );
  
  // Here we are returning a single datastore_set
  $properties['datastore_system_user_count'] = array(
    'label' => t("Absolute User Count"),
    'description' => t("Retrieves absolute user count"),
    'getter callback' => 'datastore_system_user_count',
    'type' => 'datastore_set',
    'property info' => datastore_system_datastore_set_info(),
  );
  
  // Here we are returning a list of datastore_sets
  $properties['datastore_system_term_usage'] = array(
    'label' => t("Term usage"),
    'description' => t("Retrieves term usage"),
    'getter callback' => 'datastore_system_term_usage',
    'type' => 'list<datastore_set>',
    'property info' => datastore_system_datastore_set_info(),
  );

  return $info;
}

/**
 * Provides metadata about datastore sets
 */
function datastore_system_datastore_set_info() {
  $properties['domain'] = array(
    'type' => 'token',
    'label' => t('Domain'),
  );
  $properties['category'] = array(
    'type' => 'token',
    'label' => t('Category'),
  );
  $properties['reference'] = array(
    'type' => 'token',
    'label' => t('Reference'),
  );
  $properties['values'] = array(
    'label' => t('Datastore value/timestamp set'),
    'type' => 'list<field_item_datastore_field>',
  );
  return $properties;
}