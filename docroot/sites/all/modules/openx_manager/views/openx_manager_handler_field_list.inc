<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Views integration.
 */

/**
 * Some fields are just references to values in a list.
 */
class openx_manager_handler_field_list extends views_handler_field {
  function render($values) {
    $options = _openx_manager_views_list($this->field);
    return $options[$values->{$this->field_alias}];
  }
}
