<?php

/**
 * Filter by rate shipping method id.
 */
class commerce_shipping_rate_handler_filter_method_id extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Shipping method ID');
      $this->value_options = array();
      foreach (commerce_shipping_methods() as $method_id => $infos) {
        $this->value_options[$method_id] = $infos['title'];
      }
    }
  }
}
