<?php

/**
 * @file
 * Field handler to present an order's operations links.
 */
class commerce_delivery_handler_line_item_set_delivery extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['order_id'] = 'order_id';
    $this->additional_fields['line_item_id'] = 'line_item_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $destination = drupal_get_destination();
    $order_id = $values->{$this->aliases['order_id']};
    $line_item_id = $values->{$this->aliases['line_item_id']};
    $line_item = current(entity_load('commerce_line_item', array($line_item_id)));
    $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $current_delivery_id = $wrapper->delivery_ref->value();

    $deliveries = commerce_delivery_get_deliveries($order_id, 'active');

    foreach ($deliveries as $type) {
      foreach ($type as $delivery) {
        if ($current_delivery_id != $delivery->delivery_id) {
          $query = $destination;
          $query['token'] = commerce_delivery_get_token($order_id, $line_item_id, $delivery->delivery_id);
          $links[$delivery->delivery_id] = array(
            'title' => $delivery->delivery_id,
            'href' => 'admin/commerce/orders/' . $order_id . '/deliveries/move/' . $line_item_id . '/' . $delivery->delivery_id,
            'query' => $query,
          );
        }
      }
    }
    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('arguments' => $destination, 'class' => array('links', 'inline', 'operations'))));
    }
  }
}
