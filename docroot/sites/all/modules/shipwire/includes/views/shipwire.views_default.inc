<?php

/**
 * @file
 * Views for shipwire shipments.
 */

/**
 * Implements hook_views_default_views().
 */
function shipwire_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'shipwire_shipments';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'shipwire_shipments';
  $view->human_name = 'Shipwire Shipments';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Shipments';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view all shipwire shipments';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'shipwire_shipment_id' => 'shipwire_shipment_id',
    'order_id' => 'order_id',
    'shipwire_id' => 'shipwire_id',
    'changed' => 'changed',
    'cost_total' => 'cost_total',
    'shipping_method' => 'shipping_method',
    'tracking_number' => 'tracking_number',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = 'shipwire_shipment_id';
  $handler->display->display_options['style_options']['info'] = array(
    'shipwire_shipment_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipwire_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'cost_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'shipping_method' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'tracking_number' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: Shipwire shipment: Shipment ID */
  $handler->display->display_options['fields']['shipwire_shipment_id']['id'] = 'shipwire_shipment_id';
  $handler->display->display_options['fields']['shipwire_shipment_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['shipwire_shipment_id']['field'] = 'shipwire_shipment_id';
  $handler->display->display_options['fields']['shipwire_shipment_id']['link_to_shipment'] = 'admin';
  /* Field: Shipwire shipment: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Shipwire shipment: Shipwire ID */
  $handler->display->display_options['fields']['shipwire_id']['id'] = 'shipwire_id';
  $handler->display->display_options['fields']['shipwire_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['shipwire_id']['field'] = 'shipwire_id';
  /* Field: Shipwire shipment: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Shipwire shipment: Total cost */
  $handler->display->display_options['fields']['cost_total']['id'] = 'cost_total';
  $handler->display->display_options['fields']['cost_total']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['cost_total']['field'] = 'cost_total';
  /* Field: Shipwire shipment: Shipping method */
  $handler->display->display_options['fields']['shipping_method']['id'] = 'shipping_method';
  $handler->display->display_options['fields']['shipping_method']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['shipping_method']['field'] = 'shipping_method';
  /* Field: Shipwire shipment: Tracking number */
  $handler->display->display_options['fields']['tracking_number']['id'] = 'tracking_number';
  $handler->display->display_options['fields']['tracking_number']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['tracking_number']['field'] = 'tracking_number';
  /* Field: Shipwire shipment: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'Shipment status';
  /* Filter criterion: Shipwire shipment: Shipment ID */
  $handler->display->display_options['filters']['shipwire_shipment_id']['id'] = 'shipwire_shipment_id';
  $handler->display->display_options['filters']['shipwire_shipment_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['filters']['shipwire_shipment_id']['field'] = 'shipwire_shipment_id';
  $handler->display->display_options['filters']['shipwire_shipment_id']['group'] = 1;
  $handler->display->display_options['filters']['shipwire_shipment_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['shipwire_shipment_id']['expose']['operator_id'] = 'shipwire_shipment_id_op';
  $handler->display->display_options['filters']['shipwire_shipment_id']['expose']['label'] = 'Shipment ID';
  $handler->display->display_options['filters']['shipwire_shipment_id']['expose']['operator'] = 'shipwire_shipment_id_op';
  $handler->display->display_options['filters']['shipwire_shipment_id']['expose']['identifier'] = 'shipwire_shipment_id';
  $handler->display->display_options['filters']['shipwire_shipment_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Shipwire shipment: Order ID */
  $handler->display->display_options['filters']['order_id']['id'] = 'order_id';
  $handler->display->display_options['filters']['order_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['filters']['order_id']['field'] = 'order_id';
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
  /* Filter criterion: Shipwire shipment: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'shipwire_shipments';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Status';
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );

  /* Display: Shipments */
  $handler = $view->new_display('page', 'Shipments', 'page');
  $handler->display->display_options['path'] = 'admin/shipwire/shipments';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Shipments';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Held */
  $handler = $view->new_display('page', 'Held', 'page_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Held shipments';
  $handler->display->display_options['defaults']['group_by'] = FALSE;
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'shipwire_shipment_id' => 'shipwire_shipment_id',
    'messages' => 'messages',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'shipwire_shipment_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'messages' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['relationships'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Shipwire shipment: Shipment ID */
  $handler->display->display_options['fields']['shipwire_shipment_id']['id'] = 'shipwire_shipment_id';
  $handler->display->display_options['fields']['shipwire_shipment_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['shipwire_shipment_id']['field'] = 'shipwire_shipment_id';
  $handler->display->display_options['fields']['shipwire_shipment_id']['link_to_shipment'] = 'admin';
  /* Field: Shipwire shipment: Shipwire ID */
  $handler->display->display_options['fields']['shipwire_id']['id'] = 'shipwire_id';
  $handler->display->display_options['fields']['shipwire_id']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['shipwire_id']['field'] = 'shipwire_id';
  /* Field: Shipwire shipment: Messages */
  $handler->display->display_options['fields']['messages']['id'] = 'messages';
  $handler->display->display_options['fields']['messages']['table'] = 'shipwire_shipments';
  $handler->display->display_options['fields']['messages']['field'] = 'messages';
  $handler->display->display_options['fields']['messages']['empty'] = 'No held shipments found';
  $handler->display->display_options['fields']['messages']['hide_alter_empty'] = FALSE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Shipwire shipment: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'shipwire_shipments';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'held' => 'held',
  );
  $handler->display->display_options['path'] = 'admin/shipwire';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Shipwire';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
