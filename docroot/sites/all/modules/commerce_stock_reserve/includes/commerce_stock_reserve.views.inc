<?php
/**
 * @file
 * Views integration for the Commerce Stock Reserve module.
 */

/**
 * Implements hook_views_data().
 */
function commerce_stock_reserve_views_data() {
  $data = array();
  $data['commerce_stock_reserve']['table'] = array(
    'group' => t('Commerce Stock Reserve'),
    'join' => array(
      'commerce_product' => array(
        'left_field' => 'product_id',
        'field' => 'product_id',
      ),
      'commerce_line_item' => array(
        'left_field' => 'line_item_id',
        'field' => 'line_item_id',
      ),
    ),
  );
  $data['commerce_stock_reserve']['amount'] = array(
    'field' => array(
      'title' => t('Reserved stock'),
      'help' => t('The amount of stock that is currently reserved.'),
      'handler' => 'views_handler_field_numeric',
    ),
  );
  return $data;
}
