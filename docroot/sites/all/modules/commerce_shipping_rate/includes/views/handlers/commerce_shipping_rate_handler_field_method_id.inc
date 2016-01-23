<?php

/**
 * Field handler to translate a rate method id into its readable form.
 */
class commerce_shipping_rate_handler_field_method_id extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['method'] = 'method';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $method_id = $this->get_value($values, 'method');
    $method = commerce_shipping_method_load($method_id);

    if (isset($method['title'])) {
      return $this->sanitize_value($method['title']);
    }
  }
}
