<?php
/**
 * @file
 * Documents hooks that other modules can implement.
 */

/**
 * Returns text for a product to be indexed as search keywords.
 *
 * @param $product stdClass The commerce_product to be indexed.
 *
 * @return String|Array The keywords to be indexed.
 */
function hook_commerce_search_index_product($product) {
  $keywords = explode('-', $product->sku);
  $renderable = entity_view('commerce_product', array($product), 'node_search_index');
  $keywords[] = drupal_render($renderable);
  return $keywords;
}

/**
 * Act on a product being displayed as a search result.
 *
 * This hook is invoked from commerce_search_search_execute(), after
 * commerce_product_load() and commerce_product_view() have been called.
 *
 * @param $product
 *   The product being displayed in a search result.
 *
 * @return array
 *   Extra information to be displayed with search result. This information
 *   should be presented as an associative array. It will be concatenated with
 *   the post information (last updated, author) in the default search result
 *   theming.
 *
 * @see template_preprocess_search_result()
 * @see search-result.tpl.php
 */
function hook_commerce_product_search_result($product) {
  $types = commerce_product_ui_commerce_product_type_info();
  return array('type' => check_plain($types[$product->type]['name']));
}
