<?php

/**
 * Batch processor: synchronize stock for a single product.
 *
 * @see commerce_ads_product_stock_sync_batch_submit()
 */
function commerce_ads_batch_product_stock_sync_process($stock_item, &$context) {
  // Unable to find ProductId in the stock information.
  if (!isset($stock_item['ProductId']) || !isset($stock_item['StockQuantity'])) {
    watchdog('commerce_ads', 'Stock update batch: Unable to find ProductID or StockQuantity in the stock information: !stock_item', array(
      '!stock_item' => '<pre>' . var_export($stock_item, TRUE) . '</pre>',
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Unable to load the product.
  if (!$product = commerce_product_load_by_sku($stock_item['ProductId'])) {
    watchdog('commerce_ads', 'Stock update batch: Unable to load product from ProductID @product_id', array(
      '@product_id' => $stock_item['ProductId'],
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // No commerce_stock field on the product entity.
  if (empty($product->commerce_stock)) {
    watchdog('commerce_ads', 'Stock update batch: Field @field_name could not be found on product ID @product_id', array(
      '@field_name' => 'commerce_stock',
      '@product_id' => $product->sku,
    ), WATCHDOG_ERROR);
    return FALSE;
  }

  // Update stock value in the product entity.
  commerce_ads_update_product_stock($product, $stock_item);
}

/**
 * Batch finish handler.
 *
 * @see commerce_ads_product_stock_sync_batch_submit()
 */
function commerce_ads_batch_product_stock_sync_finished($success, $results, $operations) {
  // Delete variable marking batch product update as running.
  variable_del(COMMERCE_ADS_WS_PRODUCT_UPDATE_RUNNING);
}

/**
 * Batch finish handler.
 *
 * @see commerce_ads_send_all_products_batch_submit()
 */
function commerce_ads_batch_send_all_products_finished($success, $results, $operations) {
  // Delete variable marking batch product update as running.
  variable_del(COMMERCE_ADS_WS_PRODUCT_UPDATE_RUNNING);
}

/**
 * Batch finish handler.
 *
 * @see commerce_ads_send_all_orders_batch_submit()
 */
function commerce_ads_batch_send_all_orders_finished($success, $results, $operations) {
  // Delete variable marking batch order update as running.
  variable_del(COMMERCE_ADS_WS_ORDER_UPDATE_RUNNING);
}
