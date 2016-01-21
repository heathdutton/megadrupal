<?php

/**
 * Implements hook_views_data_alter().
 */
function commerce_add_to_cart_radio_views_data_alter(&$data) {
  $data['commerce_product']['add_to_cart_radio'] = array(
    'title' => t('Add to cart (radio input form)'),
    'help' => t('Add to cart field handler that display a radio input element to force the user single selection of a product in the view.'),
    'real field' => 'product_id',
    'field' => array(
      'handler' => 'commerce_add_to_cart_radio_handler_field',
    ),
  );
}
