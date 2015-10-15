<?php

/**
 * @file
 * Views for listing rates filtered by shipping service name, exporting
 * shipping sert
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_shipping_rate_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'shipping_rates';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_shipping_rate';
  $view->human_name = 'Shipping rates';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Manage rates';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'shipping_rate_min_weight' => 'shipping_rate_min_weight',
    'shipping_rate_max_weight' => 'shipping_rate_max_weight',
    'shipping_rate_price' => 'shipping_rate_price',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'shipping_rate_min_weight';
  $handler->display->display_options['style_options']['info'] = array(
    'shipping_rate_min_weight' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipping_rate_max_weight' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipping_rate_price' => array(
      'sortable' => 1,
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
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Field: Commerce shipping Rate: Minimun weight */
  $handler->display->display_options['fields']['shipping_rate_min_weight']['id'] = 'shipping_rate_min_weight';
  $handler->display->display_options['fields']['shipping_rate_min_weight']['table'] = 'field_data_shipping_rate_min_weight';
  $handler->display->display_options['fields']['shipping_rate_min_weight']['field'] = 'shipping_rate_min_weight';
  $handler->display->display_options['fields']['shipping_rate_min_weight']['click_sort_column'] = 'weight';
  /* Field: Commerce shipping Rate: Maximum weight */
  $handler->display->display_options['fields']['shipping_rate_max_weight']['id'] = 'shipping_rate_max_weight';
  $handler->display->display_options['fields']['shipping_rate_max_weight']['table'] = 'field_data_shipping_rate_max_weight';
  $handler->display->display_options['fields']['shipping_rate_max_weight']['field'] = 'shipping_rate_max_weight';
  $handler->display->display_options['fields']['shipping_rate_max_weight']['click_sort_column'] = 'weight';
  /* Field: Commerce shipping Rate: Rate amount */
  $handler->display->display_options['fields']['shipping_rate_price']['id'] = 'shipping_rate_price';
  $handler->display->display_options['fields']['shipping_rate_price']['table'] = 'field_data_shipping_rate_price';
  $handler->display->display_options['fields']['shipping_rate_price']['field'] = 'shipping_rate_price';
  $handler->display->display_options['fields']['shipping_rate_price']['label'] = 'Amount';
  $handler->display->display_options['fields']['shipping_rate_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['shipping_rate_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce shipping rate: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['operations']['add_destination'] = 1;
  /* Contextual filter: Commerce shipping rate: Shipping method name */
  $handler->display->display_options['arguments']['method']['id'] = 'method';
  $handler->display->display_options['arguments']['method']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['arguments']['method']['field'] = 'method';
  $handler->display->display_options['arguments']['method']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['method']['exception']['title'] = 'Tout';
  $handler->display->display_options['arguments']['method']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['method']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['method']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['method']['summary_options']['items_per_page'] = '50';
  $handler->display->display_options['arguments']['method']['limit'] = '0';
  /* Contextual filter: Commerce shipping rate: Shipping service name */
  $handler->display->display_options['arguments']['name']['id'] = 'name';
  $handler->display->display_options['arguments']['name']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['arguments']['name']['field'] = 'name';
  $handler->display->display_options['arguments']['name']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['name']['summary_options']['items_per_page'] = '50';
  $handler->display->display_options['arguments']['name']['limit'] = '0';
  /* Filter criterion: Commerce shipping rate: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'rate';
  $translatables['shipping_rates'] = array(
    t('Master'),
    t('Manage rates'),
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
    t('Minimun weight'),
    t('Maximum weight'),
    t('Amount'),
    t('Operations'),
    t('Tout'),
    t('All'),
  );

  $views[$view->name] = $view;


  $view = new view();
  $view->name = 'shipping_rates_manage';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_shipping_rate';
  $view->human_name = 'shipping rates';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'shipping rates';
  $handler->display->display_options['use_ajax'] = FALSE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_shipping_rate entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '200';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'rate_id' => 'rate_id',
    'shipping_rate_min_weight' => 'shipping_rate_min_weight',
    'shipping_rate_max_weight' => 'shipping_rate_max_weight',
    'shipping_rate_price' => 'shipping_rate_price',
  );
  $handler->display->display_options['style_options']['default'] = 'shipping_rate_min_weight';
  $handler->display->display_options['style_options']['info'] = array(
    'rate_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipping_rate_min_weight' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipping_rate_max_weight' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipping_rate_price' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Commerce shipping Rate: Minimun weight */
  $handler->display->display_options['fields']['shipping_rate_min_weight']['id'] = 'shipping_rate_min_weight';
  $handler->display->display_options['fields']['shipping_rate_min_weight']['table'] = 'field_data_shipping_rate_min_weight';
  $handler->display->display_options['fields']['shipping_rate_min_weight']['field'] = 'shipping_rate_min_weight';
  $handler->display->display_options['fields']['shipping_rate_min_weight']['click_sort_column'] = 'weight';
  /* Field: Commerce shipping Rate: Maximum weight */
  $handler->display->display_options['fields']['shipping_rate_max_weight']['id'] = 'shipping_rate_max_weight';
  $handler->display->display_options['fields']['shipping_rate_max_weight']['table'] = 'field_data_shipping_rate_max_weight';
  $handler->display->display_options['fields']['shipping_rate_max_weight']['field'] = 'shipping_rate_max_weight';
  $handler->display->display_options['fields']['shipping_rate_max_weight']['click_sort_column'] = 'weight';
  /* Field: Commerce shipping Rate: Rate amount */
  $handler->display->display_options['fields']['shipping_rate_price']['id'] = 'shipping_rate_price';
  $handler->display->display_options['fields']['shipping_rate_price']['table'] = 'field_data_shipping_rate_price';
  $handler->display->display_options['fields']['shipping_rate_price']['field'] = 'shipping_rate_price';
  $handler->display->display_options['fields']['shipping_rate_price']['label'] = 'Amount';
  $handler->display->display_options['fields']['shipping_rate_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['shipping_rate_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce shipping rate: Shipping method name */
  $handler->display->display_options['fields']['method']['id'] = 'method';
  $handler->display->display_options['fields']['method']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['fields']['method']['field'] = 'method';
  $handler->display->display_options['fields']['method']['label'] = 'Shipping method';
  /* Field: Commerce shipping rate: Shipping service name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Shipping service';
  /* Field: Commerce shipping rate: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['add_destination'] = 1;
  /* Filter criterion: Commerce shipping rate: Shipping method name */
  $handler->display->display_options['filters']['method']['id'] = 'method';
  $handler->display->display_options['filters']['method']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['filters']['method']['field'] = 'method';
  $handler->display->display_options['filters']['method']['group'] = 1;
  $handler->display->display_options['filters']['method']['exposed'] = TRUE;
  $handler->display->display_options['filters']['method']['expose']['operator_id'] = 'method_op';
  $handler->display->display_options['filters']['method']['expose']['label'] = 'Method name';
  $handler->display->display_options['filters']['method']['expose']['operator'] = 'method_op';
  $handler->display->display_options['filters']['method']['expose']['identifier'] = 'method';
  /* Filter criterion: Commerce shipping rate: Shipping service name */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'commerce_shipping_rate';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['group'] = 1;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Service name';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  /* Filter criterion: Commerce shipping Rate: Minimun weight (shipping_rate_min_weight:weight) */
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['id'] = 'shipping_rate_min_weight_weight';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['table'] = 'field_data_shipping_rate_min_weight';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['field'] = 'shipping_rate_min_weight_weight';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['operator'] = '<';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['group'] = 1;
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['exposed'] = TRUE;
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['expose']['operator_id'] = 'shipping_rate_min_weight_weight_op';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['expose']['label'] = 'Minimun weight';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['expose']['operator'] = 'shipping_rate_min_weight_weight_op';
  $handler->display->display_options['filters']['shipping_rate_min_weight_weight']['expose']['identifier'] = 'shipping_rate_min_weight_weight';
  /* Filter criterion: Commerce shipping Rate: Maximum weight (shipping_rate_max_weight:weight) */
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['id'] = 'shipping_rate_max_weight_weight';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['table'] = 'field_data_shipping_rate_max_weight';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['field'] = 'shipping_rate_max_weight_weight';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['operator'] = '>=';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['group'] = 1;
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['exposed'] = TRUE;
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['expose']['operator_id'] = 'shipping_rate_max_weight_weight_op';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['expose']['label'] = 'Maximum weight';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['expose']['operator'] = 'shipping_rate_max_weight_weight_op';
  $handler->display->display_options['filters']['shipping_rate_max_weight_weight']['expose']['identifier'] = 'shipping_rate_max_weight_weight';
  $translatables['shipping_rates_manage'] = array(
    t('Master'),
    t('shipping rates'),
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
    t('Minimun weight'),
    t('Maximum weight'),
    t('Amount'),
    t('Shipping method'),
    t('Shipping service'),
    t('Operations'),
    t('Method name'),
    t('Service name'),
  );

  $views[$view->name] = $view;

  return $views;
}