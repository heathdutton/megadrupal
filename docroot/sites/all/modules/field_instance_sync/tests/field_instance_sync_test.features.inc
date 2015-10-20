<?php
/**
 * @file
 * field_instance_sync_test.features.inc
 */

/**
 * Implements hook_node_info().
 */
function field_instance_sync_test_node_info() {
  $items = array(
    'field_instance_sync_test_type_1' => array(
      'name' => t('Field Instance Sync test type 1'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'field_instance_sync_test_type_2' => array(
      'name' => t('Field Instance Sync test type 2'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
