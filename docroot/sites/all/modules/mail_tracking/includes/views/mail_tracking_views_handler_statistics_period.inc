<?php
/**
 * @file
 * Defines a Views handler for the statistics period.
 */

class mail_tracking_views_handler_statistics_period extends views_handler_field {

  /**
   * Render the field.
   *
   * @param $values
   *   The values retrieved from the database.
   *
   * @return
   *   Rendered value.
   */
  function render($values) {
    $value = $this->get_value($values);
    return _mail_tracking_format_period($value);
  }
}
