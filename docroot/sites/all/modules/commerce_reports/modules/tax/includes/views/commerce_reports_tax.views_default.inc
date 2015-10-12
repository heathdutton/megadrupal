<?php
/**
 * @file
 * Default views.
 */

/**
 * Views for commerce orders for report displays.
 */

/**
 * Implements hook_views_default_views().
 */
function commerce_reports_tax_views_default_views() {

  $view = new view();
  $view->name = 'commerce_reports_taxes';
  $view->description = '';
  $view->tag = 'commerce_reports';
  $view->base_table = 'commerce_reports_tax';
  $view->human_name = 'Commerce Tax Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Commerce Tax Report';
  $handler->display->display_options['css_class'] = 'commerce-reports';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Relationship: Commerce Tax Reports: Order ID */
  $handler->display->display_options['relationships']['order_id']['id'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['table'] = 'commerce_reports_tax';
  $handler->display->display_options['relationships']['order_id']['field'] = 'order_id';
  $handler->display->display_options['relationships']['order_id']['required'] = TRUE;
  /* Field: Commerce Tax Reports: Tax rate display name */
  $handler->display->display_options['fields']['tax_rate_display_name']['id'] = 'tax_rate_display_name';
  $handler->display->display_options['fields']['tax_rate_display_name']['table'] = 'commerce_reports_tax';
  $handler->display->display_options['fields']['tax_rate_display_name']['field'] = 'tax_rate_display_name';
  $handler->display->display_options['fields']['tax_rate_display_name']['label'] = 'Tax rate';
  /* Field: COUNT(Commerce Order: Order number) */
  $handler->display->display_options['fields']['order_number']['id'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['table'] = 'commerce_order';
  $handler->display->display_options['fields']['order_number']['field'] = 'order_number';
  $handler->display->display_options['fields']['order_number']['relationship'] = 'order_id';
  $handler->display->display_options['fields']['order_number']['group_type'] = 'count';
  /* Field: SUM(Commerce Tax Reports: Taxable) */
  $handler->display->display_options['fields']['taxable']['id'] = 'taxable';
  $handler->display->display_options['fields']['taxable']['table'] = 'commerce_reports_tax';
  $handler->display->display_options['fields']['taxable']['field'] = 'taxable';
  $handler->display->display_options['fields']['taxable']['group_type'] = 'sum';
  /* Field: SUM(Commerce Tax Reports: Taxed) */
  $handler->display->display_options['fields']['taxed']['id'] = 'taxed';
  $handler->display->display_options['fields']['taxed']['table'] = 'commerce_reports_tax';
  $handler->display->display_options['fields']['taxed']['field'] = 'taxed';
  $handler->display->display_options['fields']['taxed']['group_type'] = 'sum';
  /* Filter criterion: Commerce Order: Order state */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['state']['value'] = commerce_reports_reportable_order_states();
  $handler->display->display_options['filters']['state']['exposed'] = TRUE;
  $handler->display->display_options['filters']['state']['expose']['operator_id'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['label'] = 'Order status';
  $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['expose']['multiple'] = TRUE;
  $handler->display->display_options['filters']['state']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: Date: Date (commerce_order) */
  $handler->display->display_options['filters']['date_filter']['id'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['table'] = 'commerce_order';
  $handler->display->display_options['filters']['date_filter']['field'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['relationship'] = 'order_id';
  $handler->display->display_options['filters']['date_filter']['operator'] = 'between';
  $handler->display->display_options['filters']['date_filter']['exposed'] = TRUE;
  $handler->display->display_options['filters']['date_filter']['expose']['operator_id'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['label'] = 'Date';
  $handler->display->display_options['filters']['date_filter']['expose']['operator'] = 'date_filter_op';
  $handler->display->display_options['filters']['date_filter']['expose']['identifier'] = 'date_filter';
  $handler->display->display_options['filters']['date_filter']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  $handler->display->display_options['filters']['date_filter']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['date_filter']['default_date'] = '-1 year';
  $handler->display->display_options['filters']['date_filter']['default_to_date'] = 'now';
  $handler->display->display_options['filters']['date_filter']['year_range'] = '-10:+1';
  $handler->display->display_options['filters']['date_filter']['date_fields'] = array(
    'commerce_order.created' => 'commerce_order.created',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/commerce/reports/tax';
  $handler->display->display_options['menu']['type'] = 'tab';
  $handler->display->display_options['menu']['title'] = 'Taxes';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  $views[$view->name] = $view;

  return $views;
}
