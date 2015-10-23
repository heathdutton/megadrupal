<?php
/**
 * @file
 * kigu_admin.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_fulfilment_oms_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'commerce_fulfilment_oms_salesorders';
  $view->description = '';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_order';
  $view->human_name = 'OMS SalesOrders';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = TRUE;

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'OMS SalesOrders';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['disable_sql_rewrite'] = TRUE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
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
  /* Field: Commerce Customer profile: Address */
  $handler->display->display_options['fields']['commerce_customer_address']['id'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['table'] = 'field_data_commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['field'] = 'commerce_customer_address';
  $handler->display->display_options['fields']['commerce_customer_address']['relationship'] = 'commerce_customer_shipping_profile_id';
  $handler->display->display_options['fields']['commerce_customer_address']['label'] = '';
  $handler->display->display_options['fields']['commerce_customer_address']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_customer_address']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['element_default_classes'] = FALSE;
  $handler->display->display_options['fields']['commerce_customer_address']['click_sort_column'] = 'country';
  $handler->display->display_options['fields']['commerce_customer_address']['settings'] = array(
    'use_widget_handlers' => 0,
    'format_handlers' => array(),
  );
  /* Field: Commerce Line item: Unit price */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['commerce_unit_price']['exclude'] = TRUE;
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_raw_amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Field: Commerce Order: E-mail */
  $handler->display->display_options['fields']['mail']['id'] = 'mail';
  $handler->display->display_options['fields']['mail']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['mail']['field'] = 'mail';
  $handler->display->display_options['fields']['mail']['label'] = 'strEmailAddress';
  $handler->display->display_options['fields']['mail']['render_as_link'] = 0;
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing']['id'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['table'] = 'views';
  $handler->display->display_options['fields']['nothing']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing']['label'] = 'strExternalID';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_1']['id'] = 'nothing_1';
  $handler->display->display_options['fields']['nothing_1']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_1']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_1']['label'] = 'bEmailCustomer';
  $handler->display->display_options['fields']['nothing_1']['alter']['text'] = 'TRUE';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_2']['id'] = 'nothing_2';
  $handler->display->display_options['fields']['nothing_2']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_2']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_2']['label'] = 'strClientRoleName';
  $handler->display->display_options['fields']['nothing_2']['alter']['text'] = 'Web Customers';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_3']['id'] = 'nothing_3';
  $handler->display->display_options['fields']['nothing_3']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_3']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_3']['label'] = 'strBillToCompany';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php']['id'] = 'php';
  $handler->display->display_options['fields']['php']['table'] = 'views';
  $handler->display->display_options['fields']['php']['field'] = 'php';
  $handler->display->display_options['fields']['php']['label'] = 'strBillToFName';
  $handler->display->display_options['fields']['php']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php']['php_output'] = '<?php
  $name = trim($data->field_commerce_customer_address[0][\'raw\'][\'name_line\']);
  list($firstname, $lastname) = commerce_fulfilment_oms_split_name($name);
  $data->generated_lastname = $lastname;
  echo $firstname;
  ?>';
  $handler->display->display_options['fields']['php']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_7']['id'] = 'php_7';
  $handler->display->display_options['fields']['php_7']['table'] = 'views';
  $handler->display->display_options['fields']['php_7']['field'] = 'php';
  $handler->display->display_options['fields']['php_7']['label'] = 'strBillToLName';
  $handler->display->display_options['fields']['php_7']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_7']['php_output'] = '<?php echo $data->generated_lastname; ?>';
  $handler->display->display_options['fields']['php_7']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_7']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_1']['id'] = 'php_1';
  $handler->display->display_options['fields']['php_1']['table'] = 'views';
  $handler->display->display_options['fields']['php_1']['field'] = 'php';
  $handler->display->display_options['fields']['php_1']['label'] = 'strBillToAddress_1';
  $handler->display->display_options['fields']['php_1']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_1']['php_output'] = '<?php echo $data->field_commerce_customer_address[0][\'raw\'][\'thoroughfare\']; ?>';
  $handler->display->display_options['fields']['php_1']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_1']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_2']['id'] = 'php_2';
  $handler->display->display_options['fields']['php_2']['table'] = 'views';
  $handler->display->display_options['fields']['php_2']['field'] = 'php';
  $handler->display->display_options['fields']['php_2']['label'] = 'strBillToAddress_2';
  $handler->display->display_options['fields']['php_2']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_2']['php_output'] = '<?php echo $data->field_commerce_customer_address[0][\'raw\'][\'premise\']; ?>';
  $handler->display->display_options['fields']['php_2']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_2']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_3']['id'] = 'php_3';
  $handler->display->display_options['fields']['php_3']['table'] = 'views';
  $handler->display->display_options['fields']['php_3']['field'] = 'php';
  $handler->display->display_options['fields']['php_3']['label'] = 'strBillToCity';
  $handler->display->display_options['fields']['php_3']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_3']['php_output'] = '<?php echo $data->field_commerce_customer_address[0][\'raw\'][\'locality\']; ?>';
  $handler->display->display_options['fields']['php_3']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_3']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_4']['id'] = 'php_4';
  $handler->display->display_options['fields']['php_4']['table'] = 'views';
  $handler->display->display_options['fields']['php_4']['field'] = 'php';
  $handler->display->display_options['fields']['php_4']['label'] = 'strBillToState';
  $handler->display->display_options['fields']['php_4']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_4']['php_output'] = '<?php echo $data->field_commerce_customer_address[0][\'raw\'][\'administrative_area\']; ?>';
  $handler->display->display_options['fields']['php_4']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_4']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_5']['id'] = 'php_5';
  $handler->display->display_options['fields']['php_5']['table'] = 'views';
  $handler->display->display_options['fields']['php_5']['field'] = 'php';
  $handler->display->display_options['fields']['php_5']['label'] = 'strBillToZip';
  $handler->display->display_options['fields']['php_5']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_5']['php_output'] = '<?php echo $data->field_commerce_customer_address[0][\'raw\'][\'postal_code\']; ?>';
  $handler->display->display_options['fields']['php_5']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_5']['php_click_sortable'] = '';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_6']['id'] = 'php_6';
  $handler->display->display_options['fields']['php_6']['table'] = 'views';
  $handler->display->display_options['fields']['php_6']['field'] = 'php';
  $handler->display->display_options['fields']['php_6']['label'] = 'strBillToCountry';
  $handler->display->display_options['fields']['php_6']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_6']['php_output'] = '<?php echo $data->field_commerce_customer_address[0][\'raw\'][\'country\']; ?>';
  $handler->display->display_options['fields']['php_6']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_6']['php_click_sortable'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_5']['id'] = 'nothing_5';
  $handler->display->display_options['fields']['nothing_5']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_5']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_5']['label'] = 'strBillToPhone';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_6']['id'] = 'nothing_6';
  $handler->display->display_options['fields']['nothing_6']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_6']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_6']['label'] = 'bShipToSameAsBillTo';
  $handler->display->display_options['fields']['nothing_6']['alter']['text'] = 'TRUE';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_7']['id'] = 'nothing_7';
  $handler->display->display_options['fields']['nothing_7']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_7']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_7']['label'] = 'strShipToCompany';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_8']['id'] = 'nothing_8';
  $handler->display->display_options['fields']['nothing_8']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_8']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_8']['label'] = 'strShipToFName';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_9']['id'] = 'nothing_9';
  $handler->display->display_options['fields']['nothing_9']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_9']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_9']['label'] = 'strShipToLName';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_10']['id'] = 'nothing_10';
  $handler->display->display_options['fields']['nothing_10']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_10']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_10']['label'] = 'strShipToAddress_1';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_11']['id'] = 'nothing_11';
  $handler->display->display_options['fields']['nothing_11']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_11']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_11']['label'] = 'strShipToAddress_2';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_12']['id'] = 'nothing_12';
  $handler->display->display_options['fields']['nothing_12']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_12']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_12']['label'] = 'strShipToCity';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_13']['id'] = 'nothing_13';
  $handler->display->display_options['fields']['nothing_13']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_13']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_13']['label'] = 'strShipToState';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_14']['id'] = 'nothing_14';
  $handler->display->display_options['fields']['nothing_14']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_14']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_14']['label'] = 'strShipToZip';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_15']['id'] = 'nothing_15';
  $handler->display->display_options['fields']['nothing_15']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_15']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_15']['label'] = 'strShipToCountry';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_16']['id'] = 'nothing_16';
  $handler->display->display_options['fields']['nothing_16']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_16']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_16']['label'] = 'strShipToPhone';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_17']['id'] = 'nothing_17';
  $handler->display->display_options['fields']['nothing_17']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_17']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_17']['label'] = 'strMerchantPONumber';
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['label'] = 'strMerchantOrderNumber';
  $handler->display->display_options['fields']['order_id']['alter']['text'] = '#[order_id]';
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  /* Field: Commerce Order: Order notes */
  $handler->display->display_options['fields']['field_order_notes']['id'] = 'field_order_notes';
  $handler->display->display_options['fields']['field_order_notes']['table'] = 'field_data_field_order_notes';
  $handler->display->display_options['fields']['field_order_notes']['field'] = 'field_order_notes';
  $handler->display->display_options['fields']['field_order_notes']['label'] = 'strComments';
  /* Field: Commerce Line Item: Line item ID */
  $handler->display->display_options['fields']['line_item_id']['id'] = 'line_item_id';
  $handler->display->display_options['fields']['line_item_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_id']['field'] = 'line_item_id';
  $handler->display->display_options['fields']['line_item_id']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_id']['label'] = 'lngLineNo';
  /* Field: Commerce Line Item: Label */
  $handler->display->display_options['fields']['line_item_label']['id'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_label']['field'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_label']['label'] = 'strSKU';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_18']['id'] = 'nothing_18';
  $handler->display->display_options['fields']['nothing_18']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_18']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_18']['label'] = 'strSKUWMS';
  /* Field: Commerce Line Item: Label */
  $handler->display->display_options['fields']['line_item_label_1']['id'] = 'line_item_label_1';
  $handler->display->display_options['fields']['line_item_label_1']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['line_item_label_1']['field'] = 'line_item_label';
  $handler->display->display_options['fields']['line_item_label_1']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['line_item_label_1']['label'] = 'strItemName';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_9']['id'] = 'php_9';
  $handler->display->display_options['fields']['php_9']['table'] = 'views';
  $handler->display->display_options['fields']['php_9']['field'] = 'php';
  $handler->display->display_options['fields']['php_9']['label'] = 'lngCostPerUnit';
  $handler->display->display_options['fields']['php_9']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_9']['php_output'] = '<?php echo round($data->field_commerce_unit_price[0][\'raw\'][\'amount\'] / 100, 2); ?>';
  $handler->display->display_options['fields']['php_9']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_9']['php_click_sortable'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_20']['id'] = 'nothing_20';
  $handler->display->display_options['fields']['nothing_20']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_20']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_20']['label'] = 'lngDiscountPerUnit';
  $handler->display->display_options['fields']['nothing_20']['alter']['text'] = '0';
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['fields']['quantity']['label'] = 'lngQuantity_Ordered';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  $handler->display->display_options['fields']['quantity']['separator'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_21']['id'] = 'nothing_21';
  $handler->display->display_options['fields']['nothing_21']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_21']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_21']['label'] = 'lngPricingDiscount';
  $handler->display->display_options['fields']['nothing_21']['alter']['text'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_22']['id'] = 'nothing_22';
  $handler->display->display_options['fields']['nothing_22']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_22']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_22']['label'] = 'lngCouponDiscount';
  $handler->display->display_options['fields']['nothing_22']['alter']['text'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_23']['id'] = 'nothing_23';
  $handler->display->display_options['fields']['nothing_23']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_23']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_23']['label'] = 'lngHandling';
  $handler->display->display_options['fields']['nothing_23']['alter']['text'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_24']['id'] = 'nothing_24';
  $handler->display->display_options['fields']['nothing_24']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_24']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_24']['label'] = 'lngFlatRateShipping';
  $handler->display->display_options['fields']['nothing_24']['alter']['text'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_25']['id'] = 'nothing_25';
  $handler->display->display_options['fields']['nothing_25']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_25']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_25']['label'] = 'lngTax';
  $handler->display->display_options['fields']['nothing_25']['alter']['text'] = '0';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_26']['id'] = 'nothing_26';
  $handler->display->display_options['fields']['nothing_26']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_26']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_26']['label'] = 'lngShipping';
  $handler->display->display_options['fields']['nothing_26']['alter']['text'] = '0';
  /* Field: Global: PHP */
  $handler->display->display_options['fields']['php_8']['id'] = 'php_8';
  $handler->display->display_options['fields']['php_8']['table'] = 'views';
  $handler->display->display_options['fields']['php_8']['field'] = 'php';
  $handler->display->display_options['fields']['php_8']['label'] = 'strShipMethod';
  $handler->display->display_options['fields']['php_8']['use_php_setup'] = 0;
  $handler->display->display_options['fields']['php_8']['php_output'] = '<?php echo commerce_fulfilment_oms_get_shipping_service(commerce_order_load($data->order_id)); ?>';
  $handler->display->display_options['fields']['php_8']['use_php_click_sortable'] = '0';
  $handler->display->display_options['fields']['php_8']['php_click_sortable'] = '';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_28']['id'] = 'nothing_28';
  $handler->display->display_options['fields']['nothing_28']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_28']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_28']['label'] = 'strShipAccount';
  /* Field: Global: Custom text */
  $handler->display->display_options['fields']['nothing_29']['id'] = 'nothing_29';
  $handler->display->display_options['fields']['nothing_29']['table'] = 'views';
  $handler->display->display_options['fields']['nothing_29']['field'] = 'nothing';
  $handler->display->display_options['fields']['nothing_29']['label'] = 'strPOTerm';
  $handler->display->display_options['fields']['nothing_29']['alter']['text'] = 'Net-10';
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
  /* Filter criterion: Commerce Line Item: Line item is of a product line item type */
  $handler->display->display_options['filters']['product_line_item_type']['id'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['table'] = 'commerce_line_item';
  $handler->display->display_options['filters']['product_line_item_type']['field'] = 'product_line_item_type';
  $handler->display->display_options['filters']['product_line_item_type']['relationship'] = 'commerce_line_items_line_item_id';
  $handler->display->display_options['filters']['product_line_item_type']['value'] = '1';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['footer'] = FALSE;
  $handler->display->display_options['path'] = 'admin/commerce/orders/oms';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'OMS SalesOrders';
  $handler->display->display_options['menu']['description'] = 'View postage CSV for OMS SalesOrders.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;

  /* Display: CSV */
  $handler = $view->new_display('views_data_export', 'CSV', 'views_data_export_1');
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
  $handler->display->display_options['path'] = 'admin/commerce/orders/oms/csv';
  $handler->display->display_options['displays'] = array(
    'page_1' => 'page_1',
    'default' => 0,
  );

  $export['commerce_fulfilment_oms_salesorders'] = $view;

  return $export;
}
