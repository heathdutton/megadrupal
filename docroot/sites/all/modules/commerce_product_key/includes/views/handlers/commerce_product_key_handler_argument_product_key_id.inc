<?php

/**
 * @file
 * Argument handler to display product codes in View using product arguments.
 */
class commerce_product_key_handler_argument_product_key_id extends views_handler_argument_numeric {
  function title_query() {
    $titles = array();
    $result = db_select('commerce_product_key', 'cpk')
      ->fields('cpk', array('code'))
      ->condition('cpk.product_key_id', $this->value)
      ->execute();
    foreach ($result as $product_key) {
      $titles[] = check_plain($product_key->code);
    }
    return $titles;
  }
}
