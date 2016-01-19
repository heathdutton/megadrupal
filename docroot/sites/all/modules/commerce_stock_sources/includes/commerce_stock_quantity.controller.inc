<?php

/**
 * @file
 * The controller for the commerce_stock_quantity entity containing the CRUD operations.
 */

/**
 * The controller class for commerce_stock_quantity contains methods for the product CRUD operations.
 *
 * Mainly relies on the EntityAPIController class provided by the Entity
 * module, just overrides specific features.
 */
class CommerceStockQuantityController extends DrupalCommerceEntityController {

  /**
   * Create a default commerce_stock_quantity.
   *
   * @param array
   *   $values
   *   An array of values to set, keyed by property name.
   *
   * @return commerce_stock_quantity
   *   A commerce_stock_quantity object with all default fields initialized.
   */
  public function create(array $values = array()) {

    $values += array(
      'stq_id' => NULL,
      'source_id' => NULL,
      'product_id' => NULL,
      'quantity' => NULL,
      'is_new' => empty($values['stq_id']),
      'uid' => '',
      'status' => 1,
    );

    return parent::create($values);
  }
}
