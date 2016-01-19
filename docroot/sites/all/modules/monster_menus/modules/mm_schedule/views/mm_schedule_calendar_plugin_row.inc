<?php
/**
 * @file
 * Contains the Entity List row style plugin.
 */

class mm_schedule_calendar_plugin_row extends calendar_plugin_row {

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['colors']['#access'] = FALSE;
  }

  function explode_values($event) {
    $rows = array();

    $date_info = $this->date_argument->view->date_info;
    $item_start_date = $event->date_start;
    $item_end_date = $event->date_end;
    $to_zone = $event->to_zone;
    $granularity = $event->granularity;
    $increment = $event->increment;

    // Now that we have an 'entity' for each view result, we need
    // to remove anything outside the view date range,
    // and possibly create additional nodes so that we have a 'node'
    // for each day that this item occupies in this view.
    $now = max($date_info->min_zone_string, $item_start_date->format(DATE_FORMAT_DATE));
    $next = new DateObject($now . ' 00:00:00', $date_info->display_timezone);
    if ($date_info->display_timezone_name != $to_zone) {
      // Make $start and $end (derived from $node) use the timezone $to_zone, just as the original dates do.
      date_timezone_set($next, timezone_open($to_zone));
    }

    $entity = clone($event);

    // Get start and end of current day.
    $start = $next->format(DATE_FORMAT_DATETIME);

    // Get start and end of item, formatted the same way.
    $item_start = $item_start_date->format(DATE_FORMAT_DATETIME);
    $item_end = $item_end_date->format(DATE_FORMAT_DATETIME);

    // Get intersection of current day and the node value's duration (as strings in $to_zone timezone).
    $entity->ongoing = $item_start < $start;
    $entity->calendar_start = $item_start < $start ? $start : $item_start;
    $entity->calendar_end = !empty($item_end) ? $item_end : $entity->calendar_start;

    // Make date objects
    $entity->calendar_start_date = date_create($entity->calendar_start, timezone_open($to_zone));
    $entity->calendar_end_date = date_create($entity->calendar_end, timezone_open($to_zone));

    // Change string timezones into
    // calendar_start and calendar_end are UTC dates as formatted strings
    $entity->calendar_start = date_format($entity->calendar_start_date, DATE_FORMAT_DATETIME);
    $entity->calendar_end = date_format($entity->calendar_end_date, DATE_FORMAT_DATETIME);
    $entity->calendar_all_day = date_is_all_day($entity->calendar_start, $entity->calendar_end, $granularity, $increment);

    unset($entity->calendar_fields);
    if (!empty($entity->calendar_start)) {
      $entity->date_id .= '.0';
      $rows[] = $entity;
    }

    return $rows;
  }

  /**
   * Override the function in calendar_plugin_row, since it's not needed here.
   */
  function calendar_node_type_stripe(&$result) {
  }

  /**
   * Override the function in calendar_plugin_row, since it's not needed here.
   */
  function calendar_taxonomy_stripe(&$result) {
    return;
  }

  /**
   * Override the function in calendar_plugin_row, since it's not needed here.
   */
  function calendar_group_stripe(&$result) {
  }
}
