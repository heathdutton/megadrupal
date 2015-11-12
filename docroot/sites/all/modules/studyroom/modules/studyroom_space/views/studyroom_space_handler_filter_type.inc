<?php

/**
 * @file
 * Definition of studyroom_space_handler_filter_type.
 */

/**
 * Contains a field handler to filter by types.
 */
class studyroom_space_handler_filter_type extends views_handler_filter_in_operator {
  /**
   * Display a list of product types in the filter's options.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Location');

      foreach (studyroom_space_locations() as $type => $space) {
        $options[$type] = $space->label();
      }
      $this->value_options = $options;

    }
  }
}
