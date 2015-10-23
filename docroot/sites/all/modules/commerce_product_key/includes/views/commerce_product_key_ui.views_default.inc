<?php

/**
 * @file
 * Views for the default product UI.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_product_key_ui_views_default_views() {
  $views = array();

  // Product keys admin list at admin/commerce/product-keys.
  $view = new view;
  $view->name = 'commerce_product_keys';
  $view->description = 'Display a list of product keys for store admin.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_product_key';
  $view->human_name = 'Product Keys';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Product Keys';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'administer product keys';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 50;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'product_key_id' => 'product_key_id',
    'order_id' => 'order_id',
    'key_owner_id' => 'key_owner_id',
    'code' => 'code',
    'type' => 'type',
    'operations' => 'operations',
  );
  $handler->display->display_options['style_options']['default'] = 'product_key_id';
  $handler->display->display_options['style_options']['order'] = 'desc';
  $handler->display->display_options['style_options']['info'] = array(
    'product_key_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'key_owner_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'type' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'operations' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = 'No product keys have been created yet.';
  /* Field: Commerce Product Key: ID */
  $handler->display->display_options['fields']['product_key_id']['id'] = 'product_key_id';
  $handler->display->display_options['fields']['product_key_id']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['product_key_id']['field'] = 'product_key_id';
  $handler->display->display_options['fields']['product_key_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['product_key_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['product_key_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['product_key_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['product_key_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['product_key_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['product_key_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['product_key_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['product_key_id']['link_to_product_key'] = 'admin';

  /* Field: Commerce Product Key: code */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  $handler->display->display_options['fields']['code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['code']['empty_zero'] = 0;
  /* Field: Commerce Product Key: order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['order_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['order_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['order_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'admin';
  /* Field: Commerce Product Key: Product Key Owner ID */
  $handler->display->display_options['fields']['key_owner_id']['id'] = 'key_owner_id';
  $handler->display->display_options['fields']['key_owner_id']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['key_owner_id']['field'] = 'key_owner_id';
  $handler->display->display_options['fields']['key_owner_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['key_owner_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['key_owner_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['key_owner_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['key_owner_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['key_owner_id']['empty_zero'] = 0;
  /* Field: Commerce Product Key: Status */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;

  /* Field: Commerce Product Key: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  $handler->display->display_options['fields']['type']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['type']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['type']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['type']['alter']['external'] = 0;
  $handler->display->display_options['fields']['type']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['type']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['type']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['type']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['type']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['type']['alter']['html'] = 0;
  $handler->display->display_options['fields']['type']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['type']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['type']['hide_empty'] = 0;
  $handler->display->display_options['fields']['type']['empty_zero'] = 0;
  /* Field: Commerce Product Key: Operations links */
  $handler->display->display_options['fields']['operations']['id'] = 'operations';
  $handler->display->display_options['fields']['operations']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['operations']['field'] = 'operations';
  $handler->display->display_options['fields']['operations']['label'] = 'Operations';
  $handler->display->display_options['fields']['operations']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['external'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['operations']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['operations']['alter']['html'] = 0;
  $handler->display->display_options['fields']['operations']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['operations']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['operations']['hide_empty'] = 0;
  $handler->display->display_options['fields']['operations']['empty_zero'] = 0;
  /* Sort criterion: Commerce Product: code */
  $handler->display->display_options['sorts']['code']['id'] = 'code';
  $handler->display->display_options['sorts']['code']['table'] = 'commerce_product_key';
  $handler->display->display_options['sorts']['code']['field'] = 'code';

  /* Display: Admin page */
  $handler = $view->new_display('page', 'Admin page', 'admin_page');
  $handler->display->display_options['path'] = 'admin/commerce/product-keys/list';
  $handler->display->display_options['menu']['type'] = 'default tab';
  $handler->display->display_options['menu']['title'] = 'List';
  $handler->display->display_options['menu']['weight'] = '-10';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Product Keys';
  $handler->display->display_options['tab_options']['description'] = 'Manage product keys and sequences in the store.';
  $handler->display->display_options['tab_options']['weight'] = '';
  $handler->display->display_options['tab_options']['name'] = 'management';
  $translatables['commerce_product_keys'] = array(
    t('Defaults'),
    t('Product keys'),
    t('Order ID'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('No product keys have been created yet.'),
    t('Code'),
    t('Revoked'),
    t('Activated'),
    t('Sequence'),
    t('Operations'),
    t('Admin page'),
  );

  $views[$view->name] = $view;


  // Product keys list at user/%/product-keys.
  $view = new view;
  $view->name = 'customer_product_keys';
  $view->description = 'Display a list of product keys a user has bought.';
  $view->tag = 'commerce';
  $view->base_table = 'commerce_product_key';
  $view->human_name = 'Customer Product Keys';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Customer Product Keys';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view own product keys';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'changed' => 'changed',
    'order_id' => 'order_id',
    'code' => 'code',
  );
  $handler->display->display_options['style_options']['default'] = 'changed';
  $handler->display->display_options['style_options']['order'] = 'desc';
  $handler->display->display_options['style_options']['info'] = array(
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
    ),
    'code' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'order_id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = 'No product keys have been assigned to you yet.';
  /* Field: Commerce Product Key: code */
  $handler->display->display_options['fields']['code']['id'] = 'code';
  $handler->display->display_options['fields']['code']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['code']['field'] = 'code';
  $handler->display->display_options['fields']['code']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['code']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['code']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['code']['alter']['external'] = 0;
  $handler->display->display_options['fields']['code']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['code']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['code']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['code']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['code']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['code']['alter']['html'] = 0;
  $handler->display->display_options['fields']['code']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['code']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['code']['hide_empty'] = 0;
  $handler->display->display_options['fields']['code']['empty_zero'] = 0;
  //$handler->display->display_options['fields']['code']['link_to_product_key'] = 'customer';
  /* Field: Commerce Product Key: order ID */
  $handler->display->display_options['fields']['order_id']['id'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['order_id']['field'] = 'order_id';
  $handler->display->display_options['fields']['order_id']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['external'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['order_id']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['order_id']['alter']['html'] = 0;
  $handler->display->display_options['fields']['order_id']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['order_id']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['order_id']['hide_empty'] = 0;
  $handler->display->display_options['fields']['order_id']['empty_zero'] = 0;
  $handler->display->display_options['fields']['order_id']['link_to_order'] = 'customer';
  /* Field: Commerce Order: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'commerce_product_key';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  /* Contextual filter: Product Key Owner: UID */
  $handler->display->display_options['arguments']['uid']['id'] = 'uid';
  $handler->display->display_options['arguments']['uid']['table'] = 'commerce_product_key';
  $handler->display->display_options['arguments']['uid']['field'] = 'key_owner_id';
  $handler->display->display_options['arguments']['uid']['default_action'] = 'not found';
  $handler->display->display_options['arguments']['uid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['uid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['uid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['uid']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid']['not'] = 0;
  $handler->display->display_options['arguments']['uid']['specify_validation'] = 1;
  $handler->display->display_options['arguments']['uid']['validate']['type'] = 'current_user_or_role';
  $handler->display->display_options['arguments']['uid']['validate_options']['restrict_roles'] = '1';
  $handler->display->display_options['arguments']['uid']['validate_options']['roles'] = array(
    3 => '3',
  );
  $handler->display->display_options['arguments']['uid']['break_phrase'] = 0;
  $handler->display->display_options['arguments']['uid']['not'] = 0;
  /* Filter criterion: Status: active */
  $handler->display->display_options['filters']['type']['id'] = 'status';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_product_key';
  $handler->display->display_options['filters']['type']['field'] = 'status';
  $handler->display->display_options['filters']['type']['value'] = 1;
  /* Sort criterion: Commerce Product: changed */
  $handler->display->display_options['sorts']['changed']['id'] = 'changed';
  $handler->display->display_options['sorts']['changed']['table'] = 'commerce_product_key';
  $handler->display->display_options['sorts']['changed']['field'] = 'changed';
  $handler->display->display_options['sorts']['changed']['order'] = 'DESC';
  /* Display: Users Product Key list */
  $handler = $view->new_display('page', 'User product Keys', 'key_list_page');
  $handler->display->display_options['path'] = 'user/%/product-keys';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Product Keys';
  $handler->display->display_options['menu']['weight'] = '15';
  $handler->display->display_options['tab_options']['type'] = 'normal';
  $handler->display->display_options['tab_options']['title'] = 'Product Keys';
  $handler->display->display_options['tab_options']['description'] = 'User assigned Product Keys.';
  $handler->display->display_options['tab_options']['weight'] = '';
  $handler->display->display_options['tab_options']['name'] = 'user-menu';


  $translatables['commerce_product_keys'] = array(
    t('Defaults'),
    t('Product keys'),
    t('Associated Order'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('No product keys have been assigned to you yet.'),
    t('Code'),
    t('Sequence'),
    t('Operations'),
    t('Admin page'),
  );

  $views[$view->name] = $view;


  return $views;
}