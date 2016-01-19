<?php

/**
 * @file
 * Contains admin page callbacks.
 */

/**
 * Admin form for managing a schedule's timetable.
 *
 * @param $schedule_timeslot_field_name
 *  The name of the field on the schedule that points to timeslots.
 * @param $schedule_entity_type
 *  The entity type of the schedule entity.
 * @param $schedule_entity_id
 *  The ID of the schedule entity.
 */
function timetable_scheduling_form($form, &$form_state, $schedule_timeslot_field_name, $schedule_entity_type, $schedule_entity_id) {
  $schedule = entity_load_single($schedule_entity_type, $schedule_entity_id);

  drupal_set_title(t("Edit timetable for schedule '@label'", array(
    '@label' => entity_label($schedule_entity_type, $schedule),
  )));

  $schedule_timeslot_field = field_info_field($schedule_timeslot_field_name);

  // Sanity check.
  if (empty($schedule_timeslot_field['settings']['handler_settings']['behaviors']['schedule_timeslot']['status'])) {
    throw new Exception(t("The timeslot field on schedules must be set to use the Schedule Timeslot entityreference behavior."));
  }

  // Get the configuration of the timetable set.
  $timetable_set = timetable_get_timetable_set($schedule_timeslot_field);
  $schedule_entity_type = $timetable_set['schedule']['entity_type'];

  // Get the timeslots out of the schedule.
  $timeslots = entity_metadata_wrapper($schedule_entity_type, $schedule)->{$schedule_timeslot_field_name}->value();
  // Rekey these by entity ID for lookup.
  $schedule_timeslots = array();
  foreach ($timeslots as $timeslot) {
    $schedule_timeslots_by_id[$timeslot->id] = $timeslot;
  }

  // Get the list of entity types and bundles that are the sessions for this
  // schedule.
  $session_entity_bundles = $timetable_set['session']['bundles'];

  // Get the entity type and bundle that is used as table columns (which we
  // refer to as rooms).
  if (isset($timetable_set['room'])) {
    // We only support one bundle of room for the time being.
    $keys = array_keys($timetable_set['room']['bundles']);
    $room_entity_type = array_pop($keys);
    $room_bundle_name = $timetable_set['room']['bundles'][$room_entity_type][0];
  }

  // This will hold the names of the fields on session entities, as an array
  // nested first by session entity type then session bundle name. The values
  // are an array with the keys 'timeslot' and 'room'.
  // TODO: this is no longer needed, now that the architecture is for a common
  // field.
  $form['#session_timeslot_fieldnames'] = array();

  // Build up an option list of all session entities, along with any default
  // values we need for them.
  $session_options = array();
  $session_options[''] = t('- Select a session -');
  $session_defaults = array();

  // Iterate over all the session entity types. It's probably better if there's
  // only one session entity type! We build up:
  //  - An options array of all entities of the session bundles.
  //  - A list of the fieldnames on sessions that point to timeslots.
  //  - A list of the fieldnames on sessions that point to rooms.
  foreach ($session_entity_bundles as $session_entity_type => $session_bundle_names) {
    // Add this entity type and bundle to the data array.
    $form['#session_timeslot_fieldnames'][$session_entity_type] = array_fill_keys($session_bundle_names, array());
    // Figure out which field on each bundle points to timeslots, so that our
    // submit handler has this information.
    foreach ($session_bundle_names as $session_bundle_name) {
      // Get all instances on this bundle.
      $field_instances = field_info_instances($session_entity_type, $session_bundle_name);
      foreach ($field_instances as $field_instance) {
        $field = field_info_field($field_instance['field_name']);
        if ($field['type'] == 'entityreference') {
          if ($field['settings']['target_type'] == 'timeslot') {
            // We've found our timeslot field on sessions.
            // (There really should be only one. If there is more than one,
            // that's a configuration problem.)
            $form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['timeslot'] = $field_instance['field_name'];
          }
          elseif ($field['settings']['target_type'] == $room_entity_type) {
            // We've found our room field on sessions.
            $form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['room'] = $field_instance['field_name'];
          }
        }
      }
    }

    // Add the session entities found for this entity type to the options list
    // and defaults list.
    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', $session_entity_type)
      ->entityCondition('bundle', $session_bundle_names);

    $results = $query->execute();

    if (empty($results[$session_entity_type])) {
      // No sessions of this entity type. Skip it.
      continue;
    }

    $session_entities = entity_load($session_entity_type, array_keys($results[$session_entity_type]));

    foreach ($session_entities as $session_entity_id => $session_entity) {
      // Find out whether the session entity has a timeslot already.
      list(, , $session_bundle_name) = entity_extract_ids($session_entity_type, $session_entity);
      $session_timeslot_field_name = $form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['timeslot'];

      $timeslot_id = entity_metadata_wrapper($session_entity_type, $session_entity)->{$session_timeslot_field_name}->raw();

      if (empty($timeslot_id)) {
        // If there's no timeslot ID, this session is not yet scheduled in
        // anything, so it's fair game.
        $session_options["$session_entity_type:$session_entity_id"] = entity_label($session_entity_type, $session_entity);
      }
      elseif (isset($schedule_timeslots_by_id[$timeslot_id])) {
        // If the session's timeslot is one from our current schedule, then
        // also add it to the defaults array so it shows up in the form.

        // Now check if it's in a room. Bear in mind not all session entity
        // types may have rooms.
        if (isset($form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['room'])) {
          $session_room_field_name = $form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['room'];

          $room_id = entity_metadata_wrapper($session_entity_type, $session_entity)->{$session_room_field_name}->raw();

          // An empty field value will be NULL, but we want this to correspond
          // to the 'No room' option which is a 0.
          if (empty($room_id)) {
            $room_id = 0;
          }
        }
        else {
          // Put it in the 'no room' column.
          $room_id = 0;
        }

        $session_options["$session_entity_type:$session_entity_id"] = entity_label($session_entity_type, $session_entity);
        $session_defaults[$timeslot_id][$room_id] = "$session_entity_type:$session_entity_id";
      }
      else {
        // The session is already in a timeslot, and it's a timeslot from
        // another schedule: do nothing.
      }
    }
  }

  // Build up the columns. We have at least one.
  $room_columns = array();
  $room_columns[0] = t('Unspecified room');

  // Get all the entities for the timetable columns. Internally, we'll refer to
  // these as rooms, but they could be anything really.
  if (isset($room_entity_type)) {
    $query = new EntityFieldQuery();
    $query
      ->entityCondition('entity_type', $room_entity_type)
      ->entityCondition('bundle', $room_bundle_name);

    $results = $query->execute();
    $rooms = array();
    if (!empty($results[$room_entity_type])) {
      $rooms = entity_load($room_entity_type, array_keys($results[$room_entity_type]));
    }

    foreach ($rooms as $room_id => $room) {
      $room_columns[$room_id] = entity_label($room_entity_type, $room);
    }
  }

  // Store these for the theme function to use as table headers.
  $form['#room_columns'] = $room_columns;

  $form['timeslots'] = array(
    '#tree' => TRUE,
  );

  // Build up the session form elements, by timeslot and room.
  foreach ($timeslots as $timeslot) {
    $form['timeslots'][$timeslot->id]['label'] = array(
      '#markup' => $timeslot->label(),
    );

    foreach ($room_columns as $room_id => $room_label) {
      $form['timeslots'][$timeslot->id]['rooms'][$room_id]['session'] = array(
        '#type' => 'select',
        '#title' => t('Session'),
        '#options' => $session_options,
        '#default_value' => isset($session_defaults[$timeslot->id][$room_id]) ? $session_defaults[$timeslot->id][$room_id] : '',
      );
    }

  }

  // Store the list of session ids for the submit handler to use.
  $form['#session_options'] = $session_options;

  $form['actions'] = array();
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Place sessions into timeslots"),
  );
  $schedule_uri = entity_uri($schedule_entity_type, $schedule);
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => $schedule_uri['path'],
  );
  //dsm($form);

  return $form;
}

/**
 * Theme the timetable building form.
 */
function theme_timetable_scheduling_form($variables) {
  $form = $variables['form'];

  $output = '';

  $rows = array();
  foreach (element_children($form['timeslots']) as $timeslot_id) {
    // Start a new row for this timeslot.
    $row = array();

    // The timeslot label.
    $row[] = drupal_render($form['timeslots'][$timeslot_id]['label']);

    // One cell for each room.
    foreach (element_children($form['timeslots'][$timeslot_id]['rooms']) as $room_id) {
      // Hide element label.
      unset($form['timeslots'][$timeslot_id]['rooms'][$room_id]['session']['#title']);
      $row[] = drupal_render($form['timeslots'][$timeslot_id]['rooms'][$room_id]['session']);
    }

    $rows[] = $row;
  }

  $header = array(
    t('Time'),
  );
  $header = array_merge($header, $form['#room_columns']);

  $output .= theme('table', array(
    'rows' => $rows,
    'header' => $header,
  ));

  $output .= drupal_render_children($form);


  return $output;
}

/**
 * Validate handler for the timetable admin form.
 */
function timetable_scheduling_form_validate($form, &$form_state) {
  // Check no session is used more than once.
  // Build up an array of all the sessions that are selected, keyed by the
  // select element value, where values are an array of pairs of timeslot and
  // room where the session has been selected.
  $selected_sessions = array();
  foreach (array_keys($form_state['values']['timeslots']) as $timeslot_id) {
    foreach (array_keys($form_state['values']['timeslots'][$timeslot_id]['rooms']) as $room_id) {
      $cell_form_state_value = $form_state['values']['timeslots'][$timeslot_id]['rooms'][$room_id]['session'];
      if (!empty($cell_form_state_value)) {
        $selected_sessions[$cell_form_state_value][] = array($timeslot_id, $room_id);
      }
    }
  }

  // Check each selected session in the array. Each one should only have been
  // seen once, and therefore its count of timeslot and room pairs should only
  // be 1.
  foreach ($selected_sessions as $cell_form_state_value => $timeslot_room_pairs) {
    if (count($timeslot_room_pairs) > 1) {
      // This session is selected more than once.
      // Set the form error on each session select element that has the
      // repeated session.
      $message = t("The session @session-label cannot be placed in more than one slot.", array(
        '@session-label' => '',
      ));
      foreach ($timeslot_room_pairs as $timeslot_room_pair) {
        list($timeslot_id, $room_id) = $timeslot_room_pair;
        form_error($form['timeslots'][$timeslot_id]['rooms'][$room_id]['session'], $message);
        // Zap the message once we've set it. This ensures that the message for
        // each session only gets displayed once, while we mark all the form
        // elements the session was selected in as having an error.
        $message = '';
      }
    }
  }
}

/**
 * Submit handler for the timetable admin form.
 */
function timetable_scheduling_form_submit($form, &$form_state) {
  //dsm($form);
  //dsm($form_state);

  // We have to consider all sessions that were session options, not just those
  // found in form values, in case some have been removed from the timeslots.
  // Fill up an array with the option keys that we can build up with data from
  // form values.
  $session_options = array_fill_keys(array_keys($form['#session_options']), array(
    'timeslot_id' => NULL,
    'room_id' => NULL,
  ));
  // Remove the 'Select one' option.
  unset($session_options['']);

  // Work through each timeslot and room in the form, and update the session
  // entity to point to them, if one was selected. Remember that the $room_id
  // can be 0 to mean there's no room chosen.
  foreach (array_keys($form_state['values']['timeslots']) as $timeslot_id) {
    foreach (array_keys($form_state['values']['timeslots'][$timeslot_id]['rooms']) as $room_id) {
      $cell_form_state_value = $form_state['values']['timeslots'][$timeslot_id]['rooms'][$room_id]['session'];

      // Skip cells where no session was selected.
      if (empty($cell_form_state_value)) {
        continue;
      }

      // Skip timeslots where the value matches the form default: there's
      // nothing to do, spare us a save.
      if ($cell_form_state_value == $form['timeslots'][$timeslot_id]['rooms'][$room_id]['session']['#default_value']) {
        // Remove the session from the list so we don't touch it.
        unset($session_options[$cell_form_state_value]);
      }

      // Add the timeslot and room values to our array of data to save.
      $session_options[$cell_form_state_value] = array(
        'timeslot_id' => $timeslot_id,
        'room_id'     => $room_id,
      );
    }
  }

  foreach ($session_options as $session_option_key => $session_save_data) {
    // Split the option value back up into entity type and ID.
    list($session_entity_type, $session_entity_id) = explode(':', $session_option_key);

    // Load the session.
    // TODO: consider optimising this to do one mass entity_load() post-loop.
    $session_entity = entity_load_single($session_entity_type, $session_entity_id);

    list(, , $session_bundle_name) = entity_extract_ids($session_entity_type, $session_entity);

    // Get the field names for the session entity from our data array.
    $session_timeslot_field_name = $form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['timeslot'];
    $session_room_field_name = NULL;
    if (isset($form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['room'])) {
      $session_room_field_name = $form['#session_timeslot_fieldnames'][$session_entity_type][$session_bundle_name]['room'];
    }

    $session_entity_wrapper = entity_metadata_wrapper($session_entity_type, $session_entity);

    // The timeslot_id value is either an ID or NULL to remove the session from
    // the schedule.
    $session_entity_wrapper->{$session_timeslot_field_name}->set($session_save_data['timeslot_id']);

    // If there's a room field, set (or clear) the room.
    if (isset($session_room_field_name)) {
      // The 'no room' option requires special handling.
      $room_id = $session_save_data['room_id'];
      $value = empty($room_id) ? NULL : $room_id;
      $session_entity_wrapper->{$session_room_field_name}->set($value);
    }
    // Set the room, unless 'no room' was selected.
    entity_save($session_entity_type, $session_entity);
  }

  // Redirect to the schedule entity.
  list(, $schedule_entity_type, $schedule_entity_id) = $form_state['build_info']['args'];
  $schedule = entity_load_single($schedule_entity_type, $schedule_entity_id);
  $schedule_uri = entity_uri($schedule_entity_type, $schedule);
  $form_state['redirect'] = $schedule_uri['path'];

  drupal_set_message(t("The timetable for @label has been saved.", array(
    '@label' => entity_label($schedule_entity_type, $schedule),
  )));
}
