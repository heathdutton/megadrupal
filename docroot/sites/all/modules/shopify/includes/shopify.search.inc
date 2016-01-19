<?php
/**
 * @file
 * Contains functions related to searching Shopify products.
 */

/**
 * Implements hook_search_info().
 */
function shopify_search_info() {
  return array(
    'title' => 'Products',
  );
}

/**
 * Implements hook_search_access().
 */
function shopify_search_access() {
  return user_access('access shopify products');
}

/**
 * Implements hook_search_reset().
 */
function shopify_search_reset() {
  db_update('search_dataset')
    ->fields(array('reindex' => REQUEST_TIME))
    ->condition('type', 'shopify_product')
    ->execute();
}

/**
 * Implements hook_search_execute().
 */
function shopify_search_execute($keys = NULL, $conditions = NULL) {
  $keys = preg_replace('!\*+!', '%', $keys);
  $query = db_select('shopify_products')->extend('PagerDefault');
  $query->fields('shopify_products', array('id', 'variant_id'));
  $query->condition('variant_id', '0');
  $query->condition(db_or()
    ->condition('title', '%' . db_like($keys) . '%', 'LIKE')
    ->condition('body_html', '%' . db_like($keys) . '%', 'LIKE'));
  $product_ids = $query
    ->limit(15)
    ->execute()
    ->fetchCol();
  $products = shopify_product_load_multiple($product_ids);

  $results = array();
  foreach ($products as $product) {
    $build = entity_view('shopify_product', array($product), 'search_result');
    $rendered = drupal_render($build);
    $uri = entity_uri('shopify_product', $product);
    $results[] = array(
      'link' => url($uri['path'], array_merge($uri['options'], array('absolute' => TRUE))),
      'title' => $product->title,
      'snippet' => search_excerpt($keys, $rendered),
      'language' => entity_language('node', $product),
    );
  }

  return $results;
}

/**
 * Implements hook_search_status().
 */
function shopify_search_status() {
  $total = db_query('SELECT COUNT(*) FROM {shopify_products}')->fetchField();
  $remaining = db_query("SELECT COUNT(*) FROM {shopify_products} sp LEFT JOIN {search_dataset} d ON d.type = 'shopify_product' AND d.sid = sp.id WHERE d.sid IS NULL AND sp.variant_id = 0 OR d.reindex <> 0")->fetchField();
  return array('remaining' => $remaining, 'total' => $total);
}

/**
 * Implements hook_update_index().
 */
function shopify_update_index() {
  $limit = (int) variable_get('search_cron_limit', 100);

  $result = db_query_range("SELECT id FROM {shopify_products} sp LEFT JOIN {search_dataset} d ON d.type = 'shopify_product' AND d.sid = sp.id WHERE d.sid IS NULL AND sp.variant_id = 0 OR d.reindex <> 0  ORDER BY d.reindex ASC, sp.id ASC", 0, $limit, array(), array('target' => 'slave'));

  foreach ($result as $product) {
    _shopify_index_product($product);
  }

}

/**
 * Indexes a single product.
 *
 * @param $product
 *   The product to index.
 */
function _shopify_index_product($product) {
  $product = shopify_product_load($product->id);

  // Save the changed time of the most recent indexed product, for the search
  // results half-life calculation.
  variable_set('shopify_cron_last', $product->updated_at);

  // Render the product.
  $build = entity_view('shopify_product', array($product), 'search_index');
  unset($build['#theme']);
  $product->rendered = drupal_render($build);

  $text = '<h1>' . check_plain($product->title) . '</h1>' . $product->rendered;

  // Fetch extra data normally not visible
  $extra = module_invoke_all('shopify_product_update_index', $product);
  foreach ($extra as $t) {
    $text .= $t;
  }

  // Update index
  search_index($product->id, 'shopify_product', $text);
}
