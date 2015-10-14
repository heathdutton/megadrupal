<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Views integration.
 */

/**
 * Some fields are just references to values in a list. Let users filter
 * by any value from the list.
 */
class openx_manager_handler_filter_list extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }
    $this->value_options = _openx_manager_views_list($this->field);
  }
}
