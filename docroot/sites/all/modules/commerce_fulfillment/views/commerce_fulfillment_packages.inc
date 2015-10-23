<?php

/**
 * @file
 * Export code for the Packages view.
 */

$view = new view();
$view->name = 'commerce_fulfillment_packages';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'commerce_fulfillment_packages';
$view->human_name = 'Commerce Fulfillment Packages';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Commerce Fulfillment Packages';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['pager']['options']['offset'] = '0';
$handler->display->display_options['style_plugin'] = 'table_megarows';
$handler->display->display_options['style_options']['columns'] = array(
  'views_bulk_operations' => 'views_bulk_operations',
  'package_id' => 'package_id',
  'box_type' => 'box_type',
  'commerce_fulfillment_line_items' => 'commerce_fulfillment_line_items',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'views_bulk_operations' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'package_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'box_type' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'commerce_fulfillment_line_items' => array(
    'align' => '',
    'separator' => '',
    'empty_column' => 1,
  ),
);
$handler->display->display_options['style_options']['scroll_padding'] = '120';
$handler->display->display_options['row_plugin'] = 'entity';
$handler->display->display_options['row_options']['view_mode'] = 'token';
/* Field: Bulk operations: Commerce Fulfillment Package */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::commerce_fulfillment_add_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 1,
    'label' => 'Delete Package',
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
    'skip_confirmation' => 1,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_remove_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
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
/* Field: Commerce Fulfillment Package: Commerce fulfillment package ID */
$handler->display->display_options['fields']['package_id']['id'] = 'package_id';
$handler->display->display_options['fields']['package_id']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['package_id']['field'] = 'package_id';
/* Field: Commerce Fulfillment Package: Box_type */
$handler->display->display_options['fields']['box_type']['id'] = 'box_type';
$handler->display->display_options['fields']['box_type']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['box_type']['field'] = 'box_type';
$handler->display->display_options['fields']['box_type']['label'] = 'Package Type';
/* Field: Commerce Fulfillment Package: Line items */
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['id'] = 'commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['table'] = 'field_data_commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['field'] = 'commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['label'] = '';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['settings'] = array(
  'view' => 'commerce_fulfillment_line_items|package_line_item_block',
);
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['delta_offset'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['field_api_classes'] = TRUE;
/* Contextual filter: Commerce Fulfillment Package: Order_id */
$handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
$handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
$handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['order_id']['limit'] = '0';

/* Display: Package Page Block */
$handler = $view->new_display('block', 'Package Page Block', 'package_package_block');
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Bulk operations: Commerce Fulfillment Package */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = TRUE;
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
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 1,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 1,
    'label' => 'Delete Package',
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
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 1,
    'label' => 'Print Packing Slip',
    'settings' => array(
      'url' => 'admin/commerce/orders/commerce_fulfillment/packing_slip/',
    ),
  ),
  'action::commerce_fulfillment_remove_package_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_remove_product_action' => array(
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
/* Field: Commerce Fulfillment Package: Commerce fulfillment package ID */
$handler->display->display_options['fields']['package_id']['id'] = 'package_id';
$handler->display->display_options['fields']['package_id']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['package_id']['field'] = 'package_id';
$handler->display->display_options['fields']['package_id']['label'] = 'Package ID';
/* Field: Commerce Fulfillment Package: Box_type */
$handler->display->display_options['fields']['box_type']['id'] = 'box_type';
$handler->display->display_options['fields']['box_type']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['box_type']['field'] = 'box_type';
$handler->display->display_options['fields']['box_type']['label'] = 'Package Type';
/* Field: Commerce Fulfillment Package: Line items */
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['id'] = 'commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['table'] = 'field_data_commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['field'] = 'commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['label'] = '';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['settings'] = array(
  'view' => 'commerce_fulfillment_line_items|package_line_item_block',
);
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['delta_offset'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['field_api_classes'] = TRUE;
$handler->display->display_options['block_description'] = 'Commerce Fulfillment Package Block';

/* Display: Shipment Page Block */
$handler = $view->new_display('block', 'Shipment Page Block', 'package_shipment_block');
$handler->display->display_options['defaults']['fields'] = FALSE;
/* Field: Bulk operations: Commerce Fulfillment Package */
$handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
$handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
$handler->display->display_options['fields']['views_bulk_operations']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['views_bulk_operations']['hide_empty'] = TRUE;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '1';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
$handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
$handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
  'action::commerce_fulfillment_add_package_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_add_product_action' => array(
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::views_bulk_operations_delete_item' => array(
    'selected' => 0,
    'postpone_processing' => 0,
    'skip_confirmation' => 0,
    'override_label' => 1,
    'label' => 'Delete Package',
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
    'selected' => 0,
    'skip_confirmation' => 0,
    'override_label' => 0,
    'label' => '',
  ),
  'action::commerce_fulfillment_remove_package_action' => array(
    'selected' => 1,
    'skip_confirmation' => 1,
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
/* Field: Commerce Fulfillment Package: Commerce fulfillment package ID */
$handler->display->display_options['fields']['package_id']['id'] = 'package_id';
$handler->display->display_options['fields']['package_id']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['package_id']['field'] = 'package_id';
/* Field: Commerce Fulfillment Package: Box_type */
$handler->display->display_options['fields']['box_type']['id'] = 'box_type';
$handler->display->display_options['fields']['box_type']['table'] = 'commerce_fulfillment_packages';
$handler->display->display_options['fields']['box_type']['field'] = 'box_type';
$handler->display->display_options['fields']['box_type']['label'] = 'Package Type';
/* Field: Commerce Fulfillment Package: Line items */
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['id'] = 'commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['table'] = 'field_data_commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['field'] = 'commerce_fulfillment_line_items';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['label'] = '';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_label_colon'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_wrapper_type'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['element_default_classes'] = FALSE;
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['settings'] = array(
  'view' => 'commerce_fulfillment_line_items|package_line_item_block',
);
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['delta_offset'] = '0';
$handler->display->display_options['fields']['commerce_fulfillment_line_items']['field_api_classes'] = TRUE;
$handler->display->display_options['block_description'] = 'Commerce Fulfillment Shipment Page Package Block';
