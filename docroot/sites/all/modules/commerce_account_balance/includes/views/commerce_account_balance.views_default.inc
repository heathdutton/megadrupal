<?php

/**
 * @file
 * Generates default views for Commerce Account Balance.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_account_balance_views_default_views() {

  $views = array();
  $view = new view();
  $view->name = 'account_balance_transactions';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_account_balance_transaction';
  $view->human_name = 'Account balance transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  /* Edit this to true to make a default view disabled initially */
  $view->disabled = FALSE;

  /* Display: Master */

  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'My transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own transactions';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Commerce Account Balance Transaction: User */

  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: Commerce Account Balance Transaction: Unique transaction ID */

  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['label'] = 'Transaction ID';
  $handler->display->display_options['fields']['transaction_id']['separator'] = '';
  /* Field: Commerce Account Balance Transaction: Transaction action */

  $handler->display->display_options['fields']['action']['id'] = 'action';
  $handler->display->display_options['fields']['action']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['action']['field'] = 'action';
  $handler->display->display_options['fields']['action']['label'] = 'Action';
  /* Field: Commerce Account Balance Transaction: Transaction type */

  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['label'] = 'Type';
  /* Field: Commerce Account Balance Transaction: Time */

  $handler->display->display_options['fields']['time']['id'] = 'time';
  $handler->display->display_options['fields']['time']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['fields']['time']['field'] = 'time';
  $handler->display->display_options['fields']['time']['date_format'] = 'short';
  $handler->display->display_options['fields']['time']['second_date_format'] = 'long';
  /* Field: Commerce Account Balance Transaction: Transaction amount (formatted) */

  $handler->display->display_options['fields']['amount_formatted']['id'] = 'amount_formatted';
  $handler->display->display_options['fields']['amount_formatted']['table'] = 'views_entity_commerce_account_balance_transaction';
  $handler->display->display_options['fields']['amount_formatted']['field'] = 'amount_formatted';
  $handler->display->display_options['fields']['amount_formatted']['label'] = 'Amount';
  $handler->display->display_options['fields']['amount_formatted']['link_to_entity'] = 0;
  /* Sort criterion: Commerce Account Balance Transaction: Time */

  $handler->display->display_options['sorts']['time']['id'] = 'time';
  $handler->display->display_options['sorts']['time']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['sorts']['time']['field'] = 'time';
  $handler->display->display_options['sorts']['time']['order'] = 'DESC';
  /* Contextual filter: User: Uid */

  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'access denied';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Commerce Account Balance Transaction: Transaction type */

  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_account_balance_transaction';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Transaction type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );

  /* Display: My transactions */

  $handler = $view->new_display('page', 'My transactions', 'user_transactions');
  $handler->display->display_options['path'] = 'user/%/balance/transactions';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'My transactions';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['account_balance_transactions'] = array(
    t('Master'),
    t('My transactions'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('User'),
    t('Transaction ID'),
    t('.'),
    t('Action'),
    t('Type'),
    t('Time'),
    t('Amount'),
    t('All'),
    t('Transaction type'),
  );

  $views[$view->name] = $view;

  return $views;
}
