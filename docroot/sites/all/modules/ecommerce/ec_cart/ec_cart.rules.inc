<?php

/**
 * @file
 * Enable rules integration into ec_cart to allow the manipluation of events.
 */

/**
 * Implements hook_rules_event_info().
 */
function ec_cart_rules_event_info() {
  return array(
    'ec_cart_add' => array(
      'group' => t('cart'),
      'label' => t('After item added to cart'),
      'module' => 'ec_cart',
      'variables' => array(
        'product' => array(
          'type' => 'node',
          'label' => t('Product that was added to cart'),
        ),
      ),
    ),
    'ec_cart_delete' => array(
      'group' => t('cart'),
      'label' => t('After item has been deleted from the cart'),
      'module' => 'ec_cart',
      'variables' => array(
        'product' => array(
          'type' => 'node',
          'label' => t('Product that was deleted from the cart'),
        ),
      ),
    ),
    'ec_cart_edit' => array(
      'group' => t('cart'),
      'label' => t('After item has been edited in the cart'),
      'module' => 'ec Cart',
      'variables' => array(
        'product' => array(
          'type' => 'node',
          'label' => t('Product that was edited in the cart'),
        ),
      ),
    ),
    'ec_cart_view' => array(
      'group' => t('cart'),
      'label' => t('Before cart view page'),
      'module' => 'ec_cart',
      'variables' => array(),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function ec_cart_rules_condition_info() {
  return array(
    'ec_cart_is_empty' => array(
      'group' => t('cart'),
      'label' => t('Cart is empty'),
      'module' => 'ec_cart',
      'parameter' => array(
      ),
    ),
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_cart_is_empty() {
  $items = ec_cart_current();
  return empty($items);
}
