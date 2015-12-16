<?php

/**
 * @file
 * Provides default views.
 */

/*
 * Implements hook_views_default_views().
 */
function commerce_gc_views_default_views() {
  
  /*
   * Giftcard transactions by coupon
   */

  $view = new view();
  $view->name = 'commerce_gc_giftcard_transactions';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_gc_transaction';
  $view->human_name = 'Giftcard transactions';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Giftcard transactions';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Header: Commerce Coupon: Add transaction form */
  $handler->display->display_options['header']['add_transaction_form']['id'] = 'add_transaction_form';
  $handler->display->display_options['header']['add_transaction_form']['table'] = 'commerce_coupon';
  $handler->display->display_options['header']['add_transaction_form']['field'] = 'add_transaction_form';
  /* Footer: Commerce Coupon: Transaction balance */
  $handler->display->display_options['footer']['balance_area']['id'] = 'balance_area';
  $handler->display->display_options['footer']['balance_area']['table'] = 'commerce_coupon';
  $handler->display->display_options['footer']['balance_area']['field'] = 'balance_area';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No transactions to show.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Commerce giftcard transaction: Coupon_id */
  $handler->display->display_options['relationships']['coupon_id']['id'] = 'coupon_id';
  $handler->display->display_options['relationships']['coupon_id']['table'] = 'commerce_gc_transaction';
  $handler->display->display_options['relationships']['coupon_id']['field'] = 'coupon_id';
  $handler->display->display_options['relationships']['coupon_id']['required'] = TRUE;
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_commerce_giftcards_user']['id'] = 'reverse_commerce_giftcards_user';
  $handler->display->display_options['relationships']['reverse_commerce_giftcards_user']['table'] = 'commerce_coupon';
  $handler->display->display_options['relationships']['reverse_commerce_giftcards_user']['field'] = 'reverse_commerce_giftcards_user';
  $handler->display->display_options['relationships']['reverse_commerce_giftcards_user']['relationship'] = 'coupon_id';
  /* Field: Commerce giftcard transaction: Label */
  $handler->display->display_options['fields']['transaction_id']['id'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['table'] = 'commerce_gc_transaction';
  $handler->display->display_options['fields']['transaction_id']['field'] = 'transaction_id';
  $handler->display->display_options['fields']['transaction_id']['label'] = 'ID';
  $handler->display->display_options['fields']['transaction_id']['separator'] = '';
  /* Field: Commerce giftcard transaction: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'commerce_gc_transaction';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  /* Field: Commerce giftcard transaction: Date */
  $handler->display->display_options['fields']['date']['id'] = 'date';
  $handler->display->display_options['fields']['date']['table'] = 'commerce_gc_transaction';
  $handler->display->display_options['fields']['date']['field'] = 'date';
  $handler->display->display_options['fields']['date']['date_format'] = 'long';
  /* Field: Commerce giftcard transaction: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_gc_transaction';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Commerce giftcard transaction: Delete transaction link */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'commerce_gc_transaction';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  $handler->display->display_options['fields']['delete']['label'] = '';
  $handler->display->display_options['fields']['delete']['element_label_colon'] = FALSE;
  /* Contextual filter: Commerce Coupon: Commerce coupon ID */
  $handler->display->display_options['arguments']['coupon_id']['id'] = 'coupon_id';
  $handler->display->display_options['arguments']['coupon_id']['table'] = 'commerce_coupon';
  $handler->display->display_options['arguments']['coupon_id']['field'] = 'coupon_id';
  $handler->display->display_options['arguments']['coupon_id']['relationship'] = 'coupon_id';
  $handler->display->display_options['arguments']['coupon_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['coupon_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['coupon_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['coupon_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['coupon_id']['summary_options']['items_per_page'] = '25';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'giftcards/%/transactions';
  $handler->display->display_options['menu']['title'] = 'Giftcard Transactions';
  $handler->display->display_options['menu']['weight'] = '50';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  
  $views['commerce_gc_giftcard_transactions'] = $view;
  
  /*
   * Administer giftcards
   */

  $view = new view();
  $view->name = 'commerce_gc_giftcards';
  $view->description = 'List of giftcards';
  $view->tag = 'default';
  $view->base_table = 'commerce_coupon';
  $view->human_name = 'Giftcards';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Giftcards';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer giftcards';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No giftcards found.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Field: Commerce Coupon: Label */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  $handler->display->display_options['fields']['code']['label'] = 'Code';
  /* Field: Commerce Coupon: Recipient */
  $handler->display->display_options['fields']['commerce_coupon_recipient']['id'] = 'commerce_coupon_recipient';
  $handler->display->display_options['fields']['commerce_coupon_recipient']['table'] = 'field_data_commerce_coupon_recipient';
  $handler->display->display_options['fields']['commerce_coupon_recipient']['field'] = 'commerce_coupon_recipient';
  $handler->display->display_options['fields']['commerce_coupon_recipient']['settings'] = array(
    'link' => 0,
  );
  $handler->display->display_options['fields']['commerce_coupon_recipient']['delta_offset'] = '0';
  /* Field: Commerce Coupon: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  /* Field: Commerce Coupon: Balance summary */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  $handler->display->display_options['fields']['balance']['label'] = '';
  $handler->display->display_options['fields']['balance']['element_label_colon'] = FALSE;
  /* Field: Commerce Coupon: Edit coupon */
  $handler->display->display_options['fields']['edit']['id'] = 'edit';
  $handler->display->display_options['fields']['edit']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['edit']['field'] = 'edit';
  $handler->display->display_options['fields']['edit']['label'] = '';
  $handler->display->display_options['fields']['edit']['element_label_colon'] = FALSE;
  /* Field: Commerce Coupon: Delete coupon */
  $handler->display->display_options['fields']['delete']['id'] = 'delete';
  $handler->display->display_options['fields']['delete']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['delete']['field'] = 'delete';
  $handler->display->display_options['fields']['delete']['label'] = '';
  $handler->display->display_options['fields']['delete']['element_label_colon'] = FALSE;
  /* Filter criterion: Commerce Coupon: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_coupon';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'giftcard_coupon' => 'giftcard_coupon',
  );
  /* Filter criterion: Commerce Coupon: Label */
  $handler->display->display_options['filters']['code']['id'] = 'code';
  $handler->display->display_options['filters']['code']['table'] = 'commerce_coupon';
  $handler->display->display_options['filters']['code']['field'] = 'code';
  $handler->display->display_options['filters']['code']['exposed'] = TRUE;
  $handler->display->display_options['filters']['code']['expose']['operator_id'] = 'code_op';
  $handler->display->display_options['filters']['code']['expose']['label'] = 'Code';
  $handler->display->display_options['filters']['code']['expose']['operator'] = 'code_op';
  $handler->display->display_options['filters']['code']['expose']['identifier'] = 'code';
  $handler->display->display_options['filters']['code']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/coupons/giftcards';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Giftcards';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views['commerce_gc_giftcards'] = $view;

  /*
   * User giftcards
   */

  $view = new view();
  $view->name = 'commerce_gc_user_giftcards';
  $view->description = 'Provide a list of giftcards owned by a user';
  $view->tag = 'default';
  $view->base_table = 'commerce_coupon';
  $view->human_name = 'User giftcards';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User giftcards';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Unfiltered text */
  $handler->display->display_options['empty']['area_text_custom']['id'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['table'] = 'views';
  $handler->display->display_options['empty']['area_text_custom']['field'] = 'area_text_custom';
  $handler->display->display_options['empty']['area_text_custom']['empty'] = TRUE;
  $handler->display->display_options['empty']['area_text_custom']['content'] = 'No giftcards to show at this time.';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['commerce_coupon_recipient_target_id']['id'] = 'commerce_coupon_recipient_target_id';
  $handler->display->display_options['relationships']['commerce_coupon_recipient_target_id']['table'] = 'field_data_commerce_coupon_recipient';
  $handler->display->display_options['relationships']['commerce_coupon_recipient_target_id']['field'] = 'commerce_coupon_recipient_target_id';
  $handler->display->display_options['relationships']['commerce_coupon_recipient_target_id']['required'] = TRUE;
  /* Field: Commerce Coupon: Label */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  $handler->display->display_options['fields']['code']['label'] = 'Code';
  /* Field: Commerce Coupon: Balance summary */
  $handler->display->display_options['fields']['balance']['id'] = 'balance';
  $handler->display->display_options['fields']['balance']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['balance']['field'] = 'balance';
  $handler->display->display_options['fields']['balance']['label'] = '';
  $handler->display->display_options['fields']['balance']['element_label_colon'] = FALSE;
  /* Contextual filter: User: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'users';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'commerce_coupon_recipient_target_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'user_giftcards';
  /* Filter criterion: Commerce Coupon: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_coupon';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'giftcard_coupon' => 'giftcard_coupon',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/giftcards';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Giftcards';
  $handler->display->display_options['menu']['description'] = 'List of your giftcards';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'user-menu';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  
  $views['commerce_gc_user_giftcards'] = $view;
  
  return $views;
}
