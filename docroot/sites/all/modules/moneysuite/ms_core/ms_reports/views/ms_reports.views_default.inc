<?php

/**
 * Implementation of hook_views_default_views().
 */
function ms_reports_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'taxes';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'ms_order_adjustments';
  $view->human_name = 'Taxes';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Taxes';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view moneysuite reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'created' => 'created',
    'rendered_entity' => 'rendered_entity',
    'display' => 'display',
    'amount' => 'amount',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'rendered_entity' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'display' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'amount' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Order Adjustment: Oid */
  $handler->display->display_options['relationships']['oid']['id'] = 'oid';
  $handler->display->display_options['relationships']['oid']['table'] = 'ms_order_adjustments';
  $handler->display->display_options['relationships']['oid']['field'] = 'oid';
  $handler->display->display_options['relationships']['oid']['required'] = TRUE;
  /* Relationship: Order: Payment */
  $handler->display->display_options['relationships']['ms_payments']['id'] = 'ms_payments';
  $handler->display->display_options['relationships']['ms_payments']['table'] = 'ms_orders';
  $handler->display->display_options['relationships']['ms_payments']['field'] = 'ms_payments';
  $handler->display->display_options['relationships']['ms_payments']['relationship'] = 'oid';
  $handler->display->display_options['relationships']['ms_payments']['required'] = TRUE;
  /* Field: Payment: Created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'ms_payments';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['relationship'] = 'ms_payments';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  /* Field: Order: Rendered Order */
  $handler->display->display_options['fields']['rendered_entity']['id'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['table'] = 'views_entity_ms_order';
  $handler->display->display_options['fields']['rendered_entity']['field'] = 'rendered_entity';
  $handler->display->display_options['fields']['rendered_entity']['relationship'] = 'oid';
  $handler->display->display_options['fields']['rendered_entity']['label'] = 'Order';
  $handler->display->display_options['fields']['rendered_entity']['link_to_entity'] = 1;
  /* Field: Order Adjustment: Display name */
  $handler->display->display_options['fields']['display']['id'] = 'display';
  $handler->display->display_options['fields']['display']['table'] = 'ms_order_adjustments';
  $handler->display->display_options['fields']['display']['field'] = 'display';
  $handler->display->display_options['fields']['display']['label'] = 'Type';
  /* Field: Order Adjustment: Amount */
  $handler->display->display_options['fields']['amount']['id'] = 'amount';
  $handler->display->display_options['fields']['amount']['table'] = 'views_entity_ms_order_adjustment';
  $handler->display->display_options['fields']['amount']['field'] = 'amount';
  $handler->display->display_options['fields']['amount']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['amount']['precision'] = '2';
  $handler->display->display_options['fields']['amount']['link_to_entity'] = 0;
  /* Filter criterion: Payment: Created */
  $handler->display->display_options['filters']['created']['id'] = 'created';
  $handler->display->display_options['filters']['created']['table'] = 'ms_payments';
  $handler->display->display_options['filters']['created']['field'] = 'created';
  $handler->display->display_options['filters']['created']['relationship'] = 'ms_payments';
  $handler->display->display_options['filters']['created']['operator'] = 'between';
  $handler->display->display_options['filters']['created']['exposed'] = TRUE;
  $handler->display->display_options['filters']['created']['expose']['operator_id'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['label'] = 'Date between';
  $handler->display->display_options['filters']['created']['expose']['operator'] = 'created_op';
  $handler->display->display_options['filters']['created']['expose']['identifier'] = 'created';
  $handler->display->display_options['filters']['created']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
    7 => 0,
    8 => 0,
    9 => 0,
  );
  /* Filter criterion: Payment: Amount */
  $handler->display->display_options['filters']['amount']['id'] = 'amount';
  $handler->display->display_options['filters']['amount']['table'] = 'ms_payments';
  $handler->display->display_options['filters']['amount']['field'] = 'amount';
  $handler->display->display_options['filters']['amount']['relationship'] = 'ms_payments';
  $handler->display->display_options['filters']['amount']['operator'] = '!=';
  $handler->display->display_options['filters']['amount']['value']['value'] = '0';
  /* Filter criterion: Order Adjustment: Machine Name */
  $handler->display->display_options['filters']['id']['id'] = 'id';
  $handler->display->display_options['filters']['id']['table'] = 'ms_order_adjustments';
  $handler->display->display_options['filters']['id']['field'] = 'id';
  $handler->display->display_options['filters']['id']['operator'] = 'contains';
  $handler->display->display_options['filters']['id']['value'] = 'ms_tax';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['path'] = 'admin/moneysuite/reports/taxes';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Taxes';
  $handler->display->display_options['menu']['description'] = 'Generate tax reports';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $translatables['taxes'] = array(
    t('Master'),
    t('Taxes'),
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
    t('Order'),
    t('Payment'),
    t('Date'),
    t('Type'),
    t('Amount'),
    t('.'),
    t(','),
    t('Date between'),
    t('Page'),
  );
  $views[] = $view;

  return $views;
}