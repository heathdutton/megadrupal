<?php
/**
 * @file
 * field_instance_sync_test.features.field_instance_sync.inc
 */

/**
 * Implements hook_field_instance_sync_defaults().
 */
function field_instance_sync_test_field_instance_sync_defaults() {
  $field_data = array();

  $field_data['node-field_instance_sync_test_type_1-body'] = array(
    'synced' => TRUE,
    'slave' => FALSE,
  );
  $field_data['node-field_instance_sync_test_type_2-body'] = array(
    'synced' => TRUE,
    'slave' => TRUE,
  );

  return $field_data;
}
