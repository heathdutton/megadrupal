<?php

/**
 * @file
 * Integrates Views with Commerce Tax Reports.
 */
function commerce_eu_vat_report_views_data() {
  $data = array();

  // Base information.
  $data['commerce_eu_vat_report']['table']['group'] = t('Commerce VAT Reports');

  $data['commerce_eu_vat_report']['table']['base'] = array(
    'field' => 'order_id',
    'title' => t('Commerce VAT Information'),
    'help' => t('The tax information gathered by Commerce EU VAT Reports.'),
    'access query tag' => 'commerce_reports_access',
  );

  $data['commerce_eu_vat_report']['tax_rate'] = array(
    'title' => t('VAT rate'),
    'help' => t('The applied vat rate.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Expose the Order ID.
  $data['commerce_eu_vat_report']['order_id'] = array(
    'title' => t('Order ID', array(), array('context' => 'a drupal commerce order')),
    'help' => t('The unique internal identifier of the order.'),
    'field' => array(
      'handler' => 'commerce_order_handler_field_order',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'commerce_order_handler_argument_order_order_id',
      'name field' => 'order_number',
      'numeric' => TRUE,
      'validate type' => 'order_id',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_order',
      'field' => 'order_id',
      'label' => t('Order', array(), array('context' => 'a drupal commerce order')),
    ),
  );

  // Expose the transaction currency.
  $data['commerce_eu_vat_report']['currency_code'] = array(
    'title' => t('Currency'),
    'help' => t('The currency of the transaction.'),
    'field' => array(
      'handler' => 'commerce_payment_handler_field_currency_code',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'commerce_payment_handler_filter_currency_code',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  $data['commerce_eu_vat_report']['taxable'] = array(
    'title' => t('Taxable'),
    'help' => t('The taxable amount on the order.'),
    'field' => array(
      'handler' => 'commerce_payment_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['commerce_eu_vat_report']['taxed'] = array(
    'title' => t('Taxed'),
    'help' => t('The taxed amount on the order.'),
    'field' => array(
      'handler' => 'commerce_payment_handler_field_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter()
 */
function commerce_eu_vat_report_views_data_alter(&$data) {
  // Define the relationship from commerce_order to commerce_payment_transaction.
  $data['commerce_order']['table']['join']['commerce_eu_vat_report'] = array(
    'left_field' => 'order_id',
    'field' => 'order_id',
  );

  $data['commerce_order']['order_id_to_tax_calculation'] = array(
    'real field' => 'order_id',
    'title' => 'VAT Reports',
    'help' => t("Relate this order to its tax reports. This relationship will cause duplicated records if there are multiple tax rates per order."),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_eu_vat_report',
      'base field' => 'order_id',
      'label' => t('Tax', array(), array('context' => 'a drupal commerce tax report')),
    ),
  );
}
