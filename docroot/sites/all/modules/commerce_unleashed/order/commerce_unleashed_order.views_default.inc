<?php
/**
 * @file
 *   commerce_unleashed_order.views_default.inc
 */

/**
 * Impliments hook_views_detault_views().
 */
function commerce_unleashed_order_views_default_views() {
  $views = array();

  // Order view.
  $view = new view();
  $view->name = 'unleashed_order_json';
  $view->description = '';
  $view->tag = 'Unleashed, Commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Unleashed Order JSON';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['root_object'] = '';
  $handler->display->display_options['style_options']['top_child_object'] = '';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['remove_newlines'] = 0;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 1;
  $handler->display->display_options['style_options']['object_arrays'] = 0;
  $handler->display->display_options['style_options']['numeric_strings'] = 0;
  $handler->display->display_options['style_options']['bigint_string'] = 0;
  $handler->display->display_options['style_options']['pretty_print'] = 0;
  $handler->display->display_options['style_options']['unescaped_slashes'] = 0;
  $handler->display->display_options['style_options']['unescaped_unicode'] = 0;
  $handler->display->display_options['style_options']['char_encoding'] = array();
  /* Relationship: Commerce Order: Referenced customer profile */
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['id'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['table'] = 'field_data_commerce_customer_billing';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['field'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['required'] = TRUE;
  /* Field: Commerce Order: Commerce Order UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['label'] = 'Guid';
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = FALSE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'OrderNumber';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['order_id']['alter']['text'] = 'DC-[order_id]';
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  /* Field: Commerce Order: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'OrderDate';
  $handler->display->display_options['fields']['created']['alter']['text'] = '/Date([created])/';
  $handler->display->display_options['fields']['created']['date_format'] = 'custom';
  $handler->display->display_options['fields']['created']['custom_date_format'] = 'Y-m-d\\TH:i:s';
  /* Field: Commerce Order: Order status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['label'] = 'OrderStatus';
  $handler->display->display_options['fields']['status']['element_label_colon'] = FALSE;
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address']['label'] = 'DeliveryName';
  $handler->display->display_options['fields']['commerce_customer_address']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address']['alter']['text'] = '[commerce_customer_address-name_line]';
  $handler->display->display_options['fields']['commerce_customer_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
    'use_widget_handlers' => 0,
    'format_handlers' => array(
      'name-oneline' => 'name-oneline',
    ),
  );
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address_1']['id'] = 'commerce_customer_address_1';
  $handler->display->display_options['fields']['commerce_customer_address_1']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_1']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_1']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_1']['label'] = 'DeliveryStreetAddress';
  $handler->display->display_options['fields']['commerce_customer_address_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address_1']['alter']['text'] = '[commerce_customer_address_1-sub_premise]
    [commerce_customer_address_1-premise]
    [commerce_customer_address_1-thoroughfare]';
  $handler->display->display_options['fields']['commerce_customer_address_1']['alter']['trim_whitespace'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address_1']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address_1']['settings'] = array(
    'use_widget_handlers' => 0,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address_4']['id'] = 'commerce_customer_address_4';
  $handler->display->display_options['fields']['commerce_customer_address_4']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_4']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_4']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_4']['label'] = 'DeliveryCity';
  $handler->display->display_options['fields']['commerce_customer_address_4']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address_4']['alter']['text'] = '[commerce_customer_address_4-locality]';
  $handler->display->display_options['fields']['commerce_customer_address_4']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address_4']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address_4']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address_5']['id'] = 'commerce_customer_address_5';
  $handler->display->display_options['fields']['commerce_customer_address_5']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_5']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_5']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_5']['label'] = 'DeliverySuburb';
  $handler->display->display_options['fields']['commerce_customer_address_5']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address_5']['alter']['text'] = '[commerce_customer_address_5-locality]';
  $handler->display->display_options['fields']['commerce_customer_address_5']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address_5']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address_2']['id'] = 'commerce_customer_address_2';
  $handler->display->display_options['fields']['commerce_customer_address_2']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_2']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_2']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_2']['label'] = 'DeliveryPostCode';
  $handler->display->display_options['fields']['commerce_customer_address_2']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address_2']['alter']['text'] = '[commerce_customer_address_2-postal_code]';
  $handler->display->display_options['fields']['commerce_customer_address_2']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address_2']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address_2']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address_3']['id'] = 'commerce_customer_address_3';
  $handler->display->display_options['fields']['commerce_customer_address_3']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_3']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_3']['relationship'] = 'commerce_customer_billing_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_3']['label'] = 'DeliveryCountry';
  $handler->display->display_options['fields']['commerce_customer_address_3']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address_3']['alter']['text'] = '[commerce_customer_address_3-country]';
  $handler->display->display_options['fields']['commerce_customer_address_3']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address_3']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address_3']['settings'] = array(
    'use_widget_handlers' => 1,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  /* Field: Commerce Order: Order total */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'SubTotal';
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_unleashed_amount_without_currency';
  /* Field: Commerce Order: Order total */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Total';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_unleashed_amount_without_currency';
  /* Contextual filter: Commerce Order: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';

  $views[$view->name] = $view;

  // Line Item View.
  $view = new view();
  $view->name = 'unleashed_line_item_json';
  $view->description = '';
  $view->tag = 'Unleashed, Commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Unleashed Line Item JSON';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_json';
  $handler->display->display_options['style_options']['root_object'] = 'SalesOrderLines';
  $handler->display->display_options['style_options']['top_child_object'] = '';
  $handler->display->display_options['style_options']['plaintext_output'] = 1;
  $handler->display->display_options['style_options']['remove_newlines'] = 0;
  $handler->display->display_options['style_options']['jsonp_prefix'] = '';
  $handler->display->display_options['style_options']['using_views_api_mode'] = 1;
  $handler->display->display_options['style_options']['object_arrays'] = 0;
  $handler->display->display_options['style_options']['numeric_strings'] = 0;
  $handler->display->display_options['style_options']['bigint_string'] = 0;
  $handler->display->display_options['style_options']['pretty_print'] = 0;
  $handler->display->display_options['style_options']['unescaped_slashes'] = 0;
  $handler->display->display_options['style_options']['unescaped_unicode'] = 0;
  $handler->display->display_options['style_options']['char_encoding'] = array();
  /* Relationship: Commerce Order: Referenced line item */
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
  /* Field: Commerce Order: Line items (commerce_line_items:delta) */
  $handler->display->display_options['fields']['delta']['id'] = 'delta';
  $handler->display->display_options['fields']['delta']['table'] = 'field_data_commerce_line_items';
  $handler->display->display_options['fields']['delta']['field'] = 'delta';
  $handler->display->display_options['fields']['delta']['label'] = '';
  $handler->display->display_options['fields']['delta']['exclude'] = TRUE;
  $handler->display->display_options['fields']['delta']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['delta']['separator'] = '';
  /* Field: Global: Math expression */
  $handler->display->display_options['fields']['expression']['id'] = 'expression';
  $handler->display->display_options['fields']['expression']['table'] = 'views';
  $handler->display->display_options['fields']['expression']['field'] = 'expression';
  $handler->display->display_options['fields']['expression']['label'] = 'LineNumber';
  $handler->display->display_options['fields']['expression']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['expression']['precision'] = '0';
  $handler->display->display_options['fields']['expression']['separator'] = '';
  $handler->display->display_options['fields']['expression']['expression'] = '[delta] + 1';
  /* Field: Commerce Line Item: Commerce Line item UUID */
  $handler->display->display_options['fields']['uuid']['id'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['uuid']['field'] = 'uuid';
  $handler->display->display_options['fields']['uuid']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['uuid']['label'] = 'Guid';
  $handler->display->display_options['fields']['uuid']['element_label_colon'] = FALSE;
  /* Field: Commerce Line item: Unit price */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_unit_price']['label'] = 'UnitPrice';
  $handler->display->display_options['fields']['commerce_unit_price']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_unleashed_amount_without_currency';
  /* Field: Commerce Line item: Total */
  $handler->display->display_options['fields']['commerce_total']['id'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['table'] = 'field_data_commerce_total';
  $handler->display->display_options['fields']['commerce_total']['field'] = 'commerce_total';
  $handler->display->display_options['fields']['commerce_total']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_total']['label'] = 'LineTotal';
  $handler->display->display_options['fields']['commerce_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_total']['type'] = 'commerce_unleashed_amount_without_currency';
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['quantity']['label'] = 'OrderQuantity';
  $handler->display->display_options['fields']['quantity']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  $handler->display->display_options['fields']['quantity']['separator'] = '';
  /* Field: Commerce Line item: Product */
  $handler->display->display_options['fields']['commerce_product']['id'] = 'commerce_product';
  $handler->display->display_options['fields']['commerce_product']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['fields']['commerce_product']['field'] = 'commerce_product';
  $handler->display->display_options['fields']['commerce_product']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_product']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_product']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['commerce_product']['type'] = 'commerce_product_reference_sku_plain';
  $handler->display->display_options['fields']['commerce_product']['settings'] = array(
    'show_quantity' => 0,
    'default_quantity' => '1',
    'combine' => 0,
    'show_single_product_attributes' => 0,
    'line_item_type' => 0,
  );
  /* Field: Commerce Line item: Shipping service */
  $handler->display->display_options['fields']['commerce_shipping_service']['id'] = 'commerce_shipping_service';
  $handler->display->display_options['fields']['commerce_shipping_service']['table'] = 'field_data_commerce_shipping_service';
  $handler->display->display_options['fields']['commerce_shipping_service']['field'] = 'commerce_shipping_service';
  $handler->display->display_options['fields']['commerce_shipping_service']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_shipping_service']['label'] = 'Product';
  $handler->display->display_options['fields']['commerce_shipping_service']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_shipping_service']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_shipping_service']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_shipping_service']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['commerce_shipping_service']['type'] = 'list_key';
  $handler->display->display_options['fields']['commerce_shipping_service']['field_api_classes'] = TRUE;
  /* Contextual filter: Commerce Order: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';

  $views[$view->name] = $view;

  return $views;
}
