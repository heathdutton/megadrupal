<?php

/**
 * @file
 */

/**
 * Expose query parameters as a context condition.
 */
class mock_context_condition_query_param extends context_condition_query_param {

  /**
   * Public wrapper for the check_query_params() method.
   */
  function test_check_query_params($request, $context) {
    return $this->check_query_params($request, $context);
  }
}
