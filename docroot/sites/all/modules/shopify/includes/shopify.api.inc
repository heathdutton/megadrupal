<?php
/**
 * @file
 * Contains functions for working with the Shopify API.
 */

/**
 * Perform an API call to Shopify.
 *
 * @param string $method
 *   HTTP method: GET, POST, PUT, DELETE.
 * @param $path
 *   API path to the resource.
 * @param array $options
 *   Options to pass to the API call.
 *
 * @return array|int
 *   Returns the API response data. May be an array or count.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_call($method, $path, array $options = array()) {
  $client = shopify_api_client();
  if (!$client instanceof ShopifyClient) {
    throw new Exception('Unable to create ShopifyClient. Check your Shopify API credentials at /admin/config/services/shopify');
  }
  return $client->call($method, '/admin/' . $path . '.json', $options);
}

/**
 * Gets the Shopify shop info from the API or the variable.
 *
 * @param bool $refresh
 *   Whether to refresh the info from the API.
 *
 * @return object
 *   Return object of store information.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_shop_info($refresh = FALSE) {
  $info = (array) shopify_store_info();
  if ($refresh == TRUE || empty($info)) {
    $info = shopify_api_call('GET', 'shop');
    variable_set('shopify_store_info', $info);
  }
  return (object) $info;
}

/**
 * Gets the admin path for the Shopify store.
 */
function shopify_api_get_shop_admin_path() {
  return 'https://' . shopify_api_get_shop_info()->domain . '/admin';
}

/**
 * Gets the Shopify store product path.
 *
 * @param int $product_id
 *   Product ID.
 * @param int $variant_id
 *   Variant ID.
 *
 * @return string
 *   Product path.
 */
function shopify_api_get_product_path($product_id, $variant_id = 0) {
  $variant_path = '';
  if ($variant_id != 0) {
    $variant_path = '/variants/' . $variant_id;
  }
  return shopify_api_get_shop_admin_path() . '/products/' . $product_id . $variant_path;
}

/**
 * Get Shopify products from the API.
 *
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_products(array $options = array()) {
  return shopify_api_call('GET', 'products', $options);
}

/**
 * Get Shopify collections from the API.
 *
 * @param array $options
 *   Arguments to pass to the API.
 * @param string $type
 *   Type of collections to get, options are:
 *   - both: Get smart and custom collections.
 *   - smart: Get smart collections only.
 *   - custom: Get custom collections only.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_collections(array $options = array(), $type = 'both') {
  $smart_collections = $custom_collections = array();
  if ($type == 'both' || $type == 'smart') {
    $smart_collections = shopify_api_call('GET', 'smart_collections', $options);
  }
  if ($type == 'both' || $type == 'custom') {
    $custom_collections = shopify_api_call('GET', 'custom_collections', $options);
  }
  return array_merge($smart_collections, $custom_collections);
}

/**
 * Get Shopify collects from the API.
 *
 * @param array $options
 *   Arguments to pass the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_collects(array $options = array()) {
  return shopify_api_call('GET', 'collects', $options);
}

/**
 * Get the Shopify product count from the API.
 *
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return int
 *   Returns the product count.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_product_count(array $options = array()) {
  return shopify_api_call('GET', 'products/count', $options);
}

/**
 * Get the Shopify variants count for a specific product from the API.
 *
 * @param int $product_id
 *   Product ID.
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return int
 *   Returns the number of variants for this product.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_product_variants_count($product_id, array $options = array()) {
  return shopify_api_call('GET', "products/{$product_id}/variants/count", $options);
}

/**
 * Get Shopify collections count from the API.
 *
 * @param array $options
 *   Arguments to pass to the API.
 * @param string $type
 *   Type of collections to count, options are:
 *   - both: Get smart and custom collections count.
 *   - smart: Get smart collections count only.
 *   - custom: Get custom collections count only.
 *
 * @return int
 *   Returns the collection count.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_collection_count(array $options = array(), $type = 'both') {
  $smart_count = $custom_count = 0;
  if ($type == 'both' || $type == 'smart') {
    $smart_count = shopify_api_call('GET', 'smart_collections/count', $options);
  }
  if ($type == 'both' || $type == 'custom') {
    $custom_count = shopify_api_call('GET', 'custom_collections/count', $options);
  }
  return $custom_count + $smart_count;
}

/**
 * Get the Shopify product information.
 *
 * @param int $product_id
 *   Shopify Product ID.
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_product($product_id, array $options = array()) {
  return shopify_api_call('GET', "products/{$product_id}", $options);
}

/**
 * Get the Shopify product variants.
 *
 * @param int $product_id
 *   Shopify Product ID.
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_product_variants($product_id, array $options = array()) {
  return shopify_api_call('GET', "products/{$product_id}/variants", $options);
}

/**
 * Get the Shopify variant information.
 *
 * @param int $variant_id
 *   Shopify Variant ID.
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_variant($variant_id, array $options = array()) {
  return shopify_api_call('GET', "variants/{$variant_id}", $options);
}

/**
 * Get the Shopify product images.
 *
 * @param int $product_id
 *   Shopify Product ID.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_product_images($product_id) {
  return shopify_api_call('GET', "products/{$product_id}/images");
}

/**
 * Get Shopify webhooks.
 *
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_webhooks(array $options = array()) {
  return shopify_api_call('GET', 'webhooks', $options);
}

/**
 * Get the Shopify webhooks count.
 *
 * @param array $options
 *   Arguments to pass to the API.
 *
 * @return int
 *   Returns the webhooks count.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_get_webhook_count(array $options = array()) {
  return shopify_api_call('GET', 'webhooks/count', $options);
}

/**
 * Creates a new Shopify webhook.
 *
 * @param array $post
 *   Data to POST to the API.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_create_webhook($post) {
  return shopify_api_call('POST', 'webhooks', $post);
}

/**
 * Deletes a new Shopify webhook.
 *
 * @param int $id
 *   Webhook ID to delete.
 *
 * @return array
 *   Returns the API response data.
 *
 * @throws \ShopifyApiException
 */
function shopify_api_delete_webhook($id) {
  return shopify_api_call('DELETE', "webhooks/{$id}");
}
