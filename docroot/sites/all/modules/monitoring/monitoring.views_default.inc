<?php
/**
 * @file
 * Default views for monitoring admin UI.
 */

/**
 * Implements hook_views_default_views().
 */
function monitoring_views_default_views() {

  $view = new view();
  $view->name = 'monitoring_sensor_results';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'monitoring_sensor_result';
  $view->human_name = 'Monitoring sensor results';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Sensor calls log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'monitoring reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'mini';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'timestamp' => 'timestamp',
    'sensor_status' => 'sensor_status',
    'sensor_message' => 'sensor_message',
    'execution_time' => 'execution_time',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'timestamp' => array(
      'sortable' => 0,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sensor_status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'sensor_message' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'execution_time' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'No log entries text';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No logs available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Monitoring sensor result: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: Monitoring sensor result: Sensor_status */
  $handler->display->display_options['fields']['sensor_status']['id'] = 'sensor_status';
  $handler->display->display_options['fields']['sensor_status']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['sensor_status']['field'] = 'sensor_status';
  $handler->display->display_options['fields']['sensor_status']['label'] = 'Status';
  /* Field: Monitoring sensor result: Sensor_message */
  $handler->display->display_options['fields']['sensor_message']['id'] = 'sensor_message';
  $handler->display->display_options['fields']['sensor_message']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['sensor_message']['field'] = 'sensor_message';
  $handler->display->display_options['fields']['sensor_message']['label'] = 'Status message';
  /* Field: Monitoring sensor result: Execution_time */
  $handler->display->display_options['fields']['execution_time']['id'] = 'execution_time';
  $handler->display->display_options['fields']['execution_time']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['execution_time']['field'] = 'execution_time';
  $handler->display->display_options['fields']['execution_time']['label'] = 'Execution time';
  $handler->display->display_options['fields']['execution_time']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['execution_time']['precision'] = '2';
  $handler->display->display_options['fields']['execution_time']['suffix'] = 'ms';
  /* Sort criterion: Monitoring sensor result: Timestamp */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Sort criterion: Monitoring sensor result: Monitoring sensor result ID */
  $handler->display->display_options['sorts']['record_id']['id'] = 'record_id';
  $handler->display->display_options['sorts']['record_id']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['sorts']['record_id']['field'] = 'record_id';
  $handler->display->display_options['sorts']['record_id']['order'] = 'DESC';
  /* Contextual filter: Monitoring sensor result: Sensor_name */
  $handler->display->display_options['arguments']['sensor_name']['id'] = 'sensor_name';
  $handler->display->display_options['arguments']['sensor_name']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['arguments']['sensor_name']['field'] = 'sensor_name';
  $handler->display->display_options['arguments']['sensor_name']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['sensor_name']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['sensor_name']['title'] = '%1';
  $handler->display->display_options['arguments']['sensor_name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['sensor_name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['sensor_name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['sensor_name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['sensor_name']['limit'] = '0';

  /* Display: Overview */
  $handler = $view->new_display('page', 'Overview', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'All sensors calls log';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Monitoring sensor result: Sensor_name */
  $handler->display->display_options['fields']['sensor_name']['id'] = 'sensor_name';
  $handler->display->display_options['fields']['sensor_name']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['sensor_name']['field'] = 'sensor_name';
  $handler->display->display_options['fields']['sensor_name']['label'] = 'Sensor name';
  /* Field: Monitoring sensor result: Sensor_status */
  $handler->display->display_options['fields']['sensor_status']['id'] = 'sensor_status';
  $handler->display->display_options['fields']['sensor_status']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['sensor_status']['field'] = 'sensor_status';
  $handler->display->display_options['fields']['sensor_status']['label'] = 'Status';
  /* Field: Monitoring sensor result: Sensor_message */
  $handler->display->display_options['fields']['sensor_message']['id'] = 'sensor_message';
  $handler->display->display_options['fields']['sensor_message']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['sensor_message']['field'] = 'sensor_message';
  $handler->display->display_options['fields']['sensor_message']['label'] = 'Status message';
  $handler->display->display_options['fields']['sensor_message']['alter']['max_length'] = '255';
  $handler->display->display_options['fields']['sensor_message']['alter']['trim'] = TRUE;
  /* Field: Monitoring sensor result: Execution_time */
  $handler->display->display_options['fields']['execution_time']['id'] = 'execution_time';
  $handler->display->display_options['fields']['execution_time']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['execution_time']['field'] = 'execution_time';
  $handler->display->display_options['fields']['execution_time']['label'] = 'Execution time';
  $handler->display->display_options['fields']['execution_time']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['execution_time']['alter']['text'] = '[execution_time]ms';
  $handler->display->display_options['fields']['execution_time']['precision'] = '0';
  /* Field: Monitoring sensor result: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'monitoring_sensor_result';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['path'] = 'admin/reports/monitoring/log';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Log';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Detail Page Log */
  $handler = $view->new_display('block', 'Detail Page Log', 'detail_page_log');
  $translatables['monitoring_sensor_results'] = array(
    t('Master'),
    t('Sensor calls log'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('No log entries text'),
    t('No logs available.'),
    t('Timestamp'),
    t('Status'),
    t('Status message'),
    t('Execution time'),
    t('.'),
    t(','),
    t('ms'),
    t('All'),
    t('%1'),
    t('Overview'),
    t('All sensors calls log'),
    t('Sensor name'),
    t('[execution_time]ms'),
    t('Detail Page Log'),
  );

  $views[$view->name] = $view;

  return $views;
}
