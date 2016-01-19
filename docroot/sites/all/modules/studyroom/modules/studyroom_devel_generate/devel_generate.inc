<?php
/**
 * @file
 * Provides the functions used to actually generate content.
 */

/**
 * The main API function for generating spaces.
 */
function studyroom_devel_generate_spaces($form_state) {
  if (!empty($form_state['values']['kill_spaces'])) {
    studyroom_devel_generate_spaces_kill($form_state['values']);
  }

  if (count($form_state['values']['locations'])) {
    module_load_include('inc', 'devel_generate', 'devel_generate');
    $start = time();
    for ($i = 1; $i <= $form_state['values']['num_spaces']; $i++) {
      studyroom_devel_generate_spaces_add_space($form_state['values']);
      if (function_exists('drush_log') && $i % drush_get_option('feedback', 1000) == 0) {
        $now = time();
        drush_log(dt('Completed !feedback spaces (!rate spaces/min)',
          array(
            '!feedback' => drush_get_option('feedback', 1000),
            '!rate' => (drush_get_option('feedback', 1000) * 60) / ($now - $start))),
            'ok');
        $start = $now;
      }
    }
  }

  devel_generate_set_message(format_plural($form_state['values']['num_spaces'], '1 space created.', 'Finished creating @count spaces'));
}

/**
 * Handle deleting existing spaces.
 *
 * This function is used by both batch and non-batch code branches.
 *
 * @param array $values
 *   array of options obtained from commerce_devel_generate_products_form.
 *
 * @see studyroom_devel_generate_spaces_form()
 */
function studyroom_devel_generate_spaces_kill($values) {
  $results = db_select('studyroom_space', 's')
              ->fields('s', array('space_id'))
              ->condition('type', $values['locations'], 'IN')
              ->execute();
  foreach ($results as $result) {
    $space_ids[] = $result->space_id;
  }

  if (!empty($space_ids)) {
    studyroom_space_delete_multiple($space_ids);
    drupal_set_message(t('Deleted %count spaces.', array('%count' => count($space_ids))));
  }
}

/**
 * Create a single space.
 *
 * This function is used by both batch and non-batch code branches.
 */
function studyroom_devel_generate_spaces_add_space(&$results) {
  $type = array_rand($results['locations']);
  $space = entity_create('studyroom_space', array('type' => $type));

  $space->label = devel_create_greeking(mt_rand(2, $results['title_length']), TRUE);
  devel_generate_set_language($results, $space);
  $space->capacity = mt_rand(1, $results['max_capacity']);
  $space->devel_random_time = $results['random_times'];

  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($space, 'studyroom_space', $type);

  entity_save('studyroom_space', $space);
}

/**
 * The main API function for generating reservations.
 */
function studyroom_devel_generate_reservations($form_state) {
  if (!empty($form_state['values']['kill_reservations'])) {
    studyroom_devel_generate_reservations_kill($form_state['values']);
  }

  $count = 0;
  // Normal reservations.
  if (count($form_state['values']['types'])) {
    // Get list of users we will use.
    studyroom_devel_generate_pre_users($form_state['values']);
    module_load_include('inc', 'devel_generate');
    $start = time();
    for ($i = 1; $i <= $form_state['values']['num_reservations']; $i++) {
      $status = studyroom_devel_generate_reservations_add_reservation($form_state['values']);
      if ($status) {
        ++$count;
      }

      if (function_exists('drush_log') && $i % drush_get_option('feedback', 1000) == 0) {
        $now = time();
        drush_log(dt('Completed !feedback reservations (!rate reservations/min)',
          array(
            '!feedback' => drush_get_option('feedback', 1000),
            '!rate' => (drush_get_option('feedback', 1000) * 60) / ($now - $start))),
            'ok');
        $start = $now;
      }
    }
  }

  devel_generate_set_message(format_plural($count, '1 reservation created.', 'Finished creating @count reservations'));
}

/**
 * Handle deleting existing reservations.
 *
 * This function is used by both batch and non-batch code branches.
 *
 * @param array $values
 *   array of options obtained from commerce_devel_generate_products_form.
 */
function studyroom_devel_generate_reservations_kill($values) {
  $results = db_select('studyroom_reservation', 'r')
              ->fields('r', array('reservation_id'))
              ->condition('type', $values['types'], 'IN')
              ->execute();
  foreach ($results as $result) {
    $reservation_ids[] = $result->reservation_id;
  }

  if (!empty($reservation_ids)) {
    studyroom_reservation_delete_multiple($reservation_ids);
    drupal_set_message(t('Deleted %count %type reservations.', array(
      '%count' => count($reservation_ids),
      '%type' => implode(', ', $values['types']),
      )));
  }
}

/**
 * Create a single reservation.
 *
 * This function is used by both batch and non-batch code branches.
 */
function studyroom_devel_generate_reservations_add_reservation(&$results) {
  $type = array_rand($results['types']);
  // Get spaces allowed to use this type.
  // @todo Move to studyroom_devel_generate_reservations_form_submit().
  if (!isset($results['allowed_spaces'][$type])) {
    $results['allowed_spaces'][$type] = studyroom_space_by_resv_type($type);
  }

  $spaces = array_intersect(array_filter($results['spaces']), $results['allowed_spaces'][$type]);
  if (empty($spaces)) {
    return FALSE;
  }

  $increment = variable_get('studyroom_time_increment', '15');
  $reservation = entity_create('studyroom_reservation', array('type' => $type));
  devel_generate_set_language($results, $reservation);

  $users = $results['users'];
  $reservation->uid = $users[array_rand($users)];
  $reservation->space_id = $spaces[array_rand($spaces)];

  // Get space entity.
  $space = studyroom_space_load($reservation->space_id);

  // Get reservation type object for min/max duration values.
  $studyroom = new Studyroom();
  $duration = mt_rand($studyroom->getMinDuration($space), $studyroom->getMaxDuration($space));
  $reservation->duration = round($duration / ($increment * 60)) * ($increment * 60);
  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($reservation, 'studyroom_reservation', $type);

  $lang = $reservation->language;
  // Fix values in generated fields.
  $reservation->field_reservation_occupancy[$lang][0]['value'] = mt_rand(1, 5);

  // Generate random date part using now + random time_range value.
  $date = new DateObject();
  $date->limitGranularity(array('year', 'month', 'day', 'hour', 'minute'));
  $seconds = mt_rand(3600, $results['time_range']);
  date_modify($date, "+{$seconds} seconds");
  date_increment_round($date, $increment);

  $valid_hours = _studyroom_devel_generate_check_date($space, $date, $results['time_range'], $increment);
  $start_time = date_format_date($valid_hours['start_time'], 'custom', 'Hi');
  $end_time = date_format_date($valid_hours['end_time'], 'custom', 'Hi');
  $time = date_format_date($date, 'custom', 'Hi');

  if ($time < $start_time) {
    $add = (substr($start_time, 0, 2) - substr($time, 0, 2)) + mt_rand(1, 4);
    date_modify($date, "+$add hours");
  }

  // Check time w/o duration.
  if ($time > $end_time) {
    // If large duration, reduce by half to prevent being to long.
    if ($reservation->duration > 7200) {
      $reservation->duration = $reservation->duration / 2;
    }
    $sub = intval((($time - $end_time) / 100) + (mt_rand(2, 6) + ($reservation->duration / 3600)));
    date_modify($date, "-{$sub} hours");
  }

  // Create time plus duration to compare.
  $date2 = clone($date);
  date_modify($date2, "+{$reservation->duration} seconds");
  $time = date_format_date($date2, 'custom', 'Hi');
  if ($time > $end_time) {
    // Really should never get here, but safety check/fix.
    $sub = intval((($time - $end_time) / 100) + (mt_rand(2, 4) + ($reservation->duration / 3600)));
    date_modify($date, "-{$sub} hours");
  }

  // Drop seconds from date.
  $date = new DateObject(date_format_date($date, 'custom', 'Y-m-d H:i'));
  $reservation->field_reservation_datetime[$lang][0]['value'] = date_format($date, DATE_FORMAT_DATETIME);

  $reservation->start_time = $date;
  $reservation->end_time = clone $date;
  $reservation->end_time = $reservation->end_time->add(new DateInterval('PT' . $reservation->duration . 'S'));

  if ($reservation->checkConflict()) {
    return FALSE;
  }

  $reservation->message = FALSE;
  entity_save('studyroom_reservation', $reservation);
  return TRUE;
}

/**
 * Function to get valid hours per space per day.
 *
 * Call's itself until day with valid hours can be found.
 */
function _studyroom_devel_generate_check_date($space, &$date, $time_range, $increment) {
  $valid_hours = $space->hoursPerDate($date);

  if ($valid_hours === FALSE) {
    $seconds = mt_rand(3600, $time_range);
    date_modify($date, "+{$seconds} seconds");
    date_increment_round($date, $increment);
    return _studyroom_devel_generate_check_date($space, $date, $time_range, $increment);
  }

  return $valid_hours;
}

/**
 * The main API function for generating closed reservations.
 */
function studyroom_devel_generate_closed($values) {
  if ($values['kill_reservations']) {
    studyroom_devel_generate_closed_kill();
  }

  module_load_include('inc', 'devel_generate', 'devel_generate');
  $count = 0;
  $start = time();
  for ($i = 1; $i <= $values['num_reservations']; $i++) {
    $status = studyroom_devel_generate_closed_add($values);
    if ($status) {
      ++$count;
    }

    if (function_exists('drush_log') && $i % drush_get_option('feedback', 1000) == 0) {
      $now = time();
      drush_log(dt('Completed !feedback reservations (!rate reservations/min)',
        array(
          '!feedback' => drush_get_option('feedback', 1000),
          '!rate' => (drush_get_option('feedback', 1000) * 60) / ($now - $start))),
          'ok');
      $start = $now;
    }
  }

  devel_generate_set_message(format_plural($count, '1 closed reservation created.', 'Finished creating @count closed reservations'));
}

/**
 * Handle deleting existing closed reservations.
 *
 * This function is used by both batch and non-batch code branches.
 */
function studyroom_devel_generate_closed_kill() {
  $results = db_select('studyroom_closed', 'c')
              ->fields('c', array('studyroom_closed_id'))
              ->execute()->fetchAllAssoc('studyroom_closed_id');

  $studyroom_closed_id = array_keys($results);

  if (!empty($studyroom_closed_id)) {
    studyroom_closed_delete_multiple($studyroom_closed_id);
    drupal_set_message(t('Deleted %count reservations.', array(
      '%count' => count($studyroom_closed_id),
      )));
  }
}

/**
 * Create a single closed reservation.
 *
 * This function is used by both batch and non-batch code branches.
 */
function studyroom_devel_generate_closed_add(&$results) {
  $spaces = array_filter($results['spaces']);
  if (empty($spaces)) {
    return FALSE;
  }

  $reservation = entity_create('studyroom_closed', array('type' => 'closed'));
  devel_generate_set_language($results, $reservation);
  $reservation->name = devel_create_greeking(mt_rand(2, 4), TRUE);

  if (count($spaces) == count($results['spaces'])) {
    if (mt_rand(0, 1)) {
      $reservation->spaces_all = TRUE;
    }
    else {
      $spaces = array_rand($spaces, mt_rand(1, count($spaces)));
    }
  }
  $reservation->spaces = $spaces;

  module_load_include('inc', 'devel_generate', 'devel_generate.fields');
  devel_generate_fields($reservation, 'studyroom_closed', 'studyroom_closed');

  $lang = $reservation->language;
  foreach ($reservation->field_studyroom_closed_dates[$lang] as $field => &$value) {
    if (mt_rand(0, 1)) {
      $date = $value['value'];
      $value['all_day'] = TRUE;
      $value['value'] = $value['value2'] = substr($date, 0, 10) . ' 00:00:00';
    }
  }

  $reservation->message = FALSE;
  entity_save('studyroom_reservation', $reservation);
  return TRUE;
}
