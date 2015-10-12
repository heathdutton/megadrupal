<?php

/**
 * @file
 * Contains the area handler for render "variable_log_clear_form" form.
 */

/**
 * Area handler to provide "variable_log_clear_form" form.
 */
class variable_log_handler_area_clear_form extends views_handler_area {

  /**
   * Render the area.
   */
  public function render($empty = FALSE) {
    $output = '';
    if (!$empty || !empty($this->options['empty'])) {
      $form = drupal_get_form('variable_log_clear_form');
      $output = render($form);
    }
    return $output;
  }
}
