<?php

/**
 * @file
 * Common functions for Smart Chart.
 */

/**
 * Get node types that can be in a Smart Chart.
 *
 * @param $reverse
 *   Get node types that are not in a Smart Chart.
 *
 * @return array
 *   Array with node types(machine_name => name).
 */
function sc_get_chart_item_types($reverse = FALSE) {

  // Get content types.
  $types = node_type_get_types();
  // Get content types that don't and do have a connection entity
  // reference field.
  $types_available = array();
  $types_selected = array();

  // Get field settings.
  $field = field_read_field('field_sc_child');
  $target_bundles = $field['settings']['handler_settings']['target_bundles'];
  foreach ($target_bundles as $target_bundle) {
    $type = node_type_load($target_bundle);
    $types_selected[$type->type] = $type->name;
  }

  // Return node types that are in Smart Chart.
  if (!$reverse) {
    return $types_selected;
  }

  foreach ($types as $type) {
    if (!isset($types_selected[$type->type]) && $type->type != 'smart_connection' && $type->type != 'smart_chart') {
      $types_available[$type->type] = $type->name;
    }
  }

  // Return node types that are not in Smart Chart.
  return $types_available;
}

/**
 * Add content type to Smart Chart.
 *
 * @param $type
 *   Content type to add. Machine name.
 */
function sc_add_type($type) {

  // Run before content type is added to Smart Chart.
  module_invoke_all('sc_add_type', $type);

  // Get Content Type to be added.
  $field_new = array(
    'settings' => array(
      'handler_settings' => array(
        'target_bundles' => array(
          $type => $type,
        ),
      ),
    ),
  );

  // Merge field settings for parent.
  $field = array_merge_recursive(field_read_field('field_sc_parent'), $field_new);
  field_update_field($field);

  // Merge field settings for child.
  $field = array_merge_recursive(field_read_field('field_sc_child'), $field_new);
  field_update_field($field);
}

/**
 * Remove content type from Smart Chart.
 *
 * @param $type
 *   Content type to remove. Machine name.
 */
function sc_remove_type($type) {

  // Run before content type is removed from Smart Chart.
  module_invoke_all('sc_remove_type', $type);

  // Delete smart_connection nodes that use removed bundle.
  $query = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.type', 'smart_connection')
    ->condition('bundle.type', $type);
  $query->join('field_data_field_sc_child', 'child', 'n.nid = child.entity_id');
  $query->join('node', 'bundle', 'child.field_sc_child_target_id = bundle.nid');
  $result = $query->execute()->fetchAll();

  $nids = array();
  foreach ($result as $row) {
    $nids[] = $row->nid;
  }

  $query = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.type', 'smart_connection')
    ->condition('bundle.type', $type);
  $query->join('field_data_field_sc_parent', 'parent', 'n.nid = parent.entity_id');
  $query->join('node', 'bundle', 'parent.field_sc_parent_target_id = bundle.nid');
  $result = $query->execute()->fetchAll();

  foreach ($result as $row) {
    $nids[] = $row->nid;
  }
  // Delete content.
  node_delete_multiple($nids);

  // Remove fields.
  // Remove from parent.
  $field = field_read_field('field_sc_parent');
  if (isset($field['settings']['handler_settings']['target_bundles'][$type])) {
    unset($field['settings']['handler_settings']['target_bundles'][$type]);
  }
  field_update_field($field);

  // Remove from child.
  $field = field_read_field('field_sc_child');
  if (isset($field['settings']['handler_settings']['target_bundles'][$type])) {
    unset($field['settings']['handler_settings']['target_bundles'][$type]);
  }
  field_update_field($field);
}

/**
 * Get parent nid of item.
 *
 * @param $item_nid
 *   Current item nid.
 *
 * @return int
 *   Parent nid or FALSE.
 */
function sc_item_parent($item_nid) {
  $query = db_select('node', 'n')
    ->fields('parent_node', array('nid'));
  $query->condition('n.nid', $item_nid);
  $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id');
  $query->join('node', 'connection', 'connection.nid = child.entity_id');
  $query->join('field_data_field_sc_parent', 'parent', 'parent.entity_id = connection.nid');
  $query->join('node', 'parent_node', 'parent_node.nid = parent.field_sc_parent_target_id');

  $result = $query->execute()->fetchAssoc();

  if (isset($result['nid'])) {
    return $result['nid'];
  }
  else {
    return FALSE;
  }
}

/**
 * Get if node has children.
 *
 * @param $parent_nid
 *   Parent nid to check for.
 *
 * @return bool
 *   Has children or not.
 */
function sc_item_has_children($parent_nid) {
  // Find if item has children.
  $query = db_select('node', 'n')
    ->fields('n', array('nid'));
  $query->join('field_data_field_sc_parent', 'parent', 'parent.field_sc_parent_target_id = ' . $parent_nid);
  $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id and child.entity_id = parent.entity_id');

  $count = $query->execute()->rowCount();

  return $count > 0 ? TRUE : FALSE;
}

/**
 * Create connection.
 *
 * @param $chart_nid
 *   Chart nid.
 * @param $parent_nid
 *   parent item id.
 * @param $child_nid
 *   New item nid to add to parent.
 *
 * @return object
 *   New connection.
 */
function sc_connection_new($chart_nid, $parent_nid, $child_nid) {

  // Create new connection.
  $chart = node_load($chart_nid);
  $parent = node_load($parent_nid);
  $child = node_load($child_nid);

  $node = new stdClass();
  $node->title = 'Connection ' . $parent->title . ' -> ' . $child->title;
  $node->type = 'smart_connection';
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->uid = $chart->uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;

  $node->field_sc_chart[$node->language][] = array(
    'target_id' => $chart_nid,
    'target_type' => 'node',
  );

  $node->field_sc_parent[$node->language][] = array(
    'target_id' => $parent_nid,
    'target_type' => 'node',
  );

  $node->field_sc_child[$node->language][] = array(
    'target_id' => $child_nid,
    'target_type' => 'node',
  );

  // Creating hook to alter contents of the node.
  $node_altered = module_invoke_all('sc_connection_new', $node);
  $node = $node_altered ? $node_altered : $node;

  // Prepare node for saving.
  $node = node_submit($node);
  node_save($node);

  return $node;
}

/**
 * Create new node for Smart Chart.
 *
 * @param $title
 *   Title.
 * @param $uid
 *   Author.
 * @param $type
 *   Content type of the new node.
 *
 * @return object
 *   New node.
 */
function sc_item_new($title, $uid, $type) {

  $node = new stdClass();
  $node->title = $title;
  $node->type = $type;
  node_object_prepare($node);
  $node->language = LANGUAGE_NONE;
  $node->uid = $uid;
  $node->status = 1;
  $node->promote = 0;
  $node->comment = 0;

  // Creating hook to alter contents of the node.
  $node_altered = module_invoke_all('sc_item_new', $node);
  $node = $node_altered ? $node_altered : $node;

  // Prepare node for saving.
  $node = node_submit($node);
  node_save($node);

  return $node;
}

/**
 * Add a node to a parent node.
 *
 * @param $chart_nid
 *   Chart nid.
 * @param $parent_nid
 *   parent node id.
 * @param $child_nid
 *   New node id to add to parent.
 */
function sc_item_add($chart_nid, $parent_nid, $child_nid) {
  // Break node from this chart.
  sc_item_break($chart_nid, $child_nid);
  // Create new connection.
  sc_connection_new($chart_nid, $parent_nid, $child_nid);
}

/**
 * Break a node from a chart.
 *
 * @param $chart_nid
 *   Chart nid.
 * @param $item_nid
 *   Current item nid.
 * @param $all
 *   Delete connections referencing children too.
 */
function sc_item_break($chart_nid, $item_nid, $all = FALSE) {

  // Hook to run before node is broken from chart.
  module_invoke_all('sc_item_break', $chart_nid, $item_nid);

  // Get connections.
  $query = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('type', 'smart_connection')
    ->condition('chart.field_sc_chart_target_id', $chart_nid);
  $query->join('field_data_field_sc_chart', 'chart', 'n.nid = chart.entity_id');
  $query->leftjoin('field_data_field_sc_child', 'child', 'n.nid = child.entity_id');

  if ($all) {
    // Get all connections.
    $or = db_or();
    $or->condition('child.field_sc_child_target_id', $item_nid);
    $or->condition('parent.field_sc_parent_target_id', $item_nid);
    $query->condition($or);
    $query->leftjoin('field_data_field_sc_parent', 'parent', 'n.nid = parent.entity_id');
  }
  else {
    // Get connection that has current node as a child for this chart.
    $query->condition('child.field_sc_child_target_id', $item_nid);
  }

  $result = $query->execute()->fetchAll();
  $nids = array();
  foreach ($result as $row) {
    $nids[] = intval($row->nid);
  }

  // Delete connections.
  node_delete_multiple($nids);
}

/**
 * Delete a node.
 *
 * @param $chart_nid
 *   Chart nid.
 * @param $item_nid
 *   Current item nid.
 */
function sc_item_delete($chart_nid, $item_nid) {

  // Delete all connections to this item for this chart.
  sc_item_break($chart_nid, $item_nid, TRUE);

  // Delete item.
  node_delete($item_nid);
}
