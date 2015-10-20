<?php

/**
 * @file
 * Definition of apollo_xiii_handler_filter_string.
 */

/**
 * Filter to handle dates stored as a timestamp.
 *
 * @ingroup views_filter_handlers
 */
class apollo_xiii_handler_filter_string extends views_handler_filter_string {

  /**
   * Retrive the list of available operators.
   */
  public function operators() {
    $operators = parent::operators();
    // Remove no coded operators.
    $coded_operators = array('<', '<=', '=', '!=', '>=', '>');
    foreach ($operators as $operator => $operator_info) {
      if (!in_array($operator, $coded_operators)) {
        unset($operators[$operator]);
      }
    }
    return $operators;
  }
}
