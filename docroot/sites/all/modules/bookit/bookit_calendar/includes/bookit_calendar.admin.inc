<?php

/**
 * Returns a link widget for bookable item's calendar.
 */
function _bookit_calendar_status_widget($item_id, $date) {
  $status = db_select('bookit_calendar', 'c')
    ->fields('c', array('status'))
    ->condition('c.item_id', $item_id)
    ->condition('c.date', $date)
    ->execute()
    ->fetchField();

  if ($status === FALSE) {
    $status = 1;
  }

  $new_status = (int) ! (bool) $status;

  $query = array(
    'token' => drupal_get_token("bookit_calendar_{$item_id}_{$date}_{$new_status}"),
  ) + drupal_get_destination();

  $widget = array(
    '#title' => t('Status'),
    '#type' => 'container',
    '#attributes' => array('id' => "bookit-status-$item_id-$date", 'class' => array('bookit_status')),
    '#attached' => array(
      'library' => array(
        array('system', 'drupal.ajax'),
      )
    ),
  );

  // Handle classes.
  $class = array('use-ajax');
  if (bookit_calendar_item_status($item_id, $date) || !$status) {
    $class[] = 'status-' . (int) $status;
  }

  $widget['link']['#markup'] = l($status ? t('Disable') : t('Enable'), 'bookit_calendar/ajax/' . $item_id . '/' . $date . '/' . $new_status, array('query' => $query, 'attributes' => array('class' => $class)));

  $day = date('j', strtotime($date));
  $widget['value'] = array(
    '#type' => 'hidden',
    '#value' => $status,
    '#name' => 'calendar[' . $day . '][status]',
    '#parents' => array('calendar', $day, 'status'),
  );

  return $widget;
}

/**
 * Page callback for overview.
 */
function bookit_calendar_overview($node, $year = NULL, $month = NULL) {
  $page = array();

  // Get bookable items referenced by the node.
  $bookit_items = array();
  foreach(field_info_instances('node', $node->type) as $field_name => $instance) {
    $field = field_info_field($field_name);
    if ($field['type'] == 'entityreference' and $field['settings']['target_type'] == 'bookit_item') {
      $items = field_get_items('node', $node, $field_name);
      foreach ($items as $delta => $item) {
        $bookit_items[] = bookit_item_load($item['target_id']);
      }
    }
  }

  // If only one bookable item then render the form;
  if (count($bookit_items) == 1) {
    $bookit_item = reset($bookit_items);
    return drupal_get_form('bookit_calendar_admin_form', $bookit_item, $year, $month);
  }

  // Add admin css and js.
  $page['#attached']['css'][] = drupal_get_path('module', 'bookit_calendar') . '/theme/bookit_calendar.admin.css';
  $page['#attached']['js'][] = drupal_get_path('module', 'bookit_calendar') . '/theme/bookit_calendar.js';
  $form['#attached']['library'][] = array('system', 'ui.dialog');

  // Prepare year and month.
  $year = !empty($year) ? $year : date('Y');
  $month = !empty($month) ? $month : date('n');

  // Create a datetime from the first day of the month.
  $datetime = strtotime("$year-$month-01 00:00:00");

  // Create a datetime from the first day of the current month.
  $current_datetime = strtotime('first day of this month 00:00', REQUEST_TIME);

  // Don't allow users to edit past months.
  if ($datetime < $current_datetime) {
    $url = url('node/' . $node->nid . '/calendar/' . date('Y', $current_datetime) . '/' . date('n', $current_datetime));
    return array('#prefix' => t('You cannot edit past months. Go to <a href="@url">@name</a>', array('@url' => $url, '@name' => format_date($current_datetime, 'custom', 'F'))));
  }

  // Month days.
  $days = date('j', strtotime('last day of this month', $datetime));

  // Calendar navigation.
  $page['navigation']['#markup'] = '<div class="calendar-navigation">';
  $previous = strtotime('-1 month', $datetime);
  if ($previous >= $current_datetime) {
    $page['navigation']['#markup'] .= l(t('Previous month (@name)', array('@name' => format_date($previous, 'custom', 'F'))), 'node/' . $node->nid . '/calendar/' . date('Y', $previous) . '/' . date('n', $previous), array('attributes' => array('class' => array('calendar-previous-month'))));
  }
  $next = strtotime('+1 month', $datetime);
  $page['navigation']['#markup'] .= l(t('Next month (@name)', array('@name' => format_date($next, 'custom', 'F'))), 'node/' . $node->nid . '/calendar/' . date('Y', $next) . '/' . date('n', $next), array('attributes' => array('class' => array('calendar-next-month'))));
  $page['navigation']['#markup'] .= '</div>';

  // Build calendar table.
  $page['calendar'] = array(
    '#theme_wrappers' => array('form_bookit_calendar'),
    '#year' => $year,
    '#month' => $month,
    '#attributes' => array(
      'class' => array('bookit-calendar'),
    ),
  );

  // Create the calendar form.
  foreach ($bookit_items as $bookit_item) {
    for ($i = 1; $i <= $days; $i++) {
      $date = _bookit_calendar_create_date($year, $month, $i);
      $page['calendar'][$i][$bookit_item->item_id] = _bookit_calendar_status_widget($bookit_item->item_id, $date);
      $page['calendar'][$i][$bookit_item->item_id]['#title'] = l($bookit_item->title, 'admin/bookit/items/' . $bookit_item->item_id . '/calendar/' . $year . '/' . $month, array('query' => drupal_get_destination()));
    }
  }

  return $page;
}

/**
 * Form callback to administer bookable items in calendar.
 */
function bookit_calendar_admin_form($form, &$form_state, $bookit_item, $year = NULL, $month = NULL) {
  $form = array('#tree' => TRUE);

  // Create a breadcrumb based on referenced node, if any.
  if ($node = bookit_item_get_referencing_node($bookit_item->item_id)) {
    $breadcrumb = array();
    $breadcrumb[] = l(t('Home'), '<front>');
    $breadcrumb[] = l($node->title, 'node/' . $node->nid);
    $breadcrumb[] = l('Calendar', 'node/' . $node->nid . '/calendar');
    $breadcrumb[] = drupal_get_title();
    drupal_set_breadcrumb($breadcrumb);
  }

  // Add admin css.
  $form['#attached']['css'][] = drupal_get_path('module', 'bookit_calendar') . '/theme/bookit_calendar.admin.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'bookit_calendar') . '/theme/bookit_calendar.js';
  $form['#attached']['library'][] = array('system', 'ui.selectable');
  $form['#attached']['library'][] = array('system', 'ui.dialog');

  // Prepare year and month.
  $year = !empty($year) ? $year : date('Y');
  $month = !empty($month) ? $month : date('n');

  // Store some default values.
  $form_state['bookit_item'] = $bookit_item;
  $form_state['year'] = $year;
  $form_state['month'] = $month;

  // Create a datetime from the first day of the month.
  $datetime = strtotime("$year-$month-01 00:00:00");

  // Create a datetime from the first day of the current month.
  $current_datetime = strtotime('first day of this month 00:00', REQUEST_TIME);

  // Don't allow users to edit past months.
  if ($datetime < $current_datetime) {
    $url = url('admin/bookit/items/' . $bookit_item->item_id . '/calendar/' . date('Y', $current_datetime) . '/' . date('n', $current_datetime));
    return array('#prefix' => t('You cannot edit past months. Go to <a href="@url">@name</a>', array('@url' => $url, '@name' => format_date($current_datetime, 'custom', 'F'))));
  }

  // Month days.
  $days = date('j', strtotime('last day of this month', $datetime));

  // Calendar navigation.
  $form['#prefix'] = '<div class="calendar-navigation">';
  $previous = strtotime('-1 month', $datetime);
  if ($previous >= $current_datetime) {
    $form['#prefix'] .= l(t('Previous month (@name)', array('@name' => format_date($previous, 'custom', 'F'))), 'admin/bookit/items/' . $bookit_item->item_id . '/calendar/' . date('Y', $previous) . '/' . date('n', $previous), array('attributes' => array('class' => array('calendar-previous-month'))));
  }
  $next = strtotime('+1 month', $datetime);
  $form['#prefix'] .= l(t('Next month (@name)', array('@name' => format_date($next, 'custom', 'F'))), 'admin/bookit/items/' . $bookit_item->item_id . '/calendar/' . date('Y', $next) . '/' . date('n', $next), array('attributes' => array('class' => array('calendar-next-month'))));
  $form['#prefix'] .= '</div>';

  // Build calendar table.
  $form['calendar'] = array(
    '#theme_wrappers' => array('form_bookit_calendar'),
    '#year' => $year,
    '#month' => $month,
    '#attributes' => array(
      'class' => array('bookit-calendar'),
    ),
  );

  for ($i = 1; $i <= $days; $i++) {
    $form['calendar'][$i]['#date'] = $date = _bookit_calendar_create_date($year, $month, $i);
    $form['calendar'][$i]['status'] = _bookit_calendar_status_widget($bookit_item->item_id, $date);
  }

  $form['actions'] = array(
    '#type' => 'actions',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Submit callback for bookit_calendar_admin_form().
 */
function bookit_calendar_admin_form_submit($form, &$form_state) {
  $bookit_item = $form_state['bookit_item'];

  foreach ($form_state['values']['calendar'] as $day => $value) {
    // Save the calendar.
    db_merge('bookit_calendar')
    ->key(array(
      'item_id' => $bookit_item->item_id,
      'date' => $form['calendar'][$day]['#date'],
    ))
    ->fields(array(
      'item_id' => $bookit_item->item_id,
      'date' => $form['calendar'][$day]['#date'],
      'status' => (int) $value['status'],
    ))
    ->execute();
  }
}
