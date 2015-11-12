<?php

/**
 * Export the Corporate Disk Shipments to Views.
 */

/**
 * Implements hook_views_data_alter().
 */
function corporate_disk_company_views_data_alter(&$data) {
  $data = array();
  /*
  $data['corporate_disk_company_shipments']['table']['group'] = t('Corporate Disk Shipments');
  $data['corporate_disk_company_shipments']['table']['base'] = array(
    'field' => 'order_number',
    'title' => t('Corporate Disk Company Shipments'),
    'help' => t('Order shipment entry.'),
  );
  $data['corporate_disk_company_shipments']['table']['entity type'] = 'corporate_disk_company_shipment';
   * 
   */
  $data['corporate_disk_company_shipments']['created_at'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of EPOCH Time.'),
    //'filter' => array(
    //  'handler' => 'corporate_disk_company_handler_filter_date',
    //),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  /* Expose the Order Number.
  $data['corporate_disk_company_shipments']['order_number'] = array(
    'title' => t('Order Number'),
    'help' => t('Commerce Order Number.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'commerce_order',
      'base field' => 'order_id',
      'relationship field' => 'order_id',
      'label' => t('Order ID'),
    ),
  );*/
  return $data;
}
