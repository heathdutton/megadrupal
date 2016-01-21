<?php
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Views integration.
 */

/**
 * Display the Banner image
 */
class openx_manager_handler_image extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    $url = db_result(db_query('SELECT filepath FROM {files}
      WHERE fid = %d', $value));
    return theme('image', $url, $node->title);
  }
}
