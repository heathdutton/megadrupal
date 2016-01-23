<?php

function past_db_views_default_views() {

  $view = new view();
  $view->name = 'past_event_log';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'past_event';
  $view->human_name = 'Past event log';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Past event log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer past';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'event_id' => 'event_id',
    'severity' => 'severity',
    'timestamp' => 'timestamp',
    'module' => 'module',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'event_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'severity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'module' => array(
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
  /* Relationship: Past Event: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'past_event';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Past Event: Past event ID */
  $handler->display->display_options['fields']['event_id']['id'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['table'] = 'past_event';
  $handler->display->display_options['fields']['event_id']['field'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['label'] = 'ID';
  $handler->display->display_options['fields']['event_id']['separator'] = '';
  /* Field: Past Event: Severity */
  $handler->display->display_options['fields']['severity']['id'] = 'severity';
  $handler->display->display_options['fields']['severity']['table'] = 'past_event';
  $handler->display->display_options['fields']['severity']['field'] = 'severity';
  $handler->display->display_options['fields']['severity']['empty'] = 'no past logs found with selected severity';
  /* Field: Past Event: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'past_event';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Past Event: Machine_name */
  $handler->display->display_options['fields']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['fields']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['label'] = 'Machine name';
  /* Field: Past Event: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'past_event';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['path'] = 'admin/reports/past/[event_id]';
  $handler->display->display_options['fields']['message']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['message']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['trim'] = TRUE;
  /* Field: Past Event: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'past_event';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Sort criterion: Past Event: Past event ID */
  $handler->display->display_options['sorts']['event_id']['id'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['table'] = 'past_event';
  $handler->display->display_options['sorts']['event_id']['field'] = 'event_id';
  $handler->display->display_options['sorts']['event_id']['order'] = 'DESC';
  /* Contextual filter: Past Event: Past event ID */
  $handler->display->display_options['arguments']['event_id']['id'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['table'] = 'past_event';
  $handler->display->display_options['arguments']['event_id']['field'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['event_id']['summary']['format'] = 'default_summary';
  /* Filter criterion: Past Event: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'past_event';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['group'] = 1;
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  /* Filter criterion: Past Event: Severity */
  $handler->display->display_options['filters']['severity']['id'] = 'severity';
  $handler->display->display_options['filters']['severity']['table'] = 'past_event';
  $handler->display->display_options['filters']['severity']['field'] = 'severity';
  $handler->display->display_options['filters']['severity']['group'] = 1;
  $handler->display->display_options['filters']['severity']['exposed'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['operator_id'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['label'] = 'Severity';
  $handler->display->display_options['filters']['severity']['expose']['operator'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['identifier'] = 'severity';
  $handler->display->display_options['filters']['severity']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['multiple'] = TRUE;
  /* Filter criterion: Past Event: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'past_event';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'contains';
  $handler->display->display_options['filters']['message']['group'] = 1;
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  $handler->display->display_options['filters']['message']['expose']['remember'] = TRUE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'User';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/reports/past/default';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'Past event log';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Past event log';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['past_event_log'] = array(
    t('Master'),
    t('Past event log'),
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
    t('User'),
    t('ID'),
    t('.'),
    t('Severity'),
    t('no past logs found with selected severity'),
    t('Module'),
    t('Machine name'),
    t('Message'),
    t('Timestamp'),
    t('All'),
    t('Page'),
  );

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'past_event_log_key_ext_search';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'past_event';
  $view->human_name = 'Past event log (extended Search)';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Past event log (extended Search)';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer past';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'input_required';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['exposed_form']['options']['text_input_required'] = 'Select any filter and click on Search to see results. The search might be slow if there are many events.';
  $handler->display->display_options['exposed_form']['options']['text_input_required_format'] = 'filtered_html';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'event_id' => 'event_id',
    'severity' => 'severity',
    'timestamp' => 'timestamp',
    'module' => 'module',
    'message' => 'message',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'event_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'severity' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => 'views-align-center',
      'separator' => '',
      'empty_column' => 0,
    ),
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'module' => array(
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
  /* Relationship: Past Event: Past Event Argument */
  $handler->display->display_options['relationships']['past_event_argument']['id'] = 'past_event_argument';
  $handler->display->display_options['relationships']['past_event_argument']['table'] = 'past_event';
  $handler->display->display_options['relationships']['past_event_argument']['field'] = 'past_event_argument';
  /* Relationship: Past Event Argument: Past Event Data */
  $handler->display->display_options['relationships']['past_event_data']['id'] = 'past_event_data';
  $handler->display->display_options['relationships']['past_event_data']['table'] = 'past_event_argument';
  $handler->display->display_options['relationships']['past_event_data']['field'] = 'past_event_data';
  $handler->display->display_options['relationships']['past_event_data']['relationship'] = 'past_event_argument';
  /* Relationship: Past Event: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'past_event';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Past Event: Past event ID */
  $handler->display->display_options['fields']['event_id']['id'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['table'] = 'past_event';
  $handler->display->display_options['fields']['event_id']['field'] = 'event_id';
  $handler->display->display_options['fields']['event_id']['label'] = 'ID';
  $handler->display->display_options['fields']['event_id']['separator'] = '';
  /* Field: Past Event: Severity */
  $handler->display->display_options['fields']['severity']['id'] = 'severity';
  $handler->display->display_options['fields']['severity']['table'] = 'past_event';
  $handler->display->display_options['fields']['severity']['field'] = 'severity';
  $handler->display->display_options['fields']['severity']['empty'] = 'no past logs found with selected severity';
  /* Field: Past Event: Module */
  $handler->display->display_options['fields']['module']['id'] = 'module';
  $handler->display->display_options['fields']['module']['table'] = 'past_event';
  $handler->display->display_options['fields']['module']['field'] = 'module';
  /* Field: Past Event: Machine_name */
  $handler->display->display_options['fields']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['fields']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['label'] = 'Machine name';
  /* Field: Past Event: Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'past_event';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['path'] = 'admin/reports/past/[event_id]';
  $handler->display->display_options['fields']['message']['alter']['max_length'] = '100';
  $handler->display->display_options['fields']['message']['alter']['strip_tags'] = TRUE;
  $handler->display->display_options['fields']['message']['alter']['trim'] = TRUE;
  /* Field: Past Event: Timestamp */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'past_event';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'User';
  /* Sort criterion: Past Event: Timestamp */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'past_event';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['order'] = 'DESC';
  /* Contextual filter: Past Event: Past event ID */
  $handler->display->display_options['arguments']['event_id']['id'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['table'] = 'past_event';
  $handler->display->display_options['arguments']['event_id']['field'] = 'event_id';
  $handler->display->display_options['arguments']['event_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['event_id']['summary']['format'] = 'default_summary';
  /* Filter criterion: Past Event: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'past_event';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['group'] = 1;
  $handler->display->display_options['filters']['module']['exposed'] = TRUE;
  $handler->display->display_options['filters']['module']['expose']['operator_id'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['label'] = 'Module';
  $handler->display->display_options['filters']['module']['expose']['operator'] = 'module_op';
  $handler->display->display_options['filters']['module']['expose']['identifier'] = 'module';
  /* Filter criterion: Past Event: Severity */
  $handler->display->display_options['filters']['severity']['id'] = 'severity';
  $handler->display->display_options['filters']['severity']['table'] = 'past_event';
  $handler->display->display_options['filters']['severity']['field'] = 'severity';
  $handler->display->display_options['filters']['severity']['group'] = 1;
  $handler->display->display_options['filters']['severity']['exposed'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['operator_id'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['label'] = 'Severity';
  $handler->display->display_options['filters']['severity']['expose']['operator'] = 'severity_op';
  $handler->display->display_options['filters']['severity']['expose']['identifier'] = 'severity';
  $handler->display->display_options['filters']['severity']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['severity']['expose']['multiple'] = TRUE;
  /* Filter criterion: Past Event: Message */
  $handler->display->display_options['filters']['message']['id'] = 'message';
  $handler->display->display_options['filters']['message']['table'] = 'past_event';
  $handler->display->display_options['filters']['message']['field'] = 'message';
  $handler->display->display_options['filters']['message']['operator'] = 'contains';
  $handler->display->display_options['filters']['message']['group'] = 1;
  $handler->display->display_options['filters']['message']['exposed'] = TRUE;
  $handler->display->display_options['filters']['message']['expose']['operator_id'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['label'] = 'Message';
  $handler->display->display_options['filters']['message']['expose']['operator'] = 'message_op';
  $handler->display->display_options['filters']['message']['expose']['identifier'] = 'message';
  $handler->display->display_options['filters']['message']['expose']['remember'] = TRUE;
  /* Filter criterion: Past Event Argument: Name */
  $handler->display->display_options['filters']['name_argument']['id'] = 'name_argument';
  $handler->display->display_options['filters']['name_argument']['table'] = 'past_event_argument';
  $handler->display->display_options['filters']['name_argument']['field'] = 'name';
  $handler->display->display_options['filters']['name_argument']['relationship'] = 'past_event_argument';
  $handler->display->display_options['filters']['name_argument']['group'] = 1;
  $handler->display->display_options['filters']['name_argument']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_argument']['expose']['operator_id'] = 'name_argument_op';
  $handler->display->display_options['filters']['name_argument']['expose']['label'] = 'Argument name';
  $handler->display->display_options['filters']['name_argument']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['name_argument']['expose']['operator'] = 'name_argument_op';
  $handler->display->display_options['filters']['name_argument']['expose']['identifier'] = 'name_argument';
  $handler->display->display_options['filters']['name_argument']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Past Event Data: Name */
  $handler->display->display_options['filters']['name_data']['id'] = 'name_data';
  $handler->display->display_options['filters']['name_data']['table'] = 'past_event_data';
  $handler->display->display_options['filters']['name_data']['field'] = 'name';
  $handler->display->display_options['filters']['name_data']['relationship'] = 'past_event_data';
  $handler->display->display_options['filters']['name_data']['group'] = 1;
  $handler->display->display_options['filters']['name_data']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name_data']['expose']['operator_id'] = 'name_data_op';
  $handler->display->display_options['filters']['name_data']['expose']['label'] = 'Data key';
  $handler->display->display_options['filters']['name_data']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['name_data']['expose']['operator'] = 'name_data_op';
  $handler->display->display_options['filters']['name_data']['expose']['identifier'] = 'name_data';
  $handler->display->display_options['filters']['name_data']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Past Event Data: Value */
  $handler->display->display_options['filters']['value_data']['id'] = 'value_data';
  $handler->display->display_options['filters']['value_data']['table'] = 'past_event_data';
  $handler->display->display_options['filters']['value_data']['field'] = 'value';
  $handler->display->display_options['filters']['value_data']['relationship'] = 'past_event_data';
  $handler->display->display_options['filters']['value_data']['group'] = 1;
  $handler->display->display_options['filters']['value_data']['exposed'] = TRUE;
  $handler->display->display_options['filters']['value_data']['expose']['operator_id'] = 'value_data_op';
  $handler->display->display_options['filters']['value_data']['expose']['label'] = 'Data value';
  $handler->display->display_options['filters']['value_data']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['value_data']['expose']['operator'] = 'value_data_op';
  $handler->display->display_options['filters']['value_data']['expose']['identifier'] = 'value_data';
  $handler->display->display_options['filters']['value_data']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/reports/past/extended';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Extended Search';
  $handler->display->display_options['menu']['weight'] = '20';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['past_event_log_key_ext_search'] = array(
    t('Master'),
    t('Past event log (extended Search)'),
    t('more'),
    t('Search'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Select any filter and click on Search to see results. The search might be slow if there are many events.'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Past Event Argument'),
    t('Past Event Data'),
    t('User'),
    t('ID'),
    t('.'),
    t('Severity'),
    t('no past logs found with selected severity'),
    t('Module'),
    t('Machine name'),
    t('Message'),
    t('Timestamp'),
    t('All'),
    t('Argument name'),
    t('Data key'),
    t('Data value'),
    t('Page'),
  );

  $views[$view->name] = $view;
  return $views;
}
