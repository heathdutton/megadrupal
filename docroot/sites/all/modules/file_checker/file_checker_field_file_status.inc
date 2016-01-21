<?php
/**
 * @file file_checker_filter_file_status.inc
 * Field handler to translate a file status into its readable form.
 */
class file_checker_field_file_status extends views_handler_field_file_status {
  function render($values) { // append missing file status
    if ($values->{$this->field_alias} == FILE_STATUS_MISSING) {
      return t('Missing');
    }
    else {
      return parent::render($values);
    }
  }
}
