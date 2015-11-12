<?php

/**
 * @file
 * Views hook implementations.
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_line_item_cart_form_views_data_alter(&$data){

  // Add a textfield views handler for each instance of the commerce line item memo field.
  $instances = field_info_instances('commerce_line_item');

  foreach ($instances as $bundle => $instance) {
    foreach ($instance as $field_name => $field_info) {
        $data['commerce_line_item']['commerce_line_item_cart_form_' . $field_name] = array(
          'field' => array(
            'title' => t('Editable form field for @field_name', array('@field_name' => $field_info['label'])),
            'help' => t('This form field only works in the cart view.'),
            'handler' => 'commerce_line_item_cart_form_handler_field_cart_form_element',
            'field name' => $field_name,
          ),
        );
    }
  }
}