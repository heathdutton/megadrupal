<?php

/**
 * @file
 * Views exports.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_giftaid_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'gift_aid_report';
  $view->description = 'A view listing line item sales where the user agreed to the gift aid declaration';
  $view->tag = 'default';
  $view->base_table = 'commerce_line_item';
  $view->human_name = 'Gift aid report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Gift aid report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view giftaid report';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No completed orders were found with giftaid applied.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Commerce Line Item: Order ID */
  $handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['required'] = TRUE;
  /* Field: Commerce Line Item: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['order_id']['alter']['path'] = 'admin/commerce/orders/[order_id]/view';
  /* Field: Commerce Line item: Product */
  $handler->display->display_options['fields']['commerce_product']['id'] = 'commerce_product';
  $handler->display->display_options['fields']['commerce_product']['table'] = 'field_data_commerce_product';
  $handler->display->display_options['fields']['commerce_product']['field'] = 'commerce_product';
  $handler->display->display_options['fields']['commerce_product']['type'] = 'commerce_product_reference_title_link';
  $handler->display->display_options['fields']['commerce_product']['settings'] = array(
    'show_quantity' => 0,
    'default_quantity' => '1',
    'combine' => 1,
    'line_item_type' => 0,
  );
  /* Field: Commerce Line Item: Quantity */
  $handler->display->display_options['fields']['quantity']['id'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['table'] = 'commerce_line_item';
  $handler->display->display_options['fields']['quantity']['field'] = 'quantity';
  $handler->display->display_options['fields']['quantity']['precision'] = '0';
  /* Field: Commerce Line item: Unit price */
  $handler->display->display_options['fields']['commerce_unit_price']['id'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['table'] = 'field_data_commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['field'] = 'commerce_unit_price';
  $handler->display->display_options['fields']['commerce_unit_price']['click_sort_column'] = 'amount';
  $handler->display->display_options['fields']['commerce_unit_price']['type'] = 'commerce_price_formatted_amount';
  $handler->display->display_options['fields']['commerce_unit_price']['settings'] = array(
    'calculation' => FALSE,
  );
  /* Sort criterion: Commerce Line Item: Order ID */
  $handler->display->display_options['sorts']['order_id']['id'] = 'order_id';
  $handler->display->display_options['sorts']['order_id']['table'] = 'commerce_line_item';
  $handler->display->display_options['sorts']['order_id']['field'] = 'order_id';
  $handler->display->display_options['sorts']['order_id']['order'] = 'DESC';
  /* Filter criterion: Field: Giftaid (commerce_product_giftaid) */
  $handler->display->display_options['filters']['commerce_product_giftaid_value']['id'] = 'commerce_product_giftaid_value';
  $handler->display->display_options['filters']['commerce_product_giftaid_value']['table'] = 'field_data_commerce_product_giftaid';
  $handler->display->display_options['filters']['commerce_product_giftaid_value']['field'] = 'commerce_product_giftaid_value';
  $handler->display->display_options['filters']['commerce_product_giftaid_value']['value'] = array(
    1 => '1',
  );
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['state']['value'] = array(
    'completed' => 'completed',
  );

  /* Display: Gift aid report page */
  $handler = $view->new_display('page', 'Gift aid report page', 'gift_aid_report_page');
  $handler->display->display_options['display_description'] = 'The main gift aid report page';
  $handler->display->display_options['path'] = 'admin/commerce/orders/gift-aid-report';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Gift aid report';
  $handler->display->display_options['menu']['description'] = 'The gift aid report';
  $handler->display->display_options['menu']['weight'] = '0';

  $views[$view->name] = $view;

  return $views;
}

