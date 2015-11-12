<?php

/**
 * @file
 * Definition of studyroom_reservation_handler_field_type.
 */

/**
 * Contains a field handler to display types.
 */
class studyroom_reservation_handler_field_type extends views_handler_field {
  /**
   * Render the field.
   */
  public function render($values) {
    $studyroom_reservation_type = studyroom_reservation_type_load($values->studyroom_reservation_type);
    return $studyroom_reservation_type->label;
  }

}
