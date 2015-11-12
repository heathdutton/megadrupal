<?php

/**
 * @file
 * View templates for the Appointment Calendar module.
 */

/**
 * Implements hook_views_default_views().
 */
function appointment_calendar_views_default_views() {
  // To show available and booked in block calendar.
  $export = array();
  $view = new view();
  $view->name = 'appointment_calendar';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'Appointment Calendar';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Appointment Calendar';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '5';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'appointment_calendar' => 'appointment_calendar',
  );

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['calendar_type'] = 'day';
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '0';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['style_options']['groupby_times'] = 'custom';
  $handler->display->display_options['style_options']['groupby_times_custom'] = '10:00:00';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['header'] = FALSE;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['content'] = 'Donot Delete this(Using To set Time Slots)';
  $handler->display->display_options['header']['area']['format'] = 'filtered_html';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Slots Available..';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Appointment Date */
  $handler->display->display_options['fields']['appointment_date']['id'] = 'appointment_date';
  $handler->display->display_options['fields']['appointment_date']['table'] = 'field_data_appointment_date';
  $handler->display->display_options['fields']['appointment_date']['field'] = 'appointment_date';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: Content: Appointment Date (appointment_date) */
  $handler->display->display_options['sorts']['appointment_date_value']['id'] = 'appointment_date_value';
  $handler->display->display_options['sorts']['appointment_date_value']['table'] = 'field_data_appointment_date';
  $handler->display->display_options['sorts']['appointment_date_value']['field'] = 'appointment_date_value';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Content: Appointment Date (appointment_date) */
  $handler->display->display_options['arguments']['appointment_date_value']['id'] = 'appointment_date_value';
  $handler->display->display_options['arguments']['appointment_date_value']['table'] = 'field_data_appointment_date';
  $handler->display->display_options['arguments']['appointment_date_value']['field'] = 'appointment_date_value';
  $handler->display->display_options['arguments']['appointment_date_value']['default_action'] = 'default';
  $handler->display->display_options['arguments']['appointment_date_value']['default_argument_type'] = 'date';
  $handler->display->display_options['arguments']['appointment_date_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['appointment_date_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['appointment_date_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['appointment_date_value']['granularity'] = 'day';
  $handler->display->display_options['block_description'] = 'Appointment Calendar';

  // (Export ends here.)
  // Add view to list of views to provide.
  $export[$view->name] = $view;
  // At the end, return array of default views.
  return $export;
}
