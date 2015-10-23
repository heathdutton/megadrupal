<?php

/**
 * @file
 * Implement views hooks
 */

/**
 * Implements hook_views_data_alter()
 */
function commerce_preorder_views_data_alter(&$data) {
  // Order alters
  if (isset($data['commerce_order'])) {
    // Define a handler for an area used for order issue licenses.
    $data['commerce_order']['commerce_preorder_checkout_link'] = array(
      'title' => t('Checkout link'),
      'help' => t('Displays a link to checkout with this order if state is cart.'),
      'field' => array(
        'handler' => 'commerce_preorder_handler_field_checkout_link',
      ),
    );
  }
}
