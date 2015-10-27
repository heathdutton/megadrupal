<?php

/**
 * @file
 * Views for listing rates filtered by shipping service name, exporting
 * shipping sert
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_socolissimo_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'socolissimo_flexibility_rates';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_socolissimo_rate';
  $view->human_name = 'SoColissimo flexibility rates';
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
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'socolissimo_flex_min_weight' => 'socolissimo_flex_min_weight',
    'socolissimo_flex_max_weight' => 'socolissimo_flex_max_weight',
    'socolissimo_flex_rate_fr' => 'socolissimo_flex_rate_fr',
    'socolissimo_flex_rate_be' => 'socolissimo_flex_rate_be',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'socolissimo_flex_min_weight';
  $handler->display->display_options['style_options']['info'] = array(
    'socolissimo_flex_min_weight' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'socolissimo_flex_max_weight' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'socolissimo_flex_rate_fr' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'socolissimo_flex_rate_be' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['sticky'] = TRUE;
  /* Field: SoColissimo Flexibility Rate: Minimun weight */
  $handler->display->display_options['fields']['socolissimo_flex_min_weight']['id'] = 'socolissimo_flex_min_weight';
  $handler->display->display_options['fields']['socolissimo_flex_min_weight']['table'] = 'field_data_socolissimo_flex_min_weight';
  $handler->display->display_options['fields']['socolissimo_flex_min_weight']['field'] = 'socolissimo_flex_min_weight';
  $handler->display->display_options['fields']['socolissimo_flex_min_weight']['label'] = 'Minimun weight';
  $handler->display->display_options['fields']['socolissimo_flex_min_weight']['click_sort_column'] = 'weight';
  /* Field: SoColissimo Flexibility Rate: Maximum weight */
  $handler->display->display_options['fields']['socolissimo_flex_max_weight']['id'] = 'socolissimo_flex_max_weight';
  $handler->display->display_options['fields']['socolissimo_flex_max_weight']['table'] = 'field_data_socolissimo_flex_max_weight';
  $handler->display->display_options['fields']['socolissimo_flex_max_weight']['field'] = 'socolissimo_flex_max_weight';
  $handler->display->display_options['fields']['socolissimo_flex_max_weight']['label'] = 'Maximum weight';
  $handler->display->display_options['fields']['socolissimo_flex_max_weight']['click_sort_column'] = 'weight';
  /* Field: SoColissimo Flexibility Rate: French rate amount */
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['id'] = 'socolissimo_flex_rate_fr';
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['table'] = 'field_data_socolissimo_flex_rate_fr';
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['field'] = 'socolissimo_flex_rate_fr';
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['label'] = 'French amount';
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['socolissimo_flex_rate_fr']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: So Colissimo Flexibility rate: Belgium rate amount */
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['id'] = 'socolissimo_flex_rate_be';
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['table'] = 'field_data_socolissimo_flex_rate_be';
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['field'] = 'socolissimo_flex_rate_be';
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['label'] = 'Belgium amount';
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['socolissimo_flex_rate_be']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce So Colissimo Flexibility rate: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_socolissimo_rate';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['operations']['add_destination'] = 0;
  /* Contextual filter: Commerce So Colissimo Flexibility rate: So Colissimo shipping service name */
  $handler->display->display_options['arguments']['name']['id'] = 'name';
  $handler->display->display_options['arguments']['name']['table'] = 'commerce_socolissimo_rate';
  $handler->display->display_options['arguments']['name']['field'] = 'name';
  $handler->display->display_options['arguments']['name']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['name']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['name']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['name']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['name']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['name']['limit'] = '0';
  /* Filter criterion: Commerce So Colissimo Flexibility rate: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_socolissimo_rate';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = 'flexibility_rate';
  $translatables['socolissimo_flexibility_rates'] = array(
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
    t('Maximum weight'),
    t('Minimun weight'),
    t('French amount'),
    t('Belgium amount'),
    t('Operations'),
    t('All'),
  );
  $views[$view->name] = $view;

  return $views;
}

/**
 * Implements hook_views_default_views_alter().
 *
 * Alter commerce_order default views in order to add the "Socolissimo orders"
 * display.
 */
function commerce_socolissimo_views_default_views_alter(&$views) {
  if (isset($views['commerce_backoffice_orders'])) {
    /* Display: So Colissimo orders */
    $handler = $views['commerce_backoffice_orders']->new_display('page', 'Socolissimo', 'socolissimo_page');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'So Colissimo orders';
    $handler->display->display_options['defaults']['footer'] = FALSE;
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Commerce Order: Referenced line item */
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: Bulk operations: Commerce Order */
    $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
    $handler->display->display_options['fields']['views_bulk_operations']['label'] = '';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '50';
    $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
      'action::views_bulk_operations_delete_item' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 1,
        'label' => 'Delete order',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_commerce_order_status_canceled' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_commerce_order_status_cart' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_order_status_checkout_checkout' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_order_status_checkout_complete' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_order_status_checkout_payment' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_order_status_checkout_review' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_order_status_checkout_shipping' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_commerce_order_status_completed' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_commerce_order_status_pending' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
      'rules_component::rules_commerce_order_status_processing' => array(
        'selected' => 1,
        'skip_confirmation' => 0,
        'override_label' => 0,
        'label' => '',
        'postpone_processing' => 0,
      ),
    );
    /* Field: Commerce Order: Order number */
    $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
    $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
    $handler->display->display_options['fields']['order_number']['label'] = 'ID';
    /* Field: Commerce Order: Date de création */
    $handler->display->display_options['fields']['created']['id'] = 'created';
    $handler->display->display_options['fields']['created']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['created']['field'] = 'created';
    $handler->display->display_options['fields']['created']['label'] = 'Created';
    $handler->display->display_options['fields']['created']['date_format'] = 'short';
    /* Field: Commerce Order: Total to refund */
    $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Total';
    $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
    $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
    $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
      'calculation' => FALSE,
    );
    /* Field: Commerce Order: Order status */
    $handler->display->display_options['fields']['status']['id'] = 'status';
    $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['status']['field'] = 'status';
    $handler->display->display_options['fields']['status']['label'] = 'Status';
    /* Field: Commerce Order: Operations links (Dropbutton) */
    $handler->display->display_options['fields']['operations_dropbutton']['id'] = 'operations_dropbutton';
    $handler->display->display_options['fields']['operations_dropbutton']['table'] = 'views_entity_commerce_order';
    $handler->display->display_options['fields']['operations_dropbutton']['field'] = 'operations_dropbutton';
    $handler->display->display_options['fields']['operations_dropbutton']['label'] = 'Operations';
    $handler->display->display_options['fields']['operations_dropbutton']['add_destination'] = 1;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Line Item: Type */
    $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
    $handler->display->display_options['filters']['type_1']['table'] = 'commerce_line_item';
    $handler->display->display_options['filters']['type_1']['field'] = 'type';
    $handler->display->display_options['filters']['type_1']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['type_1']['value'] = array(
      'shipping' => 'shipping',
    );
    /* Filter criterion: Commerce Order: Order type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      'commerce_order' => 'commerce_order',
    );
    /* Filter criterion: Commerce Line item: Shipping service (commerce_shipping_service) */
    $handler->display->display_options['filters']['commerce_shipping_service_value']['id'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['field'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['value'] = array(
      'socolissimo_home_delivery' => 'socolissimo_home_delivery',
      'socolissimo_appointment' => 'socolissimo_appointment',
      'socolissimo_cityssimo' => 'socolissimo_cityssimo',
      'socolissimo_post_office' => 'socolissimo_post_office',
      'socolissimo_local_store' => 'socolissimo_local_store',
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['exposed'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator_id'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['label'] = 'Shipping service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['required'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember_roles'] = array(
      2 => '2',
      3 => '3',
      1 => 0,
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['reduce'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['label'] = 'Shipping service (commerce_shipping_service)';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['remember'] = FALSE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['group_items'] = array(
      1 => array(),
      2 => array(),
      3 => array(),
    );
    $handler->display->display_options['path'] = 'admin/commerce/orders/socolissimo';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'So Colissimo orders';
    $handler->display->display_options['menu']['description'] = 'Only orders with So Colissimo shipping services.';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['name'] = 'management';
    $handler->display->display_options['menu']['context'] = 0;
    $handler->display->display_options['menu']['context_only_inline'] = 0;

    /* Display: Data export */
    $handler = $views['commerce_backoffice_orders']->new_display('views_data_export', 'Data export', 'views_orders_socolissimo_export');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Data export';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
    $handler->display->display_options['style_options']['provide_file'] = 1;
    $handler->display->display_options['style_options']['filename'] = '%timestamp-dd.%timestamp-mm.%timestamp-yyyy.colissimo-orders.export.xls';
    $handler->display->display_options['style_options']['parent_sort'] = 0;
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Commerce Order: Referenced line item */
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: Commerce Order: Order number */
    $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
    $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
    $handler->display->display_options['fields']['order_number']['label'] = 'Numéro de commande';
    /* Field: Commerce Order: Date de création */
    $handler->display->display_options['fields']['created']['id'] = 'created';
    $handler->display->display_options['fields']['created']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['created']['field'] = 'created';
    $handler->display->display_options['fields']['created']['label'] = 'Created';
    $handler->display->display_options['fields']['created']['date_format'] = 'short';
    /* Field: Commerce Order: Order status */
    $handler->display->display_options['fields']['status']['id'] = 'status';
    $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['status']['field'] = 'status';
    $handler->display->display_options['fields']['status']['label'] = 'Status';
    /* Field: Commerce Line item: Shipping service */
    $handler->display->display_options['fields']['commerce_shipping_service']['id'] = 'commerce_shipping_service';
    $handler->display->display_options['fields']['commerce_shipping_service']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['fields']['commerce_shipping_service']['field'] = 'commerce_shipping_service';
    $handler->display->display_options['fields']['commerce_shipping_service']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['fields']['commerce_shipping_service']['label'] = 'Service de livraison';
    $handler->display->display_options['fields']['commerce_shipping_service']['element_type'] = '0';
    $handler->display->display_options['fields']['commerce_shipping_service']['element_wrapper_type'] = '0';
    $handler->display->display_options['fields']['commerce_shipping_service']['element_default_classes'] = FALSE;
    $handler->display->display_options['fields']['commerce_shipping_service']['field_api_classes'] = TRUE;
    /* Field: Commerce Order: Total to refund */
    $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Total';
    $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
    $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
    $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
      'calculation' => FALSE,
    );
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Line Item: Type */
    $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
    $handler->display->display_options['filters']['type_1']['table'] = 'commerce_line_item';
    $handler->display->display_options['filters']['type_1']['field'] = 'type';
    $handler->display->display_options['filters']['type_1']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['type_1']['value'] = array(
      'shipping' => 'shipping',
    );
    $handler->display->display_options['filters']['type_1']['group'] = 1;
    /* Filter criterion: Commerce Order: Order type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      'commerce_order' => 'commerce_order',
    );
    $handler->display->display_options['filters']['type']['group'] = 1;
    /* Filter criterion: Commerce Line item: Shipping service (commerce_shipping_service) */
    $handler->display->display_options['filters']['commerce_shipping_service_value']['id'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['field'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['value'] = array(
      'socolissimo_home_delivery' => 'socolissimo_home_delivery',
      'socolissimo_appointment' => 'socolissimo_appointment',
      'socolissimo_cityssimo' => 'socolissimo_cityssimo',
      'socolissimo_post_office' => 'socolissimo_post_office',
      'socolissimo_local_store' => 'socolissimo_local_store',
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group'] = 1;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['exposed'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator_id'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['label'] = 'Shipping service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['required'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['reduce'] = TRUE;
    $handler->display->display_options['path'] = 'admin/commerce/orders/socolissimo/export';
    $handler->display->display_options['displays'] = array(
      'socolissimo_page' => 'socolissimo_page',
      'default' => 0,
      'admin_page' => 0,
    );
    $handler->display->display_options['sitename_title'] = 0;

    /* Display: Data export: Expeditor INET */
    $handler = $views['commerce_backoffice_orders']->new_display('views_data_export', 'Expeditor INET', 'views_data_export_expeditor_inet');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Expeditor INET';
    $handler->display->display_options['display_description'] = 'Export orders as a formatted file usable by So Colissimo services.';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'views_data_export_expeditor_inet';
    $handler->display->display_options['style_options']['provide_file'] = 1;
    $handler->display->display_options['style_options']['parent_sort'] = 0;
    $handler->display->display_options['defaults']['empty'] = FALSE;
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Commerce Order: Referenced line item */
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: Commerce Order: Order ID */
    $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
    $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
    $handler->display->display_options['defaults']['sorts'] = FALSE;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Order: Order type */
    $handler->display->display_options['filters']['type']['id'] = 'type';
    $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['type']['field'] = 'type';
    $handler->display->display_options['filters']['type']['value'] = array(
      'commerce_order' => 'commerce_order',
    );
    /* Filter criterion: Commerce Line Item: Type */
    $handler->display->display_options['filters']['type_1']['id'] = 'type_1';
    $handler->display->display_options['filters']['type_1']['table'] = 'commerce_line_item';
    $handler->display->display_options['filters']['type_1']['field'] = 'type';
    $handler->display->display_options['filters']['type_1']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['type_1']['value'] = array(
      'shipping' => 'shipping',
    );
    /* Filter criterion: Commerce Line item: Shipping service (commerce_shipping_service) */
    $handler->display->display_options['filters']['commerce_shipping_service_value']['id'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['field'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['value'] = array(
      'socolissimo_home_delivery' => 'socolissimo_home_delivery',
      'socolissimo_appointment' => 'socolissimo_appointment',
      'socolissimo_cityssimo' => 'socolissimo_cityssimo',
      'socolissimo_post_office' => 'socolissimo_post_office',
      'socolissimo_local_store' => 'socolissimo_local_store',
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['exposed'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator_id'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['label'] = 'Shipping service (commerce_shipping_service)';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['required'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['reduce'] = TRUE;
    $handler->display->display_options['path'] = 'admin/commerce/orders/socolissimo/expeditor-inet';
    $handler->display->display_options['displays'] = array(
      'socolissimo_page' => 'socolissimo_page',
      'default' => 0,
      'admin_page' => 0,
    );
    $handler->display->display_options['sitename_title'] = 0;
    $translatables['commerce_backoffice_orders'] = array(
      t('Master'),
      t('Orders'),
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
      t('Customer profile'),
      t('Order owner'),
      t('ID'),
      t('Created'),
      t('Total'),
      t('Status'),
      t('Operations'),
      t('Shipping service'),
      t('[commerce_shipping_service-value]'),
      t('All'),
      t('Filter by'),
      t('Admin page'),
      t('Socolissimo'),
      t('So Colissimo orders'),
      t('Line Item'),
      t('Shipping service (commerce_shipping_service)'),
      t('Data export'),
      t('Numéro de commande'),
      t('Service de livraison'),
      t('Expeditor INET'),
      t('Export orders as a formatted file usable by So Colissimo services.'),
      t('Order ID'),
    );
  }
  elseif (isset($views['commerce_orders'])) {
    /* Display: So Colissimo orders */
    $handler = $views['commerce_orders']->new_display('page', 'So Colissimo orders', 'views_orders_socolissimo');
    $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Commerce Order: Owner */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    /* Relationship: Commerce Order: Referenced customer profile */
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['id'] = 'commerce_customer_billing_profile_id';
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['table'] = 'field_data_commerce_customer_billing';
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['field'] = 'commerce_customer_billing_profile_id';
    /* Relationship: Commerce Order: Referenced line item */
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Order: Order state */
    $handler->display->display_options['filters']['state']['id'] = 'state';
    $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['state']['field'] = 'state';
    $handler->display->display_options['filters']['state']['operator'] = 'not in';
    $handler->display->display_options['filters']['state']['value'] = array(
      'cart' => 'cart',
      'checkout' => 'checkout',
    );
    $handler->display->display_options['filters']['state']['expose']['label'] = 'Order state';
    $handler->display->display_options['filters']['state']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
    $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
    /* Filter criterion: Commerce Line item: Shipping service (commerce_shipping_service) */
    $handler->display->display_options['filters']['commerce_shipping_service_value']['id'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['field'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['value'] = array(
      'socolissimo_home_delivery' => 'socolissimo_home_delivery',
      'socolissimo_appointment' => 'socolissimo_appointment',
      'socolissimo_cityssimo' => 'socolissimo_cityssimo',
      'socolissimo_post_office' => 'socolissimo_post_office',
      'socolissimo_local_store' => 'socolissimo_local_store',
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['exposed'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator_id'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['label'] = 'Shipping service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['required'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember_roles'] = array(
      2 => '2',
      3 => '3',
      1 => 0,
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['reduce'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['label'] = 'Shipping service (commerce_shipping_service)';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['remember'] = FALSE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['group_info']['group_items'] = array(
      1 => array(),
      2 => array(),
      3 => array(),
    );
    $handler->display->display_options['path'] = 'admin/commerce/orders/socolissimo';
    $handler->display->display_options['menu']['type'] = 'tab';
    $handler->display->display_options['menu']['title'] = 'So Colissimo orders';
    $handler->display->display_options['menu']['description'] = 'Only orders with So Colissimo shipping services.';
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['context'] = 0;

    /* Display: Data export */
    $handler = $views['commerce_orders']->new_display('views_data_export', 'Data export', 'views_orders_socolissimo_export');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'So Colissimo orders export';
    $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'views_data_export_xls';
    $handler->display->display_options['style_options']['provide_file'] = 1;
    $handler->display->display_options['style_options']['filename'] = '%timestamp-dd.%timestamp-mm.%timestamp-yyyy.colissimo-orders.export.xls';
    $handler->display->display_options['style_options']['parent_sort'] = 0;
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Commerce Order: Owner */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    /* Relationship: Commerce Order: Referenced customer profile */
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['id'] = 'commerce_customer_billing_profile_id';
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['table'] = 'field_data_commerce_customer_billing';
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['field'] = 'commerce_customer_billing_profile_id';
    /* Relationship: Commerce Order: Referenced line item */
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
    $handler->display->display_options['defaults']['fields'] = FALSE;
    /* Field: Commerce Order: Order number */
    $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
    $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
    $handler->display->display_options['fields']['order_number']['link_to_order'] = 'admin';
    /* Field: Commerce Order: Created date */
    $handler->display->display_options['fields']['created']['id'] = 'created';
    $handler->display->display_options['fields']['created']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['created']['field'] = 'created';
    $handler->display->display_options['fields']['created']['label'] = 'Created';
    $handler->display->display_options['fields']['created']['date_format'] = 'medium';
    /* Field: Commerce Customer profile: Address */
    $handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
    $handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
    $handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
    $handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_billing_profile_id';
    $handler->display->display_options['fields']['commerce_customer_address']['label'] = 'Name';
    $handler->display->display_options['fields']['commerce_customer_address']['empty'] = '-';
    $handler->display->display_options['fields']['commerce_customer_address']['hide_alter_empty'] = FALSE;
    $handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
    $handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
      'use_widget_handlers' => 0,
      'format_handlers' => array(
        'name-oneline' => 'name-oneline',
      ),
    );
    /* Field: User: Name */
    $handler->display->display_options['fields']['name']['id'] = 'name';
    $handler->display->display_options['fields']['name']['table'] = 'users';
    $handler->display->display_options['fields']['name']['field'] = 'name';
    $handler->display->display_options['fields']['name']['relationship'] = 'uid';
    $handler->display->display_options['fields']['name']['label'] = 'User';
    /* Field: Commerce Line item: Shipping service */
    $handler->display->display_options['fields']['commerce_shipping_service']['id'] = 'commerce_shipping_service';
    $handler->display->display_options['fields']['commerce_shipping_service']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['fields']['commerce_shipping_service']['field'] = 'commerce_shipping_service';
    $handler->display->display_options['fields']['commerce_shipping_service']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['fields']['commerce_shipping_service']['alter']['text'] = '[commerce_shipping_service-value]';
    /* Field: Commerce Order: Order status */
    $handler->display->display_options['fields']['status']['id'] = 'status';
    $handler->display->display_options['fields']['status']['table'] = 'commerce_order';
    $handler->display->display_options['fields']['status']['field'] = 'status';
    /* Field: Commerce Order: Total to refund */
    $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
    $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Total';
    $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
    $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
    $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
      'calculation' => FALSE,
    );
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Order: Order state */
    $handler->display->display_options['filters']['state']['id'] = 'state';
    $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['state']['field'] = 'state';
    $handler->display->display_options['filters']['state']['operator'] = 'not in';
    $handler->display->display_options['filters']['state']['value'] = array(
      'cart' => 'cart',
      'checkout' => 'checkout',
    );
    $handler->display->display_options['filters']['state']['expose']['label'] = 'Order state';
    $handler->display->display_options['filters']['state']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
    $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
    /* Filter criterion: Commerce Line item: Shipping service (commerce_shipping_service) */
    $handler->display->display_options['filters']['commerce_shipping_service_value']['id'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['field'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['value'] = array(
      'socolissimo_home_delivery' => 'socolissimo_home_delivery',
      'socolissimo_appointment' => 'socolissimo_appointment',
      'socolissimo_cityssimo' => 'socolissimo_cityssimo',
      'socolissimo_post_office' => 'socolissimo_post_office',
      'socolissimo_local_store' => 'socolissimo_local_store',
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['exposed'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator_id'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['label'] = 'Shipping service (commerce_shipping_service)';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['required'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember_roles'] = array(
      2 => '2',
      3 => '3',
      1 => 0,
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['reduce'] = TRUE;
    $handler->display->display_options['path'] = 'admin/commerce/orders/socolissimo/export';
    $handler->display->display_options['displays'] = array(
      'views_orders_socolissimo' => 'views_orders_socolissimo',
      'default' => 0,
      'admin_page' => 0,
      'shopping_carts' => 0,
    );
    $handler->display->display_options['sitename_title'] = 0;

    /* Display: Expeditor INET */
    $handler = $views['commerce_orders']->new_display('views_data_export', 'Expeditor INET', 'views_data_export_expeditor_inet');
    $handler->display->display_options['defaults']['title'] = FALSE;
    $handler->display->display_options['title'] = 'Orders';
    $handler->display->display_options['display_description'] = 'Export orders as a formatted file usable by So Colissimo services.';
    $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'views_data_export_expeditor_inet';
    $handler->display->display_options['style_options']['provide_file'] = 1;
    $handler->display->display_options['style_options']['parent_sort'] = 0;
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Commerce Order: Owner */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    /* Relationship: Commerce Order: Referenced customer profile */
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['id'] = 'commerce_customer_billing_profile_id';
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['table'] = 'field_data_commerce_customer_billing';
    $handler->display->display_options['relationships']['commerce_customer_billing_profile_id']['field'] = 'commerce_customer_billing_profile_id';
    /* Relationship: Commerce Order: Referenced line item */
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['id'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['table'] = 'field_data_commerce_line_items';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['field'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['relationships']['commerce_line_items_line_item_id']['required'] = TRUE;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: Commerce Order: Order state */
    $handler->display->display_options['filters']['state']['id'] = 'state';
    $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['state']['field'] = 'state';
    $handler->display->display_options['filters']['state']['operator'] = 'not in';
    $handler->display->display_options['filters']['state']['value'] = array(
      'cart' => 'cart',
      'checkout' => 'checkout',
    );
    $handler->display->display_options['filters']['state']['expose']['label'] = 'Order state';
    $handler->display->display_options['filters']['state']['expose']['use_operator'] = TRUE;
    $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
    $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
    /* Filter criterion: Commerce Line item: Shipping service (commerce_shipping_service) */
    $handler->display->display_options['filters']['commerce_shipping_service_value']['id'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['table'] = 'field_data_commerce_shipping_service';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['field'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['relationship'] = 'commerce_line_items_line_item_id';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['value'] = array(
      'socolissimo_home_delivery' => 'socolissimo_home_delivery',
      'socolissimo_appointment' => 'socolissimo_appointment',
      'socolissimo_cityssimo' => 'socolissimo_cityssimo',
      'socolissimo_post_office' => 'socolissimo_post_office',
      'socolissimo_local_store' => 'socolissimo_local_store',
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['exposed'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator_id'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['label'] = 'Shipping service (commerce_shipping_service)';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['operator'] = 'commerce_shipping_service_value_op';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['identifier'] = 'commerce_shipping_service_value';
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['required'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['multiple'] = TRUE;
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['remember_roles'] = array(
      2 => '2',
      3 => '3',
      1 => 0,
    );
    $handler->display->display_options['filters']['commerce_shipping_service_value']['expose']['reduce'] = TRUE;
    $handler->display->display_options['path'] = 'admin/commerce/orders/socolissimo/expeditor-inet';
    $handler->display->display_options['displays'] = array(
      'views_orders_socolissimo' => 'views_orders_socolissimo',
      'default' => 0,
      'admin_page' => 0,
      'shopping_carts' => 0,
    );
    $handler->display->display_options['sitename_title'] = 0;
    $translatables['commerce_backoffice_orders'] = array(
      t('Master'),
      t('Orders'),
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
      t('Customer profile'),
      t('Order owner'),
      t('ID'),
      t('Created'),
      t('Total'),
      t('Status'),
      t('Operations'),
      t('Shipping service'),
      t('[commerce_shipping_service-value]'),
      t('All'),
      t('Filter by'),
      t('Admin page'),
      t('Socolissimo'),
      t('So Colissimo orders'),
      t('Line Item'),
      t('Shipping service (commerce_shipping_service)'),
      t('Data export'),
      t('Numéro de commande'),
      t('Service de livraison'),
      t('Expeditor INET'),
      t('Export orders as a formatted file usable by So Colissimo services.'),
      t('Order ID'),
    );
  }
}
