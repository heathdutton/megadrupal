<?php

/**
 * @file
 * Provides admin functions for Workbench Scheduler.
 */


/**
 * Display a table of workbench schedule for administration.
 *
 * @return string
 *   themed output.
 */
function workbench_scheduler_admin_page() {
  // Build a table to show the different schedules.
  $headers = array(
    array('data' => t('Name')),
    array('data' => t('Machine Name')),
    array('data' => t('Start State')),
    array('data' => t('End State')),
    array('data' => t('Content Types')),
    array('data' => t('Operations'), 'colspan' => 2),
  );

  $rows = array();
  // Retrieve any schedules that exist.
  if ($schedules = workbench_scheduler_load_schedules()) {
    // Get list of the different moderation states.
    $states = workbench_scheduler_state_labels();
    // Get list of the different content types.
    $node_types = node_type_get_types();
    // Loop through the schedules to add them to the table.
    foreach ($schedules as $name => $schedule) {
      // Format the content types the schedule is available for,
      // Based on number.
      $type_count = count($schedule->types);
      // More then one type?
      if ($type_count > 1) {
        $items = array();
        // Loop through each type.
        foreach ($schedule->types as $type) {
          // Display the human readable name.
          $items[] = $node_types[$type]->name;
        }

        // Format into an item list.
        $types = theme('item_list', array('items' => $items, 'type' => 'ul'));
      }
      // Have only a single type?
      elseif (count($schedule->types) == 1) {
        // Display the human readable name.
        $types = $node_types[array_pop($schedule->types)]->name;
      }
      // No types found (either deleted or not imported?).
      else {
        // Display null.
        $types = 'NULL';
      }
      // Format the row.
      $row = array(
        $schedule->label,
        $name,
        (!empty($states[$schedule->start_state]) ? $states[$schedule->start_state] : ''),
        (!empty($states[$schedule->end_state]) ? $states[$schedule->end_state] : ''),
        $types,
        // Link to edit the schedule.
        l(t('Edit'), 'admin/config/workbench/scheduler/schedules/' . $name . '/edit'),
        // Link to delete the scheduler.
        l(t('Delete'), 'admin/config/workbench/scheduler/schedules/' . $name . '/delete'),
      );
      // Add to the rows array.
      $rows[] = $row;
    }
  }
  // No schedules found.
  else {
    // Display message in first row.
    $rows[] = array(array(
        'data'    => t('No Schedules Found'),
        'colspan' => 7,
      ),
    );
  }
  // Add a row for a link to add a new schedule.
  $rows[] = array(
    array(
      'data'    => l(t('Add Schedule'), 'admin/config/workbench/scheduler/schedules/add'),
      'colspan' => 7,
    ),
  );
  // Returned the themed table.
  return theme('table', array('header' => $headers, 'rows' => $rows));

}

/**
 * Display a form for adding/editing a schedule.
 */
function workbench_scheduler_admin_edit_schedule($form, &$form_state, $name = FALSE) {
  $schedule = FALSE;
  // Passed a machine name? editing a schedule.
  if ($name) {
    // Attempt to load the schedule.
    if ($schedule = workbench_scheduler_load_schedules($name)) {
      // Store in the form.
      $form['#schedule']  = $schedule;
    }
  }
  // Label field.
  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Schedule Name'),
    '#required' => TRUE,
    '#description' => t('A user friendly name. Only used for admin purposes'),
    '#size' => 40,
    '#maxlength' => 127,
  );

  // Field for machine name.
  $form['name'] = array(
    '#title' => t('Machine Name'),
    '#description' => t('A machine friendly name.'),
    '#type' => 'machine_name',
    '#required' => TRUE,
    '#size' => 25,
    '#maxlength' => 25,
    '#default_value' => ($schedule ? $schedule->name : ''),
    '#machine_name' => array(
      'exists' => '_workbench_schedule_check_machine_name_exists',
      'source' => array('label'),
    ),
  );
  // Fetch a list of the available moderation states.
  $states = workbench_scheduler_state_labels();
  // Save to the form.
  $form['#states'] = $states;

  // Add none option.
  $states = array_merge(array('' => t('None')), $states);

  // Select list for start state.
  $form['start_state'] = array(
    '#type' => 'select',
    '#title' => t('Start State'),
    '#description' => t('Select the state to be set when a node reaches its "start date"'),
    '#options' => $states,
    '#required' => FALSE,
  );

  // Select list for end state.
  $form['end_state'] = array(
    '#type' => 'select',
    '#title' => t('End State'),
    '#description' => t('Select the state to be set when a node reaches its "end date"'),
    '#options' => $states,
    '#required' => FALSE,
  );

  // Fetch a list of content types that have moderation enabled.
  // From the workbench moderation module.
  module_load_include('module', 'workbench_moderation');
  $types = workbench_moderation_moderate_node_types();
  $info = entity_get_info('node');

  $tmp_types = array();
  // Make into an associative array.
  foreach ($types as $type) {
    $states = workbench_moderation_states();
    $default_state = variable_get('workbench_moderation_default_state_' . $type, workbench_moderation_state_none());
    $label = $info['bundles'][$type]['label'];
    $tmp_types[$type] = array(
      'label' => $label,
      'default_state' => $states[$default_state]->label,
    );
  }
  // Store types to the form.
  $form['#types'] = $types;
  $types = $tmp_types;
  unset($tmp_types);

  $header = array(
    'label' => t('Content Type'),
    'default_state' => t('Default Moderation State'),
  );

  // Label  and description.
  $form['table_label'] = array(
    '#type' => 'markup',
    '#markup' => '<label>' . t('Content Types') . '</label>',
  );
  $form['table_description'] = array(
    '#type' => 'markup',
    '#markup' => t('Select the content types that can use this schedule'),
  );
  // Checkboxes for content types.
  $form['types'] = array(
    '#type'           => 'tableselect',
    '#title'          => t('Content Types'),
    '#description'    => t('Select the content types that can use this schedule'),
    '#header'         => $header,
    '#options'        => $types,
    '#required'       => TRUE,
    '#empty' => t('No Content Types are configured with workbench moderation'),
  );

  $form['submit'] = array(
    '#type'   => 'submit',
    '#value' => ($schedule ? t('Update') : t('Save')),
  );

  // Editing a schedule?
  if ($schedule) {
    // Add a delete button.
    $form['delete'] = array(
      '#type'   => 'submit',
      '#value'  => t('Delete'),
    );

    // Remove the machine name field.
    unset($form['name']);
    // Set default values for the other fields.
    $form['label']['#default_value']        = $schedule->label;
    $form['start_state']['#default_value']  = $schedule->start_state;
    $form['end_state']['#default_value']    = $schedule->end_state;
    $form['types']['#default_value']        = drupal_map_assoc($schedule->types);
  }
  // Return the form.
  return $form;
}

/**
 * Validation function for adding / editing a schedule.
 */
function workbench_scheduler_admin_edit_schedule_validate($form, &$form_state) {
  // Validate label.
  if (strlen(trim(strip_tags($form_state['values']['label']))) < 1) {
    form_set_error('label', t('Invalid Schedule name provided'));
  }
  // Check that the two states are not the same.
  if ($form_state['values']['start_state'] == $form_state['values']['end_state']) {
    form_set_error('end_state', t('End state must be different from start state'));
  }
}

/**
 * Submit function for adding/editing a schedule.
 */
function workbench_scheduler_admin_edit_schedule_submit($form, &$form_state) {
  // Check to see if the delete button was clicked.
  if ($form_state['clicked_button']['#id'] == 'edit-delete') {
    // Redirect to the delete form.
    drupal_goto('admin/config/workbench/scheduler/schedules/' . $form['#schedule']->name . '/delete');
  }
  // Only other button is the submit button.
  else {
    // Build the array of schedule data.
    $schedule_data = array(
      'label'       => trim(strip_tags($form_state['values']['label'])),
      'start_state' => $form_state['values']['start_state'],
      'end_state'   => $form_state['values']['end_state'],
      // Add the checked types to the schedule data.
      'types'       => array_intersect($form['#types'], $form_state['values']['types']),
    );

    // Updating an existing schedule?
    if (isset($form['#schedule'])) {
      // Fetch the machine name form the object.
      $machine_name = $form['#schedule']->name;
    }
    else {
      // Fetch the new machine name from passed values.
      $machine_name = $form_state['values']['name'];
    }

    // Attempt a save/update
    if ($result = workbench_scheduler_save_schedule($machine_name, $schedule_data)) {
      drupal_set_message(t('Schedule @status', array('@status' => ($result == 1 ? t('Saved') : t('Updated')))), 'status', FALSE);
      // Redirect back to the schedules table.
      $form_state['redirect'] = 'admin/config/workbench/scheduler/schedules';
    }
    else {
      drupal_set_message(t('Error saving schedule'), 'error', FALSE);
    }
  }
}

/**
 * Confirmation form for deleting a schedule.
 */
function workbench_scheduler_admin_delete_schedule($form, &$form_state, $name) {
  // Attempt to load the schedule.
  if ($schedule = workbench_scheduler_load_schedules($name)) {
    // Store to form.
    $form['#schedule'] = $schedule;
    // Build confirmation form.
    return confirm_form($form,
      t('Are you sure you want to delete the schedule "@label"?', array('@label' => $schedule->label)),
      'admin/config/workbench/scheduler/schedules',
      t('This action cannot be undone')
    );
  }
  // Unable to load form, not sure what trying to delete.
  else {
    drupal_set_message(t('Invalid Schedule machine name'), 'error', FALSE);
    // Send back to schedules table.
    drupal_goto('admin/config/workbench/scheduler/schedules');
  }
}

/**
 * Submit function for deleting a schedule.
 */
function workbench_scheduler_admin_delete_schedule_submit($form, &$form_state) {
  // Fetch the schedule to delete.
  $schedule = $form['#schedule'];
  // Attempt to delete.
  if (workbench_scheduler_delete_schedules($schedule->name)) {
    drupal_set_message(t('Schedule and associated data deleted'), 'status', FALSE);
    // Go back to schedules page.
    $form_state['redirect'] = 'admin/config/workbench/scheduler/schedules';
  }
  // Unable to delete, show error message.
  else {
    drupal_set_message(t('Error deleting schedule'), 'error', FALSE);
  }
}

/**
 * Tableselect form for current schedules applied to a node.
 */
function workbench_scheduler_admin_manage_node_schedules($form, &$form_state, $node) {
  $revisions = node_revision_list($node);

  $schedules = array(
    'schedule_active' => array(),
    'schedule_inactive' => array(),
  );

  // Check to see if this node type supports more than one.
  $type_settings = variable_get('workbench_scheduler_' . $node->type, array());
  $limit_current_revision = FALSE;
  if (in_array('workbench_scheduler_limit_current_revision', $type_settings)) {
    $limit_current_revision = TRUE;
  }
  $node_schedules = array();
  foreach ($revisions as $vid => $revision) {
    // Load up all the schedules for all revisions of this node.
    $node_schedules[$vid] = workbench_scheduler_load_node_schedule($node->nid, $vid);
  }
  $node_schedules = array_filter($node_schedules);
  // Load labels of moderation states.
  $moderation_states = workbench_scheduler_state_labels();
  // If there are schedules applied to this node.
  if ($node_schedules) {

    foreach ($node_schedules as $vid => $node_schedule) {
      $start_date = t('not set');
      $end_date = t('not set');
      if (isset($node_schedule->start_date)) {
        $start_date = format_date($node_schedule->start_date, 'custom', 'Y-m-d H:i:s');
      }
      if (isset($node_schedule->end_date)) {
        $end_date = format_date($node_schedule->end_date, 'custom', 'Y-m-d H:i:s');
      }

      // Build each row of the tableselect.
      $manage_schedules = array(
        'title' => array('data' => array('#title' => $vid)),
        'vid' => l($vid, 'node/' . $node->nid . '/revisions/' . $vid . '/view'),
        'label' => $node_schedule->label,
        'start_state' => isset($moderation_states[$node_schedule->start_state]) ? $moderation_states[$node_schedule->start_state] : $node_schedule->start_state,
        'start_date' => $start_date,
        'end_state' => isset($moderation_states[$node_schedule->end_state]) ? $moderation_states[$node_schedule->end_state] : $node_schedule->end_state,
        'end_date' => $end_date,
        'completed' => ($node_schedule->completed) ? t('Yes') : t('No'),
        'vid_edit' => l(t('Edit'), 'node/' . $node->nid . '/manage_schedules/' . $vid . '/edit'),
      );

      // Limit Current revision - only run schedule on highest vid.
      if ($limit_current_revision) {
        // Active schedule?
        if ($node->workbench_moderation['current']->vid == $vid) {
          $schedules['schedule_active'][$vid] = $manage_schedules;
        }
        else {
          $schedules['schedule_inactive'][$vid] = $manage_schedules;

        }
      }
      // Run on all schedules that haven't completed yet (default behavior).
      else {
        // Active schedule?
        if (!$node_schedule->completed) {
          $schedules['schedule_active'][$vid] = $manage_schedules;
        }
        else {
          $schedules['schedule_inactive'][$vid] = $manage_schedules;

        }

      }
    }

    // Render Schedules.
    foreach ($schedules as $status => $schedule) {
      if (!empty($schedule)) {
        // Build the the tableselect form, with a hidden field for the nid.
        $form['nid'] = array('#type' => 'hidden', '#value' => $node->nid);

        // Fieldsets.
        if ($status == 'schedule_active') {
          $fieldset = array(
            '#type' => 'fieldset',
            '#title' => t('Active Schedule'),
            '#description' => t('Only active schedules are run.'),
          );

        }
        else {
          $fieldset = array(
            '#type' => 'fieldset',
            '#title' => t('Inactive Schedules'),
          );
        }

        // Output the form.
        $form[$status . '_fieldset'] = $fieldset;
        $form[$status . '_fieldset'][$status] = array(
          '#type' => 'tableselect',
          '#title' => t('Schedules applied to this node'),
          '#options' => $schedule,
          '#multiple' => TRUE,
          '#header' => array(
            'vid' => t('Revision ID'),
            'label' => t('Schedule Name'),
            'start_state' => t('Start State'),
            'start_date' => t('Start Date'),
            'end_state' => t('End State'),
            'end_date' => t('End Date'),
            'completed' => t('Completed'),
            'vid_edit' => t('Edit'),
          ),
        );
      }
    }
    // The form submit actions.
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type'   => 'submit',
      '#value'  => t('Delete selected'),
    );
    return $form;
  }
  else {
    // When no schedules applied to this node, display a status message.
    drupal_set_message(t('No schedules applied to this node'), 'status', FALSE);
  }
}

/**
 * Submit function for deleting the schedule(s) set for a node's vid.
 */
function workbench_scheduler_admin_manage_node_schedules_submit($form, &$form_state) {
  $nid = $form_state['values']['nid'];
  $fieldsets = array('schedule_active', 'schedule_inactive');
  foreach ($fieldsets as $key) {
    foreach ($form_state['values'][$key] as $vid => $checked) {
      if ($vid == $checked) {
        workbench_scheduler_delete_node_schedule($nid, $vid);
      }
    }
  }
}

/**
 * Form to edit the schedule for an existing node revision.
 */
function workbench_scheduler_admin_edit_revision_schedule($form, &$form_state, $node, $vid) {
  $form['nid'] = array('#type' => 'hidden', '#value' => $node->nid);
  $form['vid'] = array('#type' => 'hidden', '#value' => $vid);
  $type_schedules = workbench_scheduler_load_type_schedules($node->type);
  $schedule_options = array();
  $schedule_options[] = array(
    'label' => t('No Schedule'),
    'start_state' => '',
    'end_state' => '',
  );
  $moderation_states = workbench_scheduler_state_labels();
  // Add each type schedule to the options array.
  foreach ($type_schedules as $schedule) {
    if (user_access('set any workbench schedule') || user_access('set workbench schedule for ' . $schedule->name)) {
      $schedule_options[$schedule->sid] = array(
        'label' => $schedule->label,
        'start_state' => isset($moderation_states[$schedule->start_state]) ? $moderation_states[$schedule->start_state] : '',
        'end_state' => isset($moderation_states[$schedule->end_state]) ? $moderation_states[$schedule->end_state] : '',
      );
    }
  }

  $form['workbench_scheduler_sid'] = array(
    '#type' => 'tableselect',
    '#title' => t('Select Schedule'),
    '#description' => t('Select the schedule to use for this node.'),
    '#options' => $schedule_options,
    '#multiple' => FALSE,
    '#header' => array(
      'label' => t('Name'),
      'start_state' => t('Start State'),
      'end_state' => t('End State'),
    ),
  );
  // Dates.
  $form['workbench_scheduler_start_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d H:i',
    '#title' => t('Start date'),
    '#description' => t('Select the date to switch this node to the scheduled "start state".'),
  );
  $form['workbench_scheduler_end_date'] = array(
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d H:i',
    '#title' => t('End date'),
    '#description' => t('Select the date to switch this node to the scheduled "end state".'),
  );

  // Node only allow most recent revision schedule?
  $type_settings = variable_get('workbench_scheduler_' . $node->type, array());
  // Check if only process latest revision.
  if (in_array('workbench_scheduler_limit_current_revision', $type_settings)) {
    // Getting the latest revision.
    $rev_list = node_revision_list($node);
    $latest_vid = max(array_keys($rev_list));

    // Is this vid not the most recent?
    if ($latest_vid != $vid) {
      $form['note'] = array(
        '#type' => 'item',
        '#title' => t('Note'),
        '#markup' => t('THIS SCHEDULE WILL NOT BE RUN. This node type is set to run the schedule for only the most !link', array(
          '!link' => l(t('recent revision'), 'node/' . $node->nid . '/manage_schedules/' . $latest_vid . '/edit'),
        )),
      );
    }
  }

  // Add existing node schedule as defaults.
  $node_schedule = workbench_scheduler_load_node_schedule($node->nid, $vid);
  if ($node_schedule) {
    $form['workbench_scheduler_sid']['#default_value'] = $node_schedule->sid;
    if ($node_schedule->start_date) {
      $start_date = format_date($node_schedule->start_date, 'custom', 'Y-m-d H:i:s');
      $form['workbench_scheduler_start_date']['#default_value'] = $start_date;
    }
    if ($node_schedule->end_date) {
      $end_date = format_date($node_schedule->end_date, 'custom', 'Y-m-d H:i:s');
      $form['workbench_scheduler_end_date']['#default_value'] = $end_date;
    }
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array(
      'workbench_scheduler_node_form_validate',
    ),
  );
  return $form;
}

/**
 * Submit function for editing the schedule applied to a node's revision.
 */
function workbench_scheduler_admin_edit_revision_schedule_submit($form, &$form_state) {
  $schedule_data = array(
    'sid'         => $form_state['values']['workbench_scheduler_sid'],
    'start_date'  => strtotime($form_state['values']['workbench_scheduler_start_date']),
    'end_date'    => strtotime($form_state['values']['workbench_scheduler_end_date']),
  );
  $nid = $form_state['values']['nid'];
  $vid = $form_state['values']['vid'];
  if (workbench_scheduler_save_node_schedule($nid, $vid, $schedule_data)) {
    drupal_set_message(t('Revision schedule updated'), 'status', FALSE);
    drupal_goto('node/' . $nid . '/manage_schedules/');
  }
  else {
    drupal_set_message(t('Error saving workbench schedule for node'), 'error', FALSE);
  }
}
