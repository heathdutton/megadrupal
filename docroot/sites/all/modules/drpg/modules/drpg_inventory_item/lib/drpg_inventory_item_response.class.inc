<?php

/**
 * @file
 * Data response class for inventory data.
 */

class DrpgInventoryItemResponse extends DrpgDataResponse {
  public $inventory;

  /**
   * Class constructor.
   */
  public function __construct() {
    parent::__construct();
  }
}
