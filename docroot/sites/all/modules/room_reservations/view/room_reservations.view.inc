<?php
/**
 * @file
 * Miscellaneous view functionality.
 */

/**
 * Change date form.
 *
 * Constructor for the small form that appears just above the reservation
 * calendar and allows the user to change the date for which reservations
 * are displayed on the calendar.
 *
 * @param array $building_hours
 *   Contains hrs information for each day for which a reservation can be made.
 * @param array $dates
 *   An array containing information about all the possible days for which a
 *   reservtion can be made.
 * @param array $rooms
 *   An array representing all of the rooms that can be reserved.
 * @param array $categories
 *   An array of all the room categories.
 * @param string $selected_category
 *   The current category the user has selected, if any.
 */
function room_reservations_select_room_date_form(&$form_state, $building_hours, $dates, $rooms, $categories, $selected_category) {
  // Select box options.
  $date_options = array();
  foreach ($dates as $day) {
    $building_hours_day = $building_hours[$day['yyyymmdd']];
    if ($building_hours_day['open']) {
      $date_options[$day['month-number'] . "/" . $day['day']] = t($day['display']);
    }
  }
  $first = TRUE;
  foreach ($rooms as $room) {
    $name = $room['name'];
    $key = 'room ' . $name;
    $capacity = $room['capacity'];
    $room_category = $room['category'];
    $value = t('@room (capacity: !capacity persons)', array('@room' => $name, '!capacity' => $capacity));
    $room_options[$key] = $value;
    if ($first) {
      $room_default = $key;
      $first = FALSE;
    }
  }
  ksort($room_options);
  // Defaults.
  if ($selected_category) {
    $room_default = 'room ' . $selected_category;
  }
  foreach ($dates as $day) {
    if ($day['selected']) {
      $date_default = $day['month-number'] . '/' . $day['day'];
      break;
    }
  }
  // Form.
  $form['date'] = array(
    '#title' => t('View a Different Day'),
    '#type' => 'select',
    '#options' => $date_options,
    '#default_value' => $date_default,
    '#weight' => 10,
  );
  $form['room'] = array(
    '#title' => t('View a Different Room'),
    '#type' => 'select',
    '#options' => $room_options,
    '#default_value' => $room_default,
    '#weight' => 20,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Change Day or Room'),
    '#weight' => 30,
  );
  return $form;
}

/**
 * Form submission for the change date form.
 *
 * @param string $form_id
 *   Drupal form id.
 * @param array $form_state
 *   Drupal form state object.
 */
function room_reservations_select_room_date_form_submit($form_id, &$form_state) {
  $date = $form_state['values']['date'];
  $room = drupal_substr($form_state['values']['room'], 5);
  $redirect = "room_reservations/" . $date . "/" . $room;
  $form_state['redirect'] = $redirect;
}


