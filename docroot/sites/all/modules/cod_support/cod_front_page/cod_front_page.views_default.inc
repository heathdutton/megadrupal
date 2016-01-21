<?php
/**
 * @file
 * cod_front_page.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cod_front_page_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'cod_front_page_conferences';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'cod_front_page_conferences';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Upcoming Events';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Content: Image */
  $handler->display->display_options['fields']['field_event_image']['id'] = 'field_event_image';
  $handler->display->display_options['fields']['field_event_image']['table'] = 'field_data_field_event_image';
  $handler->display->display_options['fields']['field_event_image']['field'] = 'field_event_image';
  $handler->display->display_options['fields']['field_event_image']['label'] = '';
  $handler->display->display_options['fields']['field_event_image']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_event_image']['click_sort_column'] = 'fid';
  $handler->display->display_options['fields']['field_event_image']['settings'] = array(
    'image_style' => 'thumbnail',
    'image_link' => 'content',
  );
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Field: Content: Dates */
  $handler->display->display_options['fields']['field_dates']['id'] = 'field_dates';
  $handler->display->display_options['fields']['field_dates']['table'] = 'field_data_field_dates';
  $handler->display->display_options['fields']['field_dates']['field'] = 'field_dates';
  $handler->display->display_options['fields']['field_dates']['label'] = '';
  $handler->display->display_options['fields']['field_dates']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_dates']['settings'] = array(
    'format_type' => 'date_no_time',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_remaining_days' => 0,
  );
  /* Sort criterion: Content: Dates -  start date (field_dates) */
  $handler->display->display_options['sorts']['field_dates_value']['id'] = 'field_dates_value';
  $handler->display->display_options['sorts']['field_dates_value']['table'] = 'field_data_field_dates';
  $handler->display->display_options['sorts']['field_dates_value']['field'] = 'field_dates_value';
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
    'event' => 'event',
  );
  /* Filter criterion: Content: Dates - end date (field_dates:value2) */
  $handler->display->display_options['filters']['field_dates_value2']['id'] = 'field_dates_value2';
  $handler->display->display_options['filters']['field_dates_value2']['table'] = 'field_data_field_dates';
  $handler->display->display_options['filters']['field_dates_value2']['field'] = 'field_dates_value2';
  $handler->display->display_options['filters']['field_dates_value2']['operator'] = '>';
  $handler->display->display_options['filters']['field_dates_value2']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['field_dates_value2']['default_date'] = 'now';

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $export['cod_front_page_conferences'] = $view;

  return $export;
}
