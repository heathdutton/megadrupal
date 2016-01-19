<?php
/**
 * @file
 * cm_tv_schedules.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function cm_tv_schedules_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'tv_schedules';
  $view->description = '';
  $view->tag = 'Calendar';
  $view->base_table = 'cm_airing';
  $view->human_name = 'TV Schedules';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['link_display'] = 'page_1';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['expose_sort_order'] = FALSE;
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'date_views_pager';
  $handler->display->display_options['pager']['options']['date_id'] = 'month';
  $handler->display->display_options['style_plugin'] = 'calendar_style';
  $handler->display->display_options['style_options']['calendar_type'] = 'day';
  $handler->display->display_options['style_options']['name_size'] = '3';
  $handler->display->display_options['style_options']['mini'] = '0';
  $handler->display->display_options['style_options']['with_weekno'] = '0';
  $handler->display->display_options['style_options']['multiday_theme'] = '1';
  $handler->display->display_options['style_options']['theme_style'] = '0';
  $handler->display->display_options['style_options']['max_items'] = '0';
  $handler->display->display_options['style_options']['groupby_times'] = 'half';
  $handler->display->display_options['style_options']['groupby_field'] = 'field_airing_channel';
  $handler->display->display_options['row_plugin'] = 'calendar_entity';
  /* Field: Airing: Date */
  $handler->display->display_options['fields']['field_airing_date']['id'] = 'field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['table'] = 'field_data_field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['field'] = 'field_airing_date';
  $handler->display->display_options['fields']['field_airing_date']['label'] = '';
  $handler->display->display_options['fields']['field_airing_date']['element_class'] = 'airing-date';
  $handler->display->display_options['fields']['field_airing_date']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_airing_date']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_airing_date']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['fields']['field_airing_date']['settings'] = array(
    'format_type' => 'short',
    'fromto' => 'value',
    'multiple_number' => '',
    'multiple_from' => '',
    'multiple_to' => '',
  );
  $handler->display->display_options['fields']['field_airing_date']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_airing_date']['delta_offset'] = '0';
  /* Field: Airing: Channel */
  $handler->display->display_options['fields']['field_airing_channel']['id'] = 'field_airing_channel';
  $handler->display->display_options['fields']['field_airing_channel']['table'] = 'field_data_field_airing_channel';
  $handler->display->display_options['fields']['field_airing_channel']['field'] = 'field_airing_channel';
  $handler->display->display_options['fields']['field_airing_channel']['label'] = '';
  $handler->display->display_options['fields']['field_airing_channel']['exclude'] = TRUE;
  $handler->display->display_options['fields']['field_airing_channel']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_airing_channel']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_airing_channel']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_airing_channel']['delta_offset'] = '0';
  /* Field: Airing: Project */
  $handler->display->display_options['fields']['field_airing_project_ref']['id'] = 'field_airing_project_ref';
  $handler->display->display_options['fields']['field_airing_project_ref']['table'] = 'field_data_field_airing_project_ref';
  $handler->display->display_options['fields']['field_airing_project_ref']['field'] = 'field_airing_project_ref';
  $handler->display->display_options['fields']['field_airing_project_ref']['label'] = '';
  $handler->display->display_options['fields']['field_airing_project_ref']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_airing_project_ref']['alter']['text'] = '[field_airing_project_ref] - ';
  $handler->display->display_options['fields']['field_airing_project_ref']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_airing_project_ref']['element_class'] = 'tv-series';
  $handler->display->display_options['fields']['field_airing_project_ref']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_airing_project_ref']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_airing_project_ref']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_airing_project_ref']['settings'] = array(
    'link' => 1,
  );
  /* Field: Airing: Show */
  $handler->display->display_options['fields']['field_airing_show_ref']['id'] = 'field_airing_show_ref';
  $handler->display->display_options['fields']['field_airing_show_ref']['table'] = 'field_data_field_airing_show_ref';
  $handler->display->display_options['fields']['field_airing_show_ref']['field'] = 'field_airing_show_ref';
  $handler->display->display_options['fields']['field_airing_show_ref']['label'] = '';
  $handler->display->display_options['fields']['field_airing_show_ref']['element_type'] = 'span';
  $handler->display->display_options['fields']['field_airing_show_ref']['element_class'] = 'tv-show';
  $handler->display->display_options['fields']['field_airing_show_ref']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['field_airing_show_ref']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['field_airing_show_ref']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['field_airing_show_ref']['settings'] = array(
    'link' => 1,
  );
  /* Sort criterion: Airing: Date -  start date (field_airing_date) */
  $handler->display->display_options['sorts']['field_airing_date_value']['id'] = 'field_airing_date_value';
  $handler->display->display_options['sorts']['field_airing_date_value']['table'] = 'field_data_field_airing_date';
  $handler->display->display_options['sorts']['field_airing_date_value']['field'] = 'field_airing_date_value';
  /* Contextual filter: Date: Date (cm_airing) */
  $handler->display->display_options['arguments']['date_argument']['id'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['table'] = 'cm_airing';
  $handler->display->display_options['arguments']['date_argument']['field'] = 'date_argument';
  $handler->display->display_options['arguments']['date_argument']['default_action'] = 'default';
  $handler->display->display_options['arguments']['date_argument']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['date_argument']['granularity'] = 'day';
  $handler->display->display_options['arguments']['date_argument']['date_fields'] = array(
    'field_data_field_airing_date.field_airing_date_value' => 'field_data_field_airing_date.field_airing_date_value',
  );

  /* Display: Day */
  $handler = $view->new_display('page', 'Day', 'day');
  $handler->display->display_options['path'] = 'schedules';
  $handler->display->display_options['menu']['title'] = 'Day';
  $handler->display->display_options['menu']['weight'] = '3';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $export['tv_schedules'] = $view;

  return $export;
}
