<?php

/**
 * @file
 * Implementation of views for ec_checkout module.
 */

/**
 * Implements hook_views_data().
 */
function ec_checkout_views_data() {
  $data['ec_product']['checkout'] = array(
    'field' => array(
      'title' => t('Checkout'),
      'help' => t('Display either the checkout form or checkout links based on e-Commerce settings.'),
      'handler' => 'ec_checkout_views_handler_field_checkout',
      'real field' => 'nid',
    ),
  );

  $data['ec_product']['checkout_links'] = array(
    'field' => array(
      'title' => t('Checkout links'),
      'help' => t('Provide links which will allow customers to purchase a product'),
      'handler' => 'ec_checkout_handler_field_checkout_links',
      'real field' => 'nid',
    ),
  );

  $data['ec_product']['checkout_product_form'] = array(
    'field' => array(
      'title' => t('Checkout form'),
      'help' => t('Display the product form which can be used to purchase the product.'),
      'handler' => 'ec_checkout_handler_field_checkout_form',
      'real field' => 'nid',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function ec_checkout_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_checkout') . '/views',
    ),
    'handlers' => array(
      'ec_checkout_views_handler_field_checkout' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_checkout_handler_field_checkout_links' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_checkout_handler_field_checkout_form' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

