<?php

/**
 * @file
 * Delivery entity controller
 */

class CommerceDeliveryEntityController extends DrupalDefaultEntityController {

  /**
   * Create a default delivery.
   *
   * @param $type
   *   The machine-readable type of the line item.
   * @param $order_id
   *   The ID of the order the line item belongs to (if available).
   *
   * @return
   *   A line item object with all default fields initialized.
   */
  public function create(array $values = array()) {
    return (object) ($values + array(
      'delivery_id' => '',
      'order_id' => 0,
      'status' => 0,
      'created' => '',
      'changed' => '',
    ));
  }

  public function save($delivery) {

    $op = 'update';
    // Set the timestamp fields.
    if (empty($delivery->delivery_id)) {
      $delivery->created = REQUEST_TIME;
      $op = 'insert';
    }
    $delivery->changed = REQUEST_TIME;

    if ($op == 'insert') {
      drupal_write_record('commerce_delivery', $delivery);
      field_attach_insert('commerce_delivery', $delivery);
    }
    else {
      drupal_write_record('commerce_delivery', $delivery, 'delivery_id');
      field_attach_update('commerce_delivery', $delivery);
    }
    module_invoke_all('entity_insert', $delivery, 'commerce_delivery');
    rules_invoke_event('commerce_delivery_' . $op, $delivery);

    return $delivery;
//    return parent::save($delivery);
  }
}



