<?php

/**
 * @file
 * Customer View Displays
 */

function _commerce_reports_views_default_customer_views() {
  /**
   * Commerce reports: Customers
   */
  $view = new view();
  $view->name = 'commerce_reports_customers';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'users';
  $view->human_name = 'Customer reports';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Customer reports';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'order_id' => 'order_id',
    'commerce_order_total_1' => 'commerce_order_total_1',
    'commerce_order_total' => 'commerce_order_total',
  );
  $handler->display->display_options['style_options']['default'] = 'commerce_order_total_1';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
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
    'commerce_order_total_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_order_total' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: User: Orders */
  $handler->display->display_options['relationships']['uid_commerce_orders']['id'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['uid_commerce_orders']['table'] = 'users';
  $handler->display->display_options['relationships']['uid_commerce_orders']['field'] = 'uid_commerce_orders';
  $handler->display->display_options['relationships']['uid_commerce_orders']['required'] = TRUE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Customer';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  /* Field: COUNT(Commerce Order: Order ID) */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['order_id']['label'] = 'Orders';
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Total';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  /* Field: AVG(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total']['group_type'] = 'avg';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Average';
  $handler->display->display_options['fields']['commerce_order_total']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_order_total']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );
  $handler->display->display_options['fields']['commerce_order_total']['field_api_classes'] = TRUE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['filters']['state']['value'] = commerce_reports_reportable_order_states();

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['css_class'] = FALSE;
  $handler->display->display_options['css_class'] = 'commerce-reports';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: User: Name */
  $handler->display->display_options['filters']['uid']['id'] = 'uid';
  $handler->display->display_options['filters']['uid']['table'] = 'users';
  $handler->display->display_options['filters']['uid']['field'] = 'uid';
  $handler->display->display_options['filters']['uid']['value'] = '';
  $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
  $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['label'] = 'Search customers';
  $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
  $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['filters']['state']['value'] = commerce_reports_reportable_order_states();
  $handler->display->display_options['path'] = 'admin/commerce/reports/customers';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Customers';
  $handler->display->display_options['menu']['description'] = 'View customer reports.';
  $handler->display->display_options['menu']['weight'] = '10';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: Chart (by revenue) */
  $handler = $view->new_display('block', 'Chart (by revenue)', 'chart_revenue');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'chart';
  $handler->display->display_options['style_options']['library'] = 'google';
  $handler->display->display_options['style_options']['label_field'] = 'name';
  $handler->display->display_options['style_options']['data_fields'] = array(
    'commerce_order_total_1' => 'commerce_order_total_1',
    'name' => 0,
    'commerce_order_total' => 0,
  );
  $handler->display->display_options['style_options']['field_colors'] = array(
    'name' => '#2f7ed8',
    'commerce_order_total_1' => '#8bbc21',
    'commerce_order_total' => '#910000',
  );
  $handler->display->display_options['style_options']['width'] = '';
  $handler->display->display_options['style_options']['height'] = '';
  $handler->display->display_options['style_options']['xaxis_labels_rotation'] = '0';
  $handler->display->display_options['style_options']['yaxis_labels_rotation'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Customer';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['empty'] = '0';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Total';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_order_total_1']['empty'] = '0';
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_reports_visualization';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'currency_code' => 'currency_code',
  );

  /* Display: Chart (by orders) */
  $handler = $view->new_display('block', 'Chart (by orders)', 'chart_orders');
  $handler->display->display_options['defaults']['pager'] = FALSE;
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'chart';
  $handler->display->display_options['style_options']['library'] = 'google';
  $handler->display->display_options['style_options']['label_field'] = 'name';
  $handler->display->display_options['style_options']['data_fields'] = array(
    'order_id' => 'order_id',
    'name' => 0,
    'commerce_order_total_1' => 0,
    'commerce_order_total' => 0,
  );
  $handler->display->display_options['style_options']['field_colors'] = array(
    'name' => '#2f7ed8',
    'order_id' => '#0d233a',
    'commerce_order_total_1' => '#8bbc21',
    'commerce_order_total' => '#910000',
  );
  $handler->display->display_options['style_options']['width'] = '';
  $handler->display->display_options['style_options']['height'] = '';
  $handler->display->display_options['style_options']['xaxis_labels_rotation'] = '0';
  $handler->display->display_options['style_options']['yaxis_labels_rotation'] = '0';
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = 'Customer';
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['name']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['name']['empty'] = '0';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: COUNT(Commerce Order: Order ID) */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'uid_commerce_orders';
  $handler->display->display_options['fields']['order_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['order_id']['label'] = 'Orders';
  $handler->display->display_options['fields']['order_id']['empty'] = '0';

  /**
   * Integration with views data export.
   */
  if (module_exists('views_data_export')) {
    /* Display: Data export */
    $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
    $handler->display->display_options['pager']['type'] = 'none';
    $handler->display->display_options['pager']['options']['offset'] = '0';
    $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
    $handler->display->display_options['style_options']['provide_file'] = 1;
    $handler->display->display_options['style_options']['filename'] = '%view-%timestamp-full.csv';
    $handler->display->display_options['style_options']['parent_sort'] = 1;
    $handler->display->display_options['style_options']['quote'] = 1;
    $handler->display->display_options['style_options']['trim'] = 1;
    $handler->display->display_options['style_options']['replace_newlines'] = 0;
    $handler->display->display_options['style_options']['header'] = 1;
    $handler->display->display_options['style_options']['keep_html'] = 0;
    $handler->display->display_options['defaults']['filter_groups'] = FALSE;
    $handler->display->display_options['defaults']['filters'] = FALSE;
    /* Filter criterion: User: Active */
    $handler->display->display_options['filters']['status']['id'] = 'status';
    $handler->display->display_options['filters']['status']['table'] = 'users';
    $handler->display->display_options['filters']['status']['field'] = 'status';
    $handler->display->display_options['filters']['status']['value'] = '1';
    $handler->display->display_options['filters']['status']['group'] = 1;
    $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
    /* Filter criterion: Commerce Order: Order state */
    $handler->display->display_options['filters']['state']['id'] = 'state';
    $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
    $handler->display->display_options['filters']['state']['field'] = 'state';
    $handler->display->display_options['filters']['state']['value'] = commerce_reports_reportable_order_states();
    /* Filter criterion: User: Name */
    $handler->display->display_options['filters']['uid']['id'] = 'uid';
    $handler->display->display_options['filters']['uid']['table'] = 'users';
    $handler->display->display_options['filters']['uid']['field'] = 'uid';
    $handler->display->display_options['filters']['uid']['value'] = '';
    $handler->display->display_options['filters']['uid']['exposed'] = TRUE;
    $handler->display->display_options['filters']['uid']['expose']['operator_id'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['label'] = 'Name';
    $handler->display->display_options['filters']['uid']['expose']['operator'] = 'uid_op';
    $handler->display->display_options['filters']['uid']['expose']['identifier'] = 'uid';
    $handler->display->display_options['filters']['uid']['expose']['remember_roles'] = array(
      2 => '2',
      1 => 0,
      3 => 0,
    );
    $handler->display->display_options['path'] = 'admin/commerce/reports/customers/export';
    $handler->display->display_options['displays'] = array(
      'page' => 'page',
      'default' => 0,
      'chart_revenue' => 0,
      'chart_orders' => 0,
    );
  }

  $views[$view->name] = $view;

  /**
   * Commerce Reports: Customer profile map
   */
  $view = new view();
  $view->name = 'commerce_reports_customer_map';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_customer_profile';
  $view->human_name = 'Customer Map';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Customer Map';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'chart';
  $handler->display->display_options['style_options']['type'] = 'geomap';
  $handler->display->display_options['style_options']['library'] = 'google';
  $handler->display->display_options['style_options']['label_field'] = 'commerce_customer_address';
  $handler->display->display_options['style_options']['data_fields'] = array(
    'profile_id' => 'profile_id',
    'commerce_customer_address' => 0,
  );
  $handler->display->display_options['style_options']['field_colors'] = array(
    'commerce_customer_address' => '#2f7ed8',
    'profile_id' => '#2e8a6a',
  );
  $handler->display->display_options['style_options']['width'] = '';
  $handler->display->display_options['style_options']['height'] = '';
  $handler->display->display_options['style_options']['xaxis_labels_rotation'] = '0';
  $handler->display->display_options['style_options']['yaxis_labels_rotation'] = '0';
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['label'] = 'Country';
  $handler->display->display_options['fields']['commerce_customer_address']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address']['alter']['text'] = '[commerce_customer_address-country] ';
  $handler->display->display_options['fields']['commerce_customer_address']['element_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['commerce_customer_address']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
    'use_widget_handlers' => 0,
    'format_handlers' => array(
      'address' => 'address',
    ),
  );
  $handler->display->display_options['fields']['commerce_customer_address']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['commerce_customer_address']['group_columns'] = array(
    'country' => 'country',
  );
  /* Field: COUNT(Commerce Customer Profile: Profile ID) */
  $handler->display->display_options['fields']['profile_id']['id'] = 'profile_id';
  $handler->display->display_options['fields']['profile_id']['table'] = 'commerce_customer_profile';
  $handler->display->display_options['fields']['profile_id']['field'] = 'profile_id';
  $handler->display->display_options['fields']['profile_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['profile_id']['label'] = 'Customers';
  $handler->display->display_options['fields']['profile_id']['element_type'] = '0';
  $handler->display->display_options['fields']['profile_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['profile_id']['element_wrapper_type'] = '0';
  $handler->display->display_options['fields']['profile_id']['element_default_classes'] = FALSE;
  /* Filter criterion: Commerce Customer Profile: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_customer_profile';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'billing' => 'billing',
  );

  /* Display: Billing Profile Map */
  $handler = $view->new_display('block', 'Billing Profile Map', 'billing_profiles_block');

  /* Display: Shipping Profile Map */
  $handler = $view->new_display('block', 'Shipping Profile Map', 'shipping_profiles_block');
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Commerce Customer Profile: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_customer_profile';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'shipping' => 'shipping',
  );

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'commerce_reports_customer_statistics';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_order';
  $view->human_name = 'Commerce Reports Customer Statistics';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Customer Statistics';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['style_options']['row_class'] = 'row';
  $handler->display->display_options['style_options']['default_row_class'] = FALSE;
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Relationship: Commerce Order: Owner */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  /* Field: SUM(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total']['id'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total']['group_type'] = 'sum';
  $handler->display->display_options['fields']['commerce_order_total']['label'] = 'Total Revenue';
  $handler->display->display_options['fields']['commerce_order_total']['element_type'] = 'span';
  $handler->display->display_options['fields']['commerce_order_total']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['commerce_order_total']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['commerce_order_total']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['commerce_order_total']['group_columns'] = array(
    'amount' => 'amount',
  );
  /* Field: AVG(Commerce Order: Order total) */
  $handler->display->display_options['fields']['commerce_order_total_1']['id'] = 'commerce_order_total_1';
  $handler->display->display_options['fields']['commerce_order_total_1']['table'] = 'field_data_commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['field'] = 'commerce_order_total';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_type'] = 'avg';
  $handler->display->display_options['fields']['commerce_order_total_1']['label'] = 'Average Order';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_type'] = 'span';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_label_type'] = 'span';
  $handler->display->display_options['fields']['commerce_order_total_1']['element_wrapper_type'] = 'div';
  $handler->display->display_options['fields']['commerce_order_total_1']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_order_total_1']['settings'] = array(
    'calculation' => FALSE,
  );
  $handler->display->display_options['fields']['commerce_order_total_1']['group_column'] = 'entity_id';
  $handler->display->display_options['fields']['commerce_order_total_1']['group_columns'] = array(
    'amount' => 'amount',
  );
  /* Field: COUNT(DISTINCT Commerce Order: Uid) */
  $handler->display->display_options['fields']['uid']['id'] = 'uid';
  $handler->display->display_options['fields']['uid']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['uid']['field'] = 'uid';
  $handler->display->display_options['fields']['uid']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['uid']['label'] = 'Customers Total';
  /* Filter criterion: Commerce Order: Order type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'commerce_order' => 'commerce_order',
  );
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['value'] = commerce_reports_reportable_order_states();

  /* Display: Block */
  $handler = $view->new_display('block', 'Customer Order Statistics', 'customer_statistics');

  $views[$view->name] = $view;

  return $views;
}
