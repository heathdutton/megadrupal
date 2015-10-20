<?php

/**
 * @file
 * Administrative configuration functions for the Room Reservations module.
 *
 * @author Bob Humphrey, Web and Applications Developer, UNCW Randall Library
 */

/*
 * Confirmation message to display when configuration options have been saved
 * successfully.
 */
define('ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG', t('The configuration options have been saved.'));

/**
 * Error message to display when configuration options could not be saved.
 */
define('ROOM_RESERVATIONS_SAVE_ERROR_MSG', t('The configuration options could not be saved.'));

/**
 * Confirmation message to display when configuration options have been
 * successfully reset to their default values.
 */
define('ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG', t('The configuration options have been reset to their default values.'));

/**
 * Error message to display when configuration options could not be reset to
 * their default values.
 */
define('ROOM_RESERVATIONS_RESET_ERROR_MSG', t('The configuration options could not be reset to their default values.'));

/**
 * Form constructor for the Settings / General page.
 */
function room_reservations_admin_settings($form, &$form_state) {
  $options = array(
    '30' => '30',
    '60' => '60',
    '90' => '90',
    '120' => '120',
    '150' => '150',
    '180' => '180',
    '240' => '240',    
    '360' => '360',
    '480' => '480',
    '600' => '600',    
    '720' => '720',
  );
  $form['room_reservations_title'] = array(
    '#title' => t('Application name'),
    '#type' => 'textfield',
    '#maxlength' => 50,
    '#size' => 50,
    '#description' => t('The name of the room reservation application displayed on the menu, breadcrumbs, and heading of the reservation
       calendar page.'),
    '#default_value' => variable_get('room_reservations_title', 'Room Reservations'),
  );           
  $form['room_reservations_advance_standard'] = array(
    '#title' => t('Days in advance (standard)'),
    '#type' => 'select',
    '#options' => array(7 => 7, 14 => 14, 30 => 30, 60 => 60, 90 => 90), 
    '#description' => t('The number of days in advance that a reservation can be made by a standard user. Default is 14. Maximum is 90.'),
    '#default_value' => variable_get('room_reservations_advance_standard', 14),
  );
  $form['room_reservations_advance_extended'] = array(
    '#title' => t('Days in advance (extended)'),
    '#type' => 'select',
    '#options' => array(30 => 30, 60 => 60, 90 => 90, 180 => 180, 360 => 360), 
    '#description' => t('The number of days in advance that a reservation can be made by an admin. Default is 180.  Maximum is 360.'),
    '#default_value' => variable_get('room_reservations_advance_extended', 180),
  );
  $form['room_reservations_reservations_per_user'] = array(
    '#title' => t('Open reservations per user'),
    '#type' => 'textfield',
    '#maxlength' => 2,
    '#size' => 2,
    '#description' => t('The maximum number of reservations that one particular user can have open at any time. Default is 4. Enter 0 to indicate
      that there is no maximum limit.'),
    '#default_value' => variable_get('room_reservations_reservations_per_user', 4),
  );
  $form['room_reservations_reservations_per_day'] = array(
    '#title' => t('Reservations per day'),
    '#type' => 'textfield',
    '#maxlength' => 2,
    '#size' => 2,
    '#description' => t('The maximum number of reservations that one particular user can make for a single day.  Default is 1.  Enter 0 to indicate
      that there is no maximum limit.'),
    '#default_value' => variable_get('room_reservations_reservations_per_day', 1),
  );
  $form['room_reservations_max_length_standard'] = array(
    '#title' => t('Maximum reservation length (standard)'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('room_reservations_max_length_standard', 120),
    '#description' => t('The maximum amount of time, in minutes, for which a room can be reserved by a standard user. Default is 120.'),
  );  
  $form['room_reservations_max_length_extended'] = array(
    '#title' => t('Maximum reservation length (extended)'),
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => variable_get('room_reservations_max_length_extended', 120),
    '#description' => t('The maximum amount of time, in minutes, for which a room can be reserved by an admin. Default is 120.'),
  );
  $form['room_reservations_end_early'] = array(
    '#title' => t('End reservations 15 minutes before closing time.'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => variable_get('room_reservations_end_early', 0),
    '#description' => t('All reservations end 15 minutes before closing time.'),
  );  
  $form['room_reservations_before_after_hours'] = array(
    '#title' => t('Time to show on calendar before/after open slots'),
    '#type' => 'select',
    '#options' => array(0 => '0 hours', 1 => t('1 hour'), 2 => t('2 hours'), '3' => t('All day')),
    '#default_value' => variable_get('room_reservations_before_after_hours', 3),
    '#description' => t('The number of hours before the first and last open slots for the day that are shown on the calendar. The default is to display the entire day.'),
  );
  $form['room_reservations_calendar_flipped'] = array(
    '#title' => t('Flip orientation of calendar display.'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => variable_get('room_reservations_calendar_flipped', 0),
    '#description' => t('The default orientation of the calendar display has rooms listed along the top and hours along the left side. Selecting this checkbox will
      flip this to display hours along the top and the rooms listed along the left side. This can be useful for categories with a very large number of rooms. NOTE:
      custom CSS will most likely be required to allow all the hour slots to fit the width of your theme.'),
  );   
  $form['room_reservations_compressed_labels'] = array(
    '#title' => t('Use smaller labels on calendar display.'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => variable_get('room_reservations_compressed_labels', 0),
    '#description' => t('The default display shows the room title and room capacity on both sides of the calendar display. Selecting this option shows only the room title on one side.'),
  );
  
  $form['room_reservations_date_displays'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date and time display formats'),
    '#collapsible' => true,
    '#collapsed' => true,
  );
  $form['room_reservations_date_displays']['room_reservations_picker_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Date format for the popup date picker to select calendar date.'),
    '#description' => t('Enter format in the form "y/m/d", "m/d/y", etc.'),
    '#default_value' => variable_get('room_reservations_picker_format', 'y/m/d'),
  );  
  $options = array(0 => '1:00 PM', 1 => '13:00');
  $form['room_reservations_date_displays']['room_reservations_hour_format'] = array(
    '#type' => 'radios',
    '#title' => t('Hour format for the calendar display.'),
    '#description' => t('Select either 2:00 PM or 14:00 display format.'),
    '#default_value' => variable_get('room_reservations_hour_format', 0),
    '#options' => $options,
  );
    
  
  /*
  $form['room_reservations_group_size'] = array(
    '#title' => t('Include group size.'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => variable_get('room_reservations_group_size', 0),
    '#description' => t('Show the group size field on the reservation form
      and require users to enter this information.'),
    '#weight' => -40,
  );
  */
  
  return system_settings_form($form);
}

/**
 * Form validation for the Settings / General page.
 */
function room_reservations_admin_settings_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    // Open reservations per user.
    $data = $form_state['values']['room_reservations_reservations_per_user'];
    if (!ctype_digit($data)) {
      $field = 'room_reservations_reservations_per_user';
      $message = t('Open reservations per user must be numeric.');
      form_set_error($field, $message);
    }
    // Reservations per day.
    $data = $form_state['values']['room_reservations_reservations_per_day'];
    if (!ctype_digit($data)) {
      $field = 'room_reservations_reservations_per_day';
      $message = t('Reservations per day must be numeric.');
      form_set_error($field, $message);
    }
  }
}

/**
 * Form constructor for the Settings / Default Email Address config page.
 */
function room_reservations_admin_settings_default_email($form, &$form_state) {
  $options = array(
    '0' => t('Do not set a default email address.'),
    '1' => t('Set the default email address equal to the user ID.'),
    '2' => t('Set the default email address equal to the user ID plus the domain name entered below.'),
  );
  $default_option = _room_reservations_get_variable('default_email_address');
  $default_domain = _room_reservations_get_variable('default_email_domain');
  $form['options'] = array(
    '#title' => t('Options'),
    '#type' => 'radios',
    '#options' => $options,
    '#description' => t('Options for including a default email address
      in the Reminders - Email Addresses field on the reservation form.'),
    '#default_value' => $default_option,
    '#weight' => -100,
  );
  $form['domain'] = array(
    '#title' => t('Domain Name'),
    '#type' => 'textfield',
    '#description' => t('The domain name that is added to the user
      name to create the default email address on the reservation form.
      Required if the third option above is selected.'),
    '#default_value' => $default_domain,
    '#maxlength' => 80,
    '#size' => 80,
    '#weight' => -90,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );

  return $form;
}

/**
 * Form validation for the Settings / Default Email Address config page.
 */
function room_reservations_admin_settings_default_email_validate(
  $form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $option = $form_state['values']['options'];
    $domain = trim($form_state['values']['domain']);
    if (($option == 2) && (!$domain)) {
      $field = 'domain';
      $message = t('A domain name must be entered when this option is selected.');
      form_set_error($field, $message);
    }
  }
}

/**
 * Form submission for the Settings / Default Email Address config page.
 */
function room_reservations_admin_settings_default_email_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $option = $form_state['values']['options'];
    $domain = trim($form_state['values']['domain']);
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  elseif ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $option = 0;
    $domain = '';
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $errors = FALSE;
  $result = _room_reservations_set_variable('default_email_address', $option);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('default_email_domain', $domain);
  if (!$result) {
    $errors = TRUE;
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}

/**
 * Form constructor for the Settings / Reminders configuration page.
 */
function room_reservations_admin_settings_reminders($form, &$form_state) {
  $default_send_reminders = _room_reservations_get_variable('send_reminders');
  $default_reminder_time = _room_reservations_get_variable('reminder_time');
  $default_reminder_cutoff = _room_reservations_get_variable('reminder_cutoff');
  $options = array();
  $options['1300'] = t('1:00 PM');
  $options['1400'] = t('2:00 PM');
  $options['1500'] = t('3:00 PM');
  $options['1600'] = t('4:00 PM');
  $options['1700'] = t('5:00 PM');
  $options['1800'] = t('6:00 PM');
  $options['1900'] = t('7:00 PM');
  $options['2000'] = t('8:00 PM');
  $options['2100'] = t('9:00 PM');
  $options['2200'] = t('10:00 PM');
  $options['2300'] = t('11:00 PM');
  $form['send_reminders'] = array(
    '#title' => t('Send reminders.'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => $default_send_reminders,
    '#description' => t('Send reservation reminders.'),
    '#weight' => -100,
  );
  $form['reminder_time'] = array(
    '#title' => t('Send reminders daily at'),
    '#type' => 'select',
    '#description' => t('The time each day when reminders will be sent
      for the next day\'s reservations.  Note that a cron job must be
      set up to run at this time.'),
    '#options' => $options,
    '#default_value' => $default_reminder_time,
    '#multiple' => FALSE,
    '#weight' => -90,
  );
  $form['reminder_cutoff'] = array(
    '#title' => t('Send reminders for reservations created before'),
    '#type' => 'select',
    '#description' => t('For reservations created on the same day
      that the reminder is sent, only send a reminder if the
      reservation was created before this time.  This is set so that
      patrons are not sent a reminder immediately after creating
      a reservation.'),
    '#options' => $options,
    '#default_value' => $default_reminder_cutoff,
    '#multiple' => FALSE,
    '#weight' => -80,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * Form submission for the Settings / Reminders configuration page.
 */
function room_reservations_admin_settings_reminders_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $send_reminders = $form_state['values']['send_reminders'];
    $reminder_time = $form_state['values']['reminder_time'];
    $reminder_cutoff = $form_state['values']['reminder_cutoff'];
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  elseif ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $send_reminders = 0;
    $reminder_time = 1300;
    $reminder_cutoff = 1300;
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $errors = FALSE;
  $result = _room_reservations_set_variable('send_reminders', $send_reminders);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_time', $reminder_time);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_cutoff', 
    $reminder_cutoff);
  if (!$result) {
    $errors = TRUE;
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}

/**
 * Form validation for the Settings / Reminders configuration page.
 */
function room_reservations_admin_settings_reminders_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $reminder_time = $form_state['values']['reminder_time'];
    $reminder_cutoff = $form_state['values']['reminder_cutoff'];
    if ($reminder_time < $reminder_cutoff) {
      $field = 'reminder_time';
      $message = t("'Send reminder daily at' time cannot be earlier than 'Send reminders for reservations created before' time.");
      form_set_error($field, $message);
    }
  }
}

/**
 * Form constructor for the Hours / Default Hours configuration page.
 */
function room_reservations_admin_settings_default_hours($form, &$form_state) {
  $hours = _room_reservations_hours();
  // Select box options.
  $options = array();
  $options['9999'] = t('Select the time');
  $options['0000'] = t('Midnight - start of day');
  foreach ($hours as $hour) {
    $time = $hour['time'];
    $display = t($hour['display']);
    if ($time != '0000') {
      $options[$time] = $display;
    }
  }
  $options['2400'] = t('Midnight - end of day');
  // Get saved default hours.
  $default_hours
    = unserialize(_room_reservations_get_variable('default_hours'));
  if (!$default_hours) {
    for ($x = 0; $x < 28; $x++) {
      $default_hours[$x] = '9999';
    }
  }
  $days = array(
    t('Sunday'),
    t('Monday'),
    t('Tuesday'),
    t('Wednesday'),
    t('Thursday'),
    t('Friday'),
    t('Saturday'),
  );
  $form['#tree'] = TRUE;
  for ($day = 0; $day < 7; $day++) {
    $day_hours = array();
    $day_hours[] = $default_hours[($day * 4)];
    $day_hours[] = $default_hours[($day * 4) + 1];
    $day_hours[] = $default_hours[($day * 4) + 2];
    $day_hours[] = $default_hours[($day * 4) + 3];
    $display_hours = _room_reservations_hours_display($day_hours);
    $form['day_' . $day] = array(
      '#type' => 'fieldset',
      '#title' => $days[$day] . ' (' . $display_hours . ')',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -90 + ($day * 10),
    );
    $form['day_' . $day]['first_shift_open_' . $day] = array(
      '#title' => t('First shift open'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default_hours[($day * 4)],
      '#weight' => -90 + ($day * 10) + 1,
    );
    $form['day_' . $day]['first_shift_close_' . $day] = array(
      '#title' => t('First shift close'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default_hours[($day * 4) + 1],
      '#weight' => -90 + ($day * 10) + 2,
    );
    $form['day_' . $day]['second_shift_open_' . $day] = array(
      '#title' => t('Second shift open'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default_hours[($day * 4) + 2],
      '#weight' => -90 + ($day * 10) + 3,
    );
    $form['day_' . $day]['second_shift_close_' . $day] = array(
      '#title' => t('Second shift close'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $default_hours[($day * 4) + 3],
      '#weight' => -90 + ($day * 10) + 4,
    );
  }
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * Form validation for the Hours / Default Hours configuration page.
 */
function room_reservations_admin_settings_default_hours_validate($form_id, &$form_state) {
  $days = array(
    t('Sunday'),
    t('Monday'),
    t('Tuesday'),
    t('Wednesday'),
    t('Thursday'),
    t('Friday'),
    t('Saturday'),
  );
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    for ($day = 0; $day < 7; $day++) {
      $open = TRUE;
      $second_shift = FALSE;
      $first_shift_open = $form_state['values']['day_' . $day]['first_shift_open_' . $day];
      $first_shift_close = $form_state['values']['day_' . $day]['first_shift_close_' . $day];
      $second_shift_open = $form_state['values']['day_' . $day]['second_shift_open_' . $day];
      $second_shift_close = $form_state['values']['day_' . $day]['second_shift_close_' . $day];
      $int_first_shift_open = intval($form_state['values']['day_' . $day]['first_shift_open_' . $day]);
      $int_first_shift_close = intval($form_state['values']['day_' . $day]['first_shift_close_' . $day]);
      $int_second_shift_open = intval($form_state['values']['day_' . $day]['second_shift_open_' . $day]);
      $int_second_shift_close = intval($form_state['values']['day_' . $day]['second_shift_close_' . $day]);
      // Closed.
      if (($int_first_shift_open == 9999) && ($int_first_shift_close == 9999) && ($int_second_shift_open == 9999) && ($int_second_shift_close == 9999)) {
        $open = FALSE;
      }
      // First shift.
      if ($open) {
        if ($int_first_shift_open == 9999) {
          $field = 'day_' . $day . '][first_shift_open_' . $day;
          $message = t('!day - First shift open is required.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
        elseif ($int_first_shift_close == 9999) {
          $field = 'day_' . $day . '][first_shift_close_' . $day;
          $message = t('!day - First shift close is required.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
        elseif ($int_first_shift_open >= $int_first_shift_close) {
          $field = 'day_' . $day . '][first_shift_close_' . $day;
          $message = t('!day - First shift close must be later than first shift open.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
      }
      // Second shift.
      if ($open) {
        if (($int_second_shift_open != 9999) || ($int_second_shift_close != 9999)) {
          $second_shift = TRUE;
        }
      }
      if ($second_shift) {
        if (($int_first_shift_open == 9999) && ($int_first_shift_close == 9999)) {
          $field = 'day_' . $day . '][second_shift_open_' . $day;
          $message = t('!day - Cannot have a second shift without a first shift.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_open == 9999) {
          $field = 'day_' . $day . '][second_shift_open_' . $day;
          $message = t('!day - Second shift open is missing.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_close == 9999) {
          $field = 'day_' . $day . '][second_shift_close_' . $day;
          $message = t('!day - Second shift close is missing.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_open <= $int_first_shift_close) {
          $field = 'day_' . $day . '][second_shift_open_' . $day;
          $message = t('!day - Second shift open must be later than first shift close.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_open >= $int_second_shift_close) {
          $field = 'day_' . $day . '][second_shift_close_' . $day;
          $message = t('!day - Second shift close must be later than second shift opten.', array('!day' => $days[$day]));
          form_set_error($field, $message);
        }
      }
    }
  }
}

/**
 * Form submission for the Hours / Default Hours configuration page.
 */
function room_reservations_admin_settings_default_hours_submit($form_id, &$form_state) {
  $default_hours = array();
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    for ($day = 0; $day < 7; $day++) {
      $default_hours[] = $form_state['values']['day_' . $day]['first_shift_open_' . $day];
      $default_hours[] = $form_state['values']['day_' . $day]['first_shift_close_' . $day];
      $default_hours[] = $form_state['values']['day_' . $day]['second_shift_open_' . $day];
      $default_hours[] = $form_state['values']['day_' . $day]['second_shift_close_' . $day];
    }
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  if ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    for ($day = 0; $day < 7; $day++) {
      $default_hours[] = '9999';
      $default_hours[] = '9999';
      $default_hours[] = '9999';
      $default_hours[] = '9999';
    }
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $errors = FALSE;
  $result = _room_reservations_set_variable('default_hours', serialize($default_hours));
  if (!$result) {
    $errors = TRUE;
  }
  else {

  }
  // Update monthly hours records.
  $sql = "SELECT * FROM {room_reservations_variables} WHERE name LIKE 'monthly_hours_%'";
  $results = db_query($sql);
  foreach ($results as $data){
    $name = $data->name;
    $month = intval(drupal_substr($name, 19));
    $year = drupal_substr($name, 14, 4);
    $mo_hours = unserialize($data->value);
    // Days in the month.
    $days = date('t', mktime(0, 0, 0, $month, 1, (int) $year));
    $upd_mo_hours = array();
    for ($day = 0; $day < $days; $day++) {
      $default_ind = $mo_hours[($day * 5)];
      if ($default_ind == 'D') {
        // Update monthly hours with default day of week hours.
        // Day of the week.
        $dow = date('w', mktime(0, 0, 0, $month, $day + 1, (int) $year));
        $upd_mo_hours[($day * 5)] = 'D';
        $upd_mo_hours[($day * 5) + 1] = $default_hours[($dow * 4)];
        $upd_mo_hours[($day * 5) + 2] = $default_hours[($dow * 4) + 1];
        $upd_mo_hours[($day * 5) + 3] = $default_hours[($dow * 4) + 2];
        $upd_mo_hours[($day * 5) + 4] = $default_hours[($dow * 4) + 3];
      }
      elseif ($default_ind == 'O') {
        // Leave monthly hours unchanged.
        $upd_mo_hours[($day * 5)] = 'O';
        $upd_mo_hours[($day * 5) + 1] = $mo_hours[($day * 5) + 1];
        $upd_mo_hours[($day * 5) + 2] = $mo_hours[($day * 5) + 2];
        $upd_mo_hours[($day * 5) + 3] = $mo_hours[($day * 5) + 3];
        $upd_mo_hours[($day * 5) + 4] = $mo_hours[($day * 5) + 4];
      }
    }
    $result2 = _room_reservations_set_variable($name, 
      serialize($upd_mo_hours));
    if (!$result2) {
      $errors = TRUE;
    }
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}

/**
 * Form constructor for the Hours / Daily Hours configuration page.
 */
function room_reservations_admin_settings_daily_hours($form, &$form_state, $selected_month = NULL) {
  $cur_months = _room_reservations_current_months();
  
  if (!$selected_month) {
    $selected_month = date('Y_m');
  }
  $selected_month = isset($form_state['input']['month']) ? $form_state['input']['month'] : $selected_month;
  
  // create a form to pick a month.
  $month_options = array();
  $first = current($cur_months);
  foreach ($cur_months as $cur_month) {
    $yyyy_mm = $cur_month['YYYY_MM'];
    $display = t($cur_month['display']);
    $month_options[$yyyy_mm] = t($cur_month['display']);
  }
  
  // Form.
  $form['select_month'] = array(
    '#type' => 'container',
    '#weight' => -2000,
  );
  $form['select_month']['month'] = array(
    '#title' => t('Month'),
    '#type' => 'select',
    '#options' => $month_options,
    '#default_value' => $selected_month ? $selected_month : $first['YYYY_MM'],
  );
  $form['select_month']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Select a month'),
  ); 
  
  // and if no month has been selected; just return this form
  if (!$selected_month) {  
    return $form;
  }
  
  // If the month has been selected, return a form to update the hours for each day of that month.
  $hours = _room_reservations_hours();
  // Select box options.
  $options = array();
  $options['9999'] = t('Select the time');
  $options['0000'] = t('Midnight - start of day');
  foreach ($hours as $hour) {
    $time = $hour['time'];
    $display = t($hour['display']);
    if ($time != '0000') {
      $options[$time] = $display;
    }
  }
  $options['2400'] = t('Midnight - end of day');
  
  $yyyy_mm = $selected_month;
  $month = intval(drupal_substr($yyyy_mm, 5));
  $year = drupal_substr($yyyy_mm, 0, 4);
  $month_display = date('F Y', mktime(0, 0, 0, $month, 1, $year));
  // Days in the month.
  $days = date('t', mktime(0, 0, 0, $month, 1, $year));
  if (!$mo_hours = unserialize(_room_reservations_get_variable('monthly_hours_' . $yyyy_mm))) {
    $mo_hours = _room_reservations_create_mo_hours($year, $month, $yyyy_mm, true);
  }
  // Form.
  $form['#tree'] = TRUE;
  $form['month_display'] = array(
    '#prefix' => '<b>',
    '#suffix' => '</b><br />',
    '#markup' => $month_display,
    '#weight' => -100,
  );
  $form['note'] = array(
    '#markup' => t('Asterisk (*) indicates that the hours have been changed from the default'),
    '#weight' => -99,
  );
  for ($day = 0; $day < $days; $day++) {
    // Day of week.
    $dow = date('l', mktime(0, 0, 0, $month, $day + 1, $year));
    $changed_hours = ($mo_hours[($day * 5)] == 'O') ? '*' : '';
    $day_hours = array();
    $day_hours[] = $mo_hours[($day * 5) + 1];
    $day_hours[] = $mo_hours[($day * 5) + 2];
    $day_hours[] = $mo_hours[($day * 5) + 3];
    $day_hours[] = $mo_hours[($day * 5) + 4];
    $display_hours = _room_reservations_hours_display($day_hours);
    $title = ($day + 1) . ' ' . $dow . ' (' . $display_hours . ') ' . $changed_hours;
    $form['day_' . $day] = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -9 + ($day * 2),
    );
    $form['day_' . $day]['first_shift_open_' . $day] = array(
      '#title' => t('First shift open'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $mo_hours[($day * 5) + 1],
      //'#weight' => -9 + ($day * 2) + 1,
    );
    $form['day_' . $day]['first_shift_close_' . $day] = array(
      '#title' => t('First shift close'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $mo_hours[($day * 5) + 2],
      //'#weight' => -9 + ($day * 10) + 2,
    );
    $form['day_' . $day]['second_shift_open_' . $day] = array(
      '#title' => t('Second shift open'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $mo_hours[($day * 5) + 3],
      //'#weight' => -90 + ($day * 10) + 3,
    );
    $form['day_' . $day]['second_shift_close_' . $day] = array(
      '#title' => t('Second shift close'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $mo_hours[($day * 5) + 4],
      //'#weight' => -90 + ($day * 10) + 4,
    );
  }
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 400,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 401,
  );
  $form['month'] = array(
    '#type' => 'value',
    '#value' => $selected_month,
  );
  
  return $form;
}

/**
 * Form validation for the Hours / Daily Hours configuration page.
 */
function room_reservations_admin_settings_daily_hours_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $month_value = $form_state['values']['month'];
    $month = intval(drupal_substr($month_value, 5));
    $year = drupal_substr($month_value, 0, 4);
    // Days in the month.
    $days = date('t', mktime(0, 0, 0, $month, 1, $year));
    for ($day = 0; $day < $days; $day++) {
      // Day of the week.
      $dow = date('l', mktime(0, 0, 0, $month, $day + 1, $year));
      // Day of month.
      $dom = $day + 1;
      $open = TRUE;
      $second_shift = FALSE;
      $first_shift_open = $form_state['values']['day_' . $day]['first_shift_open_' . $day];
      $first_shift_close = $form_state['values']['day_' . $day]['first_shift_close_' . $day];
      $second_shift_open = $form_state['values']['day_' . $day]['second_shift_open_' . $day];
      $second_shift_close = $form_state['values']['day_' . $day]['second_shift_close_' . $day];
      $int_first_shift_open = intval($form_state['values']['day_' . $day]['first_shift_open_' . $day]);
      $int_first_shift_close = intval($form_state['values']['day_' . $day]['first_shift_close_' . $day]);
      $int_second_shift_open = intval($form_state['values']['day_' . $day]['second_shift_open_' . $day]);
      $int_second_shift_close = intval($form_state['values']['day_' . $day]['second_shift_close_' . $day]);
      // Closed.
      if (($int_first_shift_open == 9999) && ($int_first_shift_close == 9999) && ($int_second_shift_open == 9999) && ($int_second_shift_close == 9999)) {
        $open = FALSE;
      }
      // First shift.
      if ($open) {
        if ($int_first_shift_open == 9999) {
          $field = 'day_' . $day . '][first_shift_open_' . $day;
          $message = t('!day - First shift open is required.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
        elseif ($int_first_shift_close == 9999) {
          $field = 'day_' . $day . '][first_shift_close_' . $day;
          $message = t('!day - First shift close is required.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
        elseif ($int_first_shift_open >= $int_first_shift_close) {
          $field = 'day_' . $day . '][first_shift_close_' . $day;
          $message = t('!day - First shift close must be later than first shift open.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
      }
      // Second shift.
      if ($open) {
        if (($int_second_shift_open != 9999) || ($int_second_shift_close != 9999)) {
          $second_shift = TRUE;
        }
      }
      if ($second_shift) {
        if (($int_first_shift_open == 9999) && ($int_first_shift_close == 9999)) {
          $field = 'day_' . $day . '][second_shift_open_' . $day;
          $message = t('!day - Cannot have a second shift without a first shift.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_open == 9999) {
          $field = 'day_' . $day . '][second_shift_open_' . $day;
          $message = t('!day - Second shift open is missing.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_close == 9999) {
          $field = 'day_' . $day . '][second_shift_close_' . $day;
          $message = t('!day - Second shift close is missing.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_open <= $int_first_shift_close) {
          $field = 'day_' . $day . '][second_shift_open_' . $day;
          $message = t('!day - Second shift open must be later than first shift close.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
        elseif ($int_second_shift_open >= $int_second_shift_close) {
          $field = 'day_' . $day . '][second_shift_close_' . $day;
          $message = t('!day - Second shift close must be later than second shift opten.', array('!day' => $dom . ' ' . $dow));
          form_set_error($field, $message);
        }
      }
    }
  }
}

/**
 * Form submission for the Hours / Daily Hours configuration page.
 */
function room_reservations_admin_settings_daily_hours_submit($form_id, &$form_state) {
  // Select month.
  if ($form_state['clicked_button']['#value'] == t('Select a month')) {
    $month = $form_state['input']['select_month']['month'] ? $form_state['input']['select_month']['month'] : $form_state['values']['month'];
    $form_state['redirect'] = 'admin/config/system/room_reservations/hours/daily_hours/' . $month;
  }
  // Process the daily hours for the selected month.
  elseif ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
    $month_value = $form_state['values']['month'];
    $month = intval(drupal_substr($month_value, 5));
    $year = drupal_substr($month_value, 0, 4);
    // Days in the month.
    $days = date('t', mktime(0, 0, 0, $month, 1, $year));
    $mo_hours = unserialize(_room_reservations_get_variable('monthly_hours_' . $month_value));
    $updated_mo_hours = array();
    $default_hours = unserialize(_room_reservations_get_variable('default_hours'));
    for ($day = 0; $day < $days; $day++) {
      // User entered hours for a single day.
      $day_first_open = $form_state['values']['day_' . $day]['first_shift_open_' . $day];
      $day_first_close = $form_state['values']['day_' . $day]['first_shift_close_' . $day];
      $day_second_open = $form_state['values']['day_' . $day]['second_shift_open_' . $day];
      $day_second_close = $form_state['values']['day_' . $day]['second_shift_close_' . $day];
      // Default hours for the day of the week.
      // Day of week.
      $dow = date('w', mktime(0, 0, 0, $month, $day + 1, $year));
      $default_first_open = $default_hours[($dow * 4) + 0];
      $default_first_close = $default_hours[($dow * 4) + 1];
      $default_second_open = $default_hours[($dow * 4) + 2];
      $default_second_close = $default_hours[($dow * 4) + 3];
      // Compare user entered hours to default for the day of the week.
      if (($day_first_open == $default_first_open) && 
        ($day_first_close == $default_first_close) && 
        ($day_second_open == $default_second_open) && 
        ($day_second_close == $default_second_close)) {
        $day_is_default = TRUE;
      }
      else {
        $day_is_default = FALSE;
      }
      // Update the monthly hours record.
      $updated_mo_hours[($day * 5)] = ($day_is_default) ? 'D' : 'O';
      $updated_mo_hours[($day * 5) + 1] = $day_first_open;
      $updated_mo_hours[($day * 5) + 2] = $day_first_close;
      $updated_mo_hours[($day * 5) + 3] = $day_second_open;
      $updated_mo_hours[($day * 5) + 4] = $day_second_close;
    }
    $result = _room_reservations_set_variable('monthly_hours_' . $month_value, serialize($updated_mo_hours));
    if ($result) {
      drupal_set_message($confirmation);
    }
    else {
      drupal_set_message($error, 'error');
    }
  }
  // Reset the month to the default hours.
  elseif ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
    $month_value = $form_state['values']['month'];
    $month = intval(drupal_substr($month_value, 5));
    $year = drupal_substr($month_value, 0, 4);
    $result = _room_reservations_create_mo_hours($year, $month, $month_value);
    if ($result) {
      drupal_set_message($confirmation);
    }
    else {
      drupal_set_message($error, 'error');
    }
  }
}

/**
 * Form constructor for the Display Text configuration page.
 */
function room_reservations_admin_settings_page($form, &$form_state) {
  $default_calendar_text = _room_reservations_get_variable('calendar_text');
  $default_reserve_room_instructions_text
    = _room_reservations_get_variable('reserve_instructions');
  $default_reserve_form_instructions_text
    = _room_reservations_get_variable('reserve_form_instructions');
  $default_policies = _room_reservations_get_variable('policies');
  $form['calendar_text'] = array(
    '#title' => t('Calendar page text'),
    '#type' => 'textarea',
    '#rows' => 5,
    '#description' => t('Text displayed at the top of the reservation
       calendar page.'),
    '#default_value' => $default_calendar_text,
    '#weight' => -90,
  );
  $form['reserve_instructions_text'] = array(
    '#title' => t('Reserve a room instructions text'),
    '#type' => 'textarea',
    '#rows' => 2,
    '#description' => t('Text displayed just above the words Reservation
      Calendar on the reservation calendar page.'),
    '#default_value' => $default_reserve_room_instructions_text,
    '#weight' => -80,
  );
  $form['form_instructions_text'] = array(
    '#title' => t('Reservation form instructions text'),
    '#type' => 'textarea',
    '#rows' => 2,
    '#description' => t('Text displayed at the top of the reservation form.'),
    '#default_value' => $default_reserve_form_instructions_text,
    '#weight' => -70,
  );
  $form['policies'] = array(
    '#title' => t('Policies'),
    '#type' => 'textarea',
    '#rows' => 10,
    '#description' => t('Policies displayed on the reservation policies page.'),
    '#default_value' => $default_policies,
    '#weight' => -60,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * Form submission for the Display Text configuration page.
 */
function room_reservations_admin_settings_page_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $calendar_text = $form_state['values']['calendar_text'];
    $reserve_room_instructions_text
      = $form_state['values']['reserve_instructions_text'];
    $reserve_form_instructions_text
      = $form_state['values']['form_instructions_text'];
    $policies = $form_state['values']['policies'];
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  elseif ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $calendar_text = '';
    $reserve_room_instructions_text = '';
    $reserve_form_instructions_text = '';
    $policies = '';
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $errors = FALSE;
  $result = _room_reservations_set_variable('calendar_text', $calendar_text);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reserve_instructions', 
      $reserve_room_instructions_text);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reserve_form_instructions', 
      $reserve_form_instructions_text);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('policies', $policies);
  if (!$result) {
    $errors = TRUE;
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}

/**
 * Form constructor for the SMS / Wireless Carriers configuration page.
 */
function room_reservations_admin_settings_sms($form, &$form_state) {
  $default_sms_option = _room_reservations_get_variable('sms_option');
  $form['#tree'] = TRUE;
  $form['sms_option'] = array(
    '#title' => t('SMS option'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => $default_sms_option,
    '#description' => t('Give users the option of receiving confirmation
        messages and reminders as SMS text messages.'),
    '#weight' => -110,
  );
  $form['carriers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wireless carriers'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => -100,
  );
  $sql = "
    SELECT id, value
    FROM {room_reservations_variables}
    WHERE name = '%s'
    ORDER BY value
  ";
  $result = db_query($sql, 'carrier');
  if ($result) {
    $options = array();
    $form['carriers']['list_0'] = array(
      '#value' => '<table><tr><th>' . t('Wireless carrier') . '</th><th>' .
      t('Domain name') . '</th></tr>',
      '#weight' => -99,
    );
    $x = 0;
    while ($data = db_fetch_object($result)) {
      $x++;
      $id = $data->id;
      $values = explode('~', $data->value);
      $options[strval($id)] = $values[0];
      $form['carriers']['list_' . $x] = array(
        '#value' => '<tr><td>' . check_plain($values[0]) . '</td><td>' .
        check_plain($values[1]) . '</td></tr>',
        '#weight' => -99 + $x,
      );
      $x++;
    }
    $form['carriers']['list_' . $x] = array(
      '#value' => '</table>',
      '#weight' => -99 + $x,
    );
  }
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * Form submission for the SMS / Wireless Carriers configuration page.
 */
function room_reservations_admin_settings_sms_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $sms_option = $form_state['values']['sms_option'];
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  if ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $sms_option = 0;
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $result = _room_reservations_set_variable('sms_option', $sms_option);
  if (!$result) {
    $errors = TRUE;
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}

/**
 * Form constructor for the SMS / Add Carrier configuration page.
 */
function room_reservations_admin_settings_sms_add($form, &$form_state) {
  $form['carrier'] = array(
    '#title' => t('Wireless carrier'),
    '#type' => 'textfield',
    '#maxlength' => 50,
    '#size' => 50,
    '#description' => t('The name of the wireless carrier, such as !att or
      !verizon.', array(
      '!att' => 'AT&T',
      '!verizon' => 'Verizon',
    )),
    '#weight' => -20,
  );
  $form['domain'] = array(
    '#title' => t('Domain name'),
    '#type' => 'textfield',
    '#maxlength' => 50,
    '#size' => 50,
    '#description' => t('The domain name of the wireless carrier, such as
      !att or !verizon.', array(
      '!att' => '@txt.att.net',
      '!verizon' => '@vtext.com',
    )),
    '#weight' => -10,
  );
  $form['add_carrier']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 100,
  );
  return $form;
}

/**
 * Form validation for the SMS / Add Carrier configuration page.
 */
function room_reservations_admin_settings_sms_add_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Add')) {
    $carrier = $form_state['values']['carrier'];
    $domain = $form_state['values']['domain'];
    if (!$carrier) {
      $field = 'add_carrier][carrier';
      $message = t('Wireless carrier is required.');
      form_set_error($field, $message);
    }
    if (!$domain) {
      $field = 'add_carrier][domain';
      $message = t('Domain name is required.');
      form_set_error($field, $message);
    }
    else {
      if (drupal_substr($domain, 0, 1) != '@') {
        $field = 'add_carrier][domain';
        $message = t('Domain name must begin with @.');
        form_set_error($field, $message);
      }
    }
    if (($carrier) && ($domain)) {
      $sql = "
        SELECT value
        FROM {room_reservations_variables}
        WHERE name = '%s'
        ";
      $result = db_query($sql, 'carrier');
      if ($result) {
        while ($data = db_fetch_object($result)) {
          $values = explode('~', $data->value);
          if ($values[0] == $carrier) {
            $field = 'add_carrier][carrier';
            $message = t('There is already a record for this wireless
              carrier.');
            form_set_error($field, $message);
            break;
          }
        }
      }
    }
  }
}

/**
 * Form submission for the SMS / Add Carrier configuration page.
 */
function room_reservations_admin_settings_sms_add_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Add')) {
    $carrier = $form_state['values']['carrier'];
    $domain = $form_state['values']['domain'];
    $sql = "
      INSERT INTO {room_reservations_variables}
      (name, value)
      VALUES ('%s', '%s')
      ";
    $result = db_query($sql, 'carrier', $carrier . '~' . $domain);
    if (!$result) {
      drupal_set_message(t('The wireless carrier could not be added.'), 
        'error');
    }
    else {
      drupal_set_message(t('The wireless carrier has been added.'));
    }
  }
}

/**
 * Form constructor for the SMS / Delete Network configuration page.
 */
function room_reservations_admin_settings_sms_delete($form, &$form_state) {
  $sql = "
    SELECT id, value
    FROM {room_reservations_variables}
    WHERE name = '%s'
    ORDER BY value
  ";
  $result = db_query($sql, 'carrier');
  if ($result) {
    $options = array();
    $options[strval(0)] = t('Select a wireless carrier');
    $x = 0;
    while ($data = db_fetch_object($result)) {
      $x++;
      $id = $data->id;
      $values = explode('~', $data->value);
      $options[strval($id)] = $values[0];
      $x++;
    }
  }
  if ($options) {
    $form['carrier'] = array(
      '#type' => 'select',
      '#title' => t('Wireless carrier'),
      '#options' => $options,
      '#weight' => 25,
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 30,
    );
  }
  return $form;
}

/**
 * Form validation for the SMS / Delete Network configuration page.
 */
function room_reservations_admin_settings_sms_delete_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $carrier = $form_state['values']['carrier'];
    if (!$carrier) {
      $field = 'delete_carrier][carrier';
      $message = t('Wireless carrier is required.');
      form_set_error($field, $message);
    }
  }
}

/**
 * Form submission for the SMS / Delete Network configuration page.
 */
function room_reservations_admin_settings_sms_delete_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $carrier = $form_state['values']['carrier'];
    $sql = "
      DELETE FROM {room_reservations_variables}
      WHERE ID = %d
      ";
    $result = db_query($sql, $carrier);
    if (!$result) {
      drupal_set_message(t('The wireless carrier could not be deleted.'), 
        'error');
    }
    else {
      drupal_set_message(t('The wireless carrier has been deleted.'));
    }
  }
}

/**
 * Form constructor for the Messages / Email Messages configuration page.
 */
function room_reservations_admin_settings_email($form, &$form_state) {
  $default_from_address
    = _room_reservations_get_variable('from_address');
  $default_confirmation_header_text
    = _room_reservations_get_variable('confirmation_header_text');
  $default_confirmation_owner_text
    = _room_reservations_get_variable('confirmation_owner_text');
  $default_confirmation_group_text
    = _room_reservations_get_variable('confirmation_group_text');
  $default_reminder_header_text
    = _room_reservations_get_variable('reminder_header_text');
  $default_reminder_owner_text
    = _room_reservations_get_variable('reminder_owner_text');
  $default_reminder_group_text
    = _room_reservations_get_variable('reminder_group_text');
  $tokens = array(
    '%reservation_name' => t('The name given to the reservation to identify it
      on the reservation calendar.'),
    '%room' => t('The room that has been reserved.'),
    '%month' => t('The full name of the month of the reservation date.'),
    '%month_number' => t('The month number of the reservation date.'),
    '%day' => t('The numeric day of the month of the reservation date.'),
    '%day_of_the_week' => t('The full name of the day of the week of the
      reservation date.'),
    '%time' => t('The time of the reservation.'),
    '%minutes' => t('The length of the reservation in minutes.'),
  );
  $token_display = '<p>' .
    t('The following tokens will be replaced with dynamic values in any of the
    fields below.') . '</p><ul>';
  foreach ($tokens as $key => $value) {
    $token_display .= '<li>' . $key . ' - ' . $value . '</li>';
  }
  $token_display .= '</ul>';
  $form['from_address'] = array(
    '#title' => t('From address'),
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#size' => 100,
    '#description' => t('The from address on all email messages.'),
    '#default_value' => $default_from_address,
    '#weight' => -110,
  );
  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Token values'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -100,
  );
  $form['tokens']['values'] = array(
    '#value' => $token_display,
  );
  $form['confirmation_header'] = array(
    '#title' => t('Confirmation heading'),
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#size' => 100,
    '#description' => t('The heading for all confirmation messages.'),
    '#default_value' => $default_confirmation_header_text,
    '#weight' => -97,
  );
  $form['confirmation_owner'] = array(
    '#title' => t('Confirmation body'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#description' => t('Confirmation message sent to the person who has made a
      reservation.'),
    '#default_value' => $default_confirmation_owner_text,
    '#weight' => -95,
  );
  $form['confirmation_group'] = array(
    '#title' => t('Confirmation to group members body'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#description' => t('Confirmation message sent to all members of the group
      except for the person who made the reservation.'),
    '#default_value' => $default_confirmation_group_text,
    '#weight' => -90,
  );
  $form['reminder_header'] = array(
    '#title' => t('Reminder heading'),
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#size' => 100,
    '#description' => t('The heading for all reminder messages.'),
    '#default_value' => $default_reminder_header_text,
    '#weight' => -85,
  );
  $form['reminder_owner'] = array(
    '#title' => t('Reminder body'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#description' => t('Reminder message sent to the person who has made a
      reservation.'),
    '#default_value' => $default_reminder_owner_text,
    '#weight' => -80,
  );
  $form['reminder_group'] = array(
    '#title' => t('Reminder to group members body'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#description' => t('Reminder message sent to all members of the group
      except for the person who made the reservation.'),
    '#default_value' => $default_reminder_group_text,
    '#weight' => -75,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * Form validation for the Messages / Email Messages configuration page.
 */
function room_reservations_admin_settings_email_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $from_address = $form_state['values']['from_address'];
    if (valid_email_address($from_address)) {
      // Valid.
    }
    else {
      $field = 'from_address';
      $message = t('Invalid email address.');
      form_set_error($field, $message);
    }
  }
}

/**
 * Form submission for the Messages / Email Messages configuration page.
 */
function room_reservations_admin_settings_email_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $from_address = $form_state['values']['from_address'];
    $confirmation_header = $form_state['values']['confirmation_header'];
    $confirmation_owner = $form_state['values']['confirmation_owner'];
    $confirmation_group = $form_state['values']['confirmation_group'];
    $reminder_header = $form_state['values']['reminder_header'];
    $reminder_owner = $form_state['values']['reminder_owner'];
    $reminder_group = $form_state['values']['reminder_group'];
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  elseif ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $confirmation_header = '';
    $confirmation_owner = '';
    $confirmation_group = '';
    $reminder_header = '';
    $reminder_owner = '';
    $reminder_group = '';
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $errors = FALSE;
  $result = _room_reservations_set_variable('from_address', $from_address);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('confirmation_header_text', 
    $confirmation_header);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('confirmation_owner_text', 
    $confirmation_owner);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('confirmation_group_text', 
    $confirmation_group);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_header_text', 
    $reminder_header);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_owner_text', 
    $reminder_owner);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_group_text', 
    $reminder_group);
  if (!$result) {
    $errors = TRUE;
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}

/**
 * Form constructor for the Messages / SMS Messages configuration page.
 */
function room_reservations_admin_settings_text($form, &$form_state) {
  $default_enable_text
    = _room_reservations_get_variable('enable_text');
  $default_from_address
    = _room_reservations_get_variable('from_address_sms');
  $default_confirmation_header_text
    = _room_reservations_get_variable('confirmation_header_text_sms');
  $default_confirmation_owner_text
    = _room_reservations_get_variable('confirmation_owner_text_sms');
  $default_reminder_header_text
    = _room_reservations_get_variable('reminder_header_text_sms');
  $default_reminder_owner_text
    = _room_reservations_get_variable('reminder_owner_text_sms');
  $tokens = array(
    '%reservation_name' => t('The name given to the reservation to identify it
      on the reservation calendar.'),
    '%room' => t('The room that has been reserved.'),
    '%month' => t('The full name of the month of the reservation date.'),
    '%month_number' => t('The month number of the reservation date.'),
    '%day' => t('The numeric day of the month of the reservation date.'),
    '%day_of_the_week' => t('The full name of the day of the week of the
      reservation date.'),
    '%time' => t('The time of the reservation.'),
    '%minutes' => t('The length of the reservation in minutes.'),
  );
  $token_display = '<p>' .
    t('The following tokens will be replaced with dynamic values in any of the
    fields below.') . '</p><ul>';
  foreach ($tokens as $key => $value) {
    $token_display .= '<li>' . $key . ' - ' . $value . '</li>';
  }
  $token_display .= '</ul>';
  $form['enable_text'] = array(
    '#title' => t('Enable SMS text messages.'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => $default_enable_text,
    '#description' => t('Enable the ability of users to receive reservation
        confirmations and reminders by text message.'),
    '#weight' => -120,
  );
  $form['from_address'] = array(
    '#title' => t('From address'),
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#size' => 100,
    '#description' => t('The from address on all text messages.'),
    '#default_value' => $default_from_address,
    '#weight' => -110,
  );
  $form['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Token values'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -100,
  );
  $form['tokens']['values'] = array(
    '#value' => $token_display,
  );
  $form['confirmation_header'] = array(
    '#title' => t('Confirmation heading'),
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#size' => 100,
    '#description' => t('The heading for confirmation text messages.'),
    '#default_value' => $default_confirmation_header_text,
    '#weight' => -97,
  );
  $form['confirmation_owner'] = array(
    '#title' => t('Confirmation body'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#description' => t('Confirmation text message body.'),
    '#default_value' => $default_confirmation_owner_text,
    '#weight' => -95,
  );
  $form['reminder_header'] = array(
    '#title' => t('Reminder heading'),
    '#type' => 'textfield',
    '#maxlength' => 100,
    '#size' => 100,
    '#description' => t('The heading for reminder text messages.'),
    '#default_value' => $default_reminder_header_text,
    '#weight' => -85,
  );
  $form['reminder_owner'] = array(
    '#title' => t('Reminder body'),
    '#type' => 'textarea',
    '#rows' => 8,
    '#description' => t('Reminder text message body.'),
    '#default_value' => $default_reminder_owner_text,
    '#weight' => -80,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 100,
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 101,
  );
  return $form;
}

/**
 * Form validatiion for the Messages / SMS Messages configuration page.
 */
function room_reservations_admin_settings_text_validate($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $from_address = $form_state['values']['from_address'];
    if (valid_email_address($from_address)) {
      // Valid.
    }
    else {
      $field = 'from_address';
      $message = t('Invalid email address.');
      form_set_error($field, $message);
    }
  }
}

/**
 * Form submission for the Messages / SMS Messages configuration page.
 */
function room_reservations_admin_settings_text_submit($form_id, &$form_state) {
  if ($form_state['clicked_button']['#value'] == t('Save configuration')) {
    $enable_text = $form_state['values']['enable_text'];
    $from_address = $form_state['values']['from_address'];
    $confirmation_header = $form_state['values']['confirmation_header'];
    $confirmation_owner = $form_state['values']['confirmation_owner'];
    $reminder_header = $form_state['values']['reminder_header'];
    $reminder_owner = $form_state['values']['reminder_owner'];
    $confirmation = ROOM_RESERVATIONS_SAVE_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_SAVE_ERROR_MSG;
  }
  elseif ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $enable_text = 0;
    $confirmation_header = '';
    $confirmation_owner = '';
    $reminder_header = '';
    $reminder_owner = '';
    $confirmation = ROOM_RESERVATIONS_RESET_CONFIRMATION_MSG;
    $error = ROOM_RESERVATIONS_RESET_ERROR_MSG;
  }
  $errors = FALSE;
  $result = _room_reservations_set_variable('enable_text', $enable_text);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('from_address_sms', $from_address);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('confirmation_header_text_sms', 
    $confirmation_header);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('confirmation_owner_text_sms', 
    $confirmation_owner);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_header_text_sms', 
    $reminder_header);
  if (!$result) {
    $errors = TRUE;
  }
  $result = _room_reservations_set_variable('reminder_owner_text_sms', 
    $reminder_owner);
  if (!$result) {
    $errors = TRUE;
  }
  if ($errors) {
    drupal_set_message($error, 'error');
  }
  else {
    drupal_set_message($confirmation);
  }
}
