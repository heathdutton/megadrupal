<?php
/**
 * @file
 * commerce_order2pdf.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_order2pdf_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'commerce_order2pdf';
  $view->description = '';
  $view->tag = 'Commerce order2pdf';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Commerce order2pdf';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['hide_admin_links'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '1';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: Commerce Order: Line items */
  $handler->display->display_options['fields']['commerce_line_items']['id'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['field'] = 'commerce_line_items';
  $handler->display->display_options['fields']['commerce_line_items']['label'] = '';
  $handler->display->display_options['fields']['commerce_line_items']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_line_items']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_line_items']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_line_items']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_line_items']['settings'] = array(
    'view' => 'commerce_order2pdf_line_item_table|default',
  );
  $handler->display->display_options['fields']['commerce_line_items']['delta_offset'] = '0';
  $handler->display->display_options['fields']['commerce_line_items']['field_api_classes'] = TRUE;
  /* Field: Commerce Order: Order total */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = '';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_components';
  $handler->display->display_options['fields']['commerce_order_total']['field_api_classes'] = TRUE;
  /* Field: Commerce Order: Billing information */
  $handler->display->display_options['fields']['commerce_customer_billing']['id'] = 'commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['table'] = 'field_data_commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['field'] = 'commerce_customer_billing';
  $handler->display->display_options['fields']['commerce_customer_billing']['element_label_type'] = 'strong';
  $handler->display->display_options['fields']['commerce_customer_billing']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_billing']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_billing']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_billing']['field_api_classes'] = TRUE;
  /* Contextual filter: Commerce Order: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';


  $export[$view->name] = $view;


  $view = new view();
  $view->name = 'commerce_order2pdf_line_item_table';
  $view->description = 'Display a set of line items in a table.';
  $view->tag = 'Commerce order2pdf';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Commerce order2pdf line items';
  $view->core = 0;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['hide_admin_links'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'line_item_id' => 'line_item_id',
    'type' => 'type',
    'line_item_title' => 'line_item_title',
    'line_item_label' => 'line_item_title',
    'commerce_unit_price' => 'commerce_unit_price',
    'quantity' => 'quantity',
    'commerce_total' => 'commerce_total',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'line_item_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'type' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'line_item_title' => array(
      'align' => '',
      'separator' => ' ',
    ),
    'line_item_label' => array(
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
      'align' => 'views-align-right',
      'separator' => '',
    ),
  );
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['label'] = 'Empty line item text.';
  $handler->display->display_options['empty']['area']['content'] = 'No line items found.';
  $handler->display->display_options['empty']['area']['format'] = 'plain_text';
  /* Field: Commerce Line Item: Line item ID */
  $handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['fields']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['fields']['line_item_id']['label'] = 'ID';
  $handler->display->display_options['fields']['line_item_id']['exclude'] = TRUE;
  /* Field: Commerce Line Item: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['exclude'] = TRUE;
  /* Field: Commerce Line Item: Title */
  $handler->display->display_options['fields']['line_item_title']['id'] = 'line_item_title';
  $handler->display->display_options['fields']['line_item_title']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_title']['field'] = 'line_item_title';
  /* Field: Commerce Line Item: Label */
  $handler->display->display_options['fields']['line_item_label']['id'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_label']['field'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['line_item_label']['alter']['text'] = '([line_item_label])';
  /* Field: Commerce Line item: Unit price */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_formatted_amount';
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Field: Commerce Line item: Total */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_price_formatted_amount';
  /* Sort criterion: Commerce Line Item: Line item ID */
  $handler->display->display_options['sorts']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['sorts']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['line_item_id']['field'] = 'line_item_id';
  /* Contextual filter: Commerce Line Item: Line item ID */
  $handler->display->display_options['arguments']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['arguments']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['arguments']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['arguments']['line_item_id']['default_action'] = 'default';
  $handler->display->display_options['arguments']['line_item_id']['default_argument_type'] = 'node';
  $handler->display->display_options['arguments']['line_item_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['line_item_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['line_item_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['line_item_id']['break_phrase'] = TRUE;
  /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '1';
  $translatables['commerce_order2pdf_line_item_table'] = array(
    t('Defaults'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Empty line item text.'),
    t('No line items found.'),
    t('ID'),
    t('Type'),
    t('Title'),
    t('Label'),
    t('([line_item_label])'),
    t('Unit price'),
    t('Quantity'),
    t('.'),
    t(','),
    t('Total'),
    t('All'),
  );
  $export[$view->name] = $view;
  return $export;
}
