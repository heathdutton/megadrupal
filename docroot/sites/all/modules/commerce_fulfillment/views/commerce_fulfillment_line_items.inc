<?php

/**
 * @file
 * Export code for the line_items view.
 */

$view = new view();
$view->name = 'commerce_fulfillment_line_items';
$view->description = 'Display a set of line items in a table.';
$view->tag = 'commerce';
$view->base_table = 'commerce_line_item';
$view->human_name = 'Commerce Fulfillment Line items';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
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
/* Relationship: Commerce Line Item: Order ID */
$handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
$handler->display->display_options['relationships']['order_id']['table'] = 'commerce_line_item';
$handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
$handler->display->display_options['relationships']['order_id']['required'] = TRUE;
/* Field: Bulk operations: Commerce Line Item */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_line_item';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::commerce_fulfillment_add_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 1,
    'label' => 'Add to Package',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_revision' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::commerce_fulfillment_packing_slip_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_remove_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 1,
    'label' => 'Remove from Package',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
/* Field: Commerce Line Item: Line item ID */
$handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
$handler->display->display_options['fields']['line_item_id']['table'] = 'commerce_line_item';
$handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
$handler->display->display_options['fields']['line_item_id']['label'] = 'ID';
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
/* Sort criterion: Commerce Line Item: Line item ID */
$handler->display->display_options['sorts']['line_item_id']['id'] = 'line_item_id';
$handler->display->display_options['sorts']['line_item_id']['table'] = 'commerce_line_item';
$handler->display->display_options['sorts']['line_item_id']['field'] = 'line_item_id';
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

/* Display: Line Items Block */
$handler = $view->new_display('block', 'Line Items Block', 'line_item_block');
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Bulk operations: Commerce Line Item */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_line_item';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::commerce_fulfillment_add_package_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_add_product_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_revision' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_script_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_modify_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'show_all_tokens' => 1,
      'display_values' => array(
        '_all_' => '_all_',
      ),
    ),
  ),
  'action::views_bulk_operations_argument_selector_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
    'settings' => array(
      'url' => '',
    ),
  ),
  'action::commerce_fulfillment_remove_product_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_remove_package_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::system_send_email_action' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
);
/* Field: Commerce Line Item: Line item ID */
$handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
$handler->display->display_options['fields']['line_item_id']['table'] = 'commerce_line_item';
$handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
$handler->display->display_options['fields']['line_item_id']['label'] = 'ID';
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
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Commerce Order: Order ID */
$handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
$handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['relationship'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['block_description'] = 'Line Items Block';

/* Display: Line Items in Package Block */
$handler = $view->new_display('block', 'Line Items in Package Block', 'package_line_item_block');
$handler->display->display_options['defaults']['relationships'] = FALSE;
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Commerce Line Item: Line item ID */
$handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
$handler->display->display_options['fields']['line_item_id']['table'] = 'commerce_line_item';
$handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
$handler->display->display_options['fields']['line_item_id']['label'] = 'ID';
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
$handler->display->display_options['block_description'] = 'Line Items in Package Block';
