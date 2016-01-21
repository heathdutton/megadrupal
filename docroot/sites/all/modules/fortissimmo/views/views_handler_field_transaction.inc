<?php

/**
 * @file
 * Fortissimmo pand "Transaction" field handler
 */

class views_handler_field_transaction extends views_handler_field {
  function construct() {
    parent::construct();
    $transactions = fortissimmo_get_transactions();
    $this->transaction_mapping = $transactions;
  }

  function render($values) {
    return $this->transaction_mapping[$values->{$this->field_alias}];
  }
}