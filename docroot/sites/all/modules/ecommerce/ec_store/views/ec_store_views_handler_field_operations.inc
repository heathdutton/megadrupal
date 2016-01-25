<?php

/**
 * @file
 * Provide list of operation links for transactions.
 */

class ec_store_views_handler_field_operations extends views_handler_field {

  function render($values) {
    if ($txn = ec_store_transaction_load($values->{$this->field_alias})) {
      drupal_add_css(drupal_get_path('module', 'ec_store') . '/css/views.css');
      return theme('ctools_dropdown', array('title' => t('Operations'), 'links' => module_invoke_all('link', 'ec_store_transaction', $txn)));
    }
  }
}
