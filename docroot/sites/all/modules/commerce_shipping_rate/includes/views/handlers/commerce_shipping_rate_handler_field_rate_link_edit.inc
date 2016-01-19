<?php

/**
 * @file
 * Field handler to present a commerce shipping rate edit link.
 */

class commerce_shipping_rate_handler_field_rate_link_edit extends commerce_shipping_rate_handler_field_rate_link {
  /**
   * Class constructor.
   */
  function construct() {
    parent::construct();
  }

  /**
   * Display a link to edit a rate.
   *
   * @param array $values
   *   View's values.
   *
   * @return bool|string
   *   Returns a link to edit a rate.
   */
  function render($values) {
    // Ensure the user has access to delete this order.
    $name = $this->get_value($values, 'name');
    $method_id = $this->get_value($values, 'method');
    $rate_id = $this->get_value($values, 'rate_id');
    $commerce_shipping_rate = commerce_shipping_rate_new($method_id, $name);
    $commerce_shipping_rate->rate_id = $rate_id;

    if (!commerce_entity_access('update', $commerce_shipping_rate, NULL, 'commerce_shipping_rate')) {
      return FALSE;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');

    $service_name_url = 'shipping-rates' . '-' . strtr($name, '_', '-');
    $method_name_url = strtr($method_id, '_', '-');

    return l($text, 'admin/commerce/config/shipping/shipping-rates/manage/' . $rate_id . '/edit', array('query' => drupal_get_destination()));
  }
}
