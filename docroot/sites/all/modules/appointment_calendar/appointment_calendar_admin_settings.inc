<?php

/**
 * @file
 * Provide Configuration form for Appointment calendar.
 */

/**
 * Function to display form in Setting Page.
 */
function appointment_calendar_settings_form($form, $form_state) {
  $form = array();
  $default_date = date('Y-m-d', time());
  $form['#attached']['css'] = array(
    drupal_get_path('module', 'appointment_calendar') . '/css/style.css',
  );
  $form['redirect'] = array(
    '#markup' => '<a href="/admin/config/appointment-calendar/settings/list-date">Goto Date Listing Page</a>',
  );
  $form['appointment_from_date'] = array(
    '#title' => t('Appointment From date'),
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '0:+3',
    '#required' => TRUE,
    '#default_value' => $default_date,
    '#datepicker_options' => array('minDate' => 0),
  );
  $form['appointment_to_date'] = array(
    '#title' => t('Appointment To date'),
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '0:+3',
    '#required' => TRUE,
    '#default_value' => $default_date,
    '#datepicker_options' => array('minDate' => 0),
  );
  $form['appointment_slot'] = array(
    '#type' => 'textfield',
    '#title' => 'No of Slots:',
    '#size' => 10,
    '#maxlength' => 10,
    '#required' => TRUE,
  );
  $form['appointment_fill'] = array(
    '#type' => 'button',
    '#value' => 'Fill Slots',
    '#weight' => 36,
    '#ajax' => array(
      'callback' => 'appointment_calendar_filltime_slots_callback_form',
      'wrapper' => 'time-slot-check',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $no_slots = !empty($form_state['values']['appointment_slot']) ? $form_state['values']['appointment_slot'] : 0;
  // Fetching User inputted slots fields.
  $form['slots']['#prefix'] = '<div id="time-slot-check">';
  for ($i = 1; $i <= $no_slots; $i++) {
    $form['slots']['time_slot_' . $i] = array(
      '#type' => 'textfield',
      '#title' => check_plain('Time Slot ' . $i . ' :'),
      '#description' => t('Ex: 10:00-11:00, 13:00-14:00, etc.,'),
      '#prefix' => '<div class="time-slot-field-form">',
    );
    $form['slots']['time_slot_' . $i . '_capacity'] = array(
      '#type' => 'textfield',
      '#title' => check_plain('Slot ' . $i . ' Capacity'),
      '#description' => t('Only Numeric'),
      '#suffix' => '</div>',
    );
  }
  $form['slots']['#suffix'] = '</div>';
  $form['slots']['#weight'] = 39;
  if (!empty($form_state['values']['appointment_slot'])) {
    $form['slots']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
    $form['slots']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
    );
  }
  return $form;
}

/**
 * Ajax callback function to show timeslots.
 */
function appointment_calendar_filltime_slots_callback_form($form, &$form_state) {
  return $form['slots'];
}

/**
 * Implements hook_form_validate().
 */
function appointment_calendar_settings_form_validate($form, &$form_state) {
  // For settings page.
  if ($form_state['values']['op'] == 'Reset') {
    drupal_goto(current_path());
  }
  if ($form_state['values']['op'] == 'Submit') {
    $start_date = strtotime($form_state['values']['appointment_from_date']);
    $end_date = strtotime($form_state['values']['appointment_to_date']);
    // Getting all dates in between Start and End Dates.
    $dates = appointment_calendar_daysbetween($start_date, $end_date);
    $check_count = 0;
    // Checking for already filled slots.
    foreach ($dates as $each_date) {
      $date_check = db_select('appointment_date', 'ad');
      $date_check->fields('ad', array('date'));
      $date_check->condition('date', $each_date, '=');
      $date_result = $date_check->execute()->fetchField();
      if (!empty($date_result)) {
        $check_count++;
        $date = date('Y-m-d', $each_date);
        drupal_set_message($date . ' Already filled', 'error');
      }
    }
    if ($check_count > 0) {
      form_set_error('appointment_from_date', t('Verify and try adding again'));
    }
    // Date Validation.
    if ($start_date > $end_date) {
      form_set_error('appointment_to_date', t('End Date Should be greater than Start Date'));
    }
    if ($start_date < strtotime(date('Y-m-d', time()))) {
      form_set_error('appointment_start_date', t('Start Date Should be greater than Today(s) Date'));
    }
    $slots = $form_state['values']['appointment_slot'];
    // Time slot and Capacity Validation.
    $time_slots_check = array();
    for ($i = 1; $i <= $slots; $i++) {
      $time_slot = $form_state['values']['time_slot_' . $i];
      $time_capacity = $form_state['values']['time_slot_' . $i . '_capacity'];
      $regex = '/^(?:[01][0-9]|2[0-3]):[0-5][0-9]-(?:[01][0-9]|2[0-3]):[0-5][0-9]$/';
      // Timeslot.
      if (!preg_match($regex, $time_slot)) {
        form_set_error('time_slot_' . $i, t('Time slot format should be 00:00-00:00 (in between 24 hrs).'));
      }
      // Slot Capacity.
      if ($time_capacity < 0) {
        form_set_error('time_slot_' . $i . '_capacity', t('Slot Capacity should be greater than 0.'));
      }
      // Timeslot Check.
      if (empty($time_capacity)) {
        form_set_error('time_slot_' . $i . '_capacity', t('Fill time slot capacity.'));
      }
      // Checking duplicate slots.
      $time_slots_check[] = $time_slot;
      $vals = array_count_values($time_slots_check);
      if ($vals[$time_slot] > 1) {
        form_set_error('time_slot_' . $i, t('Time slot cannot redeclare twice or more.'));
      }
    }
  }
}

/**
 * Implements hook_form_submit().
 */
function appointment_calendar_settings_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Submit') {
    $start_date = $form_state['values']['appointment_from_date'];
    $end_date = $form_state['values']['appointment_to_date'];
    $slots = $form_state['values']['appointment_slot'];
    // Fetching Timeslot and capacity values.
    for ($i = 1; $i <= $slots; $i++) {
      $time_slot = $form_state['values']['time_slot_' . $i];
      $time_capacity = $form_state['values']['time_slot_' . $i . '_capacity'];
      $slots_save[$time_slot] = $time_slot;
      $slots_capacity[$time_slot] = $time_capacity;
    }
    ksort($slots_save);
    ksort($slots_capacity);
    // Getting all dates in between Start and End Dates.
    $dates = appointment_calendar_daysbetween(strtotime($start_date), strtotime($end_date));
    // Saving date with time slots and capacity.
    foreach ($dates as $each_date) {
      db_merge('appointment_date')->key(array('date' => $each_date))->fields(array(
        'no_slots' => $slots,
        'slot_values' => json_encode($slots_save),
        'slot_capacity' => json_encode($slots_capacity),
      ))->execute();
    }
  }
  // Redirect to list page.
  drupal_set_message(t('Slot(s) created successfully'));
  drupal_goto('admin/config/appointment-calendar/settings/list-date');
}
