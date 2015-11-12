<?php

/**
 * @file
 * Default views for NetCommerce e-Bill module.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_netcommerce_ebill_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'netcommerce_ebill_schedules';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_recurring';
  $view->human_name = 'NetCommerce e-Bill Payment Schedules';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Recurring Payments';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own ebill schedules';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Search';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Entity Reference: Referenced Entity */
  $handler->display->display_options['relationships']['commerce_recurring_order_target_id']['id'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['relationships']['commerce_recurring_order_target_id']['table'] = 'field_data_commerce_recurring_order';
  $handler->display->display_options['relationships']['commerce_recurring_order_target_id']['field'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['relationships']['commerce_recurring_order_target_id']['required'] = TRUE;
  /* Relationship: Commerce Order: Referenced line items */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Relationship: Commerce Order: Owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'customer';
  /* Field: Commerce recurring entity: e-Bill Schedule ID */
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['id'] = 'commerce_netcommerce_ebill_id';
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['table'] = 'field_data_commerce_netcommerce_ebill_id';
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['field'] = 'commerce_netcommerce_ebill_id';
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['exclude'] = TRUE;
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Description';
  /* Field: Commerce recurring entity: Commerce recurring start date */
  $handler->display->display_options['fields']['start_date']['id'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['start_date']['field'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['label'] = 'Start Date';
  $handler->display->display_options['fields']['start_date']['date_format'] = 'short';
  /* Field: Commerce recurring entity: Commerce recurring end date */
  $handler->display->display_options['fields']['end_date']['id'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['end_date']['field'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['label'] = 'End Date';
  $handler->display->display_options['fields']['end_date']['date_format'] = 'short';
  /* Field: Commerce recurring entity: Fixed price */
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['id'] = 'commerce_recurring_fixed_price';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['table'] = 'field_data_commerce_recurring_fixed_price';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['field'] = 'commerce_recurring_fixed_price';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['label'] = 'Amount';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce recurring entity: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Commerce recurring entity: Status */
  $handler->display->display_options['fields']['status_1']['id'] = 'status_1';
  $handler->display->display_options['fields']['status_1']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['status_1']['field'] = 'status';
  $handler->display->display_options['fields']['status_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['status_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['status_1']['alter']['text'] = 'Cancel';
  $handler->display->display_options['fields']['status_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['status_1']['alter']['path'] = 'netcommerce-ebill-cancel/[commerce_netcommerce_ebill_id]?destination=user/!1/ebill-recurring';
  $handler->display->display_options['fields']['status_1']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['status_1']['type'] = 'boolean';
  $handler->display->display_options['fields']['status_1']['not'] = 0;
  /* Contextual filter: Commerce Order: Uid */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'user';
  $handler->display->display_options['arguments']['uid']['default_argument_options']['user'] = FALSE;
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
    3 => '3',
  );
  /* Filter criterion: Commerce recurring entity: e-Bill Schedule ID (commerce_netcommerce_ebill_id) */
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['id'] = 'commerce_netcommerce_ebill_id_value';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['table'] = 'field_data_commerce_netcommerce_ebill_id';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['field'] = 'commerce_netcommerce_ebill_id_value';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['operator'] = 'not empty';

  /* Display: User Profile Tab */
  $handler = $view->new_display('page', 'User Profile Tab', 'page');
  $handler->display->display_options['path'] = 'user/%/ebill-recurring';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Recurring Payments';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Admin Page */
  $handler = $view->new_display('page', 'Admin Page', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Recurring e-Bill Payment Schedules';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view any ebill schedules';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'customer';
  /* Field: Commerce recurring entity: e-Bill Schedule ID */
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['id'] = 'commerce_netcommerce_ebill_id';
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['table'] = 'field_data_commerce_netcommerce_ebill_id';
  $handler->display->display_options['fields']['commerce_netcommerce_ebill_id']['field'] = 'commerce_netcommerce_ebill_id';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Owner';
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Description';
  /* Field: Commerce recurring entity: Commerce recurring start date */
  $handler->display->display_options['fields']['start_date']['id'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['start_date']['field'] = 'start_date';
  $handler->display->display_options['fields']['start_date']['label'] = 'Start Date';
  $handler->display->display_options['fields']['start_date']['date_format'] = 'short';
  /* Field: Commerce recurring entity: Commerce recurring end date */
  $handler->display->display_options['fields']['end_date']['id'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['end_date']['field'] = 'end_date';
  $handler->display->display_options['fields']['end_date']['label'] = 'End Date';
  $handler->display->display_options['fields']['end_date']['date_format'] = 'short';
  /* Field: Commerce recurring entity: Fixed price */
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['id'] = 'commerce_recurring_fixed_price';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['table'] = 'field_data_commerce_recurring_fixed_price';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['field'] = 'commerce_recurring_fixed_price';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['label'] = 'Amount';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_recurring_fixed_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce recurring entity: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Field: Commerce recurring entity: Status */
  $handler->display->display_options['fields']['status_1']['id'] = 'status_1';
  $handler->display->display_options['fields']['status_1']['table'] = 'commerce_recurring';
  $handler->display->display_options['fields']['status_1']['field'] = 'status';
  $handler->display->display_options['fields']['status_1']['label'] = 'Actions';
  $handler->display->display_options['fields']['status_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['status_1']['alter']['text'] = 'Cancel';
  $handler->display->display_options['fields']['status_1']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['status_1']['alter']['path'] = 'netcommerce-ebill-cancel/[commerce_netcommerce_ebill_id]?destination=user/!1/ebill-recurring';
  $handler->display->display_options['fields']['status_1']['empty_zero'] = TRUE;
  $handler->display->display_options['fields']['status_1']['type'] = 'boolean';
  $handler->display->display_options['fields']['status_1']['not'] = 0;
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce recurring entity: e-Bill Schedule ID (commerce_netcommerce_ebill_id) */
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['id'] = 'commerce_netcommerce_ebill_id_value';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['table'] = 'field_data_commerce_netcommerce_ebill_id';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['field'] = 'commerce_netcommerce_ebill_id_value';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['operator'] = 'not empty';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value']['group'] = 1;
  /* Filter criterion: Commerce Order: Order ID */
  $handler->display->display_options['filters']['order_id']['id'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['order_id']['field'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['filters']['order_id']['group'] = 1;
  $handler->display->display_options['filters']['order_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['order_id']['expose']['operator_id'] = 'order_id_op';
  $handler->display->display_options['filters']['order_id']['expose']['label'] = 'Order ID';
  $handler->display->display_options['filters']['order_id']['expose']['operator'] = 'order_id_op';
  $handler->display->display_options['filters']['order_id']['expose']['identifier'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Commerce Order: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['relationship'] = 'commerce_recurring_order_target_id';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['group'] = 1;
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Owner';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Commerce recurring entity: e-Bill Schedule ID (commerce_netcommerce_ebill_id) */
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['id'] = 'commerce_netcommerce_ebill_id_value_1';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['table'] = 'field_data_commerce_netcommerce_ebill_id';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['field'] = 'commerce_netcommerce_ebill_id_value';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['group'] = 1;
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['exposed'] = TRUE;
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['expose']['operator_id'] = 'commerce_netcommerce_ebill_id_value_1_op';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['expose']['label'] = 'e-Bill Schedule ID';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['expose']['operator'] = 'commerce_netcommerce_ebill_id_value_1_op';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['expose']['identifier'] = 'commerce_netcommerce_ebill_id_value_1';
  $handler->display->display_options['filters']['commerce_netcommerce_ebill_id_value_1']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['path'] = 'admin/commerce/ebill-recurring';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'e-Bill Recurring Payments';
  $handler->display->display_options['menu']['description'] = 'Manage recurring payments scheduled with e-Bill';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
