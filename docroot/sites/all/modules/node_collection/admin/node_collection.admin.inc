<?php
/**
 * @file
 * The file that contains the code to build up the admin form of the
 * node_collection module
 */

/**
 * Form constructor for the node collection admin settings form.
 *
 * @see node_collection_admin_settings_form_submit()
 * @ingroup forms
 */
function node_collection_admin_settings_form($form, &$form_state) {
  // Define the empty arrays for parent and child options.
  $child_options = array();
  $parent_options = array();
  // Define the empty form array().
  $form = array();

  // Get available content types.
  $content_types = node_type_get_types();
  // Get all the created Parent-Child relationship parents.
  $saved_parents = node_collection_api_get_node_types();
  // Non saved parents.
  $non_saved_parents = array_diff(array_keys($content_types), array_keys($saved_parents));

  if (empty($content_types) == FALSE) {
    // Set the options for the parent node type.
    foreach ($content_types as $content_type_key => $content_type_properties) {
      $child_options[$content_type_key] = $content_type_properties->name;
      // Add the parents that is left to be created as relationships.
      foreach ($non_saved_parents as $non_save_parent) {
        if ($non_save_parent == $content_type_key) {
          $parent_options[$content_type_key] = $content_type_properties->name;
        }
      }
    }
  }

  // Add a notice if all the node types are selected as parents.
  if (empty($parent_options)) {
    drupal_set_message(t('All the node types available has been selected as parents.'), 'warning');
  }
  else {
    $form['node_collection_admin_settings_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Node Collection Admin Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );

    $form['node_collection_admin_settings_fieldset']['relationship_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of the relationship'),
      '#size' => 60,
      '#default_value' => isset($form_state['values']['relationship_name']) ? $form_state['values']['relationship_name'] : '',
      '#required' => TRUE,
    );

    $form['node_collection_admin_settings_fieldset']['parent_node'] = array(
      '#type' => 'select',
      '#title' => t('Select a parent node'),
      '#description' => t('This will be the parent node and all the paths of the children will be relative to this.'),
      '#options' => isset($form_state['values']['parent_node']) ? array($form_state['values']['parent_node'] => $content_types[$form_state['values']['parent_node']]->name) : $parent_options,
      '#default_value' => isset($form_state['values']['parent_node']) ? $form_state['values']['parent_node'] : '',
      '#disabled' => isset($form_state['values']['parent_node']) ? TRUE : FALSE,
      '#required' => TRUE,
    );

    $form['node_collection_admin_settings_fieldset']['child_node_types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Select the child node types'),
      '#description' => t('You do not have to select the parent node on this. If you do so it will be ignored.'),
      '#options' => $child_options,
      '#default_value' => isset($form_state['values']['child_node_types']) ? $form_state['values']['child_node_types'] : array(),
      '#required' => TRUE,
    );

    $form['node_collection_admin_settings_fieldset']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save relationship'),
    );
  }

  return $form;
}

/**
 * Form submission handler for node_collection_admin_settings_form().
 */
function node_collection_admin_settings_form_submit($form, &$form_state) {
  $parent_type = NULL;
  $relationship_name = NULL;
  $child_node_types = array();

  // Set the relationship name.
  if (isset($form_state['values']['relationship_name'])) {
    $relationship_name = trim($form_state['values']['relationship_name']);
  }

  // Set the parent type.
  if (isset($form_state['values']['parent_node'])) {
    $parent_type = trim($form_state['values']['parent_node']);
  }

  // Add the child node types.
  if (isset($form_state['values']['child_node_types']) && is_array($form_state['values']['child_node_types'])) {
    foreach ($form_state['values']['child_node_types'] as $key => $child_node) {
      if ($child_node && $key != $parent_type) {
        $child_node_types[] = $child_node;
      }
    }
  }

  // Add the new relationship and catch if there are any errors.
  try {
     // Insert the relationship.
    node_collection_api_add_update_relationship($relationship_name, $parent_type, $child_node_types);
    // Set a success message.
    drupal_set_message(
      t(
        "Parent child relationship '@relationship' has been saved successfully",
        array('@relationship' => $form_state['values']['relationship_name'])
      ),
      'status'
    );

    // Redirect the form to the list.
    $form_state['redirect'] = array(
      NODE_COLLECTION_EDIT_DELETE_PATH,
      array(),
      302,
    );
  }
  catch (Exception $insert_relationship_exception) {
    drupal_set_message(t('Failed to create/update the relationship'), 'error');
    watchdog_exception('node_collection', $insert_relationship_exception);
  }
}

/**
 * Render existing relationships
 * @return string
 *   Returns a themes table of existing relationships
 */
function node_collection_admin_settings_saved_node_types() {
  // Add the Add relationship link.
  $output = l(t('Create parent > child relationship'), NODE_COLLECTION_CREATE_PATH);
  // Get the relationship details.
  $node_details = node_collection_api_get_parent_child_relationship_object();
  // Headers for the table.
  $header = array(
    t('Relationship name'),
    t('Parent node type'),
    t('Child node types'),
    t('Edit/Delete'),
  );

  // Table data collector.
  $table_data = array();

  // Set the table data.
  foreach ($node_details as $relationship) {
    $table_data[] = array(
      check_plain($relationship['relationship_name']),
      check_plain($relationship['parent_type_name']),
      theme(
        'item_list',
        array(
          'items' => array_values($relationship['children']),
          'type' => 'ul',
        )
      ),
      l(t('Edit'), NODE_COLLECTION_EDIT_DELETE_PATH_ACTION . '/edit/' . $relationship['parent_type']) . ' ' . l(t('Delete'), NODE_COLLECTION_EDIT_DELETE_PATH_ACTION . '/delete/' . $relationship['parent_type']),
    );
  }

  return "<p>$output</p>" . theme(
    'table',
    array(
      'header' => $header,
      'rows' => $table_data,
      'empty' => t('There are no relationships defined yet.'),
    )
  );
}

/**
 * Edit delete parent child relationships
 *
 * @param string $action
 *   The action to perform
 *
 * @param int $node_id
 *   The node_id
 */
function node_collection_perform_action($action, $node_id) {
  $relationship_values = array();

  // Get the relationship.
  if (isset($node_id) && empty($node_id) == FALSE) {
    $relationship_values = node_collection_api_get_parent_child_relationship_object($node_id);
  }

  switch ($action) {
    case 'edit':
      // Set the message and redirect to where the request came from.
      if (empty($relationship_values)) {
        drupal_set_message(t('Invalid arguments specified'), 'error');
        drupal_goto(NODE_COLLECTION_EDIT_DELETE_PATH);
      }
      else {
        // Form state array to set.
        $form_state = array();
        // Relationship children.
        $children = array();
        // Get the content types.
        $content_types = node_type_get_types();

        if (isset($relationship_values[0]['children']) && is_array($relationship_values[0]['children'])) {
          $saved_children = array_keys($relationship_values[0]['children']);
          foreach ($content_types as $type_key => $type_value) {
            if (in_array($type_key, $saved_children)) {
              $children[$type_key] = $type_key;
            }
            else {
              $children[$type_key] = 0;
            }
          }
        }

        // Set the form state values.
        $form_state['values']['relationship_name'] = isset($relationship_values[0]['relationship_name'])
          ? $relationship_values[0]['relationship_name'] : NULL;
        $form_state['values']['parent_node'] = isset($relationship_values[0]['parent_type'])
          ? $relationship_values[0]['parent_type'] : NULL;
        $form_state['values']['child_node_types'] = $children;
        $form_state['build_info'] = array('args' => array());

        return drupal_build_form('node_collection_admin_settings_form', $form_state);
      }
      break;

    case 'delete':
      if (empty($relationship_values)) {
        drupal_set_message(t('Invalid arguments specified'), 'error');
        drupal_goto(NODE_COLLECTION_EDIT_DELETE_PATH);
      }
      else {
        $form_state['values']['parent_node'] = $node_id;
        return drupal_build_form('node_collection_admin_settings_delete_relationship_confirm', $form_state);
      }
      break;

    default:
      // Deny access if somebody is trying to muck around.
      drupal_access_denied();
  }
}

/**
 * Form constructor for node collection admin settings delete relationship
 * confirmation.
 *
 * @see node_collection_admin_settings_delete_relationship_confirm_submit()
 * @ingroup forms
 */
function node_collection_admin_settings_delete_relationship_confirm($form, &$form_state) {
  $form['relationship'] = array(
    '#type' => 'hidden',
    '#value' => isset($form_state['values']['parent_node']) ? $form_state['values']['parent_node'] : '',
  );

  return confirm_form(
    $form,
    t('Are you sure that you need to delete this relationship ?'),
    NODE_COLLECTION_EDIT_DELETE_PATH,
    t('Please confirm that you need to delete this relationship. Performing this action will delete the relationship data. This action cannot be rolled back/undone.')
  );
}

/**
 * Form submission handler for
 * node_collection_admin_settings_delete_relationship_confirm().
 */
function node_collection_admin_settings_delete_relationship_confirm_submit($form, &$form_state) {
  // If the user confirms delete the relationship.
  if ($form_state['values']['confirm']) {
    $relationship = isset($form_state['values']['relationship']) ? check_plain($form_state['values']['relationship']) : '';
    try {
      node_collection_api_delete_relationship($relationship);
      drupal_set_message(t('@relationship successfully deleted', array('@relationship' => $relationship)), 'status');
    }
    catch (Exception $del_exception) {
      drupal_set_message(t('Failed to delete @relationship', array('@relationship' => $relationship)), 'error');
    }
    // Redirect to the list.
    drupal_goto(NODE_COLLECTION_EDIT_DELETE_PATH);
  }
}
