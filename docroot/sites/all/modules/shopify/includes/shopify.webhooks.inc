<?php
/**
 * @file
 * Contains functions relating to Shopify webhooks.
 */

/**
 * Shopify webhook menu callback.
 */
function shopify_incoming_webhook() {
  $json = file_get_contents('php://input');
  $payload = json_decode($json, TRUE);
  $hook = str_replace('/', '_', $_SERVER['HTTP_X_SHOPIFY_TOPIC']);

  // Log webhooks if the debug option is set.
  if (variable_get('shopify_webhooks_debug', FALSE)) {
    watchdog('shopify_webhook', $hook . ':' . check_plain(var_export($payload, TRUE)));
  }

  // Allow modules to hook into webhooks firing.
  module_invoke_all('shopify_webhook', $hook, $payload);
  module_invoke_all('shopify_webhook_' . $hook, $payload);

  print '';
}

/**
 * Implements hook_shopify_webhook().
 */
function shopify_shopify_webhook($hook, $payload) {
  if (module_exists('rules')) {
    rules_invoke_event('shopify_webhook_fired', $hook);
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_products_create($product) {
  shopify_shopify_webhook_products_update($product);
  if (module_exists('rules')) {
    $product_entity = shopify_product_load_single($product['id']);
    rules_invoke_event('shopify_webhook_product_created', $product_entity);
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_collections_update($collection) {
  shopify_collection_update($collection, $sync_products = TRUE);
  if (module_exists('rules')) {
    $term = shopify_collection_load($collection['id']);
    rules_invoke_event('shopify_webhook_collection_updated', $term);
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_collections_delete($collection) {
  $term = shopify_collection_load($collection['id']);
  shopify_collection_delete($collection['id']);
  if (module_exists('rules')) {
    rules_invoke_event('shopify_webhook_collection_deleted', $term);
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_orders_paid($order) {
  // Update product inventory counts for purchased products.
  foreach ($order['line_items'] as $line_item) {
    if ($variant = shopify_product_load_by_variant_id($line_item['variant_id'])) {
      shopify_product_update($line_item['product_id'], $variant->variant_id, shopify_api_get_variant($variant->variant_id, array(
        'fields' => 'inventory_quantity,old_inventory_quantity',
      )));
    }
  }
  if (module_exists('rules')) {
    rules_invoke_event_by_args('shopify_webhook_order_paid', shopify_rules_parse_order_args($order));
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_shop_update($payload) {
  // Update store info on the site.
  variable_set('shopify_store_info', $payload);
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_collections_create($collection) {
  shopify_collection_create($collection, $sync_products = TRUE);
  if (module_exists('rules')) {
    $term = shopify_collection_load($collection['id']);
    rules_invoke_event('shopify_webhook_collection_created', $term);
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_products_update($product) {
  $product_entity = shopify_product_update($product['id'], 0, $product);
  $product_entity->save();
  foreach ($product['variants'] as $v) {
    $variant = shopify_product_update($product['id'], $v['id'], $v);
    $variant->save();
  }
  if (module_exists('rules')) {
    rules_invoke_event('shopify_webhook_product_updated', $product_entity);
  }
}

/**
 * Implements hook_shopify_webhook_WEBHOOK().
 */
function shopify_shopify_webhook_products_delete($product) {
  $product_entity = shopify_product_load_single($product['id']);
  shopify_delete_by_product_id($product['id']);
  if (module_exists('rules')) {
    rules_invoke_event('shopify_webhook_product_deleted', $product_entity);
  }
}
