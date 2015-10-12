<?php

/**
 * @file
 * Block callbacks for Drupal integration with Magento.
 */

/**
 * Block callback function to add cart and checkout links to the site.
 */
function magento_customers_block_store_links() {
  return theme('magento_cart_links');
}
