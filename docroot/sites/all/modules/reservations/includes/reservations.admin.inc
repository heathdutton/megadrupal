<?php

/**
 * @file
 * Reservations
 */


/**
 * Builds the Reservations admininstration settings form.
 */
function reservations_admin_settings($form, &$form_state) {
  $form = array();

  $statuses = array();

  $statuses[RESERVATIONS_STATUS_UNCONFIRMED] = t('Unconfirmed');
  $statuses[RESERVATIONS_STATUS_PENDING] = t('Confirmed');
  $statuses[RESERVATIONS_STATUS_CHECKED_OUT] = t('Checked Out');

  $hours_of_operation = reservations_load_hours_of_operation();

  $form['reservations_default_reservation_status'] = array(
    '#type' => 'select',
    '#title' => t('Default Reservation Status'),
    '#options' => $statuses,
    '#default_value' => variable_get('reservations_default_reservation_status', ''),
    '#description' => t('New Reservations will be set to this by default.  Use Checked Out if you are using Reservations to manage the checkout, but do not allow actual Reservations.'),
  );
  
  $form['reservations_back_to_back_allowed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow Back to Back Reservation'),
    '#default_value' => variable_get('reservations_back_to_back_allowed', 0),
    '#description' => t('If this is unchecked, one second is subtracted from reservations end times. Users will still see times rounded to the 15 minute increments.'),
  );

  $form['reservations_ignore_unconfirmed_reservations'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore Unconfirmed Reservations in Conflict Check'),
    '#default_value' => variable_get('reservations_ignore_unconfirmed_reservations', 0),
    '#description' => t('This option will ignore unconfirmed reservations when requesting new reservations.  USE WITH CAUTION. This will allow multiple reservations to be requested at the same time. Conflicts will appear while confirming the reservations.  Normally if reservations are confirmed in a first come, first served manner this produces the desired result.'),
  );

  $form['reservations_max_days_advance_reservation'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Advance'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('reservations_max_days_advance_reservation', '0'),
    '#description' => t('Maximum number of days a Reservation can be made in advance.  Set to 0 if to allow Reservations to be made an unlimited time into the future.'),
  );
  
  $form['reservations_diff_warning'] = array(
    '#type' => 'textfield',
    '#title' => t('Max Minutes before Difference Warning'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('reservations_diff_warning', '0'),
    '#description' => t('Maximum of minutes a Reservation start or end time can differ from the current time without triggering a warning.  Set to 0 to disable this warning.'),
  );


  // Hours of Operation
  $hours_description = '<div>' . t('Enter military time for both opening and closing time, separated by a dash, in the format') . ' <em>hh:mm-hh:mm</em></div>' . t('ex.') . ' <em>09:00-17:00</em> ' . t('would be open at 9AM, close at 5PM. Leave blank to indicate not being open.');

  $hours_description .= '<p><div>' . t('Check the box next to <em>allow pushing</em> if the Reservations API should allowing push the due date forward past the max hours when a Reservation ends on a day the checkout is closed.  Reservations cannot be pushed unless all of the content types also allow pushing.  This is configure on BOTH the day, the content types, and role permissions.') . '</div>';

  $hours_element = array(
    '#type' => 'textfield',
    '#title' => t('Monday hours'),
    '#size' => 11,
    '#maxlength' => 11,
    '#element_validate' => array('reservations_admin_validate_hours_of_operation'),
    );

  $weekend_element = array(
    '#type' => 'checkbox',
    '#title' => t('allow pushing forward to first hour of next open day'),
  );


  $form['reservations_hours'] = array(
    '#title' => t('Hours of operation'),
    '#type' => 'fieldset',
    '#description' => $hours_description,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $days_of_the_week = array(
    'sunday',
    'monday',
    'tuesday',
    'wednesday',
    'thursday',
    'friday',
    'saturday',
  );

  foreach ($days_of_the_week as $num => $day) {
    $form['reservations_hours']['reservations_hours_' . $day] = array(
      '#title' => t('@day hours', array('@day' => drupal_ucfirst($day))),
      '#default_value' => isset($hours_of_operation[$num]) ? implode('-', $hours_of_operation[$num]) : '',
      '#type' => 'textfield',
      '#size' => 11,
      '#maxlength' => 11,
      '#element_validate' => array('reservations_admin_validate_hours_of_operation'),
    );
    $form['reservations_hours']['reservations_' . $day . '_push'] = array(
      '#default_value' => variable_get('reservations_' . $day . '_push', 0),
      '#type' => 'checkbox',
      '#title' => t('allow pushing forward to first hour of next open day'),
    );
  }

  $form['reservations_hours']['reservations_hours_admin'] = array(
    '#title' => t('Admin hours'),
    '#default_value' => variable_get('reservations_hours_admin', '07:00-22:00'),
    '#description' => t('This setting controls the hours shown on the conflict grid for users with Administer Reservations permissions.  The conflict grid is normally limited to the days and hours the set above.  Because users with Administer Reservations rights can create Reservations outside the normal hours of operation, they need to see conflicts during those times as well.'),
    '#type' => 'textfield',
    '#size' => 11,
    '#maxlength' => 11,
    '#element_validate' => array('reservations_admin_validate_hours_of_operation'),
  );

  $form['reservations_hours']['reservations_closed_dates'] = array(
    '#type' => 'textarea',
    '#title' => t('Closed dates'),
    '#rows' => 10,
    '#default_value' => variable_get('reservations_closed_dates', ''),
    '#description' => '<div>' . t('Enter dates which are closed regardless of the day of the week, one date per line, in the format') . ' <em>mm-dd</em></div>' . t('ex.') . ' <em>07-04</em> ' . t('would mean July 4th is always closed, regardless of what day of the week it falls on.'),
    '#element_validate' => array('reservations_admin_validate_closed_days'),
  );

  $form['reservations_hours']['reservations_closed_dates_push'] = array(
      '#default_value' => variable_get('reservations_closed_dates_push', 0),
      '#type' => 'checkbox',
      '#title' => t('Allow pushing forward to the next open day for ALL closed dates'),
  );
  
  $form['reservations_push_hours'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours after checkout opens that item is due when pushing forward over closed days is allowed'),
    '#size' => 11,
    '#maxlength' => 11,
    '#default_value' => variable_get('reservations_push_hours', '1'),
    '#description' => t('If the checkout is open at 9AM on Monday after being closed on Sunday, a Reservation with a due date that is pushed forward would be due before 10AM on Monday when this is set to 1 hour.'),
  );


  $form['reservations_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['reservations_advanced']['reservations_verbose_logging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Verbose Logging Mode'),
    '#default_value' => variable_get('reservations_verbose_logging', 0),
    '#description' => t('This option will add log messages at several points in the reservation process making it easier to diagnose configuration issues.'),
  );

  $field = field_info_field('field_reservations_date');

  $form['reservations_advanced']['reservations_lock'] = array(
    '#type' => 'checkbox',
    '#title' => t('Lock Fields'),
    '#default_value' => $field['locked'],
    '#description' => t('Reservations requires a Date field to function. This field is locked by default.  Altering or deleting the Date field could cause serious problems.  Always backup before altering this field!'),
  );
  
  $form['reservations_advanced']['reservations_inventory_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Add Inventory Prefix to Items'),
    '#size' => 25,
    '#maxlength' => 11,
    '#default_value' => variable_get('reservations_inventory_prefix', NULL),
    '#description' => t('Add the table of the prefix field.  If future versions you will be able to select a list of fields.'),
  );
  
  $form['reservations_advanced']['reservations_overdue_window'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours to Ignore Overdue Reservations'),
    '#size' => 25,
    '#maxlength' => 11,
    '#default_value' => variable_get('reservations_overdue_window_start', NULL),
    '#description' => t('Hours to Ignore Overdue Reservations.'),
  );
  
  $form['reservations_advanced']['reservations_overdue_buffer'] = array(
    '#type' => 'textfield',
    '#title' => t('Hours New Reservations can Start after an Overdue Reservation'),
    '#size' => 25,
    '#maxlength' => 11,
    '#default_value' => variable_get('reservations_overdue_buffer', NULL),
    '#description' => t('Number of hours new reservations can start after an overdue reservation while the overdue reservations are being ignored.'),
  );

  $form = system_settings_form($form);
  $form['#submit'][] = 'reservations_admin_settings_submit';
  return ($form);
}

/**
 * custom
 */
function reservations_admin_settings_submit($form, &$form_state) {

  // lock or unlock date field
  $field = field_info_field('field_reservations_date');
  if ($form_state['values']['reservations_lock'] != $field['locked']) {
    $field['locked'] = $form_state['values']['reservations_lock'];
    module_load_include('inc', 'field', 'field.crud');
    field_update_field($field);
  }

  // build and save hours of operation as an array
  $days_of_the_week = array(
    'sunday',
    'monday',
    'tuesday',
    'wednesday',
    'thursday',
    'friday',
    'saturday',
  );

  $hours_of_operation = array();

  foreach ($days_of_the_week as $num => $day) {
    $hours = $form_state['values']['reservations_hours_' . $day];
    if (empty($hours)) {
      $hours_of_operation[$num] = NULL;
    }
    else {
      list ($open, $close) = explode('-', $hours);
      $hours_of_operation[$num]['open'] = trim($open);
      $hours_of_operation[$num]['close'] = trim($close);
    }
  }

  $closed_days_raw = $form_state['values']['reservations_closed_dates'];
  $hours_of_operation['closed_days'] = array();
  $parts = explode("\n", $closed_days_raw);
  foreach ($parts as $date) {
    $date = trim($date);
    if (drupal_strlen($date) == 5) {
      $hours_of_operation['closed_days'][] = $date;
    }
  }

  variable_set('reservations_hours_operation', $hours_of_operation);
}

/**
 * Helper function to validate closed days formatting.
 */
function reservations_admin_validate_closed_days($element, &$form_state) {

  if (empty($element['#value'])) {
    return;
  }

  $parts = explode("\n", $element['#value']);
  foreach ($parts as $date) {
    $date = trim($date);
    $hours_of_operation['closed_days'][] = $date;
    list($month, $day) = explode('-', $date);
    if (drupal_strlen($month) != 2) {
      form_error($element, t('Month must be two digits.'));
    }
    if (drupal_strlen($day) != 2) {
      form_error($element, t('Day must be two digits.'));
    }

    if (!checkdate($month, $day, date('Y'))) {
      form_error($element, t('Date %date is invalid', array('%date' => $date)));
    }
  }
}

/**
 * Helper function to validate closed hours or operation formatting.
 */
function reservations_admin_validate_hours_of_operation($element, &$form_state) {
  if (!empty($element['#value'])) {
    $values = explode('-', $element['#value']);
    foreach ($values as $value) {
      list ($hour, $min) = explode(':', $value);
      if ($hour > 23) {
        form_error($element, t('Hour must be less than or equal to 23.'));
      }
      if ($hour < 0) {
        form_error($element, t('Hour must be greater than or equal to 0.'));
      }
      if ($min > 59) {
        form_error($element, t('Minutes must be less than or equal to 59.'));
      }
      if ($min < 0) {
        form_error($element, t('Minutes must be greater than or equal to 0.'));
      }
    }
    if ($values[1] < $values[0]) {
        form_error($element, t('Closing time must be greater then opening time.'));
    }
  }
}

