<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - field_collection plugin.
 */

$plugin = array(
  'title' => t('FC: Node Reference plugin'),
  'completeness check' => 'fc_node_reference_field_is_complete',
  'incomplete process' => 'fc_node_reference_incomplete_process',
  'can be empty' => TRUE,

  'field form' => array(
    'fc_check_nodes' => array(
      '#type' => 'checkbox',
      '#title' => t('Check fields in referenced nodes for completeness'),
      '#description' => t('If checked, any nodes referenced by this field will also be checked for completeness.'),
    ),
  ),
);

function fc_node_reference_field_is_complete($items, $instance, $field, &$completed = array()) {
  $module = $field['module'];
  $check_nodes = !empty($instance['settings']['fc']['fc_check_nodes']);

  foreach ($items as $delta => $item) {
    if ($check_nodes) {
      // Loads each entity to find out whether it is complete
      $node = node_load($item['nid']);
      $completed[] = $node->fc->complete;
    }
    else {
      // Just do a superficial check of item
      $completed[] = !module_invoke($module, 'field_is_empty', $item, $field);
    }
  }
}

function fc_node_reference_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $check_nodes = !empty($instance['settings']['fc']['fc_check_nodes']);
  $incomplete = new fcIncomplete($instance['field_name'], $parent, 'node');

  $completed = array();
  if ($check_nodes) {
    foreach ($items as $item) {
      // Loads each entity to find out whether it is complete
      $node = node_load($item['nid']);
      $sub = new fcIncomplete('', $parent, 'node', $node);
      $sub->process();
      $incomplete[] = $sub;
    }
  }

  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}
