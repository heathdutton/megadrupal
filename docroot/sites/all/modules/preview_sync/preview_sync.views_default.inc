<?php
/**
 * @file
 * preview_sync.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function preview_sync_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'preview_sync_history';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'preview_sync_log';
  $view->human_name = 'Preview Sync history';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Preview Sync history';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'pid' => 'pid',
    'qid' => 'qid',
    'start' => 'start',
    'duration' => 'duration',
    'type' => 'type',
    'status' => 'status',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = 'qid';
  $handler->display->display_options['style_options']['info'] = array(
    'pid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'qid' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'start' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'duration' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'message' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['summary'] = 'Preview Sync recent history';
  /* Field: Preview Sync: Batch ID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  /* Field: Preview Sync: Item ID */
  $handler->display->display_options['fields']['qid']['id'] = 'qid';
  $handler->display->display_options['fields']['qid']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['qid']['field'] = 'qid';
  /* Field: Preview Sync: Start date */
  $handler->display->display_options['fields']['start']['id'] = 'start';
  $handler->display->display_options['fields']['start']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['start']['field'] = 'start';
  $handler->display->display_options['fields']['start']['date_format'] = 'short';
  $handler->display->display_options['fields']['start']['second_date_format'] = 'long';
  /* Field: Preview Sync: Item duration */
  $handler->display->display_options['fields']['duration']['id'] = 'duration';
  $handler->display->display_options['fields']['duration']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['duration']['field'] = 'duration';
  $handler->display->display_options['fields']['duration']['label'] = 'Duration';
  $handler->display->display_options['fields']['duration']['suffix'] = 'ms';
  /* Field: Preview Sync: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Preview Sync: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Preview Sync: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'preview_sync_log';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['trim_whitespace'] = TRUE;
  $export['preview_sync_history'] = $view;

  return $export;
}
