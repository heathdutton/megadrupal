<?php

/**
 * @file
 * Ajax Commands Provided by Party Activity
 */

/**
 * Ajax Command to add an event to a fullcalendar.
 */
function party_activity_command_fullcalendar_add($activity, $view) {
  $command = array(
    'command' => 'fullcalendar_activity_add',
  );
  return party_activity_command_fullcalendar_build_event($command, $activity, $view);
}

/**
 * Ajax Command to update an event on a calendar.
 */
function party_activity_command_fullcalendar_update($activity, $view) {
  $command = array(
    'command' => 'fullcalendar_activity_update',
  );
  return party_activity_command_fullcalendar_build_event($command, $activity, $view);
}

/**
 * Ajax command to remove an event from a calendar.
 */
function party_activity_command_fullcalendar_delete($activity, $view) {
  $command = array(
    'fullcalendar_selector' => '.fullcalendar',
    'command' => 'fullcalendar_activity_delete',
    'entity_type' => 'party_activity',
    'eid' => $activity->id,
  );
  drupal_alter('party_activity_fullcalendar_event_command', $command, $activity, $view);
  return $command;
}

/**
 * Builds important properties into a fullcalendar ajax command.
 */
function party_activity_command_fullcalendar_build_event($command, $activity, $view) {
  $activity->fullcalendar_date_field = 'activity_date';
  $activity->bundle = $activity->type;

  $command['fullcalendar_selector'] = '.fullcalendar';

  $command['entity_type'] = 'party_activity';
  $command['eid'] = $activity->id;

  module_load_include('inc', 'fullcalendar', 'theme/theme');
  $field = field_info_field('activity_date');
  $instance = field_info_instance('party_activity', 'activity_date', $activity->type);
  $item = &$activity->activity_date[LANGUAGE_NONE][0];

  // Tweak the date format to ensure that it matches the required format.
  foreach (array('value', 'value2') as $k) {
    $item[$k] = date('Y-m-d H:i:s', strtotime($item[$k]));
  }
  $dates = _fullcalendar_process_dates($instance, $activity, $field, $item);
  list($start, $end, $all_day) = $dates;
  $command['allDay'] = $all_day;
  $command['start'] = $start;
  $command['end'] = $end;

  // Create a string of valid HTML class names and add them to the entity.
  $classes = module_invoke_all('fullcalendar_classes', $activity);
  drupal_alter('fullcalendar_classes', $classes, $activity);
  $classes = array_map('drupal_html_class', $classes);
  $command['cn'] =  implode(' ', array_unique($classes));

  $command['title'] = strip_tags(htmlspecialchars_decode($activity->title, ENT_QUOTES));

  $command['field'] = 'activity_date';
  $command['index'] = 0;

  // Allow resize/drag/drop of an event if user has proper permissions.
  $editable = module_invoke_all('fullcalendar_editable', $activity, $view);
  // If one value is FALSE, return FALSE. The identical operator is needed
  // because of the return value of array_search().
  $editable = array_search(FALSE, $editable, TRUE) === FALSE;
  drupal_alter('fullcalendar_editable', $editable, $activity, $view);
  $command['editable'] = $editable ? 1 : 0;

  // Default URL.
  $uri = entity_uri('party_activity', $activity);
  $command['href'] = isset($uri['path']) ? $uri['path'] : '';

  drupal_alter('party_activity_fullcalendar_event_command', $command, $activity, $view);

  return $command;
}
