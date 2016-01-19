<?php

/**
 * @file
 * Views hooks for Basic_cart products.
 */

/**
 * Implements hook_views_data_alter().
 */
function basic_cart_views_data_alter(&$data) {
  // alter a price field 
  if (isset($data['field_data_price'])){
    $data['field_data_price']['price']['group'] = t('Product');
    $data['field_data_price']['price']['field']['handler'] = 'basic_cart_handler_field_price';
    $data['field_data_price']['price']['field']['float'] = TRUE;
    $data['field_data_price']['price']['field']['additional fields']['nid'] = array(
                                                                                    'table' => 'node',
                                                                                    'field' => 'nid'
                                                                                    );
    $data['field_data_price']['price']['field']['additional fields']['type'] = array(
                                                                                    'table' => 'node',
                                                                                    'field' => 'type'
                                                                                    );
  }
  // we haven't own custom table, so alter node table
  $data['node']['is_product'] = array(
    'title' => t('Is a product'),
    'group' => t('Product'),
    'help' => t('Check for the fields provided by the Basic cart module.'),
    'filter' => array(
      'handler' => 'basic_cart_handler_filter_product',
      'label' => t('Is a product'),
    ),
  );

  /*$data['node']['addtocartlink'] = array(
    'title' => t('Add to cart form'),
    'help' => t("Form to put the product in the customer's cart."),
    'group' => t('Product'),
    'field' => array(
      'additional fields' => array(
        'nid' => array(
          'table' => 'node',
          'field' => 'nid',
        ),
        'type' => array(
          'table' => 'node',
          'field' => 'type',
        ),
      ),
      'handler' => 'basic_cart_handler_field_addtocart',
      'element type' => 'div',
    ),
  );*/
}
