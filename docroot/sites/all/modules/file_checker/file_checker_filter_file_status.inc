<?php
/**
 * @file file_checker_filter_file_status.inc
 * Adds and additional file status "Missing" for view filters.
 */
class file_checker_filter_file_status extends views_handler_filter_file_status {
  function get_value_options() {
    if (!isset($this->value_options)) {
      parent::get_value_options();
      // Add additional file status
      $this->value_options[FILE_STATUS_MISSING] = t('Missing');
    }
  }
}
