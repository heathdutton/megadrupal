<?php

/**
 * @file
 * Provides Edit page for selected date to change only slot capacity.
 */

/**
 * Implements hook_form().
 */
function appointment_calendar_list_date_form($form, $form_state) {
  // Date edit page.
  if (!empty($_GET['date'])) {
    $form['redirect'] = array(
      '#markup' => '<a href="/admin/config/appointment-calendar/settings">Goto Appointment Calendar Setting Page</a><br>',
    );
    $form['listredirect'] = array(
      '#markup' => '<a href="/admin/config/appointment-calendar/settings/list-date">Goto Appointment Calendar Listing Page</a>',
    );
    $form['appointment_slot_date'] = array(
      '#type' => 'textfield',
      '#title' => 'Date',
      '#default_value' => date('Y-m-d', $_GET['date']),
      '#disabled' => TRUE,
    );
    // Fetching Slot previous capacity filled.
    $capacity = appointment_calendar_slot_capacity($_GET['date']);
    $i = 1;
    // Show slots and capacity.
    foreach (json_decode($capacity) as $key => $value) {
      // Check if any appointment booked.
      $slot_check = appointment_calendar_slot_capacity_value($key);
      $form['time_slot_' . $i] = array(
        '#type' => 'textfield',
        '#title' => check_plain('Time Slot ' . $i . ' :'),
        '#description' => t('Ex: 10:00-11:00, 13:00-14:00, etc.,'),
        '#default_value' => $key,
        '#prefix' => '<div class="time-slot-field-form">',
      );
      if ($slot_check > 0) {
        $form['time_slot_' . $i]['#disabled'] = TRUE;
        $form['time_slot_' . $i]['#description'] = t('<b>Slot :i </b>booked atleast once', array(':i' => $i));
      }
      $form['time_slot_' . $i . '_capacity'] = array(
        '#type' => 'textfield',
        '#title' => check_plain('Slot ' . $i . ' Capacity'),
        '#description' => t('Only Numeric'),
        '#default_value' => $value,
        '#suffix' => '</div>',
      );
      $i++;
    }
    $form['appointment_slot'] = array(
      '#type' => 'textfield',
      '#title' => 'No of Extra Slots:',
    );
    // Display Extra slots.
    if (!empty($form_state['values'])) {
      $extra_slots = $form_state['values']['appointment_slot'];
      $extra_slots += $i - 1;
      for ($j = $i; $j <= $extra_slots; $j++) {
        $form['slots']['time_slot_' . $j] = array(
          '#type' => 'textfield',
          '#title' => check_plain('Time Slot ' . $j . ' :'),
          '#description' => t('Ex: 10:00-11:00, 13:00-14:00, etc.,'),
          '#default_value' => '',
          '#prefix' => '<div class="time-slot-field-form">',
        );
        $form['slots']['time_slot_' . $j . '_capacity'] = array(
          '#type' => 'textfield',
          '#title' => check_plain('Slot ' . $j . ' Capacity'),
          '#description' => t('Only Numeric'),
          '#default_value' => '',
          '#suffix' => '</div>',
        );
      }
      $j++;
    }
    $form['add_more'] = array(
      '#type' => 'submit',
      '#value' => t('Add More Slots'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
    return $form;
  }
}

/**
 * Implements hook_form_validate().
 */
function appointment_calendar_list_date_form_validate($form, &$form_state) {
  // In change time slots page.
  if ($form_state['values']['op'] == 'Submit') {
    $date = $form_state['values']['appointment_slot_date'];
    $capacity = appointment_calendar_slot_capacity(strtotime($date));
    $slots = count((array) json_decode($capacity));
    if (!empty($form_state['values']['appointment_slot'])) {
      $slots += $form_state['values']['appointment_slot'];
    }
    // Time slot and Capacity Validation.
    for ($i = 1; $i <= $slots; $i++) {
      $booked_capacity = '';
      $time_slot = $form_state['values']['time_slot_' . $i];
      $time_capacity = $form_state['values']['time_slot_' . $i . '_capacity'];
      $regex = '/^(?:[01][0-9]|2[0-3]):[0-5][0-9]-(?:[01][0-9]|2[0-3]):[0-5][0-9]$/';
      // Timeslot.
      if (!preg_match($regex, $time_slot)) {
        form_set_error('time_slot_' . $i, t('Time slot should be in between 00:00-23:59 (in between 24 hrs)'));
      }
      // Slot Capacity.
      if ($time_capacity < 0) {
        form_set_error('time_slot_' . $i . '_capacity', t('Slot Capacity should be greater than 0'));
      }
      $booked_capacity = appointment_calendar_slot_capacity_value($time_slot);
      if ($time_capacity < $booked_capacity) {
        form_set_error('time_slot_' . $i, t('Already :slots Appointment(s) booked in this Slot. So it should not be less than :slots', array(':slots' => $booked_capacity)));
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
function appointment_calendar_list_date_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == 'Add More Slots') {
    $form_state['rebuild'] = TRUE;
  }
  if ($form_state['values']['op'] == 'Submit') {
    $date = $form_state['values']['appointment_slot_date'];
    $capacity = appointment_calendar_slot_capacity(strtotime($date));
    $slots = count((array) json_decode($capacity));
    if (!empty($form_state['values']['appointment_slot'])) {
      $slots += $form_state['values']['appointment_slot'];
    }
    // Fetching Timeslot and capacity values.
    for ($i = 1; $i <= $slots; $i++) {
      $time_slot = $form_state['values']['time_slot_' . $i];
      $time_capacity = $form_state['values']['time_slot_' . $i . '_capacity'];
      $slots_save[$time_slot] = $time_slot;
      $slots_capacity[$time_slot] = $time_capacity;
    }
    // Saving date with time slots and capacity.
    db_merge('appointment_date')->key(array('date' => strtotime($date)))->fields(array(
      'slot_values' => json_encode($slots_save),
      'slot_capacity' => json_encode($slots_capacity),
    ))->execute();
    drupal_set_message(t('Changes made successfully'));
  }
}
