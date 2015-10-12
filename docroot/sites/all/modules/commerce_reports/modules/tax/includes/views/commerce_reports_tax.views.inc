<?php
/**
 * @file
 * Integrates Views with Commerce Tax Reports.
 */

/**
 * Implements hook_views_data().
 */
function commerce_reports_tax_views_data() {
  $data = array();

  // Base information.
  $data['commerce_reports_tax']['table']['group'] = t('Commerce Tax Reports');

  $data['commerce_reports_tax']['table']['base'] = array(
    'field'            => 'order_id',
    'title'            => t('Commerce Tax Information'),
    'help'             => t('The tax information gathered by Commerce Tax Reports.'),
    'access query tag' => 'commerce_reports_access',
  );

  $data['commerce_reports_tax']['tax_rate'] = array(
    'title' => t('Tax rate'),
    'help'  => t('The applied tax rate.'),
    'field' => array(
      'handler'        => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['commerce_reports_tax']['tax_rate_display_name'] = array(
    'title' => t('Tax rate display name'),
    'help'  => t('The tax rate display name.'),
    'field' => array(
      'handler'        => 'commerce_reports_tax_handler_field_tax_rate_display_name',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the Order ID.
  $data['commerce_reports_tax']['order_id'] = array(
    'title'        => t('Order ID', array(),
      array('context' => 'a drupal commerce order')),
    'help'         => t('The unique internal identifier of the order.'),
    'field'        => array(
      'handler'        => 'commerce_order_handler_field_order',
      'click sortable' => TRUE,
    ),
    'filter'       => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort'         => array(
      'handler' => 'views_handler_sort',
    ),
    'argument'     => array(
      'handler'       => 'commerce_order_handler_argument_order_order_id',
      'name field'    => 'order_number',
      'numeric'       => TRUE,
      'validate type' => 'order_id',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base'    => 'commerce_order',
      'field'   => 'order_id',
      'label'   => t('Order', array(),
        array('context' => 'a drupal commerce order')),
    ),
  );

  // Expose the transaction currency.
  $data['commerce_reports_tax']['currency_code'] = array(
    'title'    => t('Currency'),
    'help'     => t('The currency of the transaction.'),
    'field'    => array(
      'handler'        => 'commerce_payment_handler_field_currency_code',
      'click sortable' => TRUE,
    ),
    'filter'   => array(
      'handler' => 'commerce_payment_handler_filter_currency_code',
    ),
    'sort'     => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['commerce_reports_tax']['taxable'] = array(
    'title' => t('Taxable'),
    'help'  => t('The taxable amount on the order.'),
    'field' => array(
      'handler'        => 'commerce_payment_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['commerce_reports_tax']['taxed'] = array(
    'title' => t('Taxed'),
    'help'  => t('The taxed amount on the order.'),
    'field' => array(
      'handler'        => 'commerce_payment_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'sort'  => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function commerce_reports_tax_views_data_alter(&$data) {
  // Define relationship from commerce_order to commerce_payment_transaction.
  $data['commerce_order']['table']['join']['commerce_reports_tax'] = array(
    'left_field' => 'order_id',
    'field'      => 'order_id',
  );

  $data['commerce_order']['payment_transaction']['relationship'] = array(
    'title'      => t('Tax Reports'),
    'help'       => t("Relate this order to its tax reports. This relationship will cause duplicated records if there are multiple tax rates per order."),
    'handler'    => 'views_handler_relationship',
    'base'       => 'commerce_reports_tax',
    'base field' => 'order_id',
    'field'      => 'order_id',
    'label'      => t('Tax', array(),
      array('context' => 'a drupal commerce tax report')),
  );
}
