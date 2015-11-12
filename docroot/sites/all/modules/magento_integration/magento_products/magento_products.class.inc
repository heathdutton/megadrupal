<?php

/**
 * @file
 * Classes for interacting with the Magento Core API for Products.
 */

require_once __DIR__ . '/../magento.class.inc';

/**
 * Extends the core Magento class to add methods for interacting with products.
 */
class MagentoProducts extends Magento {
  /**
   * Get all products from the store.
   *
   * @param array $product_ids
   *   An array of product_ids to ping in one fell swoop. More efficient than
   *   individual calls.
   */
  public function getProducts($product_ids) {
    $products = array();
    $calls = array();
    foreach ($product_ids as $product_id) {
      $calls[] = array('product.info', $product_id);
      $calls[] = array('catalog_product_attribute_media.list', $product_id);
    }
    if (!($response = $this->call(NULL, $calls, TRUE))) {
      return array();
    }

    // Magento doesn't give us any product id in the media list that comes back,
    // so we have to do poor man's matching. Oh, Magento. You try so hard. We
    // add the media list to the product array to make it simple to handle.
    foreach ($response as $item) {
      if (isset($item['product_id'])) {
        $products[$item['product_id']] = $item;
      }
      else if (isset($item[0]['file'])) {
        end($products);
        $products[key($products)]['media'] = $item;
        $products[key($products)]['primary_image'] = isset($item[0]) ? $item[0] : FALSE;
      }
    }

    return $products;
  }

  /**
   * Get a specific product by ID.
   *
   * @param int $product_id
   *   The Magento product ID, oddly enough, not the SKU like the docs say it
   *   should be.
   * @return array
   *   An array of product details.
   */
  public function getProductByProductId($product_id) {
    $products = &drupal_static('magento_products');
    if (isset($products[$product_id])) {
      return $products[$product_id];
    }

    // Not cached so serve it up.
    $products[$product_id] = $this->call('product.info', $product_id);
    return $products[$product_id];
  }

  /**
   * Get a list of products updated after a certain date.
   *
   * @param string $date
   *   A date string in the format "2013-09-19 11:53:19".
   * @return array
   *   All of the products that have been updated since $date.
   */
  public function getProductsUpdatedAfter($date) {
    $filters = array();
    $filters['updated_at'] = array('gt' => $date);
    $products = $this->call('catalog_product.list', array($filters));
    return $products;
  }

  /**
   * Get product images.
   */
  public function getProductImages($product_id) {
    return $this->call('catalog_product_attribute_media.list', $product_id);
  }
}
