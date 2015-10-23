<?php
/**
 * @file
 * This file contains the nodes services.
 */

/**
 * Function to create a node.
 *
 * @param array $data
 *   The data from which the node will be created.
 *
 * @return array
 *   Returns a response when the action was completed successfully.
 * @throws ServicesException
 *   If the call is not accepted this method will throw a ServicesException 406.
 */
function _openlucius_services_create_node(array $data) {
  global $user;

  // Fetch menu item.
  $menu = menu_get_item();

  // Fetch the type.
  $type = $menu['page_arguments'][1];

  // Fetch creation requirements for this type.
  $requirements = _openlucius_services_entity_requirements($type);

  // Check if the requirements are set.
  foreach ($requirements as $requirement) {
    if (empty($data[$requirement])) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MISSING_PARAMETER, array("@parameter" => $requirement));
    }
  }

  // Fetch the node type.
  $node_type = openlucius_services_map_type($type);

  // Check if the user may create a node of this type.
  if (!user_access("create $node_type content")) {
    return openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_CREATE_ACCESS_DENIED, array("@type" => $type));
  }

  // Initialize variables.
  $node = array(
    'type'     => $node_type,
    'uid'      => $user->uid,
    'status'   => 1,
    'comment'  => 2,
    'promote'  => 0,
    'language' => LANGUAGE_NONE,
  );

  // Create the node object.
  $entity = entity_create('node', $node);

  // Get the metadata wrapper for the node object.
  $wrapper = entity_metadata_wrapper('node', $entity);

  // Loop through the data.
  foreach ($data as $field => $value) {

    // Fetch the drupal field.
    $drupal_field = openlucius_services_field_mapping($node_type, $field);

    // Process and set the data.
    _openlucius_services_node_set_field($wrapper, $drupal_field, $data);
  }

  // Save the node.
  $wrapper->save();

  // Fetch the node id.
  $nid = $wrapper->getIdentifier();

  // Check if we have files,
  if (!empty($_FILES)) {

    // Load the node resource for attaching files.
    module_load_include('inc', 'services', 'resources/node_resource');
    $drupal_field = openlucius_services_field_mapping($node_type, 'files');
    $files        = _openlucius_services_process_files($nid, $drupal_field);

    // Check if the files where uploaded successfully.
    if (empty($files)) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_UPLOAD_FAILED, array('@path' => url('node/' . $nid, array('absolute' => TRUE))));
    }
  }

  // Notify the API user.
  return openlucius_services_message($nid, t('Successfully created @type: @title', array(
        '@type'  => $type,
        '@title' => $data['title'],
      )), url('node/' . $nid, array('absolute' => TRUE)));
}

/**
 * Function to update a node.
 *
 * @param int $nid
 *   The node id to be updated.
 * @param array $data
 *   The data required for updating a node.
 *
 * @return array
 *   Returns a response when the action was completed successfully.
 * @throws ServicesException
 *   If the call is not accepted this method will throw a ServicesException 406.
 */
function _openlucius_services_update_node($nid, array $data) {
  global $user;

  // Verify that the submitted $nid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($nid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
        '@input'       => gettype($nid),
        '@requirement' => 'integer',
        '@for'         => 'id',
      ));
  }

  // Load the node.
  $node = node_load($nid);

  // Check if we have a node to work with.
  if (empty($node)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_NOT_FOUND, array("@nid" => $nid));
  }

  // Check if the user may create a node of this type.
  if (!user_access("edit any $node->type content")
      && (!user_access("edit own $node->type content") && $user->uid != $node->uid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_EDIT_ACCESS_DENIED);
  }

  // Use the metadata wrapper for quick access to fields.
  $wrapper = entity_metadata_wrapper('node', $node);

  // Check if the node has a group and if it does verify
  // that the user has access.
  if (isset($node->field_shared_group_reference)) {
    $group_node = $wrapper->field_shared_group_reference->value();
    if (!openlucius_core_user_in_group($group_node->nid, $user->uid) && $user->uid != 1) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_GROUP_ACCESS_DENIED);
    }
  }

  // Filter posted data, not all fields may be edited on the edit form.
  // Some functionality has been divided onto other api paths.
  openlucius_services_fields_filter($data, $node->type);

  // Check if we have data.
  if (!empty($data) || !empty($_FILES)) {

    // Loop through each item.
    foreach ($data as $field => $value) {

      // Fetch the drupal field.
      $drupal_field = openlucius_services_field_mapping($node->type, $field);

      // Process and set the data.
      _openlucius_services_node_set_field($wrapper, $drupal_field, $data);
    }

    // Force revision creation.
    $wrapper->revision->set(1);

    // Save the changes.
    $wrapper->save();

    // Check if we have files,
    if (!empty($_FILES)) {

      // Load the node resource for attaching files.
      module_load_include('inc', 'services', 'resources/node_resource');
      $drupal_field = openlucius_services_field_mapping($node->type, 'files');
      $files        = _openlucius_services_process_files($nid, $drupal_field);

      // Check if the files where uploaded successfully.
      if (empty($files)) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_UPLOAD_FAILED, array('@path' => url('node/' . $nid, array('absolute' => TRUE))));
      }
    }

    return openlucius_services_message($nid, t("Node successfully updated"), url('node/' . $nid, array('absolute' => TRUE)));
  }
  // Empty request so something went wrong.
  else {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_EMPTY_UPDATE_REQUEST);
  }
}

/**
 * Function for moving a node between groups and todo_lists.
 *
 * @param int $nid
 *   The node to be moved.
 * @param array $data
 *   The data for moving the node.
 *
 * @return array
 *   Returns a response when the action was completed successfully.
 * @throws \ServicesException
 *   Throws an exception if an error occurs.
 */
function _openlucius_services_move_node($nid, array $data) {
  global $user;

  // Verify that the submitted $nid is indeed an integer.
  // TODO this shouldn't be necessary, int is set in hook_services_resources.
  if (!is_numeric($nid)) {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
      '@input'       => gettype($nid),
      '@requirement' => 'integer',
      '@for'         => 'id',
    ));
  }

  // Check if the user is allowed to move nodes.
  if (user_access('move nodes')) {

    // Load the node.
    $node = node_load($nid);

    // Check if we have a node to work with.
    if (empty($node)) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_NOT_FOUND, array("@nid" => $nid));
    }

    // Check if this node type is movable.
    if (!isset($node->field_shared_group_reference)) {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MOVE_IMPOSSIBLE);
    }

    // Check if the user is allowed to edit this node.
    if (node_access('update', $node)) {

      // Fetch input type by reverse mapping.
      $type         = openlucius_services_map_type($node->type, TRUE);
      $requirements = _openlucius_services_entity_requirements($type, OPENLUCIUS_SERVICES_MOVE);

      // Check if the requirements are set.
      foreach ($requirements as $requirement) {
        if (empty($data[$requirement])) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MISSING_PARAMETER, array("@parameter" => $requirement));
        }
      }

      // Check if the user is a member of this group.
      if (!openlucius_core_user_in_group($data['group'], $user->uid) && $user->uid != 1) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_GROUP_ACCESS_DENIED);
      }

      // Use the metadata wrapper for quick access to fields.
      $wrapper = entity_metadata_wrapper('node', $node);

      foreach ($data as $field => $value) {

        // Only move specific fields are allowed.
        if (in_array($field, array('group', 'todo_list'))) {

          // Fetch the drupal field.
          $drupal_field = openlucius_services_field_mapping($node->type, $field);

          // Process and set the data.
          _openlucius_services_node_set_field($wrapper, $drupal_field, $data);
        }
      }

      // Save the changes if any.
      $wrapper->save();

      // Return response.
      return openlucius_services_message($nid, OPENLUCIUS_SERVICES_UPDATED_MESSAGE, url("node/" . $nid, array("absolute" => TRUE)));
    }

    // The user is not allowed to edit this node.
    else {
      openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NODE_EDIT_ACCESS_DENIED);
    }
  }

  // The user has no access for this action.
  else {
    openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_MOVE_ACCESS_DENIED);
  }
}

/**
 * Function for publishing nodes.
 *
 * @param mixed $data
 *   The node / nodes to be unpublished.
 *
 * @return array
 *   Returns a response when the action was completed successfully.
 * @throws \Exception
 */
function _openlucius_services_publish_node($data) {

  // Alter for single update.
  if (gettype($data) != 'array') {
    $data = array('nodes' => array(intval($data)));
  }

  // Initiate counters for checking if nodes have been published / unpublished.
  $success = $impossible = $failures = 0;

  // Process nodes.
  return _openlucius_services_batch_publish_unpublish_nodes($data['nodes'], NODE_PUBLISHED, $success, $impossible, $failures);
}

/**
 * Function for unpublishing nodes.
 *
 * @param mixed $data
 *   The node / nodes to be unpublished.
 *
 * @return array
 *   Returns a response when the action was completed successfully.
 * @throws \Exception
 */
function _openlucius_services_unpublish_node($data) {

  // Alter for single update.
  if (gettype($data) != 'array') {
    $data = array('nodes' => array(intval($data)));
  }

  // Initiate counters for checking if nodes have been published / unpublished.
  $success = $impossible = $failures = 0;

  // Process nodes.
  return _openlucius_services_batch_publish_unpublish_nodes($data['nodes'], NODE_NOT_PUBLISHED, $success, $impossible, $failures);
}

/**
 * Function for publishing / unpublishing multiple nodes.
 *
 * @param array $data
 *   The data to be processed.
 * @param int $status
 *   The status the node should receive.
 * @param int $success
 *   The amount of successful published nodes /
 * @param int $impossible
 *   The amount of impossible updates
 *   (for example: status NODE_NOT_PUBLISHED will not be unpublished)
 * @param int $failures
 *   The amount of failed updates.
 *
 * @return array
 *   Returns a message containing the amount of failures.
 * @throws \Exception
 *   Throws an exception if the parameters are wrong.
 */
function _openlucius_services_batch_publish_unpublish_nodes(array $data, $status, &$success, &$impossible, &$failures) {
  // Load nodes.
  $nodes = node_load_multiple($data);

  foreach ($nodes as $node) {

    // Verify that this user may edit the node.
    if (node_access('update', $node) && $node->status !== $status) {

      if ($status == NODE_NOT_PUBLISHED) {
        // Publish the node.
        node_publish_action($node);
      }
      else {

        // Unpublish the node.
        node_unpublish_action($node);
      }
      node_save($node);
      $success++;
    }
    elseif ($node->status == $status) {
      $impossible++;
    }
    else {
      $failures++;
    }
  }

  $parameters = array(
    '@success'  => $success,
    '@active'   => $impossible,
    '@failures' => $failures,
  );

  // Send message.
  if ($status == NODE_PUBLISHED) {
    return openlucius_services_message(NULL, t("Successfully published: @success \n skipped: @active published nodes \n  failed to publish: @failures", $parameters), NULL);
  }
  else {
    return openlucius_services_message(NULL, t("Successfully unpublished: @success \n skipped: @active unpublished nodes \n  failed to unpublish: @failures", $parameters), NULL);
  }
}

/**
 * Helper methods.
 */

/**
 * Function to set a value on a node.
 *
 * @param \EntityMetadataWrapper $wrapper
 *   The entityMetadataWrapper to set the value.
 * @param string $field
 *   The field to receive the value.
 * @param mixed $data
 *   The value or values to be set.
 *
 * @return mixed
 *   Only returns a value if an error has occurred.
 * @throws \ServicesException
 *   Throws an exception if the field could not be mapped.
 */
function _openlucius_services_node_set_field(\EntityMetadataWrapper &$wrapper, $field, $data = NULL) {
  global $user;

  // Switch on field for correct processing.
  switch ($field) {

    // Set the node title.
    case 'title':
      $wrapper->{$field}->set($data['title']);
      break;

    // Set the group or todolist (node references).
    case 'field_shared_group_reference':
      // Verify that the user trying to set this group is actually in the
      // group or super admin.
      if (!openlucius_core_user_in_group($data['group'], $user->uid) && $user->uid != 1) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_GROUP_ACCESS_DENIED);
      }
      $wrapper->{$field}->set((int) $data['group']);
      break;

    case 'field_todo_list_reference':
      $wrapper->{$field}->set((int) $data['todo_list']);
      break;

    // Set the due date of a node.
    case 'field_todo_due_date_singledate':
      $due_date = $data['date'];
      // Check if this is actually timestamp.
      if (!is_numeric($due_date)) {
        $due_date = strtotime($due_date);
      }

      // Set the due date.
      $wrapper->{$field}->set((int) $due_date);
      break;

    // Set the label "status" of a todo.
    case 'field_todo_label':

      // Get the default status "Open".
      $status = variable_get('todo_openstatus_tid');

      // Check if we have a status.
      if (!empty($data['status'])) {

        // Check if the type is numeric.
        if (!is_numeric($data['status'])) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
              '@input'       => gettype($data['status']),
              '@requirement' => 'integer',
              '@for'         => 'status',
            ));
        }

        // Check if this is an existing status.
        $statuses = openlucius_core_get_terms('labels_todo_s');
        if (!isset($statuses[$data['status']])) {
          openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NON_EXISTING_TERM, array("@type" => "status"));
        }

        // Set the status to the posted one.
        $status = $data['status'];
      }

      // Set the status.
      $wrapper->{$field}->set((int) $status);
      break;

    // Assign the node to a user.
    case 'field_todo_user_reference':

      // Check if the input is of the correct type.
      if (!is_numeric($data['user'])) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_WRONG_PARAMETER_TYPE, array(
            '@input'       => gettype($data['user']),
            '@requirement' => 'integer',
            '@for'         => 'user',
          ));
      }

      // Check if the user is actually a part of this group.
      if (!openlucius_core_user_in_group($data['group'], $data['user'])) {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_USER_NOT_IN_GROUP);
      }

      // Set the user reference.
      $wrapper->{$field}->set((int) $data['user']);
      break;

    // Set the description.
    case 'body':
    case 'field_todolist_description':
      $filtered_body = check_markup($data['body']);
      $wrapper->{$field}->set(array('value' => $filtered_body));
      break;

    case 'field_shared_show_clients':
      $wrapper->{$field}->set((int) $data['show_clients']);
      break;

    default:
      // Check if there's a module ready to process the data.
      if (count(module_implements('_openlucius_services_node_set_field')) > 0) {
        module_invoke_all('_openlucius_services_node_set_field', $wrapper, $field, $data);
      }
      // Return error as this field cannot yet be processed.
      else {
        openlucius_services_error(OPENLUCIUS_SERVICES_ERROR_NOT_IMPLEMENTED_YET, array(
          "@type"   => $field,
          '@method' => '_openlucius_services_node_set_field',
          '@see'    => 'includes/resource.node.crud.inc',
        ));
      }
  }
}

/**
 * Function for attaching files to a node.
 *
 * This method is a copy of the _node_resource_attach_file().
 * This has been done for our hidden display_field as this results in hidden
 * files when using the original method.
 *
 * @param int $nid
 *   The node id which should receive the files.
 * @param string $field_name
 *   The name of the field which should obtain the files.
 * @param bool $attach
 *   Whether the files should be added or override the previous files.
 *
 * @return array
 *   Returns an array containing the uploaded files.
 */
function _openlucius_services_process_files($nid, $field_name, $attach = TRUE) {
  // Load include file for validation and saving.
  module_load_include('inc', 'services', 'resources/node_resource');

  // Load the node.
  $node = node_load($nid);

  // Check if the field is empty.
  if (empty($node->{$field_name}[LANGUAGE_NONE])) {
    $node->{$field_name}[LANGUAGE_NONE] = array();
  }

  // Validate whether field instance exists and this node type can be edited.
  _node_resource_validate_node_type_field_name('update', array(
    $node->type,
    $field_name,
  ));

  // Check if we need to override the old files or append the files.
  $counter = 0;
  if ($attach) {
    $counter = count($node->{$field_name}[LANGUAGE_NONE]);
  }
  else {
    $node->{$field_name}[LANGUAGE_NONE] = array();
  }

  $options = array('attach' => $attach, 'file_count' => $counter);

  // Upload the files.
  list($files, $file_objs) = _node_resource_file_save_upload($node->type, $field_name, $options);

  // Attach the uploaded files to the node.
  foreach ($file_objs as $file_obj) {
    $node->{$field_name}[LANGUAGE_NONE][$counter]            = (array) $file_obj;
    $node->{$field_name}[LANGUAGE_NONE][$counter]['display'] = 1;
    $counter++;
  }

  // Save the node.
  node_save($node);

  // Return the uploaded files.
  return $files;
}
