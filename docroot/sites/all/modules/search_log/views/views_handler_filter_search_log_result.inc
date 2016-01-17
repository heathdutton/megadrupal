<?php

/**
 * @file
 *  Search log Views filter by result.
 */

class views_handler_filter_search_log_result extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Result');
      $result_options = array(
        SEARCH_LOG_RESULT_SUCCESS => t('Success'),
        SEARCH_LOG_RESULT_UNKNOWN => t('Unknown'),
        SEARCH_LOG_RESULT_FAILED => t('Failed'),
      );
      $this->value_options = $result_options;
    }
  }
}
