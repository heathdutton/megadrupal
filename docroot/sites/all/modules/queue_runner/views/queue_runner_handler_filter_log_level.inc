<?php

/**
 * Filter by item severity
 */
class queue_runner_handler_filter_log_level extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Severity');

      $this->value_options = watchdog_severity_levels();
    }
  }
}

