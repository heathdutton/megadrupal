<?php

/**
 * Field handler to translate a rate method id into its readable form.
 */
class commerce_shipping_rate_handler_field_service_name extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['name'] = 'name';
    $this->additional_fields['method'] = 'method';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $method_id = $this->get_value($values, 'method');
    $name = $this->get_value($values, 'name');
    $services = commerce_shipping_services($method_id);

    if (isset($services[$name])) {
      return $this->sanitize_value($services[$name]['title']);
    }
  }
}
