<?php
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Alter whether Commerce Stock Reserve thinks a product has stock control.
 *
 * This allows sites to have their own stock control logic (instead of Commerce
 * Simple Stock).
 *
 * @param bool &$enabled
 *   Whether stock control is enabled for the product.
 * @param object $product
 *   The Commerce product object.
 */
function hook_commerce_stock_reserve_product_is_enabled_alter(&$enabled, $product) {
  if (isset($product->my_custom_stock_field)) {
    $enabled = TRUE;
  }
}

/**
 * Alter what Commerce Stock Reserve thinks is the current product stock level.
 *
 * This helps sites that have their own stock storage (instead of Commerce
 * Simple Stock).
 *
 * @param mixed &$stock
 *   The stock level.
 * @param object $product
 *   The Commerce product object.
 */
function hook_commerce_stock_reserve_get_product_stock_alter(&$stock, $product) {
  if (isset($product->my_custom_stock_field)) {
    $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
    $stock = $product_wrapper->my_custom_stock_field->value();
  }
}

/**
 * Set the current stock level for a product.
 *
 * This helps sites that have their own stock storage (instead of Commerce
 * Simple Stock).
 *
 * The product should not be saved in implementations of this hook.
 *
 * @param object $product
 *   The Commerce product object.
 * @param mixed $stock
 *   The new stock level to set.
 *
 * @return mixed
 *   Return FALSE on failure. Any other return value will be ignored.
 */
function hook_commerce_stock_reserve_set_product_stock($product, $stock) {
  if (isset($product->my_custom_stock_field)) {
    $product_wrapper = entity_metadata_wrapper('commerce_product', $product);
    $product_wrapper->my_custom_stock_field->set($stock);
  }
  else {
    return FALSE;
  }
}
