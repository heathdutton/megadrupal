<?php

/**
 * @file
 * Implement filter to select by address types.
 */

class ec_store_views_handler_filter_address_type extends views_handler_filter_many_to_one {
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->options['add_table'] = 'ec_transaction_address';
  }

  function get_value_options() {
    $this->value_options = array(
      'billing' => t('Billing'),
      'shipping' => t('Shipping'),
    );
  }

  function query() {
    if (empty($this->value)) {
      return;
    }
    $this->ensure_my_table();
    
    // We use array_values() because the checkboxes keep keys and that can cause
    // array addition problems.
    $this->query->add_where_expression($this->options['group'], "$this->table_alias.$this->real_field IN (:address_types) OR $this->table_alias.$this->real_field IS NULL", array(':address_types' => $this->value));
  }
}
