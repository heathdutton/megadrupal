<?php

/**
 * @file
 * Defines a basic field handler to display field from.
 */

/**
 * Field handler to display a basic text field.
 */
class apollo_xiii_handler_field_machine_name extends views_handler_field_machine_name {

  /**
   * Used to keep field_alias synched with field name.
   */
  public function query() {
    $this->field_alias = $this->real_field;
  }

  /**
   * Filter the input to the right type.
   */
  public function inputFilter($value) {
    return (string) $value;
  }

}
