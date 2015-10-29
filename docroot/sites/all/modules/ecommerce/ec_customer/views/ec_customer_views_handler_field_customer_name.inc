<?php

/**
 * @file
 * Allow the displaying the customer name.
 */

class ec_customer_views_handler_field_customer_name extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['ecid'] = 'ecid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    return ec_customer_get_name($values->{$this->aliases['ecid']});
  }
}
