<?php

$view = new view();
$view->name = 'tmgmt_server_clients';
$view->description = 'List of client entities.';
$view->tag = 'default';
$view->base_table = 'tmgmt_server_client';
$view->human_name = 'Client entities';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'full';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'label' => 'label',
  'uuid' => 'uuid',
  'operations' => 'operations',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'label' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'uuid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'operations' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* No results behavior: Global: Text area */
$handler->display->display_options['empty']['area']['id'] = 'area';
$handler->display->display_options['empty']['area']['table'] = 'views';
$handler->display->display_options['empty']['area']['field'] = 'area';
$handler->display->display_options['empty']['area']['content'] = 'You do not have any remote clients attached to your account.';
$handler->display->display_options['empty']['area']['format'] = 'filtered_html';
/* Field: Label */
$handler->display->display_options['fields']['label']['id'] = 'label';
$handler->display->display_options['fields']['label']['table'] = 'tmgmt_server_client';
$handler->display->display_options['fields']['label']['field'] = 'label';
$handler->display->display_options['fields']['label']['ui_name'] = 'Label';
$handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
/* Field: Remote Client: Uuid */
$handler->display->display_options['fields']['uuid']['id'] = 'uuid';
$handler->display->display_options['fields']['uuid']['table'] = 'tmgmt_server_client';
$handler->display->display_options['fields']['uuid']['field'] = 'uuid';
$handler->display->display_options['fields']['uuid']['label'] = 'UUID';
/* Field: Remote Client: Operations */
$handler->display->display_options['fields']['operations']['id'] = 'operations';
$handler->display->display_options['fields']['operations']['table'] = 'tmgmt_server_client';
$handler->display->display_options['fields']['operations']['field'] = 'operations';
/* Contextual filter: Remote Client: Uid */
$handler->display->display_options['arguments']['uid']['id'] = 'uid';
$handler->display->display_options['arguments']['uid']['table'] = 'tmgmt_server_client';
$handler->display->display_options['arguments']['uid']['field'] = 'uid';
$handler->display->display_options['arguments']['uid']['default_action'] = 'default';
$handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
$handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
$handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
$translatables['tmgmt_server_clients'] = array(
  t('Master'),
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
  t('You do not have any remote clients attached to your account.'),
  t('Label'),
  t('UUID'),
  t('Operations'),
  t('All'),
);

