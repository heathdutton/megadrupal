<?php

/*
 * @file hotel_booking.calendars.inc
 * Provides availability functions for hotel booking
 * @copyright Copyright(c) 2010 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands leerowlands at rowlands-bcs dot com
 *
 */


/**
 * Hotel Availability Calendars page
 * @param $form_state array std fapi form state var
 * @param $node object node object
 * @param $year int year to display
 * @param $month int month to display
 */
function hotel_booking_availability_form($form, $form_state, $node, $year = NULL, $month = NULL) {
  if (!$node || $node->type != 'hotel_room_type') {
    return drupal_not_found();
  }
  if ($node->tnid && $node->nid != $node->tnid) {
    drupal_set_message(t('This room is part of a translation set, availability can only be set against the
                         translation master, you have been redirected to the translation master to edit the availability'));
    drupal_goto('node/'. $node->tnid .'/availability');
  }
  //set up defaults
  if (!$year) {
    $year = date('Y');
  }
  if (!$month) {
    $month = date('m');
  }

  if (!is_numeric($year) || !is_numeric($month)) {
    return drupal_not_found();
  }

  $ref_dt = new DateObject("$year-$month-01", date_default_timezone(), DATE_FORMAT_ISO);

  drupal_set_title(t(
    'Availability for !name in !month-name '. $ref_dt->format('F') .' !year',
    array(
      '!name' => $node->title,
      '!year' => $year,
      '!month-name' => '')
    )
  );

  $form = array(
    '#theme' => 'hotel_booking_availability_form'
  );

  $form['nid'] = array(
    '#type'  => 'value',
    '#value' => $node->nid,
  );
  $form['year'] = array(
    '#type'  => 'value',
    '#value' => $year,
  );
  $form['month'] = array(
    '#type'  => 'value',
    '#value' => $month,
  );
  $form['calendar'] = hotel_booking_calendars_day_fields($month, $year, $node);

  $current_month = (int)date('m');
  $items = array();
  for ($i = 0; $i < 12; $i++ ) {
    $new_date = strtotime('+ '. $i .' months');
    $items['node/'. $node->nid .'/availability/'. date('Y', $new_date) .'/'.
           date('n', $new_date)] =
    t(date('F', $new_date), array(), array('context' => 'month_name')) .' '. date('Y', $new_date);
  }

  $items['node/'. $node->nid] = t('Return to node page');

  $form['goto'] = array(
    '#type' => 'select',
    '#title' => t('After saving go to'),
    '#default_value' => 'node/'. $node->nid .'/availability/'. $year .'/'. $month,
    '#options' => $items,
    '#description' => t('Choose the month you wish to edit next.'),
  );


  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
  );

  return $form;

}

/**
 * Hotel Calendars Month Table function
 * @param $month int month to render
 * @param $year int year to render
 * @param $node object hotel room type node object
 * @return form array
 */
function hotel_booking_calendars_day_fields($month, $year, $node) {
  // Generate the first day of the month
  $first_day = mktime(0, 0, 0, $month, 1, $year);
  $break = mktime(0, 0, 0, $month + 1, 1, $year);

  // Find out what day of the week the first day of the month falls on
  $day_of_week = date('D', $first_day);
  $blank = date('w', $first_day);

  $end_day = mktime(0, 0, 0, $month + 1, 0, $year); //last day of previous month
  $days_in_month = date('j', $end_day);

  //store these for the theme function
  $form = array(
    '#first_day' => $first_day,
    '#day_of_week' => $day_of_week,
    '#blank' => $blank,
    '#end_day' => $end_day,
    '#tree' => TRUE,
    '#days_in_month' => $days_in_month
  );

  // Build select box options for restrictions
  $stay_options = array(0 => t('None'));
  for ($i = 2; $i <= 6; $i++) {
    $stay_options[$i] = format_plural($i, '1 night', '@count nights');
  }

  $occupancy_options = array(0 => t('None'));
  for ($i = 2; $i <= $node->capacity; $i++) {
    $occupancy_options[$i] = format_plural($i, '1 guest', '@count guests');
  }

  $check_in = array(
    t('Check in OK'),
    t("Can't check in")
  );

  $check_out = array(
    t('Check out OK'),
    t("Can't check out")
  );

  // Gather up all existing availability info
  $days = db_query(
    "SELECT *
    FROM {hotel_booking_availability_calendars}
    WHERE nid = :nid
    AND calendar_dt BETWEEN :start AND :end",
    array(
      ':nid' => $node->nid,
      ':start' => date(DATE_FORMAT_ISO, $first_day),
      ':end' => date(DATE_FORMAT_ISO, $end_day)
    )
  );

  //make assoc array of data existing availability
  $assoc = array();
  foreach ($days as $day) {
    $day = (array)$day;
    $ref_date = new DateObject($day['calendar_dt'], date_default_timezone(), DATE_FORMAT_ISO);
    $ref_day = $ref_date->format('j');
    $assoc[$ref_day] = $day;
  }

  $rows = $row = array();
  $counter = new DateObject(date(DATE_FORMAT_ISO, $first_day), date_default_timezone(), DATE_FORMAT_ISO);

  while ((int)$counter->format('U') < $break) {
    $ref_day = $counter->format('j');
    $existing = (isset($assoc[$ref_day]) ? $assoc[$ref_day] : FALSE);
    if (!$existing) {
      //use the defaults
      $existing = array(
        'available' => $node->default_available,
        'minimum_stay' => 0,
        'minimum_occupancy' => 0,
        'no_check_in' => 0,
        'no_check_out' => 0
      );
    }

    $form[$ref_day] = array(
      '#tree' => TRUE,
      'available' => array(
        '#type'          => 'textfield',
        '#title'         => t('Available'),
        '#default_value' => $existing['available'],
        '#size'          => 10,
        '#maxlength'     => 3,
        '#required'      => TRUE,
      ),
      'restrictions' => array(
        '#type'         => 'fieldset',
        '#collapsible'  => TRUE,
        '#title'        => t('Restrictions'),
        '#collapsed'    => TRUE,
        'minimums_caption' => array(
          '#markup' => '<div>'. t('Required minimums') .'</div>'
        ),
        'minimum_stay' => array(
          '#type' => 'select',
          '#default_value' => $existing['minimum_stay'],
          '#options' => $stay_options
        ),
        'minimum_occupancy' => array(
          '#type' => 'select',
          '#default_value' => $existing['minimum_occupancy'],
          '#options' => $occupancy_options
        ),
        'check_in_out_caption' => array(
          '#markup' => '<div>'. t('Check In/Out') .'</div>'
        ),
        'no_check_in' => array(
          '#type' => 'select',
          '#default_value' => $existing['no_check_in'],
          '#options' => $check_in
        ),
        'no_check_out' => array(
          '#type' => 'select',
          '#default_value' => $existing['no_check_out'],
          '#options' => $check_out
        ),
      )
    );

    //loop to next day
    $counter->modify('+ 1 day');
  }

  return $form;
}


/**
 *Submit handler for availability editing
*/
function hotel_booking_availability_form_submit($form, &$form_state) {

  $values = $form_state['values'];
  hotel_booking_inc('util');
  $expiry = hotel_booking_expiration_date();
  foreach ($values['calendar'] as $day => $record) {
    $ref_date = mktime(0, 0, 0, $values['month'], $day, $values['year']);
    db_delete("hotel_booking_availability_calendars")
    ->condition('nid', $form_state['values']['nid'])
    ->condition('calendar_dt', date(DATE_FORMAT_ISO, $ref_date))
    ->execute();
    $record = array_merge(
      $record,
      $record['restrictions'],
      array(
        'nid' => $values['nid'],
        'calendar_dt' => date(DATE_FORMAT_ISO, $ref_date)
      )
    );
    unset($record['restrictions']);
    if ($ref_date <= (int)$expiry->format('U')) {
      $record['available'] = 0;
    }
    drupal_write_record('hotel_booking_availability_calendars', $record);
  }
  drupal_set_message(t('Availability settings have been saved.'));
  $form_state['redirect'] = $form_state['values']['goto'];
}

/**
 * Display a list of node dates.
 *
 * @param $node object node object
 * @param $year int start year of calendars
 * @param $month int start month of calendars
 * @param $show int number of months to show
 * @return HTML output.
 */
function hotel_booking_calendars_node($node, $year = FALSE, $month = FALSE, $show = FALSE) {
  //if we are looking at a translated version of a node, get
  //the availability from the translation master (tnid)
  if ($node->tnid && $node->tnid != $node->nid) {
    $translated = clone $node;
    $node = node_load($node->tnid);
  }
  if (!$show) {
    $show = variable_get('hotel_booking_display_monthcount', 12);
  }
  if (!$year) {
    $year = (int)date('Y');
  }
  if (!$month) {
    $month = (int)date('m');
  }

  $start_date = date('Y-m-d', mktime(0, 0, 0, $month, 1, $year));
  $end_time = mktime(0, 0, 0, $month + $show, 0, $year);
  $end_date = date('Y-m-d', $end_time); //zero for day gives last day

  $where = $params = $results = array();
  $query = "
    SELECT
      n.nid,
      n.vid,
      hbrt.hbrmid,
      hbrt.hbrid,
      hbrm.method,
      hbrm.rate AS modifier_rate,
      hbrc.calendar_dt,
      hbrc.rate,
      hbac.available,
      hbac.minimum_occupancy,
      hbac.minimum_stay,
      hbac.no_check_in,
      hbac.no_check_out
    FROM {hotel_booking_room_types} hbrt
      LEFT JOIN {hotel_booking_rate_modifiers} hbrm
      ON hbrt.hbrmid = hbrm.hbrmid
      INNER JOIN {hotel_booking_rate_calendars} hbrc
      ON hbrc.hbrid = hbrt.hbrid
      INNER JOIN {node} n
      ON n.vid = hbrt.vid
      LEFT JOIN {hotel_booking_availability_calendars} hbac
      ON hbac.nid = n.nid
      AND hbrc.calendar_dt = hbac.calendar_dt
    WHERE (hbrc.calendar_dt BETWEEN :start AND :end OR hbrc.calendar_dt IS NULL)
      AND n.type = 'hotel_room_type'
      AND n.nid = :nid
    ORDER BY hbrc.calendar_dt ";

  $params = array(':start' => $start_date, ':end' => $end_date, ':nid' => $node->nid);

  $res = db_query($query, $params);

  //prepopulate the results so something shows if no availability or rates
  $ref_dt = new DateObject($start_date, date_default_timezone(), DATE_FORMAT_ISO);
  while ($ref_dt->format('U') <= $end_time) {
    $results[$ref_dt->format('Y')][$ref_dt->format('n')] = array();
    $ref_dt->modify('+1 months');
  }

  foreach ($res as $row) {
    $date = new DateObject($row->calendar_dt, date_default_timezone(), DATE_FORMAT_DATETIME);
    $rate = $row->rate;
    if ($row->method == 'value') {
      $rate += $row->modifier_rate;
    }
    else {
      $rate += ($rate * $row->modifier_rate / 100);
    }
    $row->rate = $rate;

    $results[$date->format('Y')][$date->format('n')][$date->format('j')] = $row;
  }
  return theme('hotel_booking_calendars', array('node' => $node, 'results' => $results));
}
