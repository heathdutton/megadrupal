<?php

/**
 * @file
 * Views hooks and callback registries.
 */

/**
 * Implements hook_views_data().
 */
function uc_volume_pricing_views_data() {
  $data['uc_volume_pricing']['table']['group'] = t('Volume Pricing');

  $data['uc_volume_pricing']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
    'node_revisions' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  $data['uc_volume_pricing']['min'] = array(
    'title' => t('Minimum Quantity'),
    'help' => t('The min setting before price can be met.'),
    'field' => array(
      'handler' => 'views_handler_field', //uc_volume_pricing_handler_field_min multiple value fix needed
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['uc_volume_pricing']['price'] = array(
    'title' => t('Volume price'),
    'help' => t("The price when bought with minimum quantity."),
    'field' => array(
      'handler' => 'uc_product_handler_field_price',
      'click sortable' => TRUE,
      'float' => TRUE,
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),
      ),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}
