<?php

/**
 * @file 
 * rules integration for the datastore_field module
 */

/**
 * Implements hook_rules_data_info().
 */
function datastore_field_rules_data_info() {
  $return = array(
    'field_item_datastore_field' => array(
      'label' => t('datastore value'),
      'wrap' => TRUE,
      'property info' => datastore_field_datastore_field_info(),
      'creation callback' => 'rules_action_data_create_array',
    ),
    'list<field_item_datastore_field>' => array(
      'label' => t('List of datastore values'),
      'wrap' => TRUE,
    )
  );
  return $return;
}