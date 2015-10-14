<?php

/**
 * @file
 * Implementation of rules hooks and actions.
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_wishlist_rules_event_info() {
  return array(
    'commerce_wishlist_event_product_added_to_wishlist' => array(
      'label' => t('After adding a product to wishlist'),
      'help' => t('Triggers when a product has been added to wishlist.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist_owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'commerce_wishlist_node' => array(
          'type'  => 'node',
          'label' => t('Product node'),
        ),
      ),
    ),
    'commerce_wishlist_event_product_added_to_cart' => array(
      'label' => t('After adding a wishlist product to the cart'),
      'help' => t('Triggers when a wishlist product has been added to the shopping cart.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist_owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'commerce_wishlist_node' => array(
          'type'  => 'node',
          'label' => t('Product node'),
        ),
      ),
    ),
    'commerce_wishlist_event_product_purchased' => array(
      'label' => t('After purchasing a product from a wishlist'),
      'help' => t('Triggers when a wishlist product is purchased.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist_owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'commerce_wishlist_node' => array(
          'type'  => 'node',
          'label' => t('Product node'),
        ),
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('The order that the product was purchased in.'),
        ),
      ),
    ),
    'commerce_wishlist_event_product_removed' => array(
      'label' => t('After removing a product from wishlist'),
      'help' => t('Triggers when a product has been removed from a wishlist.'),
      'group' => t('Commerce Wishlist'),
      'variables' => array(
        'commerce_wishlist_owner' => array(
          'type' => 'user',
          'label' => t('Wishlist owner'),
        ),
        'commerce_wishlist_product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'commerce_wishlist_node' => array(
          'type'  => 'node',
          'label' => t('Product node'),
        ),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_wishlist_rules_action_info() {
  return array(
    'commerce_wishlist_action_remove_product_for_user' => array(
      'label' => t('Remove product from wishlist'),
      'group' => t('Commerce Wishlist'),
      'parameter' => array(
        'product' => array(
          'type'  => 'commerce_product',
          'label' => t('Product'),
        ),
        'account' => array(
          'type'  => 'user',
          'label' => t('User'),
        ),
      ),
    ),
  );
}

/**
 * Rules action callback to remove a specific product from user's wishlist.
 */
function commerce_wishlist_action_remove_product_for_user($product, $account) {
  _commerce_wishlist_remove_product_for_user($product->product_id, $account->uid);
}
