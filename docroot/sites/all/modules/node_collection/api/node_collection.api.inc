<?php
/**
 * @file
 * API functions for the node_collection module
 */

/**
 * Get parent node types
 * @return array
 *   Return all the node types
 */
function node_collection_api_get_node_types() {
  // Get already created parent content types.
  $saved_parents = array();
  $parent_node_types = db_query('
    SELECT {node_collection_parent}.parent_type, {node_type}.name
    FROM {node_collection_parent}, {node_type}
    WHERE {node_collection_parent}.parent_type = {node_type}.type'
  );

  foreach ($parent_node_types as $parent_node_type) {
    $saved_parents[$parent_node_type->parent_type] = $parent_node_type->name;
  }

  return $saved_parents;
}

/**
 * Insert relationship
 * @throws NodeCollectionRuntimeException
 *
 * @param string $relationship_name
 *   The relationship name
 *
 * @param array $parent_node_type
 *   Parent node types
 *
 * @param array $child_node_types
 *   Child node types
 *
 * @return bool
 *   Return TRUE if everything went fine
 */
function node_collection_api_add_update_relationship($relationship_name, $parent_node_type, $child_node_types) {
  // All available content types
  // Note: We have to rebuild the node types because it seems like
  // node_type_get_types() does not
  // return thr proper node types when used in CI.
  cache_clear_all();
  $content_types = _node_types_build(TRUE)->names;

  // Sanitize first.
  if (empty($relationship_name)) {
    throw new NodeCollectionRuntimeException('Relationship name should be specified in node_collection_api_add_relationship');
  }
  if (empty($parent_node_type) || in_array($parent_node_type, array_keys($content_types)) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node type in node_collection_api_add_relationship parent : ' . $parent_node_type . ' children : ' . implode(', ', array_keys($content_types)));
  }
  if (empty($child_node_types)) {
    throw new NodeCollectionRuntimeException('Child relationships cannot be empty in node_collection_api_add_relationship');
  }

  // Check if all the children are valid.
  foreach ($child_node_types as $child) {
    if (in_array($child, array_keys($content_types)) == FALSE) {
      throw new NodeCollectionRuntimeException("Invalid child node type in node_collection_api_add_relationship. Child node type : $child");
    }
  }

  // Check if we already got a relationship saved.
  $saved_relationship = node_collection_api_get_parent_child_relationship_object($parent_node_type);

  // Parent Record.
  $parent_record = array(
    'parent_type' => $parent_node_type,
    'name' => $relationship_name,
  );

  if (empty($saved_relationship)) {
    $write_parent_record = drupal_write_record('node_collection_parent', $parent_record);

    // Insert the children.
    if ($write_parent_record) {
      // Add the child node types.
      foreach ($child_node_types as $child_node) {
        $child_record = array(
          'parent_type' => $parent_node_type,
          'child_type' => $child_node,
        );

        if (drupal_write_record('node_collection_child', $child_record) == FALSE) {
          throw new NodeCollectionRuntimeException('Failed to insert the children while creating parent-child relationship.');
        }
      }
    }
    else {
      throw new NodeCollectionRuntimeException('Failed to insert the parent-child relationship.');
    }
  }
  else {
    $write_update_record = drupal_write_record('node_collection_parent', $parent_record, 'parent_type');

    // Update the children.
    if ($write_update_record) {
      // Get what has been removed.
      $removed_children = array_diff(array_keys($saved_relationship[0]['children']), $child_node_types);

      // Get what has been added.
      $added_children = array_diff($child_node_types, array_keys($saved_relationship[0]['children']));

      // Add the newly added children.
      if (empty($added_children) == FALSE) {
        // Add the child node types.
        foreach ($added_children as $new_child_node) {
          $child_record = array(
            'parent_type' => $parent_node_type,
            'child_type' => $new_child_node,
          );

          if (drupal_write_record('node_collection_child', $child_record) == FALSE) {
            throw new NodeCollectionRuntimeException('Failed to update the parent-child relationship.');
          }
        }
      }

      // Remove the children that has been removed.
      if (empty($removed_children) == FALSE) {
        // Remove the unselected/removed child node types.
        foreach ($removed_children as $removed_child) {
          try {
            $delete_children = db_delete('node_collection_child');
            $delete_children->condition('parent_type', $parent_node_type);
            $delete_children->condition('child_type', $removed_child);
            // Execute delete.
            $delete_children->execute();
          }
          catch (Exception $exp) {
            watchdog_exception('node_collection', $exp);
            throw new NodeCollectionRuntimeException('Failed to remove child relationships while updating.');
          }
        }
      }
    }
  }
}

/**
 * Get all relationships. If you pass the node type it will return you the exact
 * relationship or if not will return all the relationships
 *
 * @param string $parent_node_type
 *   Parent node type
 *
 * @return array
 *   Array of node types
 */
function node_collection_api_get_parent_child_relationship_object($parent_node_type = NULL) {
  // All available content types.
  $content_types = node_type_get_types();
  $relationships = array();
  $parent_details = array();
  $child_details = array();

  if ($parent_node_type == NULL) {
    // Query and get the parent child relationships out.
    $saved_node_collection = db_query(
     "SELECT * FROM {node_collection_parent}
     INNER JOIN {node_collection_child}
     ON {node_collection_parent}.parent_type = {node_collection_child}.parent_type"
    );
  }
  else {
    // Query for the parent type.
    $saved_node_collection = db_select('node_collection_parent', 'ncp');
    $saved_node_collection->addField('ncp', 'parent_type');
    $saved_node_collection->addField('ncp', 'name');
    $saved_node_collection->addField('ncc', 'child_type');
    $saved_node_collection->condition('ncp.parent_type', $parent_node_type, '=');
    $saved_node_collection->join('node_collection_child', 'ncc', 'ncp.parent_type = ncc.parent_type');
    $saved_node_collection = $saved_node_collection->execute();
  }

  // Group the child types.
  foreach ($saved_node_collection as $collection) {
    $parent_details[$collection->parent_type] = array(
      'name' => $collection->name,
      'node_type_name' => isset($content_types[$collection->parent_type]) ? $content_types[$collection->parent_type]->name : $collection->parent_type,
    );

    $child_details[$collection->parent_type][$collection->child_type] = isset($content_types[$collection->child_type]) ? $content_types[$collection->child_type]->name : $collection->child_type;
  }

  // Restructure the array.
  foreach ($parent_details as $parent_type => $parent) {
    $relationships[] = array(
      'parent_type' => $parent_type,
      'parent_type_name' => $parent['node_type_name'],
      'relationship_name' => $parent['name'],
      'children' => $child_details[$parent_type],
    );
  }

  return $relationships;
}

/**
 * Delete a relationship and its content
 *
 * @param string $parent_node_type
 *   Parent node type
 */
function node_collection_api_delete_relationship($parent_node_type) {
  // Check if the parent node type is passed.
  if (empty($parent_node_type)) {
    throw new NodeCollectionRuntimeException('Relationship name should be specified in node_collection_api_add_relationship');
  }

  // Create the transaction.
  $db_delete_relationship_transaction = db_transaction();

  try {
    // Delete all the children.
    $delete_children = db_delete('node_collection_child');
    $delete_children->condition('parent_type', $parent_node_type);
    $delete_children->execute();

    // Delete the parent.
    $delete_parent = db_delete('node_collection_parent');
    $delete_parent->condition('parent_type', $parent_node_type);
    $delete_parent->execute();
  }
  catch (Exception $db_delete_relationship_exception) {
    // Rollback the transaction and throw an exception.
    $db_delete_relationship_transaction->rollback();
    throw $db_delete_relationship_exception;
  }
}

/**
 * Check the node is a parent type
 *
 * @param string $node_type
 *   Node type
 *
 * @return bool
 *   Return TRUE if the node type is a Parent node type or return FALSE if not
 */
function node_collection_api_is_parent_type($node_type) {
  try {
    // Do a select query to see if the node type is a parent type.
    $is_prent_type = db_query(
      "SELECT * FROM
        {node_collection_parent}
       WHERE
        {node_collection_parent}.parent_type = :type",
        array(':type' => trim($node_type))
    );

    foreach ($is_prent_type as $type) {
      if ($type->parent_type == trim($node_type)) {
        return TRUE;
      }
    }

    return FALSE;
  }
  catch (Exception $select_exception) {
    return FALSE;
  }
}

/**
 * Get all the parent node types that a child node type can have
 *
 * @param string $child_node_type
 *   Child node type
 *
 * @return array
 *   Return all the parent types
 */
function node_collection_api_get_parent_types($child_node_type) {
  $parent_types = array();
  $parent_nodes = db_query(
    "SELECT DISTINCT {node_collection_child}.parent_type
     FROM {node_collection_child}
     WHERE {node_collection_child}.child_type = :child_type",
    array(':child_type' => trim($child_node_type))
  );

  foreach ($parent_nodes as $parent_node) {
    $parent_types[] = $parent_node->parent_type;
  }

  if (empty($parent_types)) {
    return $parent_types;
  }
  else {
    return $parent_types;
  }
}

/**
 * Insert a relationship instance
 * @throws Exception|NodeCollectionRuntimeException
 *
 * @param object $parent_node_object
 *   Parent object
 *
 * @param object $child_node_object
 *   Child node object
 *
 * @return bool
 *   Return TRUE of successful
 */
function node_collection_api_add_relationship_instance($parent_node_object, $child_node_object) {
  // Do the sanitizing first.
  if (
    isset($parent_node_object->nid) == FALSE ||
    is_numeric($parent_node_object->nid) == FALSE
  ) {
    throw new NodeCollectionRuntimeException('Invalid parent object specified in node_collection_api_add_relationship_instance');
  }

  if (
    isset($child_node_object->nid) == FALSE ||
    is_numeric($child_node_object->nid) == FALSE
  ) {
    throw new NodeCollectionRuntimeException('Invalid child node object specified in node_collection_api_add_relationship_instance');
  }

  // Get all the possible parent types.
  $possible_parents = node_collection_api_get_parent_types($child_node_object->type);

  // See if the claimed node can actually be a parent.
  if (in_array($parent_node_object->type, $possible_parents) == FALSE) {
    throw new NodeCollectionRuntimeException('Specified child cannot have a parent on the given node type in node_collection_api_add_relationship_instance');
  }
  else {
    // Insert the relationship.
    $relationship_data = array(
      'nc_parent_nid' => $parent_node_object->nid,
      'nc_child_nid' => $child_node_object->nid,
    );

    if (drupal_write_record('node_collection_data', $relationship_data)) {
      return TRUE;
    }
    else {
      throw new NodeCollectionRuntimeException('Failed to add relationship data.');
    }
  }
}

/**
 * Function that returns all the child node instances for a given parent
 * instance
 * @throws NodeCollectionRuntimeException
 *
 * @param int $parent_node_id
 *   Node id
 *
 * @return array
 *   Return the child node instances
 */
function node_collection_api_get_child_node_instances($parent_node_id) {
  if (empty($parent_node_id) || is_numeric($parent_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node id specified in node_collection_api_get_child_node_instances');
  }

  // Child node collection array.
  $node_ids = array();

  try {
    $child_nid_query = db_select('node_collection_data', 'ncd');
    $child_nid_query->fields('ncd', array('nc_child_nid'));
    $child_nid_query->condition('nc_parent_nid', $parent_node_id, '=');

    // Do the query.
    $child_nids = $child_nid_query->execute();

    foreach ($child_nids as $child_nid) {
      $node_ids[] = $child_nid->nc_child_nid;
    }
  }
  catch (Exception $child_selection_exception) {
    throw new NodeCollectionRuntimeException($child_selection_exception->getMessage());
  }

  return $node_ids;
}

/**
 * Function that returns the parent node instance of a given child node id
 * @throws NodeCollectionRuntimeException
 *
 * @param int $child_node_id
 *   Child node id
 *
 * @return array
 *   Return the parent node object
 */
function node_collection_api_get_parent_node_instance($child_node_id) {
  if (empty($child_node_id) || is_numeric($child_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid child node id specified in node_collection_api_get_parent_node_instance');
  }

  // Parent node collection.
  $parent_node_object = NULL;

  try {
    $parent_nid_query = db_select('node_collection_data', 'ncd');
    $parent_nid_query->fields('ncd', array('nc_parent_nid'));
    $parent_nid_query->condition('nc_child_nid', $child_node_id, '=');
    $parent_nid_query->range(0, 1);

    // Do the query.
    $parent_nid = $parent_nid_query->execute();
    $parent_node_object = node_load($parent_nid->fetchField());
  }
  catch (Exception $parent_selection_exception) {
    throw new NodeCollectionRuntimeException($parent_selection_exception->getMessage());
  }

  return $parent_node_object;
}

/**
 * The API function that checks whether a given parent node id has any grand
 * parents associated with it.
 *
 * @param int $parent_node_id
 *   The parent node if you need to check if it has any grand parents.
 *
 * @return array|bool
 *   The array of grand parents if it has any or FALSE if none.
 *
 * @throws NodeCollectionRuntimeException
 * @throws Exception
 */
function node_collection_api_get_grand_parents($parent_node_id) {
  // Check to see if the parent belongs to some other parent.
  $grand_parent_id_collection = array();

  if (is_numeric($parent_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Parent ID should be a numeric value.');
  }

  $grand_parent_id = node_collection_api_is_child_instance($parent_node_id);

  if ($grand_parent_id) {
    $grand_parent_id_collection[] = $grand_parent_id;

    while ($child_id = node_collection_api_is_child_instance($grand_parent_id)) {
      $grand_parent_id = $child_id;
      $grand_parent_id_collection[] = $child_id;
    }
    // Reverse the parent ID array so we have the grand parent first :).
    $grand_parent_id_collection = array_reverse($grand_parent_id_collection);
  }

  return $grand_parent_id_collection;
}

/**
 * Return the child instance of a particular node type
 * @throws NodeCollectionRuntimeException
 *
 * @param int $parent_node_id
 *   Parent node id
 *
 * @param int $child_content_type
 *   Child content type
 *
 * @return array
 *   Return the child instances
 */
function node_collection_api_get_child_instances_by_content_type($parent_node_id, $child_content_type) {
  // Check the inputs first before we  hit the DB.
  if (isset($parent_node_id) == FALSE || is_numeric($parent_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node id specified in node_collection_api_get_child_instance');
  }

  if (isset($child_content_type) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid child content type specified in node_collection_api_get_child_instance');
  }

  // Node array for the child node ID collection.
  $node_ids = array();
  $child_nodes = array();

  try {
    $child_nid_query = db_select('node_collection_data', 'ncd');
    $child_nid_query->fields('ncd', array('nc_child_nid'));
    $child_nid_query->join('node', 'n', 'n.nid = ncd.nc_child_nid AND n.type = :type', array(':type' => $child_content_type));
    $child_nid_query->condition('nc_parent_nid', $parent_node_id, '=');

    // Do the query.
    $child_nids = $child_nid_query->execute();

    foreach ($child_nids as $child_nid) {
      $node_ids[] = $child_nid->nc_child_nid;
    }

    if (empty($node_ids) == FALSE) {
      foreach ($node_ids as $nid) {
        if (is_numeric($nid)) {
          $child_nodes[] = node_load($nid);
        }
      }
    }

    return $child_nodes;
  }
  catch (Exception $child_selection_exception) {
    throw new NodeCollectionRuntimeException($child_selection_exception->getMessage());
  }
}

/**
 * Delete an associated entry if the child is deleted
 * @throws Exception|NodeCollectionRuntimeException
 *
 * @param int $child_node_id
 *   Child node id
 *
 * @return bool
 *   Return TRUE if successful
 */
function node_collection_api_delete_associated_child_instance($child_node_id) {
  if (empty($child_node_id) || is_numeric($child_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node id specified in node_collection_api_delete_associated_child_instances');
  }

  $delete_query = db_delete('node_collection_data');
  $delete_query->condition('nc_child_nid', $child_node_id, '=');
  $delete_query->execute();

  return TRUE;
}

/**
 * Delete the relationship instance from the mapping table
 * @throws Exception|NodeCollectionRuntimeException
 *
 * @param int $parent_node_id
 *   Parent node id
 *
 * @return bool
 *   Return TRUE if successful
 */
function node_collection_api_delete_relationship_instance($parent_node_id) {
  if (empty($parent_node_id) || is_numeric($parent_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node id specified in node_collection_api_delete_relationship_instance');
  }

  // Delete the relationship instance.
  try {
    $delete_instance_query = db_delete('node_collection_data');
    $delete_instance_query->condition('nc_parent_nid', $parent_node_id, '=');
    $delete_instance_query->execute();

    return TRUE;
  }
  catch (Exception $del_exception) {
    throw $del_exception;
  }
}

/**
 * Check if a given node ID is a child instance of some parent. If yes it
 * will return the parent id and if not it will return false.
 * @throws Exception|NodeCollectionRuntimeException
 *
 * @param int $child_node_id
 *   Child node id
 *
 * @return bool
 *   Return parent id successful or return FALSE
 */
function node_collection_api_is_child_instance($child_node_id) {
  if (empty($child_node_id) || is_numeric($child_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node id specified in node_collection_api_is_child_instance');
  }

  // Query and see if this is a child instance.
  try {
    $parent_id = NULL;
    $child_query = db_select('node_collection_data', 'ncd');
    $child_query->fields('ncd', array('nc_parent_nid'));
    $child_query->condition('nc_child_nid', $child_node_id, '=');
    $child_query->range(0, 1);
    $parent = $child_query->execute();
    // Return the parent ID if there is any.
    $parent_id = $parent->fetchField();

    if ($parent_id) {
      return $parent_id;
    }
    else {
      return FALSE;
    }
  }
  catch (Exception $select_exception) {
    throw $select_exception;
  }
}

/**
 * Check if a given node ID is a parent instance. If yes it will return the
 * parent id and if not it will return false.
 * @throws Exception|NodeCollectionRuntimeException
 *
 * @param int $parent_node_id
 *   Parent node id
 *
 * @return bool|int
 *   Return Parent ID or FALSE
 */
function node_collection_api_is_parent_instance($parent_node_id) {
  if (empty($parent_node_id) || is_numeric($parent_node_id) == FALSE) {
    throw new NodeCollectionRuntimeException('Invalid parent node id specified in node_collection_api_is_parent_instance');
  }

  // Query and see if this is a child instance.
  try {
    $parent_id = NULL;
    $parent_query = db_select('node_collection_data', 'ncd');
    $parent_query->fields('ncd', array('nc_parent_nid'));
    $parent_query->condition('nc_parent_nid', $parent_node_id, '=');

    $parent = $parent_query->execute();

    // Return the parent ID if there is any.
    foreach ($parent as $par) {
      $parent_id = $par->nc_parent_nid;
    }

    if ($parent_id) {
      return $parent_id;
    }
    else {
      // NOTE: Ok we did not found the guy as the parent on the look-up table
      // simply because we do not have any children associated with it at the
      // moment. So lets check if this node is a parent type.
      // Load the node object.
      $node_object = node_load($parent_node_id);

      if (empty($node_object)) {
        return FALSE;
      }
      else {
        $is_parent_type = node_collection_api_is_parent_type($node_object->type);

        // If this node is a suitable parent type return TRUE.
        if ($is_parent_type) {
          return $parent_node_id;
        }
        else {
          return FALSE;
        }
      }
    }
  }
  catch (Exception $select_exception) {
    throw $select_exception;
  }
}

/**
 * Define a custom runtime exception class for Node Collection
 */
class NodeCollectionRuntimeException extends Exception {
  /**
   * Constructor for the NodeCollectionRuntimeException class
   *
   * @param string $message
   *   Exception Message
   * @param int $code
   *   Exception Code
   * @param Exception|null $previous
   *   Previous Exception
   */
  public function __construct($message, $code = 0, Exception $previous = NULL) {
    // Make sure everything is assigned properly.
    parent::__construct($message, $code, $previous);
  }

  /**
   * Override __toString
   *
   * @return string
   *   Return the converted string
   */
  public function __toString() {
    return __CLASS__ . ": [{$this->code}]: {$this->message}\n";
  }
}
