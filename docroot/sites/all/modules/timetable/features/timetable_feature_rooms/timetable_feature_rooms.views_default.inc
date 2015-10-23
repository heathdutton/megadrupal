<?php
/**
 * @file
 * timetable_feature_rooms.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function timetable_feature_rooms_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'timetable_sample_timetable_sessions_rooms';
  $view->description = 'A timetable showing sessions and rooms in a cross-table.';
  $view->tag = 'default';
  $view->base_table = 'eck_timeslot';
  $view->human_name = 'Timetable sample: rooms';
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
  $handler->display->display_options['style_plugin'] = 'views_pivot';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'field_timeslot_time' => 'field_timeslot_time',
    'title_1' => 'title_1',
    'title' => 'title',
    'field_session_track' => 'field_session_track',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
      'pivot_row_extra' => 0,
    ),
    'field_timeslot_time' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'pivot_row_extra' => 0,
    ),
    'title_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'pivot_row_extra' => 0,
    ),
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'pivot_row_extra' => 0,
    ),
    'field_session_track' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
      'pivot_row_extra' => 0,
    ),
  );
  $handler->display->display_options['style_options']['pivot_col'] = 'title_1';
  $handler->display->display_options['style_options']['pivot_row'] = 'field_timeslot_time';
  $handler->display->display_options['style_options']['pivot_mode_multi_values'] = '1';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Session */
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['id'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['table'] = 'eck_timeslot';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['field'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['ui_name'] = 'Session';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['label'] = 'Session';
  $handler->display->display_options['relationships']['reverse_field_session_timeslot_node']['required'] = TRUE;
  /* Relationship: Schedule */
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['id'] = 'reverse_field_schedule_timeslots_node';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['table'] = 'eck_timeslot';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['field'] = 'reverse_field_schedule_timeslots_node';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['ui_name'] = 'Schedule';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['label'] = 'Schedule';
  $handler->display->display_options['relationships']['reverse_field_schedule_timeslots_node']['required'] = TRUE;
  /* Relationship: Room */
  $handler->display->display_options['relationships']['field_session_room_target_id']['id'] = 'field_session_room_target_id';
  $handler->display->display_options['relationships']['field_session_room_target_id']['table'] = 'field_data_field_session_room';
  $handler->display->display_options['relationships']['field_session_room_target_id']['field'] = 'field_session_room_target_id';
  $handler->display->display_options['relationships']['field_session_room_target_id']['relationship'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['relationships']['field_session_room_target_id']['ui_name'] = 'Room';
  $handler->display->display_options['relationships']['field_session_room_target_id']['label'] = 'Room';
  $handler->display->display_options['relationships']['field_session_room_target_id']['required'] = TRUE;
  /* Field: Timeslot: Id */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'eck_timeslot';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['exclude'] = TRUE;
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
  /* Field: Room: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'eck_room';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'field_session_room_target_id';
  $handler->display->display_options['fields']['title_1']['label'] = 'Room';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'reverse_field_session_timeslot_node';
  $handler->display->display_options['fields']['title']['label'] = 'Session';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Sort criterion: Timeslot: Time (field_timeslot_time) */
  $handler->display->display_options['sorts']['field_timeslot_time_value']['id'] = 'field_timeslot_time_value';
  $handler->display->display_options['sorts']['field_timeslot_time_value']['table'] = 'field_data_field_timeslot_time';
  $handler->display->display_options['sorts']['field_timeslot_time_value']['field'] = 'field_timeslot_time_value';
  /* Contextual filter: Schedule node nid */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['relationship'] = 'reverse_field_schedule_timeslots_node';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'Schedule node nid';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Timeslot: timeslot type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'eck_timeslot';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'timeslot' => 'timeslot',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'timetable_sample_timetable_sessions_rooms';
  $export['timetable_sample_timetable_sessions_rooms'] = $view;

  return $export;
}
