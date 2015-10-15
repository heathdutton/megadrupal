<?php
/**
 * @file
 * Field handler for product price.
 */

class views_handler_field_shopify_price extends views_handler_field_numeric {
  /**
   * Render.
   */
  public function render($values) {
    $query = db_select('shopify_products', 'p');
    $query
      ->condition('p.product_id', $values->shopify_products_product_id)
      ->condition('p.variant_id', 0, '<>')
      ->fields('p', array('price'))
      ->orderBy('p.price', 'ASC')
      ->range(0, 1);
    $result = $query->execute();
    return shopify_money_format() . $result->fetchColumn();
  }

}
