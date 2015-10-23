<?php
/**
 * @file
 * Hooks provided by the Commerce Schema.org module.
 */

/**
 * Alter renderable array to display Product schema.org microdatas.
 *
 * This content should not be visible to customers, so be careful to not
 * generate visible markups.
 *
 * @param array $data
 *   Renderable array to edit to add own microdatas.
 * @param object $entity
 *   Product display entity microdata has to based on.
 */
function hook_commerce_schemaorg_product_alter(&$data, &$entity) {
  $products = entity_load('commerce_product', $entity->field_product[0]);
  $product = array_pop($products);
  $wrapper_product = entity_metadata_wrapper('commerce_product', $product);

  $data += array(
    'sku' => array(
      'priceCurrency' => array(
        '#theme' => 'html_tag',
        '#tag' => 'meta',
        '#access' => user_access('access commerce_schemaorg offers'),
        '#attributes' => array(
          'itemprop' => 'sku',
          'content' => (isset($wrapper_product->sku)) ? check_plain($wrapper_product->sku->value()) : '',
        ),
      ),
    ),
    'offers' => array(
      '#type' => 'container',
      '#access' => user_access('access commerce_schemaorg offers'),
      '#attributes' => array(
        'itemprop' => 'offers',
        'itemscope' => 'itemscope',
        'itemtype' => 'http://schema.org/Offer',
      ),
      'priceCurrency' => array(
        '#theme' => 'html_tag',
        '#tag' => 'meta',
        '#access' => isset($wrapper_product->commerce_price),
        '#attributes' => array(
          'itemprop' => 'priceCurrency',
          'content' => (isset($wrapper_product->commerce_price)) ? check_plain($wrapper_product->commerce_price->currency_code->value()) : '',
        ),
      ),
      'price' => array(
        '#theme' => 'html_tag',
        '#tag' => 'meta',
        '#access' => isset($wrapper_product->commerce_price),
        '#attributes' => array(
          'itemprop' => 'price',
          'content' => (isset($wrapper_product->commerce_price)) ? check_plain($wrapper_product->commerce_price->amount->value()) / 100 : '',
        ),
      ),
    ),
  );
}
