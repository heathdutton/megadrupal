<?php
/**
 * @file
 * timetable_feature_schedule.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function timetable_feature_schedule_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'timetable_sample_timetable_sessions';
  $view->description = 'A basic timetable showing only sessions.';
  $view->tag = 'default';
  $view->base_table = 'eck_timeslot';
  $view->human_name = 'Timetable sample: basic';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Schedule';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Schedule node */
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['id'] = 'reverse_field_schedule_timeslots_node';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['table'] = 'eck_timeslot';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['field'] = 'reverse_field_schedule_timeslots_node';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['ui_name'] = 'Schedule node';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['label'] = 'Schedule node referencing Timeslot from field_schedule_timeslots';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['required'] = TRUE;
  /* Relationship: Session node */
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['id'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['table'] = 'eck_timeslot';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['field'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['ui_name'] = 'Session node';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['label'] = 'Session node referencing Timeslot';
  /* Relationship: Content: Author */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'node';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['relationships']['uid']['label'] = 'session author';
  /* Field: Timeslot: Time */
  $handler->display->display_options['fields']['field_timeslot_time']['id'] = 'field_timeslot_time';
  $handler->display->display_options['fields']['field_timeslot_time']['table'] = 'field_data_field_timeslot_time';
  $handler->display->display_options['fields']['field_timeslot_time']['field'] = 'field_timeslot_time';
  $handler->display->display_options['fields']['field_timeslot_time']['settings'] = array(
    'display_format' => array(
      'hour' => 'G',
      'minute' => 'i',
      'separator' => ':',
      'period' => 'none',
      'period_separator' => '',
    ),
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['fields']['title']['label'] = 'Session';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  /* Sort criterion: Timeslot: Time (field_timeslot_time) */
  $handler->display->display_options['sorts']['field_timeslot_time_value']['id'] = 'field_timeslot_time_value';
  $handler->display->display_options['sorts']['field_timeslot_time_value']['table'] = 'field_data_field_timeslot_time';
  $handler->display->display_options['sorts']['field_timeslot_time_value']['field'] = 'field_timeslot_time_value';
  /* Contextual filter: Content: Nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'reverse_field_schedule_timeslots_node';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'timetable_sample_timetable_sessions';
  $export['timetable_sample_timetable_sessions'] = $view;

  return $export;
}
