<?php

/**
 * Implement hook_views_default_views().
 */
function tracekit_views_default_views() {
  $view = new view();
  $view->name = 'tracekit_reports';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'tracekit_report';
  $view->human_name = 'TraceKit reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'TraceKit reports';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view tracekit reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'tracekit_report_id' => 'tracekit_report_id',
    'mode' => 'mode',
    'created' => 'created',
    'message' => 'message',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'tracekit_report_id' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'mode' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'message' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
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
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No reports available.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Relationship: TraceKit reports: User who has reported the error. */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'tracekit_report';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: TraceKit reports: TraceKit report id */
  $handler->display->display_options['fields']['tracekit_report_id']['id'] = 'tracekit_report_id';
  $handler->display->display_options['fields']['tracekit_report_id']['table'] = 'tracekit_report';
  $handler->display->display_options['fields']['tracekit_report_id']['field'] = 'tracekit_report_id';
  $handler->display->display_options['fields']['tracekit_report_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['tracekit_report_id']['separator'] = '';
  /* Field: TraceKit reports: Mode */
  $handler->display->display_options['fields']['mode']['id'] = 'mode';
  $handler->display->display_options['fields']['mode']['table'] = 'tracekit_report';
  $handler->display->display_options['fields']['mode']['field'] = 'mode';
  /* Field: TraceKit reports: Date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'tracekit_report';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: TraceKit reports: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'tracekit_report';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['path'] = 'admin/reports/tracekit/details/[tracekit_report_id]';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Filter criterion: TraceKit reports: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'tracekit_report';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'word';
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  
  /* Display: Page (Parent) */
  $handler = $view->new_display('page', 'Page (Parent)', 'page');
  $handler->display->display_options['path'] = 'admin/reports/tracekit';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'TraceKit reports';
  $handler->display->display_options['menu']['description'] = 'View collected TraceKit reports.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  
  /* Display: Messages */
  $handler = $view->new_display('page', 'Messages', 'page_2');
  $handler->display->display_options['path'] = 'admin/reports/tracekit/messages';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Messages';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  
  /* Display: Most frequent */
  $handler = $view->new_display('page', 'Most frequent', 'page_1');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: TraceKit reports: Message */
  $handler->display->display_options['arguments']['message']['id'] = 'message';
  $handler->display->display_options['arguments']['message']['table'] = 'tracekit_report';
  $handler->display->display_options['arguments']['message']['field'] = 'message';
  $handler->display->display_options['arguments']['message']['default_action'] = 'summary';
  $handler->display->display_options['arguments']['message']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['message']['summary']['sort_order'] = 'desc';
  $handler->display->display_options['arguments']['message']['summary']['number_of_records'] = '1';
  $handler->display->display_options['arguments']['message']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['message']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['message']['limit'] = '0';
  $handler->display->display_options['path'] = 'admin/reports/tracekit/summary';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Most frequent';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['tracekit_reports'] = array(
    t('Master'),
    t('TraceKit reports'),
    t('more'),
    t('Filter'),
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
    t('No reports available.'),
    t('Reporting user'),
    t('TraceKit report id'),
    t('.'),
    t('Mode'),
    t('Date'),
    t('Message'),
    t('User'),
    t('Page (Parent)'),
    t('Messages'),
    t('Most frequent'),
    t('All'),
  );

  $views[$view->name] = $view;

  return $views;
}
