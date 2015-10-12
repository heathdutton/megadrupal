<?php

/**
 * @file
 * Shipwire shipment entity classes
 */

/**
 * Shipwire shipment entiy.
 */
class ShipwireShipment extends Entity {

  public $label;

  public function __construct($values = array()) {
    parent::__construct($values, 'shipwire_shipment');
  }

  protected function defaultLabel() {
    return shipwire_shipment_title($this);
  }

  protected function defaultUri() {
    return shipwire_shipment_uri($this);
  }

}

/**
 * Shipwire shipment entiy controller.
 */
class ShipwireShipmentController extends EntityAPIController {

  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  public function create(array $values = array()) {
    $values += array(
      'shipwire_shipment_id' => '',
      'type' => '',
      'order_id' => 0,
      'order_prefix' => '',
      'shipwire_id' => '',
      'shipping_method' => '',
      'carrier_code' => '',
      'warehouse_code' => '',
      'date_submitted' => 0,
      'date_shipped' => 0,
      'date_expected' => 0,
      'date_delivered' => 0,
      'tracking_number' => '',
      'tracking_uri' => '',
      'currency' => '',
      'cost_total' => 0,
      'cost_freight' => 0,
      'cost_handling' => 0,
      'cost_packaging' => 0,
      'cost_insurance' => 0,
      'products' => array(),
      'manually_edited' => 0,
      'data' => '',
      'created' => 0,
      'changed' => 0,
      'is_new' => TRUE,
    );

    $shipment = parent::create($values);
    return $shipment;
  }

  public function save($entity, DatabaseTransaction $transaction = NULL) {
    $time = REQUEST_TIME;
    $entity->changed = $time;
    if (empty($entity->created)) {
      $entity->created = $time;
    }
    $result = parent::save($entity);
    return $result;
  }

}
