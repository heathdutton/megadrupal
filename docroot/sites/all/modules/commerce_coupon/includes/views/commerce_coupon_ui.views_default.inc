<?php

/**
 * @file
 * Views for the default coupon UI.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_coupon_ui_views_default_views() {
  $views = array();

  // Coupon admin list at admin/commerce/coupons.
  $view = new view();
  $view->name = 'commerce_coupons';
  $view->description = 'Display a list of coupons for the store admin.';
  $view->tag = 'commerce coupon';
  $view->base_table = 'commerce_coupon';
  $view->human_name = 'Coupons';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Coupons';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer commerce_coupon entities';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'coupon_id' => 'coupon_id',
    'commerce_coupon_code' => 'commerce_coupon_code',
    'type' => 'type',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'coupon_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'commerce_coupon_code' => array(
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
  );
  /* Relationship: Commerce Coupon: Type */
  $handler->display->display_options['relationships']['type']['id'] = 'type';
  $handler->display->display_options['relationships']['type']['table'] = 'commerce_coupon';
  $handler->display->display_options['relationships']['type']['field'] = 'type';
  $handler->display->display_options['relationships']['type']['required'] = TRUE;
  /* Field: Commerce Coupon: Coupon Code */
  $handler->display->display_options['fields']['commerce_coupon_code']['id'] = 'commerce_coupon_code';
  $handler->display->display_options['fields']['commerce_coupon_code']['table'] = 'field_data_commerce_coupon_code';
  $handler->display->display_options['fields']['commerce_coupon_code']['field'] = 'commerce_coupon_code';
  /* Field: Commerce Coupon: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: Commerce Coupon: Number of Uses */
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['id'] = 'commerce_coupon_number_of_uses';
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['table'] = 'field_data_commerce_coupon_number_of_uses';
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['field'] = 'commerce_coupon_number_of_uses';
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['external'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['alter']['html'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['hide_empty'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['empty_zero'] = 0;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['settings'] = array(
    'thousand_separator' => ' ',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['fields']['commerce_coupon_number_of_uses']['field_api_classes'] = 0;
  /* Field: Commerce Coupon: Times used */
  $handler->display->display_options['fields']['times_used']['id'] = 'times_used';
  $handler->display->display_options['fields']['times_used']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['times_used']['field'] = 'times_used';
  $handler->display->display_options['fields']['times_used']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['external'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['times_used']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['times_used']['alter']['more_link'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['times_used']['alter']['html'] = 0;
  $handler->display->display_options['fields']['times_used']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['times_used']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['times_used']['hide_empty'] = 0;
  $handler->display->display_options['fields']['times_used']['empty_zero'] = 0;
  $handler->display->display_options['fields']['times_used']['hide_alter_empty'] = 1;
  /* Field: Commerce Coupon: Operations */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  /* Field: Commerce Coupon: Is active */
  $handler->display->display_options['fields']['is_active']['id'] = 'is_active';
  $handler->display->display_options['fields']['is_active']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['is_active']['field'] = 'is_active';
  $handler->display->display_options['fields']['is_active']['label'] = 'Active';
  $handler->display->display_options['fields']['is_active']['type'] = 'unicode-yes-no';
  $handler->display->display_options['fields']['is_active']['not'] = 0;
  /* Filter criterion: Commerce Coupon Type: Coupon type status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_coupon_type';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['relationship'] = 'type';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Admin page */
  $handler = $view->new_display('page', 'Admin page', 'page');
  $handler->display->display_options['display_description'] = 'Manage the coupons.';
  $handler->display->display_options['path'] = 'admin/commerce/coupons/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Coupons';
  $handler->display->display_options['tab_options']['description'] = 'Manage coupons in the store.';
  $handler->display->display_options['tab_options']['weight'] = '';
  $handler->display->display_options['tab_options']['name'] = 'management';

  /* Sort criterion: Commerce Coupon: Date updated */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'commerce_coupon';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';

  $views[$view->name] = $view;

  $view = new view();
  $view->name = 'commerce_coupon_review_pane';
  $view->description = '';
  $view->tag = 'commerce coupon';
  $view->base_table = 'commerce_coupon';
  $view->human_name = 'Commerce Coupon Review Pane';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access checkout';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'coupon_id' => 'coupon_id',
    'commerce_coupon_code' => 'commerce_coupon_code',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'coupon_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'commerce_coupon_code' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Relationship: Entity Reference: Referencing entity */
  $handler->display->display_options['relationships']['reverse_commerce_coupon_order_reference_commerce_order']['id'] = 'reverse_commerce_coupon_order_reference_commerce_order';
  $handler->display->display_options['relationships']['reverse_commerce_coupon_order_reference_commerce_order']['table'] = 'commerce_coupon';
  $handler->display->display_options['relationships']['reverse_commerce_coupon_order_reference_commerce_order']['field'] = 'reverse_commerce_coupon_order_reference_commerce_order';
  $handler->display->display_options['relationships']['reverse_commerce_coupon_order_reference_commerce_order']['label'] = 'Commerce Order referencing Commerce Coupon';
  $handler->display->display_options['relationships']['reverse_commerce_coupon_order_reference_commerce_order']['required'] = TRUE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'reverse_commerce_coupon_order_reference_commerce_order';
  $handler->display->display_options['fields']['order_id']['label'] = '';
  $handler->display->display_options['fields']['order_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  /* Field: Commerce Coupon: Coupon Code */
  $handler->display->display_options['fields']['commerce_coupon_code']['id'] = 'commerce_coupon_code';
  $handler->display->display_options['fields']['commerce_coupon_code']['table'] = 'field_data_commerce_coupon_code';
  $handler->display->display_options['fields']['commerce_coupon_code']['field'] = 'commerce_coupon_code';
  /* Field: Commerce Coupon: Granted amount */
  $handler->display->display_options['fields']['granted_amount']['id'] = 'granted_amount';
  $handler->display->display_options['fields']['granted_amount']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['granted_amount']['field'] = 'granted_amount';
  /* Field: Commerce Coupon: Remove from Order */
  $handler->display->display_options['fields']['remove_from_order']['id'] = 'remove_from_order';
  $handler->display->display_options['fields']['remove_from_order']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['remove_from_order']['field'] = 'remove_from_order';
  $handler->display->display_options['fields']['remove_from_order']['label'] = 'Actions';
  /* Contextual filter: Commerce Order: Order ID */
  $handler->display->display_options['arguments']['order_id']['id'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['arguments']['order_id']['field'] = 'order_id';
  $handler->display->display_options['arguments']['order_id']['relationship'] = 'reverse_commerce_coupon_order_reference_commerce_order';
  $handler->display->display_options['arguments']['order_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['order_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['order_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['order_id']['summary_options']['items_per_page'] = '25';

  /* Display: Checkout */
  $handler = $view->new_display('block', 'Checkout', 'checkout');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;

  /* Display: Review */
  $handler = $view->new_display('block', 'Review', 'review');
  $handler->display->display_options['defaults']['hide_admin_links'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Commerce Order: Order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['relationship'] = 'reverse_commerce_coupon_order_reference_commerce_order';
  $handler->display->display_options['fields']['order_id']['label'] = '';
  $handler->display->display_options['fields']['order_id']['exclude'] = TRUE;
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = FALSE;
  /* Field: Commerce Coupon: Coupon Code */
  $handler->display->display_options['fields']['commerce_coupon_code']['id'] = 'commerce_coupon_code';
  $handler->display->display_options['fields']['commerce_coupon_code']['table'] = 'field_data_commerce_coupon_code';
  $handler->display->display_options['fields']['commerce_coupon_code']['field'] = 'commerce_coupon_code';
  /* Field: Commerce Coupon: Granted amount */
  $handler->display->display_options['fields']['granted_amount']['id'] = 'granted_amount';
  $handler->display->display_options['fields']['granted_amount']['table'] = 'commerce_coupon';
  $handler->display->display_options['fields']['granted_amount']['field'] = 'granted_amount';

  $views[$view->name] = $view;

  return $views;
}
