<?php

/**
 * @file
 * Argument handler for Fortissimmo ID
 */
class views_handler_argument_fortissimmo extends views_handler_argument_numeric {
  /**
   * Override the behavior of title(). Get the title of the property.
   */
  function title_query() {
    $property_id = NULL;
    if (is_array($this->value)) {
      $property_id = array_shift($this->value);
    }
    else {
      $property_id = $this->value;
    }

    if (is_numeric($property_id)) {
      return array(fortissimmo_property_get_title($property_id));
    }
    return array();
  }
}