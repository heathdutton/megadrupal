<?php
/**
 * @file
 * Single reservation view functionality.
 */

/**
 * Form constructor for the reservation form.
 *
 * @param string $operation
 *   The operation being performed using the form:  add, update, view.
 * @param Reservation $res
 *   A reservation object that contains all the details related to the
 *   reservation being made, updated, or viewed.
 */
function room_reservations_res_form(&$form_state, $operation, $res) {
  $reserve_form_instructions_text = check_markup(_room_reservations_get_variable('reserve_form_instructions'));
  $room_reservations_group_size = variable_get('room_reservations_group_size', 0);
  $sms_option = _room_reservations_get_variable('sms_option');
  if (($operation == 'add') || ($operation == 'update')) {
    // Options for number of persons in group, from 2 to room capacity.
    $number_persons_options = array();
    for ($x = 2; $x <= $res->roomCapacity; $x++) {
      $number_persons_options[$x] = $x;
    }
    // Valid lengths of time.
    $length_options = array();
    foreach ($res->validLengths as $valid_length) {
      if ($valid_length['is_valid']) {
        $length_option = $valid_length['length'];
        $length_options[$length_option] = $length_option . ' ' . t('minutes');
      }
    }
  }
  if (drupal_strlen($res->phone) == 10) {
    $display_phone = drupal_substr($res->phone, 0, 3) . '-' .
      drupal_substr($res->phone, 3, 3) . '-' .
      drupal_substr($res->phone, 6);
  }
  else {
    $display_phone = '';
  }
  // Form definition.
  if ($operation === 'add') {
    $form['instructions'] = array(
      '#title' => t('Patron'),
      '#value' => t($reserve_form_instructions_text),
      '#weight' => 0,
    );
  }
  if ($operation === 'update') {
    $form['delete_top'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel My Reservation'),
      '#weight' => 4,
    );
  }
  $form['details'] = array(
    '#title' => t('Details'),
    '#type' => 'fieldset',
    '#weight' => 5,
  );
  $form['details']['room'] = array(
    '#title' => t('Room'),
    '#type' => 'item',
    '#value' => t('@room (capacity: !capacity persons)', 
      array('@room' => $res->room, '!capacity' => $res->roomCapacity)),
    '#weight' => 20,
  );
  $form['details']['display_date'] = array(
    '#title' => t('Date'),
    '#type' => 'item',
    '#value' => t($res->displayDate),
    '#weight' => 25,
  );
  $form['details']['time'] = array(
    '#title' => t('Time'),
    '#type' => 'item',
    '#value' => t($res->displayTime),
    '#weight' => 30,
  );
  if ($operation == 'add') {
    $form['details']['length'] = array(
      '#title' => t('Length'),
      '#type' => 'radios',
      '#options' => $length_options,
      '#required' => TRUE,
      '#weight' => 35,
    );
  }
  elseif ($operation == 'update') {
    $form['details']['length'] = array(
      '#title' => t('Length'),
      '#type' => 'radios',
      '#options' => $length_options,
      '#default_value' => $res->length,
      '#required' => TRUE,
      '#weight' => 35,
    );
  }
  else {
    $form['details']['length'] = array(
      '#title' => t('Length'),
      '#type' => 'item',
      '#value' => $res->length,
      '#weight' => 35,
    );
  }
  if (($operation == 'add') || ($operation == 'update')) {
    $form['details']['group_name'] = array(
      '#title' => t('Group name'),
      '#type' => 'textfield',
      '#description' => t('Identifies your group on the reservation calendar.
        14 character limit.'),
      '#default_value' => $res->name,
      '#maxlength' => 14,
      '#size' => 14,
      '#required' => TRUE,
      '#weight' => 40,
    );
  }
  else {
    $form['details']['group_name'] = array(
      '#title' => t('Group name'),
      '#type' => 'item',
      '#value' => $res->name,
      '#weight' => 40,
    );
  }
  if ($room_reservations_group_size) {
    if ($operation == 'add') {
      $form['details']['group_size'] = array(
        '#title' => t('Size of the group'),
        '#type' => 'radios',
        '#options' => $number_persons_options,
        '#required' => TRUE,
        '#weight' => 45,
      );
    }
    elseif ($operation == 'update') {
      $form['details']['group_size'] = array(
        '#title' => t('Size of the group'),
        '#type' => 'radios',
        '#options' => $number_persons_options,
        '#default_value' => $res->groupSize,
        '#required' => TRUE,
        '#weight' => 45,
      );
    }
    else {
      $form['details']['group_size'] = array(
        '#title' => t('Size of the group'),
        '#type' => 'item',
        '#value' => $res->groupSize,
        '#weight' => 45,
      );
    }
  }
  if (_room_reservations_full_access()) {
    $form['details']['user_name'] = array(
      '#title' => t('User name'),
      '#type' => 'item',
      '#value' => $res->userName,
      '#weight' => 55,
    );
  }
  $form['reminders'] = array(
    '#title' => t('Reminders'),
    '#type' => 'fieldset',
    '#weight' => 60,
  );
  if ($sms_option) {
    if (($operation == 'add') || ($operation == 'update')) {
      $form['reminders']['textmsg'] = array(
        '#title' => t('I want to receive my confirmation and reminder as a text
          message.'),
        '#type' => 'checkbox',
        '#return_value' => 1,
        '#default_value' => $res->textmsg,
        '#weight' => 65,
      );
      $form['reminders']['textmsg_fields_start'] = array(
        '#value' => '<div id="txtmsg-fields">',
        '#weight' => 66,
      );
      $form['reminders']['phone'] = array(
        '#title' => t('Phone number'),
        '#type' => 'textfield',
        '#maxlength' => 15,
        '#size' => 15,
        '#default_value' => $display_phone,
        '#weight' => 70,
      );
      $carriers = _room_reservations_carriers();
      $form['reminders']['carrier'] = array(
        '#title' => t('Carrier'),
        '#type' => 'select',
        '#options' => $carriers,
        '#default_value' => $res->carrier,
        '#weight' => 75,
      );
      $form['reminders']['charges'] = array(
        '#value' => '<div class="form-item">' .
        t('NOTE:  Carrier charges may apply if your cell phone service
          plan does not include free text messaging.') . '</div>',
        '#weight' => 80,
      );
      $form['reminders']['testmsg_fields_end'] = array(
        '#value' => '</div>',
        '#weight' => 85,
      );
    }
  }
  $reminder_columns = 60;
  if (($operation == 'add') || ($operation == 'update')) {
    $form['reminders']['email_addresses'] = array(
      '#title' => t('Email Addresses'),
      '#type' => 'textarea',
      '#default_value' => $res->emailAddresses,
      '#description' => t('Reservation confirmation and reminder will be sent
        to any email addresses entered here.  Separate addresses with a
        comma.'),
      '#cols' => $reminder_columns,
      '#rows' => 1,
      '#weight' => 90,
    );
  }
  else {
    $form['reminders']['email_addresses'] = array(
      '#title' => t('Email Addresses'),
      '#type' => 'item',
      '#value' => $res->emailAddresses,
      '#weight' => 90,
    );
  }
  if ($operation == 'add') {
    $form['add'] = array(
      '#type' => 'submit',
      '#value' => t('Save My Reservation'),
      '#weight' => 100,
    );
  }
  elseif ($operation == 'update') {
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update My Reservation'),
      '#weight' => 100,
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel My Reservation'),
      '#weight' => 101,
    );
  }
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $res->id,
  );
  $form['month_number'] = array(
    '#type' => 'value',
    '#value' => $res->monthNumber,
  );
  $form['day'] = array(
    '#type' => 'value',
    '#value' => $res->day,
  );
  $form['year'] = array(
    '#type' => 'value',
    '#value' => $res->year,
  );
  $form['time_number'] = array(
    '#type' => 'value',
    '#value' => $res->time,
  );
  $form['room_name'] = array(
    '#type' => 'value',
    '#value' => $res->room,
  );
  $form['category'] = array(
    '#type' => 'value',
    '#value' => $res->category,
  );
  $form['yyyymmdd'] = array(
    '#type' => 'value',
    '#value' => $res->date,
  );
  $form['user_name_hold'] = array(
    '#type' => 'value',
    '#value' => $res->userName,
  );
  return $form;
}

/**
 * Form validation for the reservation form.
 *
 * @param string $form_id
 *   Drupal form id.
 * @param array $form_state
 *   Drupal form state object.
 */
function room_reservations_res_form_validate($form_id, &$form_state) {
  // When a user is filling out the form to reserve a room, it is possible that
  // before she can submit the form, someone else will complete a reservation
  // for that same room, same date, and at least part of the same time period.
  // If that should occur, and the user has requested a length of time that
  // is no longer available, Drupal automatically creates the following error:
  // 'An illegal choice has been detected. Please contact the site
  // administrator.'  Since this message is somewhat alarming to the user and
  // not very informative, it is replaced in this function with a more
  // descriptive message.
  $errors = drupal_get_messages();
  foreach ($errors as $type => $id) {
    foreach ($id as $message) {
      // Loop through individual messages, looking for ones to remove or
      // replace.
      if (room_reservations_rewrite_error($message) === FALSE) {
        drupal_set_message($message, $type);
      }
      elseif (room_reservations_rewrite_error($message) !== TRUE) {
        drupal_set_message(room_reservations_rewrite_error($message), $type);
      }
    }
  }
  // Validate email addresses.
  $email_addresses = trim($form_state['values']['email_addresses']);
  if (drupal_strlen($email_addresses)) {
    $valid_addresses = TRUE;
    $list_items = explode(',', $email_addresses);
    foreach ($list_items as $address) {
      if (!valid_email_address(trim($address))) {
        $valid_addresses = FALSE;
      }
    }
    if (!$valid_addresses) {
      form_set_error('email_addresses', t('Email Addresses contains an invalid
        email address.'));
    }
  }
  if ((!drupal_strlen($email_addresses)) && 
    (!$form_state['values']['textmsg'])) {
    $default_sms_option = _room_reservations_get_variable('sms_option');
    if ($default_sms_option) {
      form_set_error('email_addresses', t('Your email address or phone number is
        required.'));
    }
    else {
      form_set_error('email_addresses', t('Your email address is
        required.'));
    }
  }

  // Text message information.
  if ($form_state['values']['textmsg']) {
    $phone = $form_state['values']['phone'];
    $phone_number = preg_replace('/[^\d]/', '', $phone);
    if (drupal_strlen($phone_number) <> 10) {
      form_set_error('phone', t('Ten digit phone number is required
        when receiving confirmation by text message.'));
    }
    if (!$form_state['values']['carrier']) {
      form_set_error('carrier', t('Carrier is required
        when receiving confirmation by text message.'));
    }
  }

}

/**
 * Form submission for the reservation form.
 *
 * @param string $form_id
 *   Drupal form id.
 * @param array $form_state
 *   Drupal form state object.
 */
function room_reservations_res_form_submit($form_id, &$form_state) {
  $id = $form_state['values']['id'];
  $month_number = $form_state['values']['month_number'];
  $day = $form_state['values']['day'];
  $year = $form_state['values']['year'];
  $time = $form_state['values']['time_number'];
  $length = $form_state['values']['length'];
  $room = $form_state['values']['room_name'];
  $group_name = $form_state['values']['group_name'];
  $group_size = $form_state['values']['group_size'];
  $user_name = $form_state['values']['user_name_hold'];
  $category = $form_state['values']['category'];
  $yyyymmdd = $form_state['values']['yyyymmdd'];
  $email_addresses = trim($form_state['values']['email_addresses']);
  $textmsg_checkbox = $form_state['values']['textmsg'];
  $carrier = $form_state['values']['carrier'];
  $phone_full = $form_state['values']['phone'];
  $phone = preg_replace('/[^\d]/', '', $phone_full);
  if ($textmsg_checkbox) {
    $textmsg = 'Y';
  }
  else {
    $textmsg = 'N';
    $carrier = 0;
    $phone = '';
  }
  if ($time < 1000) {
    $time = str_pad($time, 4, '0', STR_PAD_LEFT);
  }
  $hours = _room_reservations_hours();
  foreach ($hours as $individual_hour) {
    if ($individual_hour['time'] == $time) {
      $display_hour = $individual_hour['display'];
      break;
    }
  }
  $day_of_week = date("l", strtotime($yyyymmdd));
  $month_name = date("F", strtotime($yyyymmdd));
  // Add.
  if ($form_state['clicked_button']['#value'] == t('Save My Reservation')) {
    $result = db_query("INSERT INTO {room_reservations} (date, time, length, room, name, group_size, user_name, email_addresses, create_date, textmsg, carrier, phone )
      VALUES ('%s', '%s', %d, '%s', '%s', %d, '%s', '%s', '%s', '%s', %d, '%s')",
      $yyyymmdd, $time, $length, $room, $group_name, $group_size, $user_name, $email_addresses, date('Y-m-d H:i:s', REQUEST_TIME), $textmsg, $carrier, $phone);
    // Send a confirmation email or text message.
    $id = db_last_insert_id('room_reservations', 'id');
    $params = array(
      'room' => $room,
      'month' => $month_name,
      'month_number' => $month_number,
      'day' => $day,
      'day_of_week' => $day_of_week,
      'time' => $display_hour,
      'minutes' => $length,
      'name' => $group_name,
      'id' => $id,
      'carrier' => $carrier,
      'phone' => $phone,
    );
    $from = _room_reservations_get_variable('from_address');
    if ($result) {
      drupal_set_message(t('Your group study room reservation has been made.'));
      // Send an email to each person in the group.  If the person is the one
      // who made the reservation, send the confirmation message.  Otherwise,
      // send the notification message.
      if (drupal_strlen($email_addresses)) {
        $to_addresses = explode(',', $email_addresses);
        foreach ($to_addresses as $to_address) {
          $to_address = trim($to_address);
          $pos = strpos($to_address, $user_name);
          if ($pos === FALSE) {
            $key = 'notification';
          }
          else {
            $key = 'confirmation';
          }
          $response = drupal_mail(
            'room_reservations', $key, $to_address, language_default(), 
            $params, $from, TRUE);
        }
      }
      // If requested, send a text message confirmation to the person who made
      // the reservation.
      if ($textmsg == 'Y') {
        _room_reservations_send_sms('confirmation', $params);
      }
    }
    else {
      drupal_set_message(t('Your group study room reservation could not be
         made.'), 'error');
    }
    $redirect = "room_reservations/" . $month_number . "/" . $day . "/" . $category;
    $form_state['redirect'] = $redirect;
  }
  // Update.
  elseif ($form_state['clicked_button']['#value'] ==
    t('Update My Reservation')) {
    $result = db_query("
      UPDATE {room_reservations}
      SET length = %d,
      name = '%s',
      group_size = %d,
      email_addresses = '%s',
      update_date = '%s',
      textmsg = '%s',
      carrier = %d,
      phone = '%s'
      WHERE id = %d", $length, $group_name, $group_size, $email_addresses, date('Y-m-d H:i:s', REQUEST_TIME), $textmsg, $carrier, $phone, $id);
    if ($result) {
      drupal_set_message(t('Your group study room reservation has been
         updated.'));
    }
    else {
      drupal_set_message(t('Your group study room reservation could not be
         updated.'), 'error');
    }
    $redirect = "room_reservations/" . $month_number . "/" . $day . "/" . $category;
    $form_state['redirect'] = $redirect;
  }
  elseif ($form_state['clicked_button']['#value'] == t('Cancel My Reservation')) {
    $redirect = "room_reservations/delete/" . $id;
    $form_state['redirect'] = $redirect;
  }
}

/**
 * Form constructor for the delete reservation form.
 *
 * @param Reservation $res
 *   A reservation object that contains all the details related to the
 *   reservation being deleted.
 */
function room_reservations_cancel_form(&$form_state, $res) {
  $room_capacity = $res->roomCapacity;
  // Form definition.
  $form['room'] = array(
    '#title' => t('Room'),
    '#type' => 'item',
    '#value' => t('@room (capacity: !capacity persons)', array('@room' => $res->room, '!capacity' => $room_capacity)),
    '#weight' => 10,
  );
  $form['display_date'] = array(
    '#title' => t('Date'),
    '#type' => 'item',
    '#value' => t($res->displayDate),
    '#weight' => 20,
  );
  $form['time'] = array(
    '#title' => t('Time'),
    '#type' => 'item',
    '#value' => t($res->displayTime),
    '#weight' => 30,
  );
  $form['length'] = array(
    '#title' => t('Length'),
    '#type' => 'item',
    '#value' => t('!length minutes', array('!length' => $res->length)),
    '#weight' => 40,
  );
  $form['group_name'] = array(
    '#title' => t('Group name'),
    '#type' => 'item',
    '#value' => $res->name,
    '#weight' => 50,
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $res->id,
  );
  $form['month_number'] = array(
    '#type' => 'value',
    '#value' => $res->monthNumber,
  );
  $form['day'] = array(
    '#type' => 'value',
    '#value' => $res->day,
  );
  $form['category'] = array(
    '#type' => 'value',
    '#value' => $res->roomCategory,
  );
  $form['user_name_hold'] = array(
    '#type' => 'value',
    '#value' => $res->userName,
  );
  return confirm_form($form, 
    t('Cancel'), 'room_reservations/' . $res->monthNumber . '/' . $res->day . '/' . $res->roomCategory, 
    t('Are you sure you want to cancel this reservation?  This action cannot be undone.'), 
    t('Cancel My Reservation'), 
    t("Don't Cancel My Reservation")
  );
}

/**
 * Form submission for the delete reservation form.
 *
 * @param string $form_id
 *   Drupal form id.
 * @param array $form_state
 *   Drupal form state object.
 */
function room_reservations_cancel_form_submit($form_id, &$form_state) {
  $id = $form_state['values']['id'];
  $month_number = $form_state['values']['month_number'];
  $day = $form_state['values']['day'];
  $category = $form_state['values']['category'];
  // Delete.
  if ($form_state['clicked_button']['#value'] == t('Cancel My Reservation')) {
    $result = db_query("UPDATE {room_reservations} SET deleted = '%s', update_date = '%s' WHERE id = %d", 'Y', date('Y-m-d H:i:s', REQUEST_TIME), $id);
    if ($result) {
      drupal_set_message(t('Your group study room reservation has been cancelled.'));
    }
    else {
      drupal_set_message(t('Your group study room reservation could not be cancelled.'), 'error');
    }
  }
  $redirect = "room_reservations/" . $month_number . "/" . $day . "/" . $category;
  $form_state['redirect'] = $redirect;
}
