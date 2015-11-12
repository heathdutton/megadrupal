<?php

/**
 *
 */

/**
 * Implements hook_views_data_alter().
 */
function commerce_reorder_views_data_alter(&$data) {
  // Reorder button.
  $data['commerce_order']['commerce_reorder_button'] = array(
    'field' => array(
      'title' => t('Reorder button.'),
      'help' => t('Display a button to copy the line items to a new order in cart status.'),
      'handler' => 'commerce_reorder_handler_field_commerce_reorder_button',
    ),
  );
}
