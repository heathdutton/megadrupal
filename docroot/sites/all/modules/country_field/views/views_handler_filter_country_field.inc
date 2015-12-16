<?php

/**
 * @file
 * Contains views_handler_filter_country_field.
 */

/**
 * Filter by country field.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_country_field extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = _country_field_countries();
    }
  }
}
