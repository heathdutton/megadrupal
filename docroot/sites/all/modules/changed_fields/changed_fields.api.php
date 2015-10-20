<?php

/**
 * @file
 * Describe hooks provided by the Changed Fields module.
 */

/**
 * Register node types and fields for watching.
 *
 * @return array
 *   Array which contains node types as keys and array with fields names
 *   for watching as values.
 */
function hook_changed_fields_info() {
  return array(
    'node_type' => array(
      'field_name_1',
      'field_name_2',
      'field_name_n',
    ),
  );
}

/**
 * Allow modules to react on changed fields in node.
 *
 * Calls from hook_node_presave().
 *
 * @param \stdClass $node
 *   Node object.
 * @param array $changed_fields
 *   Array which contains field names as keys and arrays with old and new
 *   field values as values.
 */
function hook_changed_fields_reaction(stdClass $node, array $changed_fields) {
  // Do something...
}

/**
 * Allow modules to implement additional fields comparison.
 *
 * @param array $data
 *   Array which contains 'fields_info' array, 'old_value' array,
 *   'new_value' array and 'result' array.
 */
function hook_changed_fields_compare_field_values_alter(array $data) {
  // Example implementation of link fields comparison.
  // Link field values are similar if 'url' AND 'title' field properties
  // are similar fore each value in $data['old_value'] and $data['new_value'].
  if ($data['field_info']['type'] == 'link_field') {
    $data['result'] = changed_fields_compare_field_values($data['old_value'], $data['new_value'], array(
      'url',
      'title',
    ));
  }
}
