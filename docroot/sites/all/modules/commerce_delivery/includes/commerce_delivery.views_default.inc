<?php

/**
 * @file
 * Implements hook_views_default_views().
 */
function commerce_delivery_views_default_views() {

  $field_id = variable_get('commerce_delivery_shippable_field', '');
  $field_info = field_info_field_by_id($field_id);
  $field_shippable = $field_info['field_name'];

  $views = array();

  $view = new view;
  $view->name = 'commerce_delivery';
  $view->description = 'Display a list of deliveries for the store admin.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_delivery';
  $view->human_name = 'Commerce delivery list';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Deliveries';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'commerce_delivery_manage_deliveries';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'delivery_id' => 'delivery_id',
    'order_id' => 'order_id',
    'status' => 'status',
    'delivery_id_1' => 'delivery_id_1',
    'delivery_id_2' => 'delivery_id_1',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'delivery_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'delivery_id_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '&nbsp;&nbsp;&nbsp;',
    ),
    'delivery_id_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Commerce Delivery: Delivery id */
  $handler->display->display_options['fields']['delivery_id']['id'] = 'delivery_id';
  $handler->display->display_options['fields']['delivery_id']['table'] = 'commerce_delivery';
  $handler->display->display_options['fields']['delivery_id']['field'] = 'delivery_id';
  $handler->display->display_options['fields']['delivery_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delivery_id']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['delivery_id']['alter']['path'] = 'admin/commerce/deliveries/[delivery_id]/view';
  $handler->display->display_options['fields']['delivery_id']['alter']['absolute'] = 1;
  $handler->display->display_options['fields']['delivery_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delivery_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delivery_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delivery_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delivery_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delivery_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delivery_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delivery_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['delivery_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delivery_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delivery_id']['empty_zero'] = 0;
  /* Field: Commerce Delivery: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_delivery';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['path'] = 'admin/commerce/orders/[order_id]/deliveries';
  $handler->display->display_options['fields']['order_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['order_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['order_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['order_id']['empty_zero'] = 0;
  /* Field: Commerce Delivery: Delivery status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_delivery';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  /* Field: Commerce Delivery: Delivery id */
  $handler->display->display_options['fields']['delivery_id_1']['id'] = 'delivery_id_1';
  $handler->display->display_options['fields']['delivery_id_1']['table'] = 'commerce_delivery';
  $handler->display->display_options['fields']['delivery_id_1']['field'] = 'delivery_id';
  $handler->display->display_options['fields']['delivery_id_1']['label'] = 'Operations';
  $handler->display->display_options['fields']['delivery_id_1']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['text'] = 'view';
  $handler->display->display_options['fields']['delivery_id_1']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['path'] = 'admin/commerce/deliveries/[delivery_id]/view';
  $handler->display->display_options['fields']['delivery_id_1']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['delivery_id_1']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delivery_id_1']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delivery_id_1']['hide_alter_empty'] = 0;
  /* Field: Commerce Delivery: Delivery id */
  $handler->display->display_options['fields']['delivery_id_2']['id'] = 'delivery_id_2';
  $handler->display->display_options['fields']['delivery_id_2']['table'] = 'commerce_delivery';
  $handler->display->display_options['fields']['delivery_id_2']['field'] = 'delivery_id';
  $handler->display->display_options['fields']['delivery_id_2']['label'] = '';
  $handler->display->display_options['fields']['delivery_id_2']['alter']['alter_text'] = 1;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['text'] = 'edit';
  $handler->display->display_options['fields']['delivery_id_2']['alter']['make_link'] = 1;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['path'] = 'admin/commerce/deliveries/[delivery_id]/edit';
  $handler->display->display_options['fields']['delivery_id_2']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delivery_id_2']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delivery_id_2']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delivery_id_2']['hide_alter_empty'] = 0;
  /* Sort criterion: Commerce Delivery: Delivery id */
  $handler->display->display_options['sorts']['delivery_id']['id'] = 'delivery_id';
  $handler->display->display_options['sorts']['delivery_id']['table'] = 'commerce_delivery';
  $handler->display->display_options['sorts']['delivery_id']['field'] = 'delivery_id';
  /* Filter criterion: Commerce Delivery: Delivery status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_delivery';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['exposed'] = TRUE;
  $handler->display->display_options['filters']['status']['expose']['operator_id'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['label'] = 'Delivery status';
  $handler->display->display_options['filters']['status']['expose']['use_operator'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = 'status_op';
  $handler->display->display_options['filters']['status']['expose']['identifier'] = 'status';
  $handler->display->display_options['filters']['status']['expose']['multiple'] = 1;
  $handler->display->display_options['filters']['status']['expose']['reduce'] = 0;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['path'] = 'admin/commerce/deliveries/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Deliveries';
  $handler->display->display_options['tab_options']['description'] = 'Manage deliveries in the store.';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $handler->display->display_options['tab_options']['name'] = 'management';

  $views[$view->name] = $view;

  $view = new view;
  $view->name = 'commerce_delivery_view';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Commerce delivery view';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Commerce delivery view';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access orders';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['autosubmit'] = 0;
  $handler->display->display_options['exposed_form']['options']['autosubmit_hide'] = 1;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '25';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = 'commerce_customer_billing';
  $handler->display->display_options['style_options']['columns'] = array(
    'sku' => 'sku',
    'line_item_label' => 'line_item_label',
    'quantity' => 'quantity',
    'commerce_customer_billing' => 'commerce_customer_billing',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'sku' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'line_item_label' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'quantity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'commerce_customer_billing' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = FALSE;
  /* Relationship: Line item: Referenced product */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['required'] = 0;
  /* Relationship: Commerce Line Item: Order ID */
  $handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['required'] = 0;
  /* Field: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['sku']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['external'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['sku']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['sku']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['sku']['alter']['html'] = 0;
  $handler->display->display_options['fields']['sku']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['sku']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['sku']['hide_empty'] = 0;
  $handler->display->display_options['fields']['sku']['empty_zero'] = 0;
  $handler->display->display_options['fields']['sku']['link_to_product'] = 0;
  /* Field: Commerce Line Item: Label */
  $handler->display->display_options['fields']['line_item_label']['id'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_label']['field'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['external'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['line_item_label']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['line_item_label']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['line_item_label']['alter']['html'] = 0;
  $handler->display->display_options['fields']['line_item_label']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['line_item_label']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['line_item_label']['hide_empty'] = 0;
  $handler->display->display_options['fields']['line_item_label']['empty_zero'] = 0;
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['quantity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['quantity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['quantity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['quantity']['set_precision'] = 0;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  $handler->display->display_options['fields']['quantity']['format_plural'] = 0;
  /* Field: Order: Billing information */
  $handler->display->display_options['fields']['commerce_customer_billing']['id'] = 'commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['table'] = 'field_data_commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['field'] = 'commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['relationship'] = 'order_id';
  $handler->display->display_options['fields']['commerce_customer_billing']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['external'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['alter']['html'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['commerce_customer_billing']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['commerce_customer_billing']['hide_empty'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['empty_zero'] = 0;
  $handler->display->display_options['fields']['commerce_customer_billing']['type'] = 'commerce_customer_profile_reference_display';
  $handler->display->display_options['fields']['commerce_customer_billing']['field_api_classes'] = 0;
  /* Contextual filter: Line item: Delivery id (delivery_ref) */
  $handler->display->display_options['arguments']['delivery_ref_value']['id'] = 'delivery_ref_value';
  $handler->display->display_options['arguments']['delivery_ref_value']['table'] = 'field_data_delivery_ref';
  $handler->display->display_options['arguments']['delivery_ref_value']['field'] = 'delivery_ref_value';
  $handler->display->display_options['arguments']['delivery_ref_value']['title_enable'] = 1;
  $handler->display->display_options['arguments']['delivery_ref_value']['title'] = 'Delivery note # %1';
  $handler->display->display_options['arguments']['delivery_ref_value']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['delivery_ref_value']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['delivery_ref_value']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['delivery_ref_value']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['delivery_ref_value']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['delivery_ref_value']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['delivery_ref_value']['not'] = 0;
  /* Filter criterion: Product: Shippable (field_shippable) */
  $handler->display->display_options['filters'][$field_shippable . '_value']['id'] = $field_shippable . '_value';
  $handler->display->display_options['filters'][$field_shippable . '_value']['table'] = 'field_data_' . $field_shippable;
  $handler->display->display_options['filters'][$field_shippable . '_value']['field'] = $field_shippable . '_value';
  $handler->display->display_options['filters'][$field_shippable . '_value']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['filters'][$field_shippable . '_value']['value'] = array(
    1 => '1',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/deliveries/%/view';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'View';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['tab_options']['weight'] = '0';
  $translatables['commerce_delivery_view'] = array(
    t('Master'),
    t('Commerce delivery view'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Product'),
    t('Order'),
    t('SKU'),
    t('Label'),
    t('Quantity'),
    t('.'),
    t(','),
    t('Billing information'),
    t('All'),
    t('Delivery note # %1'),
    t('Page'),
  );

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'commerce_delivery_by_order';
  $view->description = 'Display a given order line items by delivery';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Commerce delivery by order';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer orders';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = 'delivery_ref';
  $handler->display->display_options['style_options']['columns'] = array(
    'delivery_ref' => 'delivery_ref',
    'order_id' => 'order_id',
    'sku' => 'sku',
    'line_item_label' => 'line_item_label',
    'quantity' => 'quantity',
    'set_delivery' => 'set_delivery',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'delivery_ref' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'sku' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'line_item_label' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'quantity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'set_delivery' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['order'] = 'desc';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['content'] = 'No results for this case.';
  /* Relationship: Commerce Line item: Referenced product */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['required'] = TRUE;
  /* Field: Commerce Line item: Delivery id */
  $handler->display->display_options['fields']['delivery_ref']['id'] = 'delivery_ref';
  $handler->display->display_options['fields']['delivery_ref']['table'] = 'field_data_delivery_ref';
  $handler->display->display_options['fields']['delivery_ref']['field'] = 'delivery_ref';
  $handler->display->display_options['fields']['delivery_ref']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delivery_ref']['alter']['text'] = 'admin/commerce';
  $handler->display->display_options['fields']['delivery_ref']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['delivery_ref']['alter']['path'] = 'admin/commerce/deliveries/[delivery_ref-value]/view';
  $handler->display->display_options['fields']['delivery_ref']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['delivery_ref']['empty'] = 'Not currently in a delivery';
  $handler->display->display_options['fields']['delivery_ref']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  /* Field: Commerce Line Item: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['exclude'] = TRUE;
  /* Field: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['sku']['link_to_product'] = 0;
  /* Field: Commerce Line Item: Label */
  $handler->display->display_options['fields']['line_item_label']['id'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_label']['field'] = 'line_item_label';
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Field: Commerce Line Item: Move item to delivery */
  $handler->display->display_options['fields']['set_delivery']['id'] = 'set_delivery';
  $handler->display->display_options['fields']['set_delivery']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['set_delivery']['field'] = 'set_delivery';
  /* Sort criterion: Commerce Line item: Delivery id (delivery_ref) */
  $handler->display->display_options['sorts']['delivery_ref_value']['id'] = 'delivery_ref_value';
  $handler->display->display_options['sorts']['delivery_ref_value']['table'] = 'field_data_delivery_ref';
  $handler->display->display_options['sorts']['delivery_ref_value']['field'] = 'delivery_ref_value';
  /* Contextual filter: Commerce Line Item: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'empty';
  $handler->display->display_options['arguments']['order_id']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['order_id']['title'] = 'Order %1';
  $handler->display->display_options['arguments']['order_id']['breadcrumb_enable'] = TRUE;
  $handler->display->display_options['arguments']['order_id']['breadcrumb'] = 'Title';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['order_id']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['order_id']['validate']['type'] = 'numeric';
  $handler->display->display_options['arguments']['order_id']['validate']['fail'] = 'empty';
  /* Filter criterion: Fields: Shippable (field_shippable) */
  $handler->display->display_options['filters'][$field_shippable . '_value']['id'] = $field_shippable . '_value';
  $handler->display->display_options['filters'][$field_shippable . '_value']['table'] = 'field_data_' . $field_shippable;
  $handler->display->display_options['filters'][$field_shippable . '_value']['field'] = $field_shippable . '_value';
  $handler->display->display_options['filters'][$field_shippable . '_value']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['filters'][$field_shippable . '_value']['value'] = array(
    1 => '1',
  );
  $translatables['commerce_delivery_by_order'] = array(
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
    t('No results for this case.'),
    t('Product'),
    t('Delivery id'),
    t('admin/commerce'),
    t('admin/commerce/deliveries/[delivery_ref-value]/view'),
    t('Order ID'),
    t('SKU'),
    t('Label'),
    t('Quantity'),
    t('.'),
    t(','),
    t('Move item to delivery'),
    t('All'),
    t('Order %1'),
    t('Title'),
  );

  $views[$view->name] = $view;


  $view = new view;
  $view->name = 'commerce_delivery_for_order';
  $view->description = 'Displays orders deliveries for customers.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Commerce delivery for order';
  $view->core = 7;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['grouping'] = 'delivery_ref';
  $handler->display->display_options['style_options']['columns'] = array(
    'title' => 'title',
    'commerce_unit_price' => 'commerce_unit_price',
    'quantity' => 'quantity',
    'commerce_total' => 'commerce_total',
    'delivery_ref' => 'delivery_ref',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'title' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'commerce_unit_price' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'quantity' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'commerce_total' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'delivery_ref' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Relationship: Commerce Line item: Referenced product */
  $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
  $handler->display->display_options['relationships']['commerce_product_product_id']['required'] = 1;
  /* Field: Commerce Line item: Delivery id */
  $handler->display->display_options['fields']['delivery_ref']['id'] = 'delivery_ref';
  $handler->display->display_options['fields']['delivery_ref']['table'] = 'field_data_delivery_ref';
  $handler->display->display_options['fields']['delivery_ref']['field'] = 'delivery_ref';
  $handler->display->display_options['fields']['delivery_ref']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delivery_ref']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['external'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delivery_ref']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delivery_ref']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['delivery_ref']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delivery_ref']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['empty_zero'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['delivery_ref']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['fields']['delivery_ref']['field_api_classes'] = 0;
  /* Field: Commerce Product: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['title']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['title']['alter']['external'] = 0;
  $handler->display->display_options['fields']['title']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['title']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['title']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['title']['alter']['html'] = 0;
  $handler->display->display_options['fields']['title']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['title']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['title']['hide_empty'] = 0;
  $handler->display->display_options['fields']['title']['empty_zero'] = 0;
  $handler->display->display_options['fields']['title']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Field: Commerce Line item: Unit price */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['external'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['alter']['html'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['commerce_unit_price']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['commerce_unit_price']['hide_empty'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['empty_zero'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_unit_price']['field_api_classes'] = 0;
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['external'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['quantity']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['quantity']['alter']['html'] = 0;
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['quantity']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['quantity']['hide_empty'] = 0;
  $handler->display->display_options['fields']['quantity']['empty_zero'] = 0;
  $handler->display->display_options['fields']['quantity']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['quantity']['set_precision'] = 0;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  $handler->display->display_options['fields']['quantity']['format_plural'] = 0;
  /* Field: Commerce Line item: Total */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['external'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['commerce_total']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['commerce_total']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['commerce_total']['alter']['html'] = 0;
  $handler->display->display_options['fields']['commerce_total']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['commerce_total']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['commerce_total']['hide_empty'] = 0;
  $handler->display->display_options['fields']['commerce_total']['empty_zero'] = 0;
  $handler->display->display_options['fields']['commerce_total']['hide_alter_empty'] = 0;
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_total']['field_api_classes'] = 0;
  /* Sort criterion: Commerce Line item: Delivery id (delivery_ref) */
  $handler->display->display_options['sorts']['delivery_ref_value']['id'] = 'delivery_ref_value';
  $handler->display->display_options['sorts']['delivery_ref_value']['table'] = 'field_data_delivery_ref';
  $handler->display->display_options['sorts']['delivery_ref_value']['field'] = 'delivery_ref_value';
  /* Contextual filter: Commerce Line Item: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['order_id']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['order_id']['not'] = 0;
  /* Filter criterion: Commerce Product: Levererbar (field_prod_ent_deliverable) */
  $handler->display->display_options['filters']['field_prod_ent_deliverable_value']['id'] = 'field_prod_ent_deliverable_value';
  $handler->display->display_options['filters']['field_prod_ent_deliverable_value']['table'] = 'field_data_field_prod_ent_deliverable';
  $handler->display->display_options['filters']['field_prod_ent_deliverable_value']['field'] = 'field_prod_ent_deliverable_value';
  $handler->display->display_options['filters']['field_prod_ent_deliverable_value']['relationship'] = 'commerce_product_product_id';
  $handler->display->display_options['filters']['field_prod_ent_deliverable_value']['value'] = array(
    1 => '1',
  );
  $translatables['commerce_delivery_for_order'] = array(
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
    t('Product'),
    t('Delivery id'),
    t('Title'),
    t('Unit price'),
    t('Quantity'),
    t('.'),
    t(','),
    t('Total'),
    t('All'),
  );

  $views[$view->name] = $view;

  return $views;
}

