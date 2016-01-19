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
 * Get Smart Chart type machine name.
 *
 * @param $chart_nid
 *   Smart Chart nid.
 *
 * @return string
 *   Chart type machine name.
 */
function sc_get_chart_type($chart_nid) {
  $wrapper = entity_metadata_wrapper('node', $chart_nid);

  // Chart Type value.
  $chart_type = $wrapper->field_sc_type->value();
  if (isset($chart_type)) {
    // Get Chart Type entity.
    $chart_type = $wrapper->field_sc_type;
    // Get Chart Type machine name.
    $chart_type = $chart_type->machine_name->value();
    if (isset($chart_type)) {
      return $chart_type;
    }
  }

  return NULL;
}

/**
 * Add Smart Chart type as term.
 *
 * @param $chart_type
 *   Chart type array.
 *   array(
 *     'name' => 'Name',
 *     'machine_name' => 'Machine name(optional)',
 *     'description' => 'Description(optional),
 *   )
 *
 * @return int
 *   Chart type tid or NULL.
 */
function sc_add_chart_type($chart_type) {
  // Load vocabulary.
  $vocabulary = taxonomy_vocabulary_machine_name_load('smart_chart_type');

  // Get machine name to save and look for.
  if (isset($chart_type['machine_name'])) {
    $term_machine_name = $chart_type['machine_name'];
  }
  else {
    $term_machine_name = str_replace(' ', '_', strtolower($chart_type['name']));
  }

  // Check term exists.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'taxonomy_term')
    ->propertyCondition('vid', $vocabulary->vid)
    ->propertyCondition('machine_name', $term_machine_name)
    ->execute();

  if (!empty($result['taxonomy_term'])) {
    // If it exists, return tid.
    $term = array_keys($result['taxonomy_term']);
    if (isset($term[0])) {
      $term = entity_metadata_wrapper('taxonomy_term', $term[0]);

      return $term->getIdentifier();
    }
  }
  else {
    // If it does not exist, create and return tid.
    $data = array(
      'name' => $chart_type['name'],
      'vid' => $vocabulary->vid,
      'machine_name' => $term_machine_name,
    );

    // Description.
    if (isset($chart_type['description'])) {
      $data['description'] = $chart_type['description'];
    }

    $term = entity_create('taxonomy_term', $data);
    $term = entity_metadata_wrapper('taxonomy_term', $term);
    $term->save();

    return $term->getIdentifier();
  }

  return FALSE;
}

/**
 * Remove Smart Chart type as term.
 *
 * @param $chart_type
 *   Smart chart type machine name.
 */
function sc_remove_chart_type($chart_type) {
  // Load vocabulary.
  $vocabulary = taxonomy_vocabulary_machine_name_load('smart_chart_type');

  // Check term exists.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'taxonomy_term')
    ->propertyCondition('vid', $vocabulary->vid)
    ->propertyCondition('machine_name', $chart_type)
    ->execute();

  if (!empty($result['taxonomy_term'])) {
    $term = array_keys($result['taxonomy_term']);
    if (isset($term[0])) {
      taxonomy_term_delete($term[0]);
    }
  }
}

/**
 * Get tid from machine name field of a taxonomy term.
 *
 * @param string $machine_name
 *   The taxonomy term machine name field value.
 *
 * @return integer|bool
 *   Returns the tid otherwise FALSE.
 */
function sc_get_chart_type_tid_from_machine_name($machine_name) {

  // Load term.
  $term = taxonomy_term_machine_name_load($machine_name, 'smart_chart_type');

  return !$term ? FALSE : $term->tid;
}

/**
 * Get parent nid of item.
 *
 * @param int $item_nid
 *   Current item nid.
 * @param int $chart_nid
 *   Chart nid, if non is passed check on personnel.
 *
 * @return int|bool
 *   Parent nid or FALSE.
 */
function sc_item_parent($item_nid, $chart_nid = NULL) {

  $parent_name_var = __FUNCTION__;
  $parent_name_var .= '_' . (string) $item_nid;
  $parent_name_var .= !is_null($chart_nid) ? '_' . (string) $chart_nid : '';
  $parent = &drupal_static($parent_name_var);
  if (!isset($parent)) {
    // Not entity wrapper for faster response.
    $query = db_select('node', 'n')
      ->fields('parent_node', array('nid'));
    $query->condition('n.nid', $item_nid);
    $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id');
    $query->join('node', 'connection', 'connection.nid = child.entity_id');
    $query->join('field_data_field_sc_parent', 'parent', 'parent.entity_id = connection.nid');
    $query->join('node', 'parent_node', 'parent_node.nid = parent.field_sc_parent_target_id');

    // Join the chart.
    $query->join('field_data_field_sc_chart', 'chart', 'chart.entity_id = child.entity_id');

    if ($chart_nid != NULL) {
      // Search for the given chart.
      $query->condition('chart.field_sc_chart_target_id', $chart_nid);
    }
    else {
      // Get the personnel chart tid.
      $tid = sc_get_chart_type_tid_from_machine_name('personnel');

      // Join the chart type.
      $query->join('field_data_field_sc_type', 'chart_type', 'chart_type.entity_id = chart.field_sc_chart_target_id');
      $query->condition('chart_type.field_sc_type_tid', $tid);
    }

    $result = $query->execute()->fetchAssoc();
    $parent = isset($result['nid']) ? $result['nid'] : FALSE;
  }

  return $parent;
}

/**
 * Get children nids of item.
 *
 * @param int $item_nid
 *   Current item nid.
 * @param int $chart_nid
 *   Chart nid, if non is passed check on personnel.
 *
 * @return array
 *   Returns an array containing the item's children, otherwise an empty array.
 */
function sc_item_children($item_nid, $chart_nid = NULL) {

  // Not entity wrapper for faster response.
  $query = db_select('node', 'n')
    ->fields('child', array('field_sc_child_target_id'))
    ->fields('parent', array('field_sc_parent_target_id'));

  // Join the parent using the connection id and will be the given sci id.
  $query->join('field_data_field_sc_parent', 'parent', 'parent.entity_id = n.nid');
  $query->condition('parent.field_sc_parent_target_id', $item_nid);

  // Join the child using the connection id.
  $query->join('field_data_field_sc_child', 'child', 'parent.entity_id = child.entity_id');

  // Join the chart.
  $query->join('field_data_field_sc_chart', 'chart', 'chart.entity_id = child.entity_id');

  if (!empty($chart_nid)) {
    // Search for the given chart.
    $query->condition('chart.field_sc_chart_target_id', $chart_nid);
  }
  else {
    // Get the personnel chart tid.
    $tid = sc_get_chart_type_tid_from_machine_name('personnel');

    // Join the chart type.
    $query->join('field_data_field_sc_type', 'chart_type', 'chart_type.entity_id = chart.field_sc_chart_target_id');
    $query->condition('chart_type.field_sc_type_tid', $tid);
  }

  $results = $query->execute()->fetchAll();

  $children = array();
  foreach ($results as $result) {
    // Add child in array.
    $children[] = $result->field_sc_child_target_id;

    // We have to check if this child have children also.
    $below_children = sc_item_children($result->field_sc_child_target_id, $chart_nid);

    // Merge children and below children in the same array.
    if (!empty($below_children)) {
      $children = array_merge($children, $below_children);
    }
  }

  return $children;
}

/**
 * Check if an item is the parent of another item. Refactored to use
 * sc_get_intermediate_items(), since it is more efficient.
 *
 * @param int $parent_item_id
 *   The parent Smart Chart Item id to be searched as parent.
 * @param int $child_item_id
 *   The Smart Chart Item child id to be checked.
 * @param int $chart_nid
 *   Check for specific chart. NULL to check for personnel.
 *
 * @return bool
 *   Returns TRUE if it is the parent, otherwise FALSE.
 */
function sc_item_is_parent($parent_item_id, $child_item_id, $chart_nid = NULL) {
  $intermediate_items = sc_get_intermediate_items($parent_item_id, $child_item_id, $chart_nid);
  return is_array($intermediate_items) && count($intermediate_items) > 1;
}

/**
 * Get intermediate items of two items.
 *
 * @param int $parent_item_id
 *   The parent Smart Chart Item id.
 * @param int $child_item_id
 *   The Smart Chart Item child id.
 * @param int $chart_nid
 *   Check for specific chart. NULL to check for personnel.
 *
 * @return array|bool
 *   Returns array of items or FALSE. The array starts from child all the way to
 *   the upper parent.
 */
function sc_get_intermediate_items($parent_item_id, $child_item_id, $chart_nid = NULL) {

  // Getting static variable.
  $intermediate_items_var_name
    = 'sc_get_intermediate_items_' . (string) $parent_item_id
    . '_' . (string) $child_item_id;
  $intermediate_items_var_name .= !is_null($chart_nid) ? (string) $chart_nid : '';
  $intermediate_items = &drupal_static($intermediate_items_var_name);

  if (!isset($intermediate_items)) {

    // Initializing variables.
    $intermediate_items = array();
    $intermediate_items[] = $child_item_id;

    do {

      // Recurring child item. The first time get child item.
      $child_item = isset($parent_item) ? $parent_item : $child_item_id;

      // Getting parent.
      $parent_item = sc_item_parent($child_item, $chart_nid);

      // If parent found and it is the same as input, add it and break.
      if ($parent_item == $parent_item_id) {
        $intermediate_items[] = $parent_item;
        break;

      }

      // Add it to the intermediate items.
      if ($parent_item) {
        $intermediate_items[] = $parent_item;
      }
    } while ($parent_item != FALSE);
  }

  return count($intermediate_items) > 1 && in_array($parent_item_id, $intermediate_items)
    ? $intermediate_items : FALSE;
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

  $has_children_name_var = 'sc_item_has_children_' . (string) $parent_nid;
  $has_children = &drupal_static($has_children_name_var);

  if (!isset($has_children)) {
    // Find if item has children.
    $query = db_select('node', 'n')
      ->fields('n', array('nid'));
    $query->join('field_data_field_sc_parent', 'parent', 'parent.field_sc_parent_target_id = ' . $parent_nid);
    $query->join('field_data_field_sc_child', 'child', 'n.nid = child.field_sc_child_target_id and child.entity_id = parent.entity_id');

    $count = $query->execute()->rowCount();

    $has_children = $count > 0 ? TRUE : FALSE;
  }

  return $has_children;
}

/**
 * Create chart.
 *
 * @param array $values
 *   Chart array values. An associative array containing:
 *     - uid: Author uid,
 *     - title: Title,
 *     - type: Chart type, String, optional.
 *
 * @return \EntityMetadataWrapper
 */
function sc_chart_new(array $values) {

  $node_values = array(
    'type' => 'smart_chart',
    'uid' => $values['uid'],
  );

  // Chart type.
  $tid = FALSE;
  if (isset($values['type'])) {
    $tid = sc_get_chart_type_tid_from_machine_name($values['type']);
  }

  $tid = !$tid ? sc_get_chart_type_tid_from_machine_name('personnel') : $tid;
  $entity = entity_create('node', $node_values);
  $chart_entity = entity_metadata_wrapper('node', $entity);
  $chart_entity->title = $values['title'];
  $chart_entity->field_sc_type = $tid;
  $chart_entity->save();

  $nid = $chart_entity->getIdentifier();
  if ($nid) {
    return $chart_entity;
  }
  else {
    // On error.
    watchdog('smart_chart', 'Error on Smart Chart creation, values: @values.',
      array('@values' => serialize($values)), WATCHDOG_ERROR);
    return FALSE;
  }
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
 *   New connection entity.
 */
function sc_connection_new($chart_nid, $parent_nid, $child_nid) {

  // Get other entities.
  $chart = entity_metadata_wrapper('node', $chart_nid);
  $parent = entity_metadata_wrapper('node', $parent_nid);
  $child = entity_metadata_wrapper('node', $child_nid);

  // Create new connection.
  $conn_values = array(
    'type' => 'smart_connection',
    'comment' => 0,
    'promote' => 0,
  );

  $entity = entity_create('node', $conn_values);
  $entity->uid = $chart->author->getIdentifier();
  $entity = entity_metadata_wrapper('node', $entity);
  $entity->title = 'Connection ' . $parent->title->value() . ' -> ' . $child->title->value();

  $entity->field_sc_chart = $chart_nid;
  $entity->field_sc_parent = $parent_nid;
  $entity->field_sc_child = $child_nid;

  // Creating hook to alter contents of the node.
  $entity_altered = module_invoke_all('sc_connection_new', $entity);
  $entity = $entity_altered ? $entity_altered : $entity;

  $entity->save();

  // Hook after new connection is saved.
  module_invoke_all('sc_connection_post_new', $entity);

  return $entity;
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
 * @return \EntityMetadataWrapper
 *   New node entity.
 */
function sc_item_new($title, $uid, $type) {

  // Create item.
  $item_values = array(
    'type' => $type,
    'comment' => 0,
    'promote' => 0,
  );

  $entity = entity_create('node', $item_values);
  $entity->uid = $uid;
  $entity = entity_metadata_wrapper('node', $entity);
  $entity->title = $title;

  // Creating hook to alter contents of the node.
  $entity_altered = module_invoke_all('sc_item_new', $entity);
  $entity = $entity_altered ? $entity_altered : $entity;

  $entity->save();

  return $entity;
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

  // Hook to run after node is broken from chart.
  module_invoke_all('sc_item_post_break', $chart_nid, $item_nid);
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
