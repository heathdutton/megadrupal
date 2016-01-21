<?php

/**
 * @file
 * Argument handler to display product owner in View using product arguments.
 */
class commerce_product_key_handler_argument_product_key_owner extends views_handler_argument_numeric {

  function title_query() {
    $titles = array();
    /*
    $result = db_select('commerce_product_key', 'cpk')
      ->fields('cpk', array('key_owner_id'))
      ->condition('cpk.key_owner_id', $this->value)
      ->execute();
    */

    $result = db_query("SELECT cpk.key_owner_id FROM {commerce_product_key} cpk WHERE cpk.key_owner_id IN (:key_owner_ids)", array(':key_owner_ids' => $this->value));
    foreach ($result as $product_key) {
      $titles[] = check_plain($product_key->key_owner_id);
    }
    return $titles;
  }
}
