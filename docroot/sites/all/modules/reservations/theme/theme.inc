<?php

/**
 * @file
 * Reservations - Managed Equipment Reservation Checkout and Inventory
 */

function template_preprocess_reservations_conflict_grid(&$variables) {

  $type = $variables['type'];
  $title = $variables['title'];
  $start = $variables['start'];
  $end = $variables['end'];
  $nid = $variables['nid'];
  $reservation_vid = $variables['reservation_vid'];

  $hours = array();
  if (user_access('create reservations outside hours of operation')) {
    $adminhours = explode('-', variable_get('reservations_hours_admin', '07:00-23:00'));

    for ($day = 0; $day < 7; $day++) {
      $hours[$day]['open'] = $adminhours[0];
      $hours[$day]['close'] = $adminhours[1];
    }
  }
  else {
    $hours = reservations_load_hours_of_operation(NULL, TRUE);
  }

  $timezone = variable_get('date_default_timezone', 'UTC');
  // Convert the start and end dates to the site timezone.
  /*
  $start = date_create($start, "UTC");
  $end = date_create($end, "UTC");
  date_timezone_set($start, timezone_open($timezone));
  date_timezone_set($end, timezone_open($timezone));
   */
  $start_object       = reservations_create_local_date_object($start);
  $end_object         = reservations_create_local_date_object($end);

  // Expand our search to the entire day in the users timezone.
  $start_day = date_format($start_object, 'w');
  $end_day = date_format($end_object, 'w');
  $start_date = $dates['value']['date'] = date_format($start_object, 'Y-m-d');
  $dates['value']['time'] = $hours[$start_day]['open'];
  $end_date = $dates['value2']['date'] = date_format($end_object, 'Y-m-d');
  $dates['value2']['time'] = $hours[$end_day]['close'];

  $dates = reservations_convert_date_popup($dates, 'Y-m-d H:i');

  $start_mysql = $dates['value'];
  $end_mysql = $dates['value2'];

  $reservations = reservations_load_reservations_for_node_in_timespan($nid, $type, $start_mysql, $end_mysql, $reservation_vid);

  $reservations_by_date = array();

  //for each day we are looking at.
  while ($start_date <= $end_date) {

    $day = date('w', strtotime($start_date));

    // loop through all the items.
    if (!empty($hours[$day])) {
      //$reservations_by_date[$start_date] = array();
      foreach ($reservations as $item => $values) {

        $reservations_by_date[$start_date][$item] = array();

        $time = strtotime($start_date . ' ' . $hours[$day]['open']);
        $close = strtotime($start_date . ' ' . $hours[$day]['close']);

        // all the reservations for that item.
        foreach ($values as $reservation) {

          // Convert to site timezone.
          $tz_start_date       = reservations_create_local_date_object($reservation->field_reservations_date_value);
          $tz_end_date         = reservations_create_local_date_object($reservation->field_reservations_date_value2);

          $tz_start_date = $tz_start_date->format('U');
          $tz_end_date = $tz_end_date->format('U');

          $tz_start_date = $tz_start_date < $close ? $tz_start_date : $close;
          $tz_end_date   = $tz_end_date < $close ? $tz_end_date : $close;
          if ($tz_end_date <= $time) {
            continue;
          }
          while ($time < $tz_start_date) {
            $reservations_by_date[$start_date][$item][date('H:i', $time)] = array(
              'class' => 'available',
              'data' => '',
            );
            $time += 15 * 60;
          }
          while ($time >= $tz_start_date && $time < $tz_end_date) {
            $reservations_by_date[$start_date][$item][date('H:i', $time)] = array(
              'class' => 'unavailable',
              'data' => $reservation->nid,
            );
            $time += 15 * 60;
          }
          $time = $tz_end_date;
        }
        while ($time < $close) {
          $reservations_by_date[$start_date][$item][date('H:i', $time)] = array(
            'class' => 'available',
            'data' => '',
          );
          $time += 15 * 60;
        }
      }
    }
    $start_date = date('Y-m-d', strtotime($start_date . ' +1 day'));
  }

  $variables['reservations_by_date'] = $reservations_by_date;

  $pretty_hours = array();
  
  foreach ($reservations_by_date as $date => $values) {
    $day = date('w', strtotime($date));
    $time = $hours[$day]['open'];

    while ($time < $hours[$day]['close']) {
      $pretty_hours[$date][] = date('ga', strtotime($time));
      $time = date('H:i', strtotime($time . ' +1 hour'));
      //avoids overflow to next day
      if($time < '01:00') {
        break;
       }
    }
  }

  $variables['pretty_hours'] = $pretty_hours;
}

/**
 * Theme the reservation form for choices.
 */
function theme_reservations_choices($variables) {
  $form = $variables['form'];
  // Change the button title to reflect the behavior when using JavaScript.
  //drupal_add_js('if (Drupal.jsEnabled) { $(document).ready(function() { $("#edit-reservations-more").val("'. t('Add another item') {  .'"); }); }', 'inline');}

  $output = '';
  $output .= drupal_render_children($form);
  return $output;
}
/**
 * Builds the reserved items table.
 */
function theme_reservations_build_reservation_table_display($variables) {

  $form = $variables['form'];

  $output = theme('status_messages');
  $header = $form['#header'];
  // fix to deal with missing header col
  if (!isset($header[1])) {
    $header[1] = NULL;
  }

  $node   = $form['#node'];
  $rows   = array();
  foreach (element_children($form) as $did) {

    if (strstr($did, 'form_')) {
      continue;
    }
    $values = $form[$did];

    $values['display_name']['#type'] = 'markup';
    $name = drupal_render($values['display_name']);

    if ($values['reservations_item_nid']['#type'] == 'value') {
      $item = ($values['display_item_title']['#markup']) ? drupal_render($values['display_item_title']) : t('SPECIFIC ITEM NOT SELECTED');
    }
    else {
      $item = drupal_render($values['reservations_item_nid']);
    }

    $row = array(
      $item,
      $name,
    );

    foreach ($form['#header'] as $index => $value) {
      if ($value == t('Accessories')) {
        $row[$index] = drupal_render($values['accessories']);
      }
      elseif ($value == t('Operations')) {
        $row[$index] = drupal_render($values['ops']);
      }
    }

    $rows[$did] = $row;
  }

  if (!empty($rows)) {
    if (isset($variables['form']['#action'])) {
      $table_caption = in_array((int) $node->reservations_reservation_status, array(RESERVATIONS_STATUS_CHECKED_OUT, RESERVATIONS_STATUS_CHECKED_IN)) ? t('Checked out items') : t('Reserved Items');
    }
    else {
      $table_caption = in_array((int) $node->reservations_reservation_status, array(RESERVATIONS_STATUS_CHECKED_OUT, RESERVATIONS_STATUS_CHECKED_IN)) ? t('Checked out items') : t('Select items to reserve');
    }

    $output .= '<div class="existing-items-table-header">' . $table_caption . ':</div>';
    $output .= theme('table', array('header' => $header, 'rows' => $rows));
  }
  $output .= theme('status_messages');

  return $output;
}


