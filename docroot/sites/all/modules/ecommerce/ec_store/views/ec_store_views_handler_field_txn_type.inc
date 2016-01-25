<?php

/**
 * @file
 * Format output of transaction types.
 */

class ec_store_views_handler_field_txn_type extends views_handler_field {
  function render($values) {
    $types = & drupal_static(__FUNCTION__ . '_types', NULL);

    if (!$types) {
      $types = ec_store_transaction_types();
    }

    return check_plain(isset($types[$values->{$this->field_alias}]) ? $types[$values->{$this->field_alias}] : $values->{$this->field_alias});
  }
}
