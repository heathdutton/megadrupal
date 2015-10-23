<?php

/**
 * @file
 * The admin functions for the module.
 *
 * Handles the administration interface callbacks.
 */

/**
 * Callback for the main Block Inject page.
 */
function block_inject_list() {
  $result = block_inject_get_regions();
  $header = array(
    array('data' => t('Inject Regions')),
    array('data' => t('Condition by default')),
    array('data' => t('Operations'), 'colspan' => 2),
  );
  $rows = array();
  foreach ($result as $row) {
    // Check if there is condition.
    if ($row->bi_condition) {
      $condition = unserialize($row->bi_condition);
    }
    $tablerow = array(
      array('data' => check_plain($row->region) . ' | ' . check_plain($row->node_name)),
      array('data' => isset($condition) ? 'Yes' : 'No'),
      array('data' => l(t('Edit'), 'admin/structure/block-inject/' . $row->id . '/edit')),
      array('data' => l(t('Delete'), 'admin/structure/block-inject/' . $row->id . '/delete')),
    );
    $rows[] = $tablerow;
  }
  if (!$rows) {
    $rows[] = array(array('data' => t('No inject regions available.'), 'colspan' => 4));
  }
  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'block_inject'),
  );

  return $build;
}

/**
 * Callback for the Block Inject Exceptions page.
 */
function block_inject_exceptions_list() {
  $result = block_inject_get_exceptions();
  $header = array(
    array('data' => t('Excepted Nodes')),
    array('data' => t('Node type')),
    array('data' => t('Type of exception')),
    array('data' => t('Offset')),
    array('data' => t('Offset from default condition')),
    array('data' => t('Edit')),
  );
  $rows = array();
  if (isset($result)) {
    foreach ($result as $row) {
      $node = node_load($row->nid);
      $node_type = node_type_load($node->type);
      
      // Check the exception value for the node.
      $get_exception = block_inject_green_light($row->nid);
      if ($get_exception['except_injection'] == 1) {
        $exception = 'Not injected';
      }
      else {
        $exception = 'Injected';
      }
      
      // Check the offset for the node.
      $get_offset = block_inject_get_offset($row->nid);
      if ($get_offset != 0) {
        $offset = $get_offset['offset'] . ' paragraphs';
      }
      else {
        $offset = 'None';
      }
      
      // Check for existing condition on this node.
      $region_id = block_inject_find_bi_id($node->type);
      $region = block_inject_get_region_by_id($region_id->id);
      if ($region->bi_condition) {
        $condition = unserialize($region->bi_condition);
        // Get the body field.
        $body_field = field_get_items('node', $node, 'body');
        $body = $body_field[0]['safe_value'];
        $condition_offset = block_inject_process_condition($node->type, $body);
      }

      $tablerow = array(
        array('data' => l(check_plain($node->title), 'node/' . $node->nid)),
        array('data' => check_plain($node_type->name)),
        array('data' => $exception),
        array('data' => $offset),
        array('data' => isset($condition_offset) ? $condition_offset . ' paragraphs' : 'No'),
        array('data' => l(t('Edit'), 'node/' . $node->nid . '/edit')),
      );
      $rows[] = $tablerow;
    }
  }

  if (!$rows) {
    $rows[] = array(array('data' => t('No exceptions available.'), 'colspan' => 6));
  }
  $build = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => array('id' => 'block_inject_exceptions'),
  );

  return $build;
}

/**
 * Callback form to add new region.
 */
function block_inject_add_inject_form($form, &$form_state) {
  $form['block_inject_region_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Region name'),
    '#description' => t('What is the name of the region you would like to inject?'),
    '#required' => TRUE,
  );
  // Return available node types to have as options for the select form element.
  $node_types = block_inject_get_node_types();
  $form['block_inject_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Available Content Type(s)'),
    '#description' => t('Which content type would you like this region to be 
      injected in the middle of?'),
    '#options' => $node_types,
    '#multiple' => TRUE,
  );
  $form['block_inject_conditionals_toggle'] = array(
    '#title' => t('Would you like to condition the placement of the region injection?'),
    '#type' => 'checkbox',
  );
  $form['block_inject_conditionals'] = array(
    '#title' => t('The condition'),
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">', 
    '#suffix' => '</div>',
    // Show only if conditional checkbox is selected.
    '#states' => array(
      'visible' => array(
        'input[name="block_inject_conditionals_toggle"]' => array('checked' => TRUE),
      ),
    )
  );
  $form['block_inject_conditionals']['block_inject_explanation_1'] = array(
    '#type' => 'markup',
    '#markup' => t('The destination needs to have ')  
  );
  $form['block_inject_conditionals']['block_inject_paragraph_operator'] = array(
    '#type' => 'select',
    '#options' => array('<' => 'less than', '=' => 'exactly', '>' => 'more than'),
    '#multiple' => FALSE,
  );
  $form['block_inject_conditionals']['block_inject_paragraph_number'] = array(
    '#type' => 'textfield',
    '#maxlength' => 3,
    '#size' => 4,
  ); 
  $form['block_inject_conditionals']['block_inject_explanation_2'] = array(
    '#type' => 'markup',
    '#markup' => t(' paragraphs in order to fire the offset action below.')  
  );
  $form['block_inject_action'] = array(
    '#title' => t('The action'),
    '#type' => 'fieldset',
    '#description' => t('The action to take if the condition above is met.'),
    // Show only if conditional checkbox is selected.
    '#states' => array(
      'visible' => array(
        'input[name="block_inject_conditionals_toggle"]' => array('checked' => TRUE),
      ),
    )
  );
  $form['block_inject_action']['block_inject_paragraph_offset'] = array(
    '#title' => t('Paragraph offset'),
    '#type' => 'textfield',
    '#description' => t('Please specify a positive or negative number to 
      offset the injection (e.g. 1 to move down by one paragraph / 
      -1 to move up by one paragraph).'),
  ); 
  $form['block_inject_region_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create region'),
  );

  return $form;
}

/**
 * Validation function for the Block Inject add/edit form.
 */
function block_inject_add_inject_form_validate($form, &$form_state) {
  // Check for duplicate name.
  $region_name = $form_state['values']['block_inject_region_name'];
  $test = block_inject_check_region_name($region_name); 
  if ($test == FALSE && (!isset($form_state['block_inject_region_id']))) {
    form_set_error('', t('Duplicate region name. Please choose another one!'));
  }
  if ($form_state['values']['block_inject_conditionals_toggle'] == 1) {
    // Check if the paragraph condition was set and is integer.
    if (!is_numeric($form_state['values']['block_inject_paragraph_number'])) {
       form_set_error('', t('Please make sure paragraph number is an integer.'));
    }
    // Check if the paragraph offset was set and is integer.
    if (!is_numeric($form_state['values']['block_inject_paragraph_offset'])) {
       form_set_error('', t('Please make sure paragraph offset is an integer.'));
    }
  }
}

/**
 * Submit form that adds the new region to the database.
 */
function block_inject_add_inject_form_submit($form, &$form_state) {
  $region = $form_state['values']['block_inject_region_name'];
  if (isset($form_state['values']['block_inject_content_type'])) {
    // If set, retrieve the machine name of the selected node typs.
    $node_type_array = $form_state['values']['block_inject_content_type'];
    // Retrieve the names of the selected node types.
    $node_type_name_array = array_flip(array_intersect(array_flip($form_state['complete form']['block_inject_content_type']['#options']), $node_type_array));
    // Implode the node type arrays.
    $node_type_name = check_plain(implode(', ', $node_type_name_array));
    $node_type = check_plain(implode(', ', $node_type_array));
  }
  else {
    $node_type = '';
    $node_type_name = '';
  }
  // Check if region name was provided, process condition and add to database.
  if ($region) {
    // Process conditions for the inject if there are any
    if ($form_state['values']['block_inject_conditionals_toggle'] == 1) {
      if (is_numeric($form_state['values']['block_inject_paragraph_number']) &&
        is_numeric($form_state['values']['block_inject_paragraph_offset'])) {
          $condition = array(
            'condition' => array(
              'operator' => $form_state['values']['block_inject_paragraph_operator'],
              'paragraph_no' => $form_state['values']['block_inject_paragraph_number'],  
          ),
            'action' => array(
              'offset' => $form_state['values']['block_inject_paragraph_offset'],  
            ),
          );
        }
        else {
          // Need to do a better validation handling here with _validate.
           return drupal_set_message(t('Please make sure the paragraph number is an 
            integer'), 'error');
        }
        $serialized = serialize($condition);
        block_inject_add_region($region, $node_type, $node_type_name, $serialized);
    }
    else {
      block_inject_add_region($region, $node_type, $node_type_name);
    }
    
    // Redirect back to main Block Inject page.
    $form_state['redirect'] = 'admin/structure/block-inject';
    // Rebuild data and clear cache.
    block_inject_rebuild_data();
    drupal_set_message(t('Your region has been added.'), 'status');
  }
  else {
    drupal_set_message(t('There was an error'), 'error');
  }
}

/**
 * Callback form to edit a region.
 *
 * @param int $id
 *   Id of the region to be edited.
 */
function block_inject_edit_form($form, &$form_state, $id) {
  // Get the region by the ID that came through the the URL.
  $region = block_inject_get_region_by_id($id);
  // Pass the region id through the form_state for the submit function.
  $form_state['block_inject_region_id'] = $id;
  $form['block_inject_region_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Region name'),
    '#description' => t('What is the name of the region you would like to inject?'),
    '#required' => TRUE,
    '#default_value' => check_plain($region->region),
  );
  // Get the available node types to be selected.
  $available_node_types = block_inject_get_node_types();
  // Get the node types selected by this region.
  $sel_node_type_machine_array = explode(', ', $region->node_type);
  $sel_node_type_name_array = explode(', ', $region->node_name);
  $selected_node_types = array_combine($sel_node_type_machine_array, $sel_node_type_name_array);
  // Add to the available node types the ones selected by this region.
  $node_types_options = array_merge($available_node_types, $selected_node_types);
  $form['block_inject_content_type'] = array(
    '#type' => 'select',
    '#title' => t('Available Content Type(s)'),
    '#description' => t('Which content type would you like this region to be injected in the middle of?'),
    '#options' => $node_types_options,
    '#multiple' => TRUE,
    '#default_value' => array_flip($selected_node_types),
  );
  $form['block_inject_conditionals_toggle'] = array(
    '#title' => t('Would you like to condition the placement of the region injection?'),
    '#type' => 'checkbox',
    '#default_value' => $region->bi_condition ? 1 : 0,
  );
  // Unserialize condition.
  if ($region->bi_condition) {
    $condition = unserialize($region->bi_condition);
  }
  $form['block_inject_conditionals'] = array(
    '#title' => t('The condition'),
    '#type' => 'fieldset',
    '#prefix' => '<div class="container-inline">', 
    '#suffix' => '</div>',
    // Show only if conditional checkbox is selected.
    '#states' => array(
      'visible' => array(
        'input[name="block_inject_conditionals_toggle"]' => array('checked' => TRUE),
      ),
    )
  );
  $form['block_inject_conditionals']['block_inject_explanation_1'] = array(
    '#type' => 'markup',
    '#markup' => t('The destination needs to have ')  
  );
  $form['block_inject_conditionals']['block_inject_paragraph_operator'] = array(
    '#type' => 'select',
    '#options' => array('<' => 'less than', '=' => 'exactly', '>' => 'more than'),
    '#multiple' => FALSE,
    '#default_value' => isset($condition) ? $condition['condition']['operator'] : '<',
  );
  $form['block_inject_conditionals']['block_inject_paragraph_number'] = array(
    '#type' => 'textfield',
    '#maxlength' => 3,
    '#size' => 4,
    '#default_value' => isset($condition) ? $condition['condition']['paragraph_no'] : '',
  ); 
  $form['block_inject_conditionals']['block_inject_explanation_2'] = array(
    '#type' => 'markup',
    '#markup' => t(' paragraphs in order to fire the offset action below.')  
  );
  $form['block_inject_action'] = array(
    '#title' => t('The action'),
    '#type' => 'fieldset',
    '#description' => t('The action to take if the condition above is met.'),
    // Show only if conditional checkbox is selected.
    '#states' => array(
      'visible' => array(
        'input[name="block_inject_conditionals_toggle"]' => array('checked' => TRUE),
      ),
    )
  );
  $form['block_inject_action']['block_inject_paragraph_offset'] = array(
    '#title' => t('Paragraph offset'),
    '#type' => 'textfield',
    '#description' => t('Please specify a positive or negative number to 
      offset the injection (e.g. 1 to move down by one paragraph / 
      -1 to move up by one paragraph).'),
    '#default_value' => isset($condition) ? $condition['action']['offset'] : '',
  ); 
  $form['block_inject_region_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
    '#validate' => array('block_inject_add_inject_form_validate'),
  );

  return $form;
}

/**
 * Submit form to edit a region.
 */
function block_inject_edit_form_submit($form, &$form_state) {
  // Get region
  $region = $form_state['values']['block_inject_region_name'];
  // Run the name through the function to check if it is already in use.
  $name_test = block_inject_check_region_name($region);
  if (($name_test == TRUE) || ($region == $form['block_inject_region_name']['#default_value'])) {
    if (isset($form_state['input']['block_inject_content_type'])) {
      // If set, retrieve the machine name of the selected node typs.
      $node_type_array = $form_state['input']['block_inject_content_type'];
      // Retrieve the names of the selected node types.
      $node_type_name_array = array_flip(array_intersect(array_flip($form_state['complete form']['block_inject_content_type']['#options']), $node_type_array));
      // Implode the node type arrays.
      $node_type_name = check_plain(implode(', ', $node_type_name_array));
      $node_type = check_plain(implode(', ', $node_type_array));
    }
    else {
      $node_type = '';
      $node_type_name = '';
    }
    // Check if region name was provided and add to database.
    if ($region) {     
      $id = $form_state['block_inject_region_id'];
      // Process conditions for the inject if there are any
      if ($form_state['values']['block_inject_conditionals_toggle'] == 1) {
        if (is_numeric($form_state['values']['block_inject_paragraph_number']) &&
          is_numeric($form_state['values']['block_inject_paragraph_offset'])) {
            $condition = array(
              'condition' => array(
                'operator' => $form_state['values']['block_inject_paragraph_operator'],
                'paragraph_no' => $form_state['values']['block_inject_paragraph_number'],  
            ),
              'action' => array(
                'offset' => $form_state['values']['block_inject_paragraph_offset'],  
              ),
            );
          }
          $serialized = serialize($condition);
          block_inject_update_region($id, $region, $node_type, $node_type_name, $serialized);
      }
      else {
        block_inject_update_region($id, $region, $node_type, $node_type_name);
      } 
      // Redirect back to main Block Inject page.
      $form_state['redirect'] = 'admin/structure/block-inject';
      block_inject_rebuild_data();
      drupal_set_message(t('Your region has been updated.'), 'status');
    }
    else {
      drupal_set_message(t('There was an error'), 'error');
    }
  }
  else {
    drupal_set_message(t('Duplicate region name. Please choose another one!'), 'error');
  }
}

/**
 * Callback function for confirming the inject region delete.
 *
 * @param int $region_id
 *   Id of the region to be deleted.
 */
function block_inject_delete_confirm($form, &$form_state, $region_id) {
  $region = block_inject_get_region_by_id($region_id);
  // Pass the region through the form_state for the submit function.
  $form_state['block_inject_region'] = $region;
  return confirm_form($form, t('Are you sure you want to delete the block inject region %title ?', array('%title' => $region->region)), 'admin/structure/block-inject', 'The blocks that are assigned to this region will be reassigned. This action cannot be undone!', t('Delete'), t('Cancel'));
}

/**
 * Callback function to delete the inject region from the database.
 */
function block_inject_delete_confirm_submit($form, &$form_state) {
  $region = $form_state['block_inject_region'];
  block_inject_remove_region($region->id);
  block_inject_remove_exception('bi_id', $region->id);
  drupal_set_message(t('The region %title has been removed.', array('%title' => $region->region)));
  $form_state['redirect'] = 'admin/structure/block-inject';
}
