<?php

/**
 * @file
 * Definition of studyroom_space_handler_field_max_days_advance.
 */

/**
 * Field handler to format a max_days_advance.
 */
class studyroom_space_handler_field_max_days_advance extends views_handler_field_time_interval {
  /**
   * Construct form element.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['granularity']);
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $value = $values->{$this->field_alias};
    return studyroom_format_day($value);
  }
}
