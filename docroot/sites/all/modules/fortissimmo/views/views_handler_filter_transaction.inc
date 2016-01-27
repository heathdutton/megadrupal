<?php

/**
 * @file
 * Filter by transaction type
 */
class views_handler_filter_transaction extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Transaction type');
      $types = fortissimmo_get_transactions();
      foreach ($types as $value => $name) {
        $options[$value] = t($name);
      }
      $this->value_options = $options;
    }
  }
}
