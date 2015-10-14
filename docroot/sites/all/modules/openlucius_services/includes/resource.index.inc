<?php
/**
 * @file
 * This file contains the methods for the index requests.
 */

/**
 * Function to fetch an array containing all the groups a user has access to.
 */
function openlucius_services_fetch_groups() {
  $groups = db_select('node', 'n')
    ->fields('n', array('nid', 'title'))
    ->condition('type', 'ol_group', '=')
    ->condition('status', 1, '=')
    ->addTag('node_access')
    ->execute()
    ->fetchAllKeyed(0, 1);

  // Create uniform list of items.
  openlucius_services_process_references($groups);

  return $groups;
}

/**
 * Function to fetch all comments for a node.
 *
 * @param int $nid
 *   The node for which the comments are to be fetched.
 *
 * @return mixed
 *   Returns either a formatted array of items or FALSE.
 */
function openlucius_services_fetch_comments($nid) {

  $query = db_select('comment', 'c')
    ->fields('c', array('cid', 'subject'))
    ->condition('c.nid', $nid, '=')
    ->condition('c.status', 1, '=');

  // Hide hidden comments for clients.
  if (openlucius_core_user_is_client()) {

    // Join on node table for title.
    $query->leftJoin('field_data_field_todo_comm_show_clients', 'f', 'f.entity_id = c.cid');
    $query->condition('f.field_todo_comm_show_clients_value', 0, '=');
  }

  // Fetch all.
  $comments = $query->execute()->fetchAllKeyed(0, 1);

  // Create uniform list of items.
  openlucius_services_process_references($comments);

  return $comments;
}

/**
 * Function to fetch a global overview of all groups, lists etc.
 */
function openlucius_services_fetch_overview() {

  // Fetch groups.
  $data['group_data'] = openlucius_services_fetch_groups();

  // Append the todo-lists and users to the data array.
  foreach ($data['group_data'] as &$item) {

    // Fetch all node types.
    $types = openlucius_services_node_types();

    foreach ($types as $type) {

      // Fetch the system name.
      $node_type = openlucius_services_map_type($type);

      // Fetch all items and comments.
      $item[$type] = openlucius_services_fetch_attached_nodes_of_type($item['id'], $node_type);
    }

    // Fetch users.
    $item['users'] = openlucius_core_fetch_group_users($item['id']);

    // Make format uniform.
    openlucius_services_process_references($item['users']);
  }

  // Fetch the available statuses.
  $data['statuses'] = openlucius_core_get_terms('labels_todo_s');

  // Fetch the available roles.
  $data['roles'] = user_roles(TRUE);

  // Unset default drupal.
  unset($data['roles'][DRUPAL_AUTHENTICATED_RID]);

  // Make format uniform.
  openlucius_services_process_references($data['statuses']);
  openlucius_services_process_references($data['roles']);

  return $data;
}

/**
 * Function to fetch attached nodes and their comments.
 *
 * @param int $nid
 *   The parent element for which the children are to be searched.
 * @param string $type
 *   The type to be searched.
 *
 * @return mixed
 *   Returns either an array or FALSE.
 */
function openlucius_services_fetch_attached_nodes_of_type($nid, $type) {
  $query = db_select('field_data_field_shared_group_reference', 'r')
    ->fields('n', array('nid', 'title'))
    ->condition('r.bundle', $type, '=')
    ->condition('r.field_shared_group_reference_nid', $nid, '=')
    ->condition('n.status', 1, '=')
    ->addTag('node_access');

  // Join on node table for title.
  $query->join('node', 'n', 'r.entity_id = n.nid');

  // Fetch all.
  $items = $query->execute()->fetchAllKeyed(0, 1);

  // Create uniform list of items.
  openlucius_services_process_references($items);

  // Attach todo's and their comments.
  if ($type == 'ol_todo_list') {

    foreach ($items as &$item) {
      $item['todo'] = openlucius_services_fetch_todos($item['id']);
    }
  }
  else {

    // Fetch comments.
    foreach ($items as &$item) {
      $item['comments'] = openlucius_services_fetch_comments($item['id']);
    }
  }

  return $items;
}

/**
 * Function to fetch all todos of a group.
 *
 * @param int $todo_list_id
 *   The todo-list for which the todo's should be fetched.
 *
 * @return mixed
 *   Returns either a formatted array of items or FALSE.
 */
function openlucius_services_fetch_todos($todo_list_id) {
  $query = db_select('field_data_field_todo_list_reference', 'r')
    ->fields('n', array('nid', 'title'))
    ->condition('r.bundle', 'ol_todo', '=')
    ->condition('r.field_todo_list_reference_nid', $todo_list_id, '=')
    ->condition('n.status', 1, '=')
    ->addTag('node_access');

  // Join on node table for title.
  $query->join('node', 'n', 'r.entity_id = n.nid');

  // Fetch all.
  $todos = $query->execute()->fetchAllKeyed(0, 1);

  // Create uniform list of items.
  openlucius_services_process_references($todos);

  // Append comments to todos.
  foreach ($todos as &$todo) {
    $todo['comments'] = openlucius_services_fetch_comments($todo['id']);
  }

  return $todos;
}

/**
 * Function to fetch de fields of a node.
 *
 * @param int $nid
 *   The node id of the node.
 *
 * @return array
 *   Returns the field content of the node.
 * @throws Exception
 *   Throws an exception if there's an error.
 */
function openlucius_services_fetch_node_details($nid) {

  // Verify that the submitted $nid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($nid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($nid),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Fetch menu item for type comparing.
  $menu = menu_get_item();
  $type = $menu['page_arguments'][1];
  $real_type = openlucius_services_map_type($type);

  // Perform a quick fetch for type comparison.
  $node_type = openlucius_core_get_type_by_nid($nid);

  // Cannot find the node.
  if (empty($node_type)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_NOT_FOUND, array("@nid" => $nid));
  }

  // Verify that the requested node is of the correct type.
  if ($node_type != $real_type) {

    // Throw error.
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_WRONG_TYPE, array(
      '@type'  => $node_type,
      '@type2' => $real_type,
    ));
  }

  // Load the node.
  $node = node_load($nid);

  // Verify that the user has access to this node.
  if (!node_access('view', $node)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_ACCESS_DENIED, array("@nid" => $nid));
  }

  // Initialize array for result.
  $result = array('id' => $nid);

  // Fetch the metadata wrapper for quick access.
  $wrapper = entity_metadata_wrapper('node', $node);

  // Default fields.
  $defaults = array('title', 'created', 'changed');
  foreach ($defaults as $default_field) {
    $result[$default_field] = $wrapper->{$default_field}->value();
  }

  // Fetch the fields for this node type.
  $fields = field_info_instances('node', $node->type);

  // Loop through items and reverse map them.
  foreach ($fields as $field_name => $values) {

    // Skip this field as it is not meant for clients.
    if (openlucius_core_user_is_client() && $field_name == 'field_shared_show_clients') {
      continue;
    }

    // Process the results.
    _openlucius_services_process_result($wrapper, $node_type, $field_name, $result);
  }

  // Append the comments.
  $result['comments'] = openlucius_services_fetch_comments($nid);

  return $result;
}

/**
 * Function to fetch de fields of a comment.
 *
 * @param int $cid
 *   The node id of the comment.
 *
 * @return array
 *   Returns the field content of the comment.
 * @throws Exception
 *   Throws an exception if there's an error.
 */
function openlucius_services_get_comment_details($cid) {

  // Verify that the submitted $cid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($cid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($cid),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Try to load the comment.
  $comment = comment_load($cid);

  // Verify that the comment exists.
  if (empty($comment)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_COMMENT_NOT_FOUND, array("@cid" => $cid));
  }

  // Load the view for access checking because comment access can only check
  // if a user may edit a comment.
  $parent = node_load($comment->nid);

  // Verify that the user may view the comment.
  if (node_access('view', $parent)) {

    // Check if the user is a client and the comment is set to hidden.
    if (openlucius_core_user_is_client() && !empty($comment->field_todo_comm_show_clients[LANGUAGE_NONE])
        && $comment->field_todo_comm_show_clients[LANGUAGE_NONE][0]['value'] == 1) {

      // Throw error.
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_COMMENT_ACCESS_DENIED, array("@cid" => $cid));
    }
  }
  else {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_COMMENT_ACCESS_DENIED, array("@cid" => $cid));
  }

  // Fetch the metadata wrapper for easy field access.
  $wrapper = entity_metadata_wrapper('comment', $comment);

  // Initialize result array.
  $result = array('id' => $cid);

  // Currently only the body and files are returned.
  // Todo replace by field_info_instances().
  $fields = array(
    'comment_body',
    'field_shared_files',
    'field_todo_comm_show_clients',
  );
  foreach ($fields as $field_name) {

    // Process the result.
    _openlucius_services_process_result($wrapper, 'comment', $field_name, $result);
  }

  return $result;
}

/**
 * Function to fetch fields of a user.
 *
 * @param int $uid
 *   The id for the user.
 *
 * @return array
 *   Returns the user data.
 * @throws Exception
 *   Throws an exception if there's an error.
 */
function openlucius_services_get_user_details($uid) {
  global $user;

  // Verify that the submitted $uid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($uid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($uid),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Try to load the user.
  $account = user_load($uid);

  // Check if the user could be loaded.
  if (empty($account)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_NOT_FOUND, array('@uid' => $uid));
  }

  // Compare user groups to know if this user may view the other user.
  $account_user_groups = openlucius_core_fetch_user_groups($uid);
  $user_groups = openlucius_core_fetch_user_groups($user->uid);

  // Check if we have at least one matching group or the correct permission.
  $matching_groups = array_intersect($user_groups, $account_user_groups);
  if (empty($matching_groups) && !user_access('administer users')) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_ACCESS_DENIED);
  }

  // Fetch the metadata wrapper for easy field access.
  $wrapper = entity_metadata_wrapper('user', $account);

  // Default user fields.
  $default_fields = array('name', 'mail', 'roles');

  // Initialize result array.
  $result = array('id' => $uid);

  // Add image if set as the metadata wrapper does not have that field.
  // TODO replace @see https://www.drupal.org/node/2224645.
  if (isset($account->picture)) {
    $result['picture'] = array(
      'filename' => $account->picture->filename,
      'path'     => file_create_url($account->picture->uri),
      'filemime' => $account->picture->filemime,
    );
  }
  else {
    // Set default picture if it is missing.
    $default_picture       = drupal_get_path('profile', 'openlucius') . '/themes/openlucius/images/avatar.png';
    $result['picture'] = array(
      'filename' => 'avatar.png',
      'path'     => $default_picture,
      'filemime' => 'image/png',
    );
  }

  // Loop through default fields.
  foreach ($default_fields as $field_name) {

    // Process the result.
    _openlucius_services_process_result($wrapper, 'user', $field_name, $result);
  }

  // Fetch all user fields.
  $fields = field_info_instances('user', 'user');

  // Loop through the fields and process them.
  foreach ($fields as $field_name => $values) {

    // Process the result.
    _openlucius_services_process_result($wrapper, 'user', $field_name, $result);
  }

  return $result;
}

/**
 * Function for outputting field content.
 *
 * @param \EntityMetadataWrapper $wrapper
 *   The metadata wrapper of the object containing the information.
 * @param string $type
 *   The entity type for which the data needs to be outputted.
 * @param string $field_name
 *   The name of the field to be outputted.
 * @param array $result
 *   The result array where the data is placed.
 */
function _openlucius_services_process_result(\EntityMetadataWrapper $wrapper, $type, $field_name, array &$result) {

  // Verify that the entity has this specific field.
  if ($wrapper->__isset($field_name)) {

    // Fetch the values.
    $value = $wrapper->{$field_name}->value();

    // Fetch the output field.
    $output_field = openlucius_services_field_mapping($type, $field_name, TRUE, TRUE);

    if (!empty($output_field)) {

      // Multivalue field.
      if (gettype($value) == 'array') {

        // Process the array values.
        _openlucius_services_process_array($value, $field_name, $result[$output_field]);
      }

      // Check if this is a reference.
      elseif (gettype($value) == 'object') {

        // Process the object values.
        _openlucius_services_process_object($value, $field_name, $result[$output_field]);
      }

      // Normal fields.
      else {
        $result[$output_field] = $value;
      }
    }
  }
}

/**
 * Function for processing objects.
 *
 * @param \stdClass $value
 *   The object to be processed.
 * @param string $field_name
 *   The name of the field.
 * @param array $result
 *   The array to place the result.
 */
function _openlucius_services_process_object($value, $field_name, &$result) {

  // This is a node.
  if (isset($value->nid)) {
    $result[] = array(
      'id' => $value->nid,
      'title' => $value->title,
    );
  }
  // This is a taxonomy term.
  elseif (isset($value->tid)) {
    $result[] = array(
      'id' => $value->tid,
      'title' => $value->name,
    );
  }
  // We can't check if uid because most references have that parameter.
  elseif ($field_name == 'field_todo_user_reference') {
    $result[] = array(
      'id' => $value->uid,
      'title' => $value->name,
    );
  }

  // Defaults to value for values that we've not encountered before.
  else {
    $result[] = $value;
  }
}

/**
 * Function for processing arrays.
 *
 * @param array $value
 *   The array to be processed.
 * @param string $field_name
 *   The name of the field.
 * @param array $result
 *   The array to place the result.
 */
function _openlucius_services_process_array($value, $field_name, &$result) {
  // Check if the value field is set.
  if (isset($value['value'])) {
    $result[] = $value['value'];
  }

  // Check if we're dealing with files.
  elseif ($field_name == 'field_shared_files') {
    $files = array();

    // Loop through each file and check if it needs to be displayed.
    foreach ($value as $file) {

      if ($file['status'] == 1 && $file['display'] == 1) {
        $files[] = array(
          'filename' => $file['filename'],
          'path' => file_create_url($file['uri']),
          'filemime' => $file['filemime'],
        );
      }
    }

    // Append the files to the output.
    $result[] = $files;
  }
  else {

    // To prevent repetition reuse methods.
    foreach ($value as $item) {

      // Multivalue field.
      if (gettype($item) == 'array') {

        _openlucius_services_process_array($item, $field_name, $result);
      }

      // Check if this is a reference.
      elseif (gettype($item) == 'object') {

        _openlucius_services_process_object($item, $field_name, $result);
      }
      else {
        $result[] = $item;
      }
    }
  }
}
