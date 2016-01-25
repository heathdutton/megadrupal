<?php

/**
 * @file
 * Provide list of operation links for a receipt.
 */

class ec_receipt_views_handler_field_operations extends views_handler_field {

  function render($values) {
    if ($receipt = ec_receipt_load($values->{$this->field_alias})) {
      return theme('ctools_dropdown', array('title' => t('Operations'), 'links' => module_invoke_all('link', 'ec_receipt', $receipt)));
    }
  }
}
