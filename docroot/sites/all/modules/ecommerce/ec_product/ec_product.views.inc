<?php

/**
 * @file
 * Implement views hooks for ec_product module.
 */

/**
 * Implements hook_views_data().
 */
function ec_product_views_data() {
  $data = array();

  $data['ec_product']['table']['group'] = t('Product');

  $data['ec_product']['table']['join'] = array(
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
      'type' => 'LEFT',
    ),
  );

  $data['ec_product']['is_product'] = array(
    'title' => t('Is product'),
    'help' => t('Filter out nodes which are products'),
    'filter' => array(
      'field' => 'ptype',
      'handler' => 'ec_product_handler_filter_is_product',
      'label' => t('Is product'),
      'type' => 'yes-no',
    ),
  );

  $data['ec_product']['ptype'] = array(
    'title' => t('Type'),
    'help' => t('The product type.'),
    'field' => array(
      'handler' => 'ec_product_handler_field_ptype',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_product_handler_filter_ptype',
    ),
  );

  $data['ec_product']['sku'] = array(
    'title' => t('SKU'),
    'field' => array(
      'field' => 'sku',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'numeric' => FALSE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['ec_product']['price'] = array(
    'title' => t('Price'),
    'field' => array(
      'handler' => 'ec_product_views_handler_field_price',
    ),
  );



  $data['ec_product_features']['table']['group'] = t('Product');

  $data['ec_product_features']['table']['join'] = array(
    'node' => array(
      'left_table' => 'ec_product',
      'left_field' => 'ptype',
      'field' => 'ptype',
      'type' => 'LEFT',
    ),
  );

  $data['ec_product_features']['ftype'] = array(
    'title' => t('Features'),
    'help' => t('The product features.'),
    'field' => array(
      'handler' => 'ec_product_handler_field_ftype',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_product_handler_filter_ftype',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function ec_product_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_product') . '/views',
    ),
    'handlers' => array(
      'ec_product_handler_filter_is_product' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'ec_product_handler_filter_ptype' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_product_handler_filter_ftype' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_product_handler_field_ptype' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_product_handler_field_ftype' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'ec_product_handler_field_checkout_links' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_product_handler_field_checkout_form' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_product_views_handler_field_price' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
