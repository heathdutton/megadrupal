<?php

/**
 * @file
 * Contains functions relating to path integration.
 *
 * Largely lifted from the path module.
 */

/**
 * Callback function for shopify_entity_insert().
 *
 * Since products are inserted via the sync methods, this function is usually
 * not hit directly, although it is called from _shopify_path_product_update().
 *
 * @see _shopify_product_update_path()
 */
function _shopify_product_insert_path($product) {
  if (isset($product->path)) {
    $path = $product->path;
    $path['alias'] = trim($path['alias']);
    // Only save a non-empty alias.
    if (!empty($path['alias'])) {
      // Ensure fields for programmatic executions.
      $langcode = entity_language('shopify_product', $product);
      $path['source'] = 'product/' . $product->id;
      $path['language'] = isset($langcode) ? $langcode : LANGUAGE_NONE;
      path_save($path);
    }
  }
}

/**
 * Callback function for shopify_entity_update().
 *
 * Updates product paths
 */
function _shopify_product_update_path($product) {
  if (isset($product->path)) {
    $path = $product->path;
    $path['alias'] = trim($path['alias']);
    // Delete old alias if user erased it.
    if (!empty($path['pid']) && empty($path['alias'])) {
      path_delete($path['pid']);
    }
    _shopify_product_insert_path($product);
  }
}

/**
 * Callback function for shopify_entity_delete().
 *
 * Deletes product paths
 */
function _shopify_product_delete_path($product) {
  // Delete all aliases associated with this product.
  path_delete(array('source' => 'product/' . $product->id));
}
