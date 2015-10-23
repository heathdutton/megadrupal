<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_filter_type.
 */

/**
 * Contains a field filter handler to list reservations types.
 */
class studyroom_reservation_handler_filter_type extends views_handler_filter_in_operator {
  /**
   * Display a list of reservation types in the filter's options.
   */
  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Reservation type');
      $this->value_options = studyroom_reservation_types_options(FALSE);
    }
  }
}
