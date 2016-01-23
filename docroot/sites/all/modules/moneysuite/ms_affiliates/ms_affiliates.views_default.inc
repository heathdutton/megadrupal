<?php

/**
 * Implements hook_views_default_views().
 */
function ms_affiliates_views_default_views() {
  $views['affiliates_history'] = ms_affiliates_history_exported_default_view();

  return $views;
}

/**
 * Exported Default View: Orders.
 */
function ms_affiliates_history_exported_default_view() {
  $view = new view();
  $view->name = 'affiliate_history';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ms_affiliates_history';
  $view->human_name = 'Affiliates Suite History';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer affiliates';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '20';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'amount' => 'amount',
    'balance' => 'balance',
    'date' => 'date',
    'info' => 'info',
    'type' => 'type',
    'order_number' => 'order_number',
    'rendered_entity' => 'rendered_entity',
    'name' => 'name',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'balance' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'date' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'info' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_number' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Affiliates Suite History: Oid */
  $handler->display->display_options['relationships']['oid']['id'] = 'oid';
  $handler->display->display_options['relationships']['oid']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['relationships']['oid']['field'] = 'oid';
  /* Relationship: Affiliates Suite History: Pid */
  $handler->display->display_options['relationships']['pid']['id'] = 'pid';
  $handler->display->display_options['relationships']['pid']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['relationships']['pid']['field'] = 'pid';
  /* Relationship: Affiliates Suite History: Rid */
  $handler->display->display_options['relationships']['rid']['id'] = 'rid';
  $handler->display->display_options['relationships']['rid']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['relationships']['rid']['field'] = 'rid';
  $handler->display->display_options['relationships']['rid']['label'] = 'Referral';
  /* Relationship: Affiliates Suite History: Uid */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['label'] = 'Affiliate';
  /* Field: Affiliates Suite History: Date */
  $handler->display->display_options['fields']['date']['id'] = 'date';
  $handler->display->display_options['fields']['date']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['date']['field'] = 'date';
  $handler->display->display_options['fields']['date']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Affiliate';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'rid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Referral';
  /* Field: Affiliates Suite History: Info */
  $handler->display->display_options['fields']['info']['id'] = 'info';
  $handler->display->display_options['fields']['info']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['info']['field'] = 'info';
  /* Field: Affiliates Suite History: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Order: Order Number */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'views_entity_ms_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['relationship'] = 'oid';
  /* Field: Payment: Rendered Payment */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_ms_payment';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'pid';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Payment';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  $handler->display->display_options['fields']['rendered_entity']['bypass_access'] = 0;
  /* Field: Affiliates Suite History: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  /* Field: Affiliates Suite History: Balance */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  $handler->display->display_options['fields']['balance']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['balance']['precision'] = '2';
  /* Sort criterion: Affiliates Suite History: Date */
  $handler->display->display_options['sorts']['date']['id'] = 'date';
  $handler->display->display_options['sorts']['date']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['sorts']['date']['field'] = 'date';
  $handler->display->display_options['sorts']['date']['order'] = 'DESC';
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Affiliate';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    5 => 0,
    7 => 0,
    6 => 0,
    19 => 0,
    3 => 0,
    1 => 0,
    8 => 0,
    9 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
  );
  /* Filter criterion: Affiliates Suite History: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    4 => 0,
    5 => 0,
    7 => 0,
    6 => 0,
    19 => 0,
    3 => 0,
    1 => 0,
    8 => 0,
    9 => 0,
    14 => 0,
    15 => 0,
    16 => 0,
    17 => 0,
    18 => 0,
  );

  /* Display: Affiliates History Admin Page */
  $handler = $view->new_display('page', 'Affiliates History Admin Page', 'page_1');
  $handler->display->display_options['display_description'] = 'Shows in the People admin menus.';
  $handler->display->display_options['path'] = 'admin/people/affiliates-history';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Affiliates History';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: User Affiliate History Page */
  $handler = $view->new_display('page', 'User Affiliate History Page', 'page_2');
  $handler->display->display_options['display_description'] = 'Page that shows on the user account with their own history.';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'php';
  $handler->display->display_options['access']['php_access'] = 'return (arg(1) == $account->uid);';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Affiliates Suite History: Date */
  $handler->display->display_options['fields']['date']['id'] = 'date';
  $handler->display->display_options['fields']['date']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['date']['field'] = 'date';
  $handler->display->display_options['fields']['date']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'rid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Referral';
  /* Field: Affiliates Suite History: Info */
  $handler->display->display_options['fields']['info']['id'] = 'info';
  $handler->display->display_options['fields']['info']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['info']['field'] = 'info';
  /* Field: Affiliates Suite History: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Affiliates Suite History: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  /* Field: Affiliates Suite History: Balance */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  $handler->display->display_options['fields']['balance']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['balance']['precision'] = '2';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Affiliates Suite History: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'user/%/ms_affiliate/history';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'History';
  $handler->display->display_options['menu']['description'] = 'View your Affiliate History Here';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Admin CSV Export */
  $handler = $view->new_display('views_data_export', 'Admin CSV Export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view-%timestamp-full.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = 'admin/people/affiliates-history/export';
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'default' => 0,
    'page_2' => 0,
  );

  /* Display: Affiliate CSV Export */
  $handler = $view->new_display('views_data_export', 'Affiliate CSV Export', 'views_data_export_2');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'php';
  $handler->display->display_options['access']['php_access'] = 'return (arg(1) == $account->uid);';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['filename'] = '%view-%timestamp-full.csv';
  $handler->display->display_options['style_options']['parent_sort'] = 1;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Affiliates Suite History: Date */
  $handler->display->display_options['fields']['date']['id'] = 'date';
  $handler->display->display_options['fields']['date']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['date']['field'] = 'date';
  $handler->display->display_options['fields']['date']['date_format'] = 'short';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'rid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Referral';
  /* Field: Affiliates Suite History: Info */
  $handler->display->display_options['fields']['info']['id'] = 'info';
  $handler->display->display_options['fields']['info']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['info']['field'] = 'info';
  /* Field: Affiliates Suite History: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Affiliates Suite History: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  /* Field: Affiliates Suite History: Balance */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  $handler->display->display_options['fields']['balance']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['balance']['precision'] = '2';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Affiliates Suite History: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'ms_affiliates_history';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'default';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'current_user';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['path'] = 'user/%/ms_affiliate/history/export';
  $handler->display->display_options['displays'] = array(
    'page_2' => 'page_2',
    'default' => 0,
    'page_1' => 0,
  );

  return $view;
}
