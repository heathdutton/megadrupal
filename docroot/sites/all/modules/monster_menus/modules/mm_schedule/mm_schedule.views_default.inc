<?php
/**
 * @file
 * mm_schedule.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function mm_schedule_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'mm_calendar';
  $view->description = 'A multi-dimensional calendar view with back/next navigation.';
  $view->tag = 'Calendar, Monster Menus';
  $view->base_table = 'node';
  $view->human_name = 'mm_calendar';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'month_grid';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'month';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '10';
  $handler->display->display_options['style_options']['multiday_hidden'] = array(
    'field_start_datetime' => 'field_start_datetime',
  );
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'time',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'hide',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['exclude'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['label'] = '';
  $handler->display->display_options['fields']['vid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['vid']['element_label_colon'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Date/time -  start date (field_start_datetime) */
  $handler->display->display_options['sorts']['field_start_datetime_value']['id'] = 'field_start_datetime_value';
  $handler->display->display_options['sorts']['field_start_datetime_value']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['sorts']['field_start_datetime_value']['field'] = 'field_start_datetime_value';
  $handler->display->display_options['sorts']['field_start_datetime_value']['relationship'] = 'field_mm_events_nid';
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: Individual events */
  $handler->display->display_options['arguments']['date_argument_1']['id'] = 'date_argument_1';
  $handler->display->display_options['arguments']['date_argument_1']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument_1']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['arguments']['date_argument_1']['ui_name'] = 'Individual events';
  $handler->display->display_options['arguments']['date_argument_1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument_1']['date_fields'] = array(
    'field_data_field_start_datetime.field_start_datetime_value' => 'field_data_field_start_datetime.field_start_datetime_value',
  );
  $handler->display->display_options['filter_groups']['operator'] = 'OR';

  /* Display: Month Grid */
  $handler = $view->new_display('page', 'Month Grid', 'month_grid');
  $handler->display->display_options['display_comment'] = 'Display a month worth of events in grid format';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'month';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '5';
  $handler->display->display_options['style_options']['multiday_hidden'] = array(
    'field_start_datetime_1' => 'field_start_datetime_1',
    'body' => 'body',
    'vid' => 'vid',
    'type' => 'type',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['row_options']['colors']['legend'] = 'type';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Month';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Calendar';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'secondary-menu';

  /* Display: Month List */
  $handler = $view->new_display('page', 'Month List', 'month_list');
  $handler->display->display_options['display_comment'] = 'Display a month worth of events in list format';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_schedule_list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => '',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'mm_calendar_entity';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'There are no events in the chosen period.';
  $handler->display->display_options['empty']['area']['format'] = '4';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Month';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Calendar';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'secondary-menu';

  /* Display: Day Grid */
  $handler = $view->new_display('page', 'Day Grid', 'day_grid');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'day';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['calendar_type'] = 'day';
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['style_options']['multiday_hidden'] = array(
    'field_start_datetime_1' => 'field_start_datetime_1',
    'body' => 'body',
    'vid' => 'vid',
    'type' => 'type',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Day';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['name'] = 'secondary-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Day List */
  $handler = $view->new_display('page', 'Day List', 'day_list');
  $handler->display->display_options['display_comment'] = 'Display a month worth of events in list format';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_schedule_list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => '',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'mm_calendar_entity';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'There are no events in the chosen period.';
  $handler->display->display_options['empty']['area']['format'] = '4';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: Individual events */
  $handler->display->display_options['arguments']['date_argument_1']['id'] = 'date_argument_1';
  $handler->display->display_options['arguments']['date_argument_1']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument_1']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument_1']['ui_name'] = 'Individual events';
  $handler->display->display_options['arguments']['date_argument_1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument_1']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument_1']['date_fields'] = array(
    'field_data_field_start_datetime.field_start_datetime_value' => 'field_data_field_start_datetime.field_start_datetime_value',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Day';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Calendar';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'secondary-menu';

  /* Display: Year Grid */
  $handler = $view->new_display('page', 'Year Grid', 'year_grid');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'year';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['calendar_type'] = 'year';
  $handler->display->display_options['style_options']['name_size'] = '1';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Year';
  $handler->display->display_options['menu']['weight'] = '4';
  $handler->display->display_options['menu']['name'] = 'secondary-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Year List */
  $handler = $view->new_display('page', 'Year List', 'year_list');
  $handler->display->display_options['display_comment'] = 'Display a year worth of events in list format';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_schedule_list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'mm_calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'There are no events in the chosen period.';
  $handler->display->display_options['empty']['area']['format'] = '4';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: Individual events */
  $handler->display->display_options['arguments']['date_argument_1']['id'] = 'date_argument_1';
  $handler->display->display_options['arguments']['date_argument_1']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument_1']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument_1']['ui_name'] = 'Individual events';
  $handler->display->display_options['arguments']['date_argument_1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument_1']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument_1']['date_fields'] = array(
    'field_data_field_start_datetime.field_start_datetime_value' => 'field_data_field_start_datetime.field_start_datetime_value',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Year';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Calendar';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'secondary-menu';

  /* Display: Week Grid */
  $handler = $view->new_display('page', 'Week Grid', 'week_grid');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'week';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['calendar_type'] = 'week';
  $handler->display->display_options['style_options']['name_size'] = '99';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['style_options']['multiday_hidden'] = array(
    'field_start_datetime_1' => 'field_start_datetime_1',
    'body' => 'body',
    'vid' => 'vid',
    'type' => 'type',
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Week';
  $handler->display->display_options['menu']['weight'] = '2';
  $handler->display->display_options['menu']['name'] = 'secondary-menu';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Week List */
  $handler = $view->new_display('page', 'Week List', 'week_list');
  $handler->display->display_options['display_comment'] = 'Display a month worth of events in list format';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['link_format'] = 'clean';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_schedule_list';
  $handler->display->display_options['style_options']['grouping'] = array(
    0 => array(
      'field' => '',
      'rendered' => 1,
      'rendered_strip' => 0,
    ),
  );
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'mm_calendar_entity';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'There are no events in the chosen period.';
  $handler->display->display_options['empty']['area']['format'] = '4';
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: Individual events */
  $handler->display->display_options['arguments']['date_argument_1']['id'] = 'date_argument_1';
  $handler->display->display_options['arguments']['date_argument_1']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument_1']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument_1']['ui_name'] = 'Individual events';
  $handler->display->display_options['arguments']['date_argument_1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument_1']['granularity'] = 'week';
  $handler->display->display_options['arguments']['date_argument_1']['date_fields'] = array(
    'field_data_field_start_datetime.field_start_datetime_value' => 'field_data_field_start_datetime.field_start_datetime_value',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['title'] = 'Week';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Calendar';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'secondary-menu';

  /* Display: iCal */
  $handler = $view->new_display('feed', 'iCal', 'ical');
  $handler->display->display_options['display_comment'] = 'Export events in iCal format';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'date_ical';
  $handler->display->display_options['row_plugin'] = 'mm_date_ical';
  $handler->display->display_options['row_options']['date_field'] = 'field_data_field_start_datetime.field_start_datetime_value';
  $handler->display->display_options['defaults']['empty'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $handler->display->display_options['path'] = '.mm_calendar';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Month';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Calendar';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'secondary-menu';
  $handler->display->display_options['displays'] = array(
    'month_grid' => 'month_grid',
    'month_list' => 'month_list',
    'day_grid' => 'day_grid',
    'day_list' => 'day_list',
    'year_grid' => 'year_grid',
    'year_list' => 'year_list',
    'week_grid' => 'week_grid',
    'week_list' => 'week_list',
    'default' => 0,
    'ical' => 0,
  );

  /* Display: Individual Events */
  $handler = $view->new_display('attachment', 'Individual Events', 'attachment_1');
  $handler->display->display_options['display_comment'] = 'Nodes from this attachment are combined with any other attachments to form the result set for any of the grid/list/etc pages. When you create a new page view, you must check it in the "Attach to" setting of all attachments.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Individual events (field_mm_events) */
  $handler->display->display_options['relationships']['field_mm_events_nid']['id'] = 'field_mm_events_nid';
  $handler->display->display_options['relationships']['field_mm_events_nid']['table'] = 'field_data_field_mm_events';
  $handler->display->display_options['relationships']['field_mm_events_nid']['field'] = 'field_mm_events_nid';
  $handler->display->display_options['relationships']['field_mm_events_nid']['label'] = 'Individual events';
  $handler->display->display_options['relationships']['field_mm_events_nid']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'field_mm_events_nid';
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['relationship'] = 'field_mm_events_nid';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: Individual events */
  $handler->display->display_options['arguments']['date_argument_1']['id'] = 'date_argument_1';
  $handler->display->display_options['arguments']['date_argument_1']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument_1']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['arguments']['date_argument_1']['ui_name'] = 'Individual events';
  $handler->display->display_options['arguments']['date_argument_1']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument_1']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument_1']['date_fields'] = array(
    'field_data_field_start_datetime.field_start_datetime_value' => 'field_data_field_start_datetime.field_start_datetime_value',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'mm_event' => 'mm_event',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: MM: Recycled */
  $handler->display->display_options['filters']['recycled']['id'] = 'recycled';
  $handler->display->display_options['filters']['recycled']['table'] = 'mm_recycle';
  $handler->display->display_options['filters']['recycled']['field'] = 'recycled';
  $handler->display->display_options['filters']['recycled']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['recycled']['value'] = '0';
  $handler->display->display_options['filters']['recycled']['group'] = 1;
  $handler->display->display_options['displays'] = array(
    'month_grid' => 'month_grid',
    'month_list' => 'month_list',
    'day_grid' => 'day_grid',
    'day_list' => 'day_list',
    'year_grid' => 'year_grid',
    'year_list' => 'year_list',
    'week_grid' => 'week_grid',
    'week_list' => 'week_list',
    'ical' => 'ical',
    'default' => 0,
  );
  $handler->display->display_options['inherit_exposed_filters'] = TRUE;

  /* Display: Nodes on MM Page(s) */
  $handler = $view->new_display('attachment', 'Nodes on MM Page(s)', 'attachment_2');
  $handler->display->display_options['display_comment'] = 'Nodes from this attachment are combined with any other attachments to form the result set for any of the grid/list/etc pages. When you create a new page view, you must check it in the "Attach to" setting of all attachments.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: All events on these page(s) */
  $handler->display->display_options['relationships']['field_mm_pages_value']['id'] = 'field_mm_pages_value';
  $handler->display->display_options['relationships']['field_mm_pages_value']['table'] = 'field_data_field_mm_pages';
  $handler->display->display_options['relationships']['field_mm_pages_value']['field'] = 'field_mm_pages_value';
  $handler->display->display_options['relationships']['field_mm_pages_value']['ui_name'] = 'All events on these page(s)';
  $handler->display->display_options['relationships']['field_mm_pages_value']['label'] = 'All events on these page(s)';
  /* Relationship: MM: Nodes on MM page */
  $handler->display->display_options['relationships']['mmtid']['id'] = 'mmtid';
  $handler->display->display_options['relationships']['mmtid']['table'] = 'mm_node2tree';
  $handler->display->display_options['relationships']['mmtid']['field'] = 'mmtid';
  $handler->display->display_options['relationships']['mmtid']['relationship'] = 'field_mm_pages_value';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'mmtid';
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime']['id'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['field_start_datetime']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body_1']['id'] = 'body_1';
  $handler->display->display_options['fields']['body_1']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body_1']['field'] = 'body';
  $handler->display->display_options['fields']['body_1']['relationship'] = 'mmtid';
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid_1']['id'] = 'vid_1';
  $handler->display->display_options['fields']['vid_1']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid_1']['field'] = 'vid';
  $handler->display->display_options['fields']['vid_1']['relationship'] = 'mmtid';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'node';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['type_1']['label'] = '';
  $handler->display->display_options['fields']['type_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: Nodes on MM page(s) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'node';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['relationship'] = 'mmtid';
  $handler->display->display_options['arguments']['date_argument']['ui_name'] = 'Nodes on MM page(s)';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'field_data_field_start_datetime.field_start_datetime_value' => 'field_data_field_start_datetime.field_start_datetime_value',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'mmtid';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'mmtid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'mm_event' => 'mm_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  $handler->display->display_options['displays'] = array(
    'month_grid' => 'month_grid',
    'month_list' => 'month_list',
    'day_grid' => 'day_grid',
    'day_list' => 'day_list',
    'year_grid' => 'year_grid',
    'year_list' => 'year_list',
    'week_grid' => 'week_grid',
    'week_list' => 'week_list',
    'ical' => 'ical',
    'default' => 0,
  );
  $handler->display->display_options['inherit_exposed_filters'] = TRUE;

  /* Display: Individual Events Refs */
  $handler = $view->new_display('references', 'Individual Events Refs', 'references_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['display_description'] = 'Helper for node references typeahead';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'references_style';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'references_fields';
  $handler->display->display_options['row_options']['separator'] = ':';
  $handler->display->display_options['row_options']['hide_empty'] = TRUE;
  $handler->display->display_options['row_options']['default_field_elements'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'field_start_datetime_default',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'hide',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'mm_event' => 'mm_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: MM: Recycled */
  $handler->display->display_options['filters']['recycled']['id'] = 'recycled';
  $handler->display->display_options['filters']['recycled']['table'] = 'mm_recycle';
  $handler->display->display_options['filters']['recycled']['field'] = 'recycled';
  $handler->display->display_options['filters']['recycled']['value'] = '0';
  $handler->display->display_options['filters']['recycled']['group'] = 1;
  /* Filter criterion: Content: Date/time - end date (field_start_datetime:value2) */
  $handler->display->display_options['filters']['field_start_datetime_value2']['id'] = 'field_start_datetime_value2';
  $handler->display->display_options['filters']['field_start_datetime_value2']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['filters']['field_start_datetime_value2']['field'] = 'field_start_datetime_value2';
  $handler->display->display_options['filters']['field_start_datetime_value2']['operator'] = '>=';
  $handler->display->display_options['filters']['field_start_datetime_value2']['default_date'] = 'now';
  $export['mm_calendar'] = $view;

  $view = new view();
  $view->name = 'mm_calendar_export';
  $view->description = 'A calendar view for RSS feeds and upcoming events';
  $view->tag = 'Calendar, Monster Menus';
  $view->base_table = 'node';
  $view->human_name = 'mm_calendar Export/Upcoming';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'upcoming';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'month';
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '1';
  $handler->display->display_options['style_options']['max_items'] = '10';
  $handler->display->display_options['style_options']['multiday_hidden'] = array(
    'field_start_datetime' => 'field_start_datetime',
  );
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['label'] = '';
  $handler->display->display_options['fields']['vid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['vid']['element_label_colon'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Sort criterion: Content: Date/time -  start date (field_start_datetime) */
  $handler->display->display_options['sorts']['field_start_datetime_value']['id'] = 'field_start_datetime_value';
  $handler->display->display_options['sorts']['field_start_datetime_value']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['sorts']['field_start_datetime_value']['field'] = 'field_start_datetime_value';
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';

  /* Display: RSS Feed */
  $handler = $view->new_display('feed', 'RSS Feed', 'feed');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'rss_fields';
  $handler->display->display_options['style_options']['channel'] = array(
    'core' => array(
      'views_rss_core' => array(
        'description' => '',
        'language' => 'en_us',
        'category' => '',
        'image' => '',
        'copyright' => '',
        'managingEditor' => '',
        'webMaster' => '',
        'generator' => '',
        'docs' => '',
        'cloud' => '',
        'ttl' => '',
        'skipHours' => '',
        'skipDays' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['item'] = array(
    'core' => array(
      'views_rss_core' => array(
        'title' => 'title',
        'link' => 'nid',
        'description' => 'body',
        'author' => '',
        'category' => '',
        'comments' => '',
        'enclosure' => '',
        'guid' => 'nid',
        'pubDate' => 'changed',
      ),
    ),
    'xCal' => array(
      'views_rss_xcal' => array(
        'dtstart' => 'field_start_datetime_1',
        'dtend' => 'field_start_datetime',
        'summary' => 'title',
        'location' => '',
        'uid' => 'nid',
      ),
    ),
  );
  $handler->display->display_options['style_options']['feed_settings'] = array(
    'absolute_paths' => 1,
    'feed_in_links' => 0,
  );
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Start date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['ui_name'] = 'Start date/time';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['alter']['text'] = '[field_start_datetime_1]';
  $handler->display->display_options['fields']['field_start_datetime_1']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'ical',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'hide',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['label'] = '';
  $handler->display->display_options['fields']['vid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['vid']['element_label_colon'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: End date/time */
  $handler->display->display_options['fields']['field_start_datetime']['id'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['ui_name'] = 'End date/time';
  $handler->display->display_options['fields']['field_start_datetime']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_start_datetime']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['settings'] = array(
    'format_type' => 'ical',
    'fromto' => 'value2',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime']['delta_offset'] = '0';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'https://www.amherst.edu/node/[nid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'feed';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  $handler->display->display_options['path'] = '.mm_calendar/feed';

  /* Display: Upcoming */
  $handler = $view->new_display('page', 'Upcoming', 'upcoming');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_schedule_list';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'mm_calendar_entity';
  $handler->display->display_options['row_options']['calendar_date_link'] = 'mm_event';
  $handler->display->display_options['row_options']['colors']['calendar_colors_type'] = array(
    'amherst_book' => '#ffffff',
    'amherst_publisher' => '#ffffff',
    'story' => '#ffffff',
    'biblio' => '#ffffff',
    'bloglink' => '#ffffff',
    'classics_collection' => '#ffffff',
    'mm_college_event' => '#ffffff',
    'course' => '#ffffff',
    'course_assign' => '#ffffff',
    'course_xlist' => '#ffffff',
    'course_links' => '#ffffff',
    'crs_guide' => '#ffffff',
    'date' => '#ffffff',
    'digital_sign' => '#ffffff',
    'dining_hall_menu_item' => '#ffffff',
    'mm_event' => '#ffffff',
    'mm_calendar' => '#ffffff',
    'featured_content_slider' => '#ffffff',
    'featured_content_slider_item' => '#ffffff',
    'mm_media' => '#ffffff',
    'forena_node' => '#ffffff',
    'gallery' => '#ffffff',
    'knowledge_base' => '#ffffff',
    'multimedia' => '#ffffff',
    'news_story' => '#ffffff',
    'course_noncourse' => '#ffffff',
    'rss_page' => '#ffffff',
    'redirect' => '#ffffff',
    'reunion_event' => '#ffffff',
    'roster' => '#ffffff',
    'service_offering' => '#ffffff',
    'service_outage' => '#ffffff',
    'study_abroad_program' => '#ffffff',
    'subpglist' => '#ffffff',
    'alert' => '#ffffff',
    'test' => '#ffffff',
    'viewref' => '#ffffff',
    'webform' => '#ffffff',
    'new_media' => '#ffffff',
    'portal_page' => '#ffffff',
  );
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['empty'] = FALSE;
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'There are no upcoming events.';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Start date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['ui_name'] = 'Start date/time';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['alter']['text'] = '[field_start_datetime_1]';
  $handler->display->display_options['fields']['field_start_datetime_1']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'ical',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'hide',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['label'] = '';
  $handler->display->display_options['fields']['body']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['body']['element_label_colon'] = FALSE;
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['label'] = '';
  $handler->display->display_options['fields']['vid']['exclude'] = TRUE;
  $handler->display->display_options['fields']['vid']['element_label_colon'] = FALSE;
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: End date/time */
  $handler->display->display_options['fields']['field_start_datetime']['id'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['ui_name'] = 'End date/time';
  $handler->display->display_options['fields']['field_start_datetime']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['field_start_datetime']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['settings'] = array(
    'format_type' => 'ical',
    'fromto' => 'value2',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime']['delta_offset'] = '0';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['nid']['alter']['text'] = 'https://www.amherst.edu/node/[nid]';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['nid']['element_default_classes'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'feed';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  /* Contextual filter: dummy date arg */
  $handler->display->display_options['arguments']['field_start_datetime_value2']['id'] = 'field_start_datetime_value2';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['field'] = 'field_start_datetime_value2';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['ui_name'] = 'dummy date arg';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['default_action'] = 'default';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['default_argument_skip_url'] = TRUE;
  $handler->display->display_options['arguments']['field_start_datetime_value2']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_start_datetime_value2']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $handler->display->display_options['path'] = '.mm_calendar/upcoming';

  /* Display: Individual Events */
  $handler = $view->new_display('attachment', 'Individual Events', 'attachment_1');
  $handler->display->display_options['display_comment'] = 'Nodes from this attachment are combined with any other attachments to form the result set for any of the grid/list/etc pages. When you create a new page view, you must check it in the "Attach to" setting of all attachments.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_rss_fields';
  $handler->display->display_options['style_options']['channel'] = array(
    'core' => array(
      'views_rss_core' => array(
        'description' => '',
        'language' => '',
        'category' => '',
        'image' => '',
        'copyright' => '',
        'managingEditor' => '',
        'webMaster' => '',
        'generator' => '',
        'docs' => '',
        'cloud' => '',
        'ttl' => '',
        'skipHours' => '',
        'skipDays' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['item'] = array(
    'core' => array(
      'views_rss_core' => array(
        'title' => '',
        'link' => '',
        'description' => '',
        'author' => '',
        'category' => '',
        'comments' => '',
        'enclosure' => '',
        'guid' => '',
        'pubDate' => '',
      ),
    ),
    'xCal' => array(
      'views_rss_xcal' => array(
        'dtstart' => '',
        'dtend' => '',
        'summary' => '',
        'location' => '',
        'uid' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['feed_settings'] = array(
    'absolute_paths' => 1,
    'feed_in_links' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: Content: Individual events (field_mm_events) */
  $handler->display->display_options['relationships']['field_mm_events_nid']['id'] = 'field_mm_events_nid';
  $handler->display->display_options['relationships']['field_mm_events_nid']['table'] = 'field_data_field_mm_events';
  $handler->display->display_options['relationships']['field_mm_events_nid']['field'] = 'field_mm_events_nid';
  $handler->display->display_options['relationships']['field_mm_events_nid']['label'] = 'Individual events';
  $handler->display->display_options['relationships']['field_mm_events_nid']['delta'] = '-1';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body']['id'] = 'body';
  $handler->display->display_options['fields']['body']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body']['field'] = 'body';
  $handler->display->display_options['fields']['body']['relationship'] = 'field_mm_events_nid';
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid']['id'] = 'vid';
  $handler->display->display_options['fields']['vid']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid']['field'] = 'vid';
  $handler->display->display_options['fields']['vid']['relationship'] = 'field_mm_events_nid';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'node';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['type']['label'] = '';
  $handler->display->display_options['fields']['type']['element_label_colon'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime']['id'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['field_start_datetime']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime']['delta_offset'] = '0';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['fields']['changed']['label'] = '';
  $handler->display->display_options['fields']['changed']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status_1']['id'] = 'status_1';
  $handler->display->display_options['filters']['status_1']['table'] = 'node';
  $handler->display->display_options['filters']['status_1']['field'] = 'status';
  $handler->display->display_options['filters']['status_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['status_1']['value'] = '1';
  $handler->display->display_options['filters']['status_1']['group'] = 1;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
  $handler->display->display_options['filters']['type_1']['table'] = 'node';
  $handler->display->display_options['filters']['type_1']['field'] = 'type';
  $handler->display->display_options['filters']['type_1']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['type_1']['value'] = array(
    'mm_event' => 'mm_event',
  );
  $handler->display->display_options['filters']['type_1']['group'] = 1;
  /* Filter criterion: MM: Recycled */
  $handler->display->display_options['filters']['recycled']['id'] = 'recycled';
  $handler->display->display_options['filters']['recycled']['table'] = 'mm_recycle';
  $handler->display->display_options['filters']['recycled']['field'] = 'recycled';
  $handler->display->display_options['filters']['recycled']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['recycled']['value'] = '0';
  $handler->display->display_options['filters']['recycled']['group'] = 1;
  /* Filter criterion: Content: Date/time - end date (field_start_datetime:value2) */
  $handler->display->display_options['filters']['field_start_datetime_value2']['id'] = 'field_start_datetime_value2';
  $handler->display->display_options['filters']['field_start_datetime_value2']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['filters']['field_start_datetime_value2']['field'] = 'field_start_datetime_value2';
  $handler->display->display_options['filters']['field_start_datetime_value2']['relationship'] = 'field_mm_events_nid';
  $handler->display->display_options['filters']['field_start_datetime_value2']['operator'] = '>=';
  $handler->display->display_options['filters']['field_start_datetime_value2']['granularity'] = 'second';
  $handler->display->display_options['filters']['field_start_datetime_value2']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['field_start_datetime_value2']['default_date'] = 'now';
  $handler->display->display_options['displays'] = array(
    'feed' => 'feed',
    'upcoming' => 'upcoming',
    'default' => 0,
  );

  /* Display: Nodes on MM Page(s) */
  $handler = $view->new_display('attachment', 'Nodes on MM Page(s)', 'attachment_2');
  $handler->display->display_options['display_comment'] = 'Nodes from this attachment are combined with any other attachments to form the result set for any of the grid/list/etc pages. When you create a new page view, you must check it in the "Attach to" setting of all attachments.';
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'mm_rss_fields';
  $handler->display->display_options['style_options']['channel'] = array(
    'core' => array(
      'views_rss_core' => array(
        'description' => '',
        'language' => '',
        'category' => '',
        'image' => '',
        'copyright' => '',
        'managingEditor' => '',
        'webMaster' => '',
        'generator' => '',
        'docs' => '',
        'cloud' => '',
        'ttl' => '',
        'skipHours' => '',
        'skipDays' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['item'] = array(
    'core' => array(
      'views_rss_core' => array(
        'title' => '',
        'link' => '',
        'description' => '',
        'author' => '',
        'category' => '',
        'comments' => '',
        'enclosure' => '',
        'guid' => '',
        'pubDate' => '',
      ),
    ),
    'xCal' => array(
      'views_rss_xcal' => array(
        'dtstart' => '',
        'dtend' => '',
        'summary' => '',
        'location' => '',
        'uid' => '',
      ),
    ),
  );
  $handler->display->display_options['style_options']['feed_settings'] = array(
    'absolute_paths' => 1,
    'feed_in_links' => 0,
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  /* Relationship: All events on these page(s) */
  $handler->display->display_options['relationships']['field_mm_pages_value']['id'] = 'field_mm_pages_value';
  $handler->display->display_options['relationships']['field_mm_pages_value']['table'] = 'field_data_field_mm_pages';
  $handler->display->display_options['relationships']['field_mm_pages_value']['field'] = 'field_mm_pages_value';
  $handler->display->display_options['relationships']['field_mm_pages_value']['ui_name'] = 'All events on these page(s)';
  $handler->display->display_options['relationships']['field_mm_pages_value']['label'] = 'All events on these page(s)';
  /* Relationship: MM: Nodes on MM page */
  $handler->display->display_options['relationships']['mmtid']['id'] = 'mmtid';
  $handler->display->display_options['relationships']['mmtid']['table'] = 'mm_node2tree';
  $handler->display->display_options['relationships']['mmtid']['field'] = 'mmtid';
  $handler->display->display_options['relationships']['mmtid']['relationship'] = 'field_mm_pages_value';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title_1']['id'] = 'title_1';
  $handler->display->display_options['fields']['title_1']['table'] = 'node';
  $handler->display->display_options['fields']['title_1']['field'] = 'title';
  $handler->display->display_options['fields']['title_1']['relationship'] = 'mmtid';
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime']['id'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['field_start_datetime']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime']['group_columns'] = array(
    'entity_id' => 'entity_id',
  );
  $handler->display->display_options['fields']['field_start_datetime']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime']['delta_offset'] = '0';
  /* Field: Content: Body */
  $handler->display->display_options['fields']['body_1']['id'] = 'body_1';
  $handler->display->display_options['fields']['body_1']['table'] = 'field_data_body';
  $handler->display->display_options['fields']['body_1']['field'] = 'body';
  $handler->display->display_options['fields']['body_1']['relationship'] = 'mmtid';
  /* Field: Content revision: Vid */
  $handler->display->display_options['fields']['vid_1']['id'] = 'vid_1';
  $handler->display->display_options['fields']['vid_1']['table'] = 'node_revision';
  $handler->display->display_options['fields']['vid_1']['field'] = 'vid';
  $handler->display->display_options['fields']['vid_1']['relationship'] = 'mmtid';
  /* Field: Content: Type */
  $handler->display->display_options['fields']['type_1']['id'] = 'type_1';
  $handler->display->display_options['fields']['type_1']['table'] = 'node';
  $handler->display->display_options['fields']['type_1']['field'] = 'type';
  $handler->display->display_options['fields']['type_1']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['type_1']['label'] = '';
  $handler->display->display_options['fields']['type_1']['element_label_colon'] = FALSE;
  /* Field: Content: Date/time */
  $handler->display->display_options['fields']['field_start_datetime_1']['id'] = 'field_start_datetime_1';
  $handler->display->display_options['fields']['field_start_datetime_1']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['field'] = 'field_start_datetime';
  $handler->display->display_options['fields']['field_start_datetime_1']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['field_start_datetime_1']['label'] = '';
  $handler->display->display_options['fields']['field_start_datetime_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_start_datetime_1']['settings'] = array(
    'format_type' => 'long',
    'fromto' => 'both',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
    'show_repeat_rule' => 'show',
  );
  $handler->display->display_options['fields']['field_start_datetime_1']['delta_offset'] = '0';
  /* Field: Content: Nid */
  $handler->display->display_options['fields']['nid']['id'] = 'nid';
  $handler->display->display_options['fields']['nid']['table'] = 'node';
  $handler->display->display_options['fields']['nid']['field'] = 'nid';
  $handler->display->display_options['fields']['nid']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['nid']['label'] = '';
  $handler->display->display_options['fields']['nid']['element_label_colon'] = FALSE;
  /* Field: Content: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'node';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['relationship'] = 'mmtid';
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: nid of mm_calendar */
  $handler->display->display_options['arguments']['nid']['id'] = 'nid';
  $handler->display->display_options['arguments']['nid']['table'] = 'node';
  $handler->display->display_options['arguments']['nid']['field'] = 'nid';
  $handler->display->display_options['arguments']['nid']['ui_name'] = 'nid of mm_calendar';
  $handler->display->display_options['arguments']['nid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['nid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['nid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['nid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['nid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['nid']['break_phrase'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['filter_groups']['operator'] = 'OR';
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'mmtid';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'mmtid';
  $handler->display->display_options['filters']['type']['value'] = array(
    'mm_event' => 'mm_event',
  );
  $handler->display->display_options['filters']['type']['group'] = 1;
  /* Filter criterion: Content: Date/time - end date (field_start_datetime:value2) */
  $handler->display->display_options['filters']['field_start_datetime_value2']['id'] = 'field_start_datetime_value2';
  $handler->display->display_options['filters']['field_start_datetime_value2']['table'] = 'field_data_field_start_datetime';
  $handler->display->display_options['filters']['field_start_datetime_value2']['field'] = 'field_start_datetime_value2';
  $handler->display->display_options['filters']['field_start_datetime_value2']['relationship'] = 'mmtid';
  $handler->display->display_options['filters']['field_start_datetime_value2']['operator'] = '>=';
  $handler->display->display_options['filters']['field_start_datetime_value2']['granularity'] = 'second';
  $handler->display->display_options['filters']['field_start_datetime_value2']['form_type'] = 'date_text';
  $handler->display->display_options['filters']['field_start_datetime_value2']['default_date'] = 'now';
  $handler->display->display_options['displays'] = array(
    'feed' => 'feed',
    'upcoming' => 'upcoming',
    'default' => 0,
  );
  $export['mm_calendar_export'] = $view;

  return $export;
}
