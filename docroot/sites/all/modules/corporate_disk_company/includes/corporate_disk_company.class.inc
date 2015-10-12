<?php

/**
 * Main class for the Corporate Disk Company Shipment entity type.
 */
class CorporateDiskCompanyShipment extends Entity {

  /**
   * The id of the order number the shipment is attached to.
   *
   * @var int
   */
  public $order_number = 0;

  /**
   * The confirmation number from Delivra.
   *
   * @var string
   */
  public $confirmation = 0;

  /**
   * The tracking numbers.
   *
   * @var string
   */
  public $tracking_number = '';

  /**
   * Has the order been processed to Delivra yet.
   * 
   * @var int
   */
  public $order_updates = 0;

  /**
   * Shipping service choosen.
   *
   * @var string
   */
  public $shipping_service = '';

  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'corporate_disk_company_shipments');
  }

  public function save() {
    if (empty($this->created_at)) {
      $this->created_at = REQUEST_TIME;
    }

    parent::save();
  }

}
