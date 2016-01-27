<?php

/**
 * Export Drupal Commerce orders to Views.
 */




function commerce_option_views_data_alter(&$data){
  foreach ($data as $table_name => &$table){
    if ($table_name == 'commerce_line_item'){
      /*foreach($table as $field_name => &$field) {
        if($field_name == 'type') {
          $field['relationship'] = array(
            'base' => 'commerce_order_total_line_type',
            'handler' => 'views_handler_relationship',
            'label' => t('Order Total Type'),
            'base field' => 'line_item_type',
            'field' => 'type',
          );
        }
      }*/

    }
  }
}
