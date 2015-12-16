<?php

/**
 * Filter by rate shipping service name.
 */
class commerce_shipping_rate_handler_filter_service_name extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Shipping service name');
      $this->value_options = array();
      foreach (commerce_shipping_methods() as $method_id => $method_infos) {
        foreach (commerce_shipping_services($method_id) as $service_name => $infos) {
          $this->value_options[$method_infos['title']][$service_name] = $infos['title'];
        }
      }
    }
  }
}
