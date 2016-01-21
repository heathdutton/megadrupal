<?php

/**
 * Implements hook_views_data_alter().
 */
function commerce_fulfilment_oms_views_data_alter(&$data) {
  $data['commerce_product']['omsstocklevel'] = array(
    'title' => t('OMS Stock level'),
    'help' => t('The stock level in all warehouses at OMS.'),
    'field' => array(
      'handler' => 'commerce_fulfilment_oms_handler_field_omsstocklevel',
      'click sortable' => FALSE,
    ),
  );

  $data['commerce_product']['omsstockdiff'] = array(
    'title' => t('Stock difference'),
    'help' => t('The stock difference between Drupal and OMS.'),
    'field' => array(
      'handler' => 'commerce_fulfilment_oms_handler_field_omsstockdiff',
      'click sortable' => FALSE,
    ),
  );
}
