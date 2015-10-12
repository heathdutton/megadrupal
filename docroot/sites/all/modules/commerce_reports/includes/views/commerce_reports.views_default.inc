<?php
/**
 * @file
 * Default views.
 */

// This module uses so many Views they've been split up.
// @link: https://www.drupal.org/node/2333949
include 'commerce_reports.views_default.customers.inc';
include 'commerce_reports.views_default.payment_methods.inc';
include 'commerce_reports.views_default.products.inc';
include 'commerce_reports.views_default.sales.inc';

/**
 * Views for commerce orders for report displays.
 */

function commerce_reports_data_export($view) {
  if (empty($view->display['page']) || empty($view->display['page']->display_options['path'])) {
    return;
  }

  $handler = $view->new_display('views_data_export', 'Export to CSV', 'views_data_export');
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['path'] = $view->display['page']->display_options['path'] . '/csv';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
  );
}

/**
 * Implements hook_views_default_views().
 */
function commerce_reports_views_default_views() {
  $views = array();

  // @todo: Simpler way to do this?
  $views = array_merge($views, _commerce_reports_views_default_products_views());
  $views = array_merge($views, _commerce_reports_views_default_customer_views());
  $views = array_merge($views, _commerce_reports_views_default_payment_method_views());
  $views = array_merge($views, _commerce_reports_views_default_sales_views());

  return $views;
}
