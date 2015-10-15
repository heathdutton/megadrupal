<?php

/**
 * @file
 * Provides Listing page for Created date(s) in Appointment calendar.
 */

/**
 * Implements hook_form().
 */
function appointment_calendar_date_list_form($form, $form_state) {
  // For date list appointment calendar.
  global $base_url;
  $form['redirect'] = array(
    '#markup' => '<a href="/admin/config/appointment-calendar/settings">Add Appointment Date and Slot(s)</a>',
  );
  $form['filter_date'] = array(
    '#title' => t('From Date'),
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '0:+3',
    '#default_value' => isset($_GET['date']) ? $_GET['date'] : date('Y-m-d', time()),
  );
  $form['filter_to_date'] = array(
    '#title' => t('To Date'),
    '#type' => 'date_popup',
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '0:+3',
    '#default_value' => isset($_GET['todate']) ? $_GET['todate'] : date('Y-m-d', time()),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Filter',
  );
  $form['reset'] = array(
    '#type' => 'submit',
    '#value' => 'Reset',
  );
  $date_query = db_select('appointment_date', 'ad');
  $date_query->fields('ad');
  if (!empty($_GET['date'])) {
    $date_query->condition('date', strtotime($_GET['date']), '>=');
  }
  if (!empty($_GET['todate'])) {
    $date_query->condition('date', strtotime($_GET['todate']), '<=');
  }
  $date_query->orderBy('date');
  $date_result = $date_query->execute()->fetchAll();
  $header = array('Date', 'No. Slots', '', '');
  $rows = array();
  $output = '';
  foreach ($date_result as $date) {
    $capacity = appointment_calendar_slot_capacity($date->date);
    $slots = count((array) json_decode($capacity));
    $edit = check_plain($base_url . '/admin/appointment-calendar/edit?date=' . $date->date);
    $delete = check_plain($base_url . '/admin/appointment-calendar/delete?date=' . $date->date);
    $row = '';
    $row[] = date('Y-m-d', $date->date);
    $row[] = $slots;
    $row[] = '<a href="' . $edit . '">Edit</a>';
    $row[] = '<a href="' . $delete . '">Delete</a>';
    $rows[] = $row;
  }
  if (count($rows) > 0) {
    $per_page = 25;
    $current_page = pager_default_initialize(count($rows), $per_page);
    $chunks = array_chunk($rows, $per_page, TRUE);
    $output = theme('table', array('header' => $header, 'rows' => $chunks[$current_page]));
    $output .= theme('pager', array('quantity', count($rows)));
    $form['ouptut'] = array(
      '#markup' => $output,
    );
  }
  else {
    $form['ouptut'] = array(
      '#markup' => t('No Dates Available for selected date.'),
    );
  }
  return $form;
}

/**
 * Implements hook_form_state().
 */
function appointment_calendar_date_list_form_submit($form, &$form_state) {
  // Goto current path if reset.
  if ($form_state['values']['op'] == 'Reset') {
    drupal_goto(current_path());
  }
  // Pass values to url.
  if ($form_state['values']['op'] == 'Filter') {
    $filter_date = $form_state['values']['filter_date'];
    $filter_to_date = $form_state['values']['filter_to_date'];
    $params['date'] = check_plain($filter_date);
    $params['todate'] = check_plain($filter_to_date);
    drupal_goto('admin/config/appointment-calendar/settings/list-date', array('query' => $params));
  }
}
