<?php
/**
 * @file
 *   Contains menu callbacks.
 */

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing products.
 */
function commerce_direct_buy_search($string = '') {
  $matches = array();
  
  if ($string) {
    $result = db_select('commerce_product', 'p')
      ->fields('p', array('title', 'sku'))
      ->condition(db_or()->condition('title', db_like($string) . '%', 'LIKE')->condition('sku', db_like($string) . '%', 'LIKE'))
      ->condition('status', 1)
      ->range(0, 5)
      ->execute();
    foreach ($result as $product) {
      $matches[$product->sku] = check_plain($product->title) . '<br />' . check_plain($product->sku);;
    }
  }

  drupal_json_output($matches);
}