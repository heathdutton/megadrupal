<?php
/**
 * @file
 * Single reservation controller functionality.
 */

//module_load_include('inc', 'room_reservations', '/view/room_reservations_reservation.view');

/**
 * Make a new reservation.
 *
 * Validates whether the user can make a
 * reservation for the requested date, time, and room.
 *
 * @global object $user
 *   The Drupal user object.
 *
 * @param int $month_number
 *   The month for which the reservation is being made.
 * @param int $xday
 *   The day for which the reservation is being made.
 * @param string $time
 *   The time at which the reservation is beeing made.
 * @param int $xroom
 *   The room that is being reserved.
 *
 * @return string
 *   A form that can be used for making a reservation.
 */
function room_reservations_res_add($month_number, $xday, $time, $xroom) {
  $res = new Reservation();
  $res->monthNumber = $month_number;
  $res->day = $xday;
  $res->time = $time;
  $res->room = $xroom;
  $parameter_errors = FALSE;
  $output = '';
  global $user;
  $logged_in = ($user->uid);
  $res->userName = ($logged_in) ? $user->name : "";
  $res->emailAddresses = _room_reservations_default_email();
  if (!$logged_in) {
    drupal_set_message(t("You must be logged in to make a reservation."), 'error');
    $parameter_errors = TRUE;
  }
  // Validate date and get day of week, month name, and year.
  $dates = _room_reservations_dates();
  $reservation_is_for_today = FALSE;
  $day_found = FALSE;
  foreach ($dates as $day) {
    if (($res->monthNumber == $day['month-number']) && ($res->day == $day['day'])) {
      $day_found = TRUE;
      $month = $day['month'];
      $res->year = $day['year'];
      $day_of_week = $day['day-of-week'];
      $res->displayDate = $day['display'];
      $res->date = $day['yyyymmdd'];
      $unix_timestamp = strtotime($res->date);
      $res->displayDate = date("l, F j, Y", $unix_timestamp);
      if ($day['today']) {
        $reservation_is_for_today = TRUE;
      }
      break;
    }
  }
  if (!$day_found) {
    drupal_set_message(t("An invalid date has been entered."), 'error');
    $parameter_errors = TRUE;
  }
  else {
    // Determine if the building is open.
    $_room_reservations_building_hours = _room_reservations_facility_hours();
    $building_hours_day = $_room_reservations_building_hours[$res->date];
    if (!$building_hours_day['open']) {
      drupal_set_message(t("An invalid date has been entered."), 'error');
      $parameter_errors = TRUE;
    }
  }
  // Validate time.
  $hours = _room_reservations_hours();
  $hour_found = FALSE;
  foreach ($hours as $time_slot) {
    if ($res->time == $time_slot['time']) {
      $res->displayTime = $time_slot['display'];
      $hour_found = TRUE;
      break;
    }
  }
  if (!$hour_found) {
    drupal_set_message(t("An invalid time has been entered."), 'error');
    $parameter_errors = TRUE;
  }
  // Determine if the building is open for the time selected.
  if ($day_found) {
    $building_is_open = FALSE;
    $int_time_slot_time = intval($res->time);
    $int_first_shift_open = intval($building_hours_day['first_shift_open']);
    $int_first_shift_close = intval($building_hours_day['first_shift_close']);
    $int_second_shift_open = intval($building_hours_day['second_shift_open']);
    $int_second_shift_close = intval($building_hours_day['second_shift_close']);
    $open_24 = $building_hours_day['open_24_hours'];
    if ($open_24) {
      $building_is_open = TRUE;
    }
    elseif ((($int_time_slot_time >= $int_first_shift_open) &&
      ($int_time_slot_time < $int_first_shift_close)) || 
      (($int_time_slot_time >= $int_second_shift_open) &&
      ($int_time_slot_time < $int_second_shift_close))) {
      $building_is_open = TRUE;
    }
    else {
      $building_is_open = FALSE;
    }
    if (!$building_is_open) {
      drupal_set_message(t("An invalid time has been entered."), 'error');
      $parameter_errors = TRUE;
    }
  }
  // If the reservation is being made for the current date, it must be for a
  // time that is later than the current time.
  if (!$parameter_errors) {
    $reservation_date_time = strtotime($res->date . ' ' .
      drupal_substr($res->time, 0, 2) . ':' .
      drupal_substr($res->time, 2) . ':00');
    $now = strtotime(date('Y-m-d H:i:s'));
    if ($reservation_date_time < $now) {
      drupal_set_message(t("A reservation cannot be made for a time that has already passed."), 'error');
      $parameter_errors = TRUE;
    }
  }
  // Validate room.
  $rooms = _room_reservations_rooms();
  $room_found = FALSE;
  foreach ($rooms as $room) {
    if ($res->room == $room['name']) {
      $res->roomCapacity = $room['capacity'];
      $res->roomCategory = $room['category'];
      $room_found = TRUE;
      break;
    }
  }
  if (!$room_found) {
    drupal_set_message(t("An invalid room has been entered."), 'error');
    $parameter_errors = TRUE;
  }
  
  // Determine if the room and time conflicts with another reservation.
  if (!$parameter_errors) {
    $conflicts_found = _room_reservations_start_conflicts($res->room, $res->date, $res->time);
    if ($conflicts_found) {
      $hours = _room_reservations_hours();
      foreach ($hours as $individual_hour) {
        if ($individual_hour['time'] == $time) {
          $display_time = $individual_hour['display'];
          break;
        }
      }
      $unix_timestamp = strtotime($res->date);
      $display_date = date("F j, Y", $unix_timestamp);
      drupal_set_message(t('Room @room has already been reserved for !date at
        !time.', array(
        '@room' => $res->room,
        '!date' => $display_date,
        '!time' => $display_time,
      )), 'error');
      $parameter_errors = TRUE;
    }
  }
  // Determine the valid lengths of time for this reservation.
  if (!$parameter_errors) {
    $res->validLengths = _room_reservations_valid_lengths($res->room, $res->date, $res->time);
  }
  // Limit the total number of active user reservations.
  $user_reservations = _room_reservations_user_reservations();
  $count = count($user_reservations);
  $max = variable_get('room_reservations_reservations_per_user', 4);
  if ($max) {
    if ($count >= $max) {
      drupal_set_message(t("You have already made the allowable number of reservations."), 'error');
      $parameter_errors = TRUE;
    }
  }
  // Limit the daily number of user reservations.
  if ($res->date) {
    if (_room_reservations_daily_max_exceeded($res->date)) {
      drupal_set_message(t('You have already made the allowable number of reservations for !date.', array('!date' => $res->displayDate)), 'error');
      $parameter_errors = TRUE;
    }
  }
  // Show either errors or form.
  if ($parameter_errors) {
    drupal_goto('room_reservations');
  }
  else {
    $res->length = 0;
    $res->name = '';
    $res->groupSize = 0;
    $res->id = 0;
    $res->textmsg = 0;
    $res->carrier = 0;
    $res->phone = '';
    $output .= drupal_get_form('room_reservations_res_form', 'add', $res);
  }
  return $output;
}

/**
 * View an existing reservation.
 *
 * Determine if the current user has the right to view the requested
 * reservation.
 *
 * @global object $user
 *   Drupal user object.
 *
 * @param int $id
 *   The record key of the requested reservation.
 *
 * @return string
 *   A form used for displaying the reservation.
 */
function room_reservations_res_view($id) {
  $parameter_errors = FALSE;
  $output = '';
  // Current user.
  global $user;
  $logged_in = ($user->uid);
  $user_login_name = ($logged_in) ? $user->name : "";
  if (!$logged_in) {
    drupal_set_message(t("You must be logged in to view reservation details.", 'error'));
    $parameter_errors = TRUE;
  }
  // Get the reservation record.
  $res = new Reservation($id);
  if (!$res->id) {
    drupal_set_message(t("A reservation with this record ID could not be found."), 'error');
    $parameter_errors = TRUE;
  }
  // Format the display date and time; get room capacity.
  if ($res->id) {
    $res->displayTime = '';
    $res->roomCapacity = 0;
    $unix_timestamp = strtotime($res->date);
    $res->displayDate = date("l, F j, Y", $unix_timestamp);
    $hours = _room_reservations_hours();
    foreach ($hours as $time_slot) {
      if ($res->time == $time_slot['time']) {
        $res->displayTime = $time_slot['display'];
        break;
      }
    }
    $rooms = _room_reservations_rooms();
    foreach ($rooms as $room) {
      if ($res->room == $room['name']) {
        $res->roomCapacity = $room['capacity'];
        $res->roomCategory = $room['category'];
        break;
      }
    }
  }
  // Determine if the person requesting the record is either the one who made
  // the reservation Or an employee that has access to the record.
  $current_user_is_owner = FALSE;
  if ($res->id) {
    if ($res->userName == $user_login_name) {
      $current_user_is_owner = TRUE;
    }
    elseif (_room_reservations_full_access()) {
      // Valid user.
    }
    else {
      drupal_set_message(t("You are not allowed to view this reservation record."), 'error');
      $parameter_errors = TRUE;
    }
  }
  // If the reservation is for a future date and/or time, it can be updated by
  // the person who made the reservation.
  $reservation_can_still_be_updated = FALSE;
  if ($res->id) {
    $reservation_date_time = strtotime($res->date . ' ' . drupal_substr($res->time, 0, 2) . ':' . drupal_substr($res->time, 2) . ':00');
    $now = strtotime(date('Y-m-d H:i:s'));
    if ($reservation_date_time > $now) {
      $reservation_can_still_be_updated = TRUE;
    }
  }
  // Set the operation for the form to either 'view' or 'update'.
  $operation = 'view';
  if ($res->id) {
    if (($current_user_is_owner) && ($reservation_can_still_be_updated)) {
      $operation = 'update';
    }
  }
  $res->validLengths = NULL;
  if ((!$parameter_errors) && ($operation == 'update')) {
    $res->validLengths = _room_reservations_valid_lengths($res->room, $res->date, $res->time, $res->id);
  }
  // Show either errors or form.
  if ($parameter_errors) {
    drupal_goto('room_reservations');
  }
  else {
    $res->monthNumber = drupal_substr($res->date, 5, 2);
    $res->day = drupal_substr($res->date, 8);
    $res->year = drupal_substr($res->date, 0, 4);
    $res->textmsg = ($res->textmsg == 'Y') ? 1 : 0;
    $output .= drupal_get_form('room_reservations_res_form', $operation, $res);
  }
  return $output;
}

/**
 * Delete a reservation.
 *
 * Determine if the current user has the right to delete the requested
 * reservation.
 *
 * @global object $user
 *   Drupal user object.
 *
 * @param int $id
 *   The record key of the requested reservation.
 *
 * @return string
 *   A form used for deleting the reservation.
 */
function room_reservations_res_delete($id) {
  $parameter_errors = FALSE;
  $output = '';
  // Current user.
  global $user;
  $logged_in = ($user->uid);
  $user_login_name = ($logged_in) ? $user->name : "";
  if (!$logged_in) {
    drupal_set_message(t("You must be logged in to cancel a reservation."), 'error');
    $parameter_errors = TRUE;
  }
  // Get the reservation record.
  $res = new Reservation($id);
  if (!$res->id) {
    drupal_set_message(t("A reservation with this record ID could not be found."), 'error');
    $parameter_errors = TRUE;
  }
  // Format the display date and time; get room capacity.
  if ($res->id) {
    $res->displayTime = '';
    $res->roomCapacity = 0;
    $unix_timestamp = strtotime($res->date);
    $res->displayDate = date("l, F j, Y", $unix_timestamp);
    $hours = _room_reservations_hours();
    foreach ($hours as $time_slot) {
      if ($res->time == $time_slot['time']) {
        $res->displayTime = $time_slot['display'];
        break;
      }
    }
    $rooms = _room_reservations_rooms();
    foreach ($rooms as $room) {
      if ($res->room == $room['name']) {
        $res->roomCapacity = $room['capacity'];
        $res->roomCategory = $room['category'];
        break;
      }
    }
  }
  // Determine if the person requesting the record is either the one who made
  // the reservation or an employee that has access to the record.
  $current_user_is_owner = FALSE;
  if ($res->id) {
    if ($res->userName == $user_login_name) {
      $current_user_is_owner = TRUE;
    }
    else {
      drupal_set_message(t("This reservation can only be cancelled by the person who made it."), 'error');
      $parameter_errors = TRUE;
    }
  }
  // If the reservation is for a future date and/or time, it can be deleted by
  // the person who made the reservation.
  $reservation_can_still_be_updated = FALSE;
  if ($res->id) {
    $reservation_date_time = strtotime($res->date . ' ' .
      drupal_substr($res->time, 0, 2) . ':' .
    drupal_substr($res->time, 2) . ':00');
    $now = strtotime(date('Y-m-d H:i:s'));
    if ($reservation_date_time > $now) {
      $reservation_can_still_be_updated = TRUE;
    }
    else {
      drupal_set_message(t("This reservation can no longer be cancelled."), 'error');
      $parameter_errors = TRUE;
    }
  }

  // Show either the validation errors or the form.
  if ($parameter_errors) {
    drupal_goto('room_reservations');
  }
  else {
    $res->monthNumber = drupal_substr($res->date, 5, 2);
    $res->day = drupal_substr($res->date, 8);
    $res->year = drupal_substr($res->date, 0, 4);
    $output .= drupal_get_form('room_reservations_cancel_form', $res);
  }
  return $output;
}
