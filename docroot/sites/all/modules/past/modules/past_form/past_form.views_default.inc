<?php

/**
 * Implements hook_views_default_views().
 */
function past_form_views_default_views() {

  $view = new view();
  $view->name = 'past_event_log_form_and_validation';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'past_event';
  $view->human_name = 'Past event log (Form and Validation)';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Form log';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer past';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['distinct'] = TRUE;
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
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
  /* Field: Past Event: Machine_name */
  $handler->display->display_options['fields']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['fields']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['fields']['machine_name']['label'] = 'Machine name';
  /* Field: Past Event: Argument data */
  $handler->display->display_options['fields']['argument_data']['id'] = 'argument_data';
  $handler->display->display_options['fields']['argument_data']['table'] = 'past_event';
  $handler->display->display_options['fields']['argument_data']['field'] = 'argument_data';
  $handler->display->display_options['fields']['argument_data']['label'] = 'Form ID';
  $handler->display->display_options['fields']['argument_data']['argument_name'] = 'form_id';
  $handler->display->display_options['fields']['argument_data']['data_key'] = '';
  /* Field: Past Event: Argument data */
  $handler->display->display_options['fields']['argument_data_1']['id'] = 'argument_data_1';
  $handler->display->display_options['fields']['argument_data_1']['table'] = 'past_event';
  $handler->display->display_options['fields']['argument_data_1']['field'] = 'argument_data';
  $handler->display->display_options['fields']['argument_data_1']['label'] = 'Operation';
  $handler->display->display_options['fields']['argument_data_1']['argument_name'] = 'operation';
  $handler->display->display_options['fields']['argument_data_1']['data_key'] = '';
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
  /* Filter criterion: Past Event: Machine_name */
  $handler->display->display_options['filters']['machine_name']['id'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['table'] = 'past_event';
  $handler->display->display_options['filters']['machine_name']['field'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['group'] = 1;
  $handler->display->display_options['filters']['machine_name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['machine_name']['expose']['operator_id'] = 'machine_name_op';
  $handler->display->display_options['filters']['machine_name']['expose']['label'] = 'Machine name';
  $handler->display->display_options['filters']['machine_name']['expose']['description'] = 'submit, validate';
  $handler->display->display_options['filters']['machine_name']['expose']['operator'] = 'machine_name_op';
  $handler->display->display_options['filters']['machine_name']['expose']['identifier'] = 'machine_name';
  $handler->display->display_options['filters']['machine_name']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Past Event: Module */
  $handler->display->display_options['filters']['module']['id'] = 'module';
  $handler->display->display_options['filters']['module']['table'] = 'past_event';
  $handler->display->display_options['filters']['module']['field'] = 'module';
  $handler->display->display_options['filters']['module']['value'] = 'past_form';
  $handler->display->display_options['filters']['module']['group'] = 1;
  /* Filter criterion: Form ID */
  $handler->display->display_options['filters']['argument_data']['id'] = 'argument_data';
  $handler->display->display_options['filters']['argument_data']['table'] = 'past_event';
  $handler->display->display_options['filters']['argument_data']['field'] = 'argument_data';
  $handler->display->display_options['filters']['argument_data']['ui_name'] = 'Form ID';
  $handler->display->display_options['filters']['argument_data']['group'] = 1;
  $handler->display->display_options['filters']['argument_data']['exposed'] = TRUE;
  $handler->display->display_options['filters']['argument_data']['expose']['operator_id'] = 'argument_data_op';
  $handler->display->display_options['filters']['argument_data']['expose']['label'] = 'Form ID';
  $handler->display->display_options['filters']['argument_data']['expose']['operator'] = 'argument_data_op';
  $handler->display->display_options['filters']['argument_data']['expose']['identifier'] = 'argument_form_id';
  $handler->display->display_options['filters']['argument_data']['argument_name'] = 'form_id';
  $handler->display->display_options['filters']['argument_data']['data_key'] = '';
  /* Filter criterion: Operation */
  $handler->display->display_options['filters']['argument_data_1']['id'] = 'argument_data_1';
  $handler->display->display_options['filters']['argument_data_1']['table'] = 'past_event';
  $handler->display->display_options['filters']['argument_data_1']['field'] = 'argument_data';
  $handler->display->display_options['filters']['argument_data_1']['ui_name'] = 'Operation';
  $handler->display->display_options['filters']['argument_data_1']['group'] = 1;
  $handler->display->display_options['filters']['argument_data_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['argument_data_1']['expose']['operator_id'] = 'argument_data_1_op';
  $handler->display->display_options['filters']['argument_data_1']['expose']['label'] = 'Operation';
  $handler->display->display_options['filters']['argument_data_1']['expose']['operator'] = 'argument_data_1_op';
  $handler->display->display_options['filters']['argument_data_1']['expose']['identifier'] = 'argument_operation';
  $handler->display->display_options['filters']['argument_data_1']['argument_name'] = 'operation';
  $handler->display->display_options['filters']['argument_data_1']['data_key'] = '';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Username';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/reports/past/forms';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Forms';
  $handler->display->display_options['menu']['weight'] = '1';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['past_event_log_form_and_validation'] = array(
    t('Master'),
    t('Form log'),
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
    t('Past Event Argument'),
    t('Past Event Data'),
    t('User'),
    t('ID'),
    t('.'),
    t('Machine name'),
    t('Form ID'),
    t('Operation'),
    t('Message'),
    t('Timestamp'),
    t('All'),
    t('submit, validate'),
    t('Apply, Save, ...'),
    t('Username'),
    t('Page'),
  );

  $views[$view->name] = $view;

  return $views;
}
