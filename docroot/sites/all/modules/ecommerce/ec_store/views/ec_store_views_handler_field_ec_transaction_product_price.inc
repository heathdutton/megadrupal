<?php

/**
 * @file
 * Views handler for displaying the price of a product on a transaction.
 */

/**
 * Field handler to provide simple renderer to display the customer name as
 * a single field
 */
class ec_store_views_handler_field_ec_transaction_product_price extends views_handler_field {
  /**
   * Override the init function to add the additional fields for the names
   */
  function init(&$view, &$data) {
    parent::init($view, $data);
    $this->additional_fields['txnid'] = 'txnid';
    $this->additional_fields['nid'] = 'nid';
  }

  function render($values) {
    if ($txn = ec_store_transaction_load($values->{$this->aliases['txnid']})) {
      return format_currency(ec_store_adjust_misc($txn, $txn->items[$values->{$this->aliases['nid']}]));
    }
  }
}
