<?php
/**
 * @file
 * Provide handler to replace status codes with text.
 */

class views_handler_filter_by_resources_status extends views_handler_filter_in_operator  {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_options = _resource_booking_field_get_rb_status();
    }
  }
}
