<?php

/**
 * @file
 * Default Views configuration for Commerce Reports Stock.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_reports_stock_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'commerce_reports_stock';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'commerce_product';
  $view->human_name = 'Commerce Reports: Stock';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Commerce Reports: Stock';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access commerce reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: Commerce Product: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['link_to_product'] = 0;
  /* Field: Commerce Product: SKU */
  $handler->display->display_options['fields']['sku']['id'] = 'sku';
  $handler->display->display_options['fields']['sku']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['sku']['field'] = 'sku';
  $handler->display->display_options['fields']['sku']['link_to_product'] = 0;
  /* Field: Commerce Product: Stock */
  $handler->display->display_options['fields']['commerce_stock']['id'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['field'] = 'commerce_stock';
  $handler->display->display_options['fields']['commerce_stock']['settings'] = array(
    'thousand_separator' => '',
    'decimal_separator' => '.',
    'scale' => '0',
    'prefix_suffix' => 0,
  );
  /* Field: Commerce Product: Monthly sales */
  $handler->display->display_options['fields']['monthlysales']['id'] = 'monthlysales';
  $handler->display->display_options['fields']['monthlysales']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['monthlysales']['field'] = 'monthlysales';
  /* Field: Commerce Product: Weekly sales */
  $handler->display->display_options['fields']['weeklysales']['id'] = 'weeklysales';
  $handler->display->display_options['fields']['weeklysales']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['weeklysales']['field'] = 'weeklysales';
  /* Field: Commerce Product: Stock lifetime */
  $handler->display->display_options['fields']['stocklifetime']['id'] = 'stocklifetime';
  $handler->display->display_options['fields']['stocklifetime']['table'] = 'commerce_product';
  $handler->display->display_options['fields']['stocklifetime']['field'] = 'stocklifetime';
  /* Sort criterion: Commerce Product: Stock (commerce_stock) */
  $handler->display->display_options['sorts']['commerce_stock_value']['id'] = 'commerce_stock_value';
  $handler->display->display_options['sorts']['commerce_stock_value']['table'] = 'field_data_commerce_stock';
  $handler->display->display_options['sorts']['commerce_stock_value']['field'] = 'commerce_stock_value';
  $handler->display->display_options['sorts']['commerce_stock_value']['exposed'] = TRUE;
  $handler->display->display_options['sorts']['commerce_stock_value']['expose']['label'] = 'Stock';
  /* Filter criterion: Commerce Product: Status */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  /* Filter criterion: Commerce Product: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'commerce_product';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['exposed'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['operator_id'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['label'] = 'Type';
  $handler->display->display_options['filters']['type']['expose']['operator'] = 'type_op';
  $handler->display->display_options['filters']['type']['expose']['identifier'] = 'type';
  $handler->display->display_options['filters']['type']['expose']['remember'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['type']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
    4 => 0,
    5 => 0,
    6 => 0,
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/reports/stock';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Stock';
  $handler->display->display_options['menu']['description'] = 'View stock weekly and monthly sales, and projected inventory lifetime.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['commerce_reports_stock'] = array(
    t('Master'),
    t('Commerce Reports: Stock'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Title'),
    t('SKU'),
    t('Stock'),
    t('Monthly sales'),
    t('Weekly sales'),
    t('Stock lifetime'),
    t('Type'),
    t('Page'),
  );

  $views['commerce_reports_stock'] = $view;

  return $views;
}
