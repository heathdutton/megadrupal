<?php

/**
 * @file
 * Views defaults for Commerce discount module.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_return_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'commerce_return_overview';
  $view->description = 'Display a list of returns';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_return';
  $view->human_name = 'Returns';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['css_class'] = 'commerce-return-overview-form';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'return_number' => 'return_number',
    'commerce_order_reference' => 'commerce_order_reference',
    'changed' => 'changed',
    'commerce_customer_address_name_line' => 'commerce_customer_address_name_line',
    'commerce_return_total' => 'commerce_return_total',
    'status' => 'status',
    'operations_dropbutton' => 'operations_dropbutton',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'return_number' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_reference' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_customer_address_name_line' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_return_total' => array(
      'sortable' => 1,
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
    'operations_dropbutton' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['style_options']['empty_table'] = TRUE;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No returns found. Please reach an order\'s edit page in order to add a new return.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Commerce Return: Referenced customer profile */
  $handler->display->display_options['relationships']['commerce_customer_rma_profile_id']['id'] = 'commerce_customer_rma_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_rma_profile_id']['table'] = 'field_data_commerce_customer_rma';
  $handler->display->display_options['relationships']['commerce_customer_rma_profile_id']['field'] = 'commerce_customer_rma_profile_id';
  $handler->display->display_options['relationships']['commerce_customer_rma_profile_id']['required'] = TRUE;
  /* Field: Commerce Return: Return number */
  $handler->display->display_options['fields']['return_number']['id'] = 'return_number';
  $handler->display->display_options['fields']['return_number']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['return_number']['field'] = 'return_number';
  $handler->display->display_options['fields']['return_number']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['return_number']['alter']['text'] = 'Return [return_number]';
  $handler->display->display_options['fields']['return_number']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['return_number']['alter']['path'] = 'admin/commerce/store/returns/[return_number]/view';
  $handler->display->display_options['fields']['return_number']['alter']['absolute'] = TRUE;
  $handler->display->display_options['fields']['return_number']['alter']['alt'] = 'Return [return_number]';
  /* Field: Commerce Return: Order Reference */
  $handler->display->display_options['fields']['commerce_order_reference']['id'] = 'commerce_order_reference';
  $handler->display->display_options['fields']['commerce_order_reference']['table'] = 'field_data_commerce_order_reference';
  $handler->display->display_options['fields']['commerce_order_reference']['field'] = 'commerce_order_reference';
  $handler->display->display_options['fields']['commerce_order_reference']['label'] = 'Order Id';
  $handler->display->display_options['fields']['commerce_order_reference']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['commerce_order_reference']['alter']['path'] = 'admin/commerce/orders/[commerce_order_reference-target_id]';
  $handler->display->display_options['fields']['commerce_order_reference']['settings'] = array(
    'link' => 0,
  );
  /* Field: Commerce Return: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'medium';
  /* Field: Commerce Customer profile: Address - Full name */
  $handler->display->display_options['fields']['commerce_customer_address_name_line']['id'] = 'commerce_customer_address_name_line';
  $handler->display->display_options['fields']['commerce_customer_address_name_line']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address_name_line']['field'] = 'commerce_customer_address_name_line';
  $handler->display->display_options['fields']['commerce_customer_address_name_line']['relationship'] = 'commerce_customer_rma_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address_name_line']['label'] = 'Name';
  /* Field: Commerce Return: Total to refund */
  $handler->display->display_options['fields']['commerce_return_total']['id'] = 'commerce_return_total';
  $handler->display->display_options['fields']['commerce_return_total']['table'] = 'field_data_commerce_return_total';
  $handler->display->display_options['fields']['commerce_return_total']['field'] = 'commerce_return_total';
  $handler->display->display_options['fields']['commerce_return_total']['label'] = 'Total';
  $handler->display->display_options['fields']['commerce_return_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_return_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce Return: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Field: Commerce Return: Operations */
  $handler->display->display_options['fields']['operations_dropbutton']['id'] = 'operations_dropbutton';
  $handler->display->display_options['fields']['operations_dropbutton']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['operations_dropbutton']['field'] = 'operations_dropbutton';
  /* Filter criterion: Commerce Return: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_return';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'commerce_return' => 'commerce_return',
  );
  /* Filter criterion: Commerce Return: Return number */
  $handler->display->display_options['filters']['return_number']['id'] = 'return_number';
  $handler->display->display_options['filters']['return_number']['table'] = 'commerce_return';
  $handler->display->display_options['filters']['return_number']['field'] = 'return_number';
  $handler->display->display_options['filters']['return_number']['exposed'] = TRUE;
  $handler->display->display_options['filters']['return_number']['expose']['operator_id'] = 'return_number_op';
  $handler->display->display_options['filters']['return_number']['expose']['label'] = 'Return number';
  $handler->display->display_options['filters']['return_number']['expose']['operator'] = 'return_number_op';
  $handler->display->display_options['filters']['return_number']['expose']['identifier'] = 'return_number';
  $handler->display->display_options['filters']['return_number']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['return_number']['group_info']['label'] = 'Return number';
  $handler->display->display_options['filters']['return_number']['group_info']['identifier'] = 'return_number';
  $handler->display->display_options['filters']['return_number']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['return_number']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );
  /* Filter criterion: Commerce Return: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_return';
  $handler->display->display_options['filters']['status']['field'] = 'status';
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
  /* Filter criterion: Commerce Return: Order Reference (commerce_order_reference) */
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['id'] = 'commerce_order_reference_target_id';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['table'] = 'field_data_commerce_order_reference';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['field'] = 'commerce_order_reference_target_id';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['exposed'] = TRUE;
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['expose']['operator_id'] = 'commerce_order_reference_target_id_op';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['expose']['label'] = 'Order ID';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['expose']['operator'] = 'commerce_order_reference_target_id_op';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['expose']['identifier'] = 'commerce_order_reference_target_id';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['group_info']['label'] = 'Order Reference (commerce_order_reference)';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['group_info']['identifier'] = 'commerce_order_reference_target_id';
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['group_info']['remember'] = FALSE;
  $handler->display->display_options['filters']['commerce_order_reference_target_id']['group_info']['group_items'] = array(
    1 => array(),
    2 => array(),
    3 => array(),
  );

  $views['commerce_return_overview'] = $view;

  $view = new view();
  $view->name = 'user_returns';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_return';
  $view->human_name = 'User returns';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'User returns';
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own commerce_return entities of bundle commerce_return';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Commerce Return: Owner ID */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['exclude'] = TRUE;
  /* Field: Commerce Return: Return ID */
  $handler->display->display_options['fields']['return_id']['id'] = 'return_id';
  $handler->display->display_options['fields']['return_id']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['return_id']['field'] = 'return_id';
  $handler->display->display_options['fields']['return_id']['exclude'] = TRUE;
  /* Field: Commerce Return: Return number */
  $handler->display->display_options['fields']['return_number']['id'] = 'return_number';
  $handler->display->display_options['fields']['return_number']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['return_number']['field'] = 'return_number';
  $handler->display->display_options['fields']['return_number']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['return_number']['alter']['text'] = 'Return [return_id]';
  $handler->display->display_options['fields']['return_number']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['return_number']['alter']['path'] = 'user/[uid]/returns/[return_id]';
  /* Field: Commerce Return: Date created */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: Commerce Return: Date changed */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Updated';
  $handler->display->display_options['fields']['changed']['date_format'] = 'long';
  /* Field: Commerce Return: Total to refund */
  $handler->display->display_options['fields']['commerce_return_total']['id'] = 'commerce_return_total';
  $handler->display->display_options['fields']['commerce_return_total']['table'] = 'field_data_commerce_return_total';
  $handler->display->display_options['fields']['commerce_return_total']['field'] = 'commerce_return_total';
  $handler->display->display_options['fields']['commerce_return_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_return_total']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce Return: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_return';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  /* Contextual filter: Commerce Return: Owner ID */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'commerce_return';
  $handler->display->display_options['arguments']['uid']['field'] = 'uid';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['specify_validation'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = TRUE;
  $handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
    3 => '3',
  );
  /* Filter criterion: Commerce Return: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_return';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'commerce_return' => 'commerce_return',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'user/%/returns';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Returns';
  $handler->display->display_options['menu']['weight'] = '16';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['user_returns'] = array(
    t('Master'),
    t('User returns'),
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
    t('Owner ID'),
    t('.'),
    t(','),
    t('Return ID'),
    t('Return number'),
    t('Return [return_id]'),
    t('Created'),
    t('Updated'),
    t('Total to refund'),
    t('Status'),
    t('All'),
    t('Page'),
  );

  $views['user_returns'] = $view;

  return $views;
}

/**
 * Implements hook_views_default_views_alter().
 */
function commerce_return_views_default_views_alter(&$views) {
  if (isset($views['commerce_line_item_table'])) {
    /* Display: Returns */
    $handler = $views['commerce_line_item_table']->new_display('default', 'Returns', 'return');
    $handler->display->display_options['display_description'] = 'Displaying line items for a return.';
    $handler->display->display_options['use_more_always'] = FALSE;
    $handler->display->display_options['access']['type'] = 'none';
    $handler->display->display_options['cache']['type'] = 'none';
    $handler->display->display_options['query']['type'] = 'views_query';
    $handler->display->display_options['query']['options']['query_comment'] = FALSE;
    $handler->display->display_options['exposed_form']['type'] = 'basic';
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['style_plugin'] = 'table';
    $handler->display->display_options['style_options']['columns'] = array(
      'title' => 'title',
      'sku' => 'sku',
      'quantity' => 'quantity',
      'commerce_total' => 'commerce_total',
      'commerce_return_reason' => 'commerce_return_reason',
      'commerce_return_status' => 'commerce_return_status',
    );
    $handler->display->display_options['style_options']['default'] = '-1';
    $handler->display->display_options['style_options']['info'] = array(
      'title' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      ),
      'sku' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      ),
      'quantity' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      ),
      'commerce_total' => array(
        'sortable' => 0,
        'default_sort_order' => 'asc',
        'align' => 'views-align-right',
        'separator' => '',
        'empty_column' => 0,
      ),
      'commerce_return_reason' => array(
        'align' => '',
        'separator' => '',
        'empty_column' => 0,
      ),
      'commerce_return_status' => array(
        'align' => '',
        'separator' => '',
        'empty_column' => 1,
      ),
    );
    $handler->display->display_options['style_options']['empty_table'] = TRUE;
    /* No results behavior: Global: Text area */
    $handler->display->display_options['empty']['area']['id'] = 'area';
    $handler->display->display_options['empty']['area']['table'] = 'views';
    $handler->display->display_options['empty']['area']['field'] = 'area';
    $handler->display->display_options['empty']['area']['empty'] = TRUE;
    $handler->display->display_options['empty']['area']['content'] = 'No product was found.';
    $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
    /* Relationship: Commerce Line item: Referenced products */
    $handler->display->display_options['relationships']['commerce_product_product_id']['id'] = 'commerce_product_product_id';
    $handler->display->display_options['relationships']['commerce_product_product_id']['table'] = 'field_data_commerce_product';
    $handler->display->display_options['relationships']['commerce_product_product_id']['field'] = 'commerce_product_product_id';
    $handler->display->display_options['relationships']['commerce_product_product_id']['required'] = TRUE;
    /* Field: Commerce Product: Title */
    $handler->display->display_options['fields']['title']['id'] = 'title';
    $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
    $handler->display->display_options['fields']['title']['field'] = 'title';
    $handler->display->display_options['fields']['title']['relationship'] = 'commerce_product_product_id';
    $handler->display->display_options['fields']['title']['link_to_product'] = 0;
    /* Field: Commerce Product: SKU */
    $handler->display->display_options['fields']['sku']['id'] = 'sku';
    $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
    $handler->display->display_options['fields']['sku']['field'] = 'sku';
    $handler->display->display_options['fields']['sku']['relationship'] = 'commerce_product_product_id';
    $handler->display->display_options['fields']['sku']['link_to_product'] = 0;
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
    $handler->display->display_options['fields']['commerce_total']['settings'] = array(
      'calculation' => FALSE,
    );
    /* Field: Commerce Line Item: Return reason */
    $handler->display->display_options['fields']['commerce_return_reason']['id'] = 'commerce_return_reason';
    $handler->display->display_options['fields']['commerce_return_reason']['table'] = 'commerce_line_item';
    $handler->display->display_options['fields']['commerce_return_reason']['field'] = 'commerce_return_reason';
    $handler->display->display_options['fields']['commerce_return_reason']['label'] = 'Customer reason';
    /* Field: Commerce Line Item: Return status */
    $handler->display->display_options['fields']['commerce_return_status']['id'] = 'commerce_return_status';
    $handler->display->display_options['fields']['commerce_return_status']['table'] = 'commerce_line_item';
    $handler->display->display_options['fields']['commerce_return_status']['field'] = 'commerce_return_status';
    $handler->display->display_options['fields']['commerce_return_status']['label'] = 'Status';
    /* Contextual filter: Commerce Line Item: Line item ID */
    $handler->display->display_options['arguments']['line_item_id']['id'] = 'line_item_id';
    $handler->display->display_options['arguments']['line_item_id']['table'] = 'commerce_line_item';
    $handler->display->display_options['arguments']['line_item_id']['field'] = 'line_item_id';
    $handler->display->display_options['arguments']['line_item_id']['default_action'] = 'empty';
    $handler->display->display_options['arguments']['line_item_id']['default_argument_type'] = 'fixed';
    $handler->display->display_options['arguments']['line_item_id']['summary']['number_of_records'] = '0';
    $handler->display->display_options['arguments']['line_item_id']['summary']['format'] = 'default_summary';
    $handler->display->display_options['arguments']['line_item_id']['summary_options']['items_per_page'] = '25';
    $handler->display->display_options['arguments']['line_item_id']['break_phrase'] = TRUE;
    $translatables['commerce_line_item_table'] = array(
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
      t('.'),
      t(','),
      t('Type'),
      t('Title'),
      t('Label'),
      t('([line_item_label])'),
      t('Unit price'),
      t('Quantity'),
      t('Total'),
      t('All'),
      t('Returns'),
      t('Displaying line items for a return.'),
      t('No product was found.'),
      t('Products referenced by commerce_product'),
      t('SKU'),
      t('Customer reason'),
      t('Status'),
    );
  }
}
