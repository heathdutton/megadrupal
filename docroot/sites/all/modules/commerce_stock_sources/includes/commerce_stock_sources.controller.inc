<?php

/**
 * @file
 * The controller for the commerce_stock_sources entity containing the CRUD operations.
 */

/**
 * The controller class for commerce_stock_sources contains methods for the product CRUD operations.
 *
 * Mainly relies on the EntityAPIController class provided by the Entity
 * module, just overrides specific features.
 */
class CommerceStockSourcesController extends DrupalCommerceEntityController {

  /**
   * Create a default commerce_stock_source.
   *
   * @param array $values
   *   An array of values to set, keyed by property name.
   *
   * @return commerce_stock_source
   *   A commerce_stock_source object with all default fields initialized.
   */
  public function create(array $values = array()) {
    $values += array(
      'source_id' => NULL,
      'is_new' => TRUE,
      'name' => '',
      'uid' => '',
      'status' => 1,
    );

    return parent::create($values);
  }
}
