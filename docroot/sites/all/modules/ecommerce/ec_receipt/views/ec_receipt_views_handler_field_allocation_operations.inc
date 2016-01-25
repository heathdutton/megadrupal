<?php

/**
 * @file
 * Handles list of operation links for a receipt.
 */

class ec_receipt_views_handler_field_allocation_operations extends views_handler_field {
  function render($values) {
    if ($receipt = db_query('SELECT * FROM {ec_receipt_allocation} WHERE eaid = :eaid', array(':eaid' => $values->{$this->field_alias}))->fetchObject()) {
      $links = module_invoke_all('link', 'ec_receipt_allocation', $receipt);
      if (!empty($links)) {
        return theme('ctools_dropdown', array('title' => t('Operations'), 'links' => $links));
      }
    }
  }
}
