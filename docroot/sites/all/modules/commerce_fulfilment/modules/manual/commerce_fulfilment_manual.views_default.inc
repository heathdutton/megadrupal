<?php
/**
 * @file
 * kigu_admin.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_fulfilment_manual_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'commerce_fulfilment_manual_shippinglabels';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Postage labels';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Postage labels';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce fulfilment';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'grid';
  $handler->display->display_options['style_options']['columns'] = '3';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Commerce Order: Owner */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'commerce_order';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  /* Relationship: Commerce Order: Referenced line item */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = '';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['order_id']['alter']['text'] = 'Order #[order_id]';
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid_1';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['text'] = '[name] ([order_id])';
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: Commerce Order: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = '';
  $handler->display->display_options['fields']['created']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created']['date_format'] = 'time ago';
  /* Field: Commerce Order: Order total */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = '';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Sort criterion: Commerce Order: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'commerce_order';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
  );
  /* Filter criterion: Commerce Line Item: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['type']['value'] = array(
    'shipping' => 'shipping',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = '';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['order_id']['alter']['text'] = 'Order #[order_id]';
  $handler->display->display_options['fields']['order_id']['element_type'] = 'h4';
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Commerce Order: Shipping information */
  $handler->display->display_options['fields']['commerce_customer_shipping']['id'] = 'commerce_customer_shipping';
  $handler->display->display_options['fields']['commerce_customer_shipping']['table'] = 'field_data_commerce_customer_shipping';
  $handler->display->display_options['fields']['commerce_customer_shipping']['field'] = 'commerce_customer_shipping';
  $handler->display->display_options['fields']['commerce_customer_shipping']['label'] = 'Ship to';
  $handler->display->display_options['fields']['commerce_customer_shipping']['element_label_type'] = 'strong';
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Shipping service';
  $handler->display->display_options['fields']['line_item_title']['element_type'] = 'div';
  $handler->display->display_options['fields']['line_item_title']['element_label_type'] = 'strong';
  /* Field: Commerce Order: Line items */
  $handler->display->display_options['fields']['commerce_line_items']['id'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['field'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['label'] = 'Package contents';
  $handler->display->display_options['fields']['commerce_line_items']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['commerce_line_items']['settings'] = array(
    'view' => 'commerce_fulfilment_manual_manifest_lineitems|default',
  );
  $handler->display->display_options['fields']['commerce_line_items']['delta_offset'] = '0';
  $handler->display->display_options['path'] = 'admin/commerce/orders/labels';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Postage labels';
  $handler->display->display_options['menu']['description'] = 'View a list of postage labels for pending orders.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $export['commerce_fulfilment_manual_shippinglabels'] = $view;


  $view = new view();
  $view->name = 'commerce_fulfilment_manual_manifest_lineitems';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Postage manifest line items';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce fulfilment';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['exclude'] = TRUE;
  /* Field: Commerce Line Item: Label */
  $handler->display->display_options['fields']['line_item_label']['id'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_label']['field'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['label'] = '';
  $handler->display->display_options['fields']['line_item_label']['exclude'] = TRUE;
  $handler->display->display_options['fields']['line_item_label']['element_label_colon'] = FALSE;
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['label'] = '';
  $handler->display->display_options['fields']['quantity']['exclude'] = TRUE;
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  $handler->display->display_options['fields']['quantity']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = '';
  $handler->display->display_options['fields']['nothing']['alter']['text'] = '[quantity]x [line_item_label]';
  $handler->display->display_options['fields']['nothing']['element_label_colon'] = FALSE;
  /* Contextual filter: Commerce Line Item: Line item ID */
  $handler->display->display_options['arguments']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['arguments']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['arguments']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['arguments']['line_item_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['line_item_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['line_item_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['line_item_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['line_item_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['line_item_id']['break_phrase'] = TRUE;
  /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '1';
  $export['commerce_fulfilment_manual_manifest_lineitems'] = $view;



  $view = new view();
  $view->name = 'commerce_fulfilment_manual_manifest';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Postage manifest';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Postage manifest';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce fulfilment';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'order_id' => 'order_id',
    'created_1' => 'created_1',
    'commerce_customer_address' => 'commerce_customer_address',
    'commerce_line_items' => 'commerce_line_items',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'order_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_customer_address' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_line_items' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Commerce Order: Owner */
  $handler->display->display_options['relationships']['uid_1']['id'] = 'uid_1';
  $handler->display->display_options['relationships']['uid_1']['table'] = 'commerce_order';
  $handler->display->display_options['relationships']['uid_1']['field'] = 'uid';
  /* Relationship: Commerce Order: Referenced customer profile */
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['id'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['table'] = 'field_data_commerce_customer_shipping';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['field'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_shipping_profile_id']['label'] = 'Shipping';
  /* Relationship: Commerce Order: Referenced line item */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'Reference';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['order_id']['alter']['text'] = '#[order_id]';
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Commerce Order: Created date */
  $handler->display->display_options['fields']['created_1']['id'] = 'created_1';
  $handler->display->display_options['fields']['created_1']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['created_1']['field'] = 'created';
  $handler->display->display_options['fields']['created_1']['label'] = 'Purchase date';
  $handler->display->display_options['fields']['created_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['created_1']['date_format'] = 'short';
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
    'use_widget_handlers' => 0,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_title']['label'] = 'Shipping service';
  /* Field: Commerce Order: Line items */
  $handler->display->display_options['fields']['commerce_line_items']['id'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['field'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['label'] = 'Order';
  $handler->display->display_options['fields']['commerce_line_items']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_line_items']['settings'] = array(
    'view' => 'commerce_fulfilment_manual_manifest_lineitems|default',
  );
  $handler->display->display_options['fields']['commerce_line_items']['delta_offset'] = '0';
  /* Sort criterion: Commerce Order: Created date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'commerce_order';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Commerce Order: Order status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = array(
    'pending' => 'pending',
  );
  /* Filter criterion: Commerce Line Item: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['type']['value'] = array(
    'shipping' => 'shipping',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['path'] = 'admin/commerce/orders/manifest';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Postage manifest';
  $handler->display->display_options['menu']['description'] = 'View postage details for pending orders.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: CSV */
  $handler = $view->new_display('views_data_export', 'CSV', 'export');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 0;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 1;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['path'] = 'admin/commerce/orders/manifest/csv';
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'default' => 0,
  );
  $export['commerce_fulfilment_manual_manifest'] = $view;

  return $export;
}
