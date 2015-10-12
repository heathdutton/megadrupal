<?php
/**
 * @file
 * Integrates Views with Commerce Reports.
 */

/**
 * Implements hook_views_handlers().
 */
function commerce_reports_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'commerce_reports') . '/includes/views/handlers',
    ),
    'handlers' => array(
      'commerce_reports_handler_sort_date' => array(
        'parent' => 'views_handler_sort_date',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function commerce_reports_views_data_alter(&$data) {
  //Changed created to use our date sort handler which adds weekly granularity.
  $data['commerce_order']['created']['sort']['handler'] = 'commerce_reports_handler_sort_date';

  $data['users']['uid_commerce_orders'] = array(
    'relationship' => array(
      'title' => t('Orders'),
      'help' => t("Relate this user to his or hers orders."),
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_order',
      'base field' => 'uid',
      'field' => 'uid',
      'label' => t('Order'),
    ),
  );
}

/**
 * Implements hook_views_query_alter().
 *
 * Provides basic support for multiple currencies.
 */
function commerce_reports_views_query_alter(&$view, &$query) {
  if ($view->tag == 'commerce_reports') {
    foreach ($query->fields as &$field) {
      if ($field['field'] == 'commerce_total_currency_code') {
        unset($field['function']);
      }
    }
  }
}
