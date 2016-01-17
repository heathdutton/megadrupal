<?php

/**
 * @file
 * Define the view field as a part of the commerce line item. 
 *
 */

/**
 * Alter the views data to enable some additional features for coupons in views
 */
function commerce_product_attributes_views_data_alter(&$data){
  foreach ($data as $table_name => &$table){
    if ($table_name == 'commerce_line_item'){
      $table['product_attributes'] = array(
        'field' => array(
          'title' => t('Product Attributes'),
          'help' => t('Display all the product attributes for the line item.'),
          'handler' => 'commerce_product_attributes_handler_field_attributes',
        ),
      );      
      
    }
  }
}
