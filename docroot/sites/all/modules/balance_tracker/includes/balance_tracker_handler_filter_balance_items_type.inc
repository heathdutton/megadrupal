<?php

/**
 * @file
 * Contains the credit/debit filter handler.
 */

/**
 * Handler to filter whether balance item is a credit or debit.
 *
 * @ingroup views
 */
class balance_tracker_handler_filter_balance_items_type extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Type');
      $options = array(
        'debit' => t('Debit'),
        'credit' => t('Credit'),
      );
    }
    $this->value_options = $options;
  }
}
