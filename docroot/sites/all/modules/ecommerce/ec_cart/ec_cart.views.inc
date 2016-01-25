<?php

/**
 * @file
 * Implement views integration for ec_cart module.
 */

/**
 * Implements hook_views_data().
 */
function ec_cart_views_data() {
  $data['ec_product']['hide_cart_link'] = array(
    'title' => t('Hide add to cart link'),
    'help' => t('Lists products where the add to cart link is not used.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Hide cart'),
    ),
  );

  return $data;
}

