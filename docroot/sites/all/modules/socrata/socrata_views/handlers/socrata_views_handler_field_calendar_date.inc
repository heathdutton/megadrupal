<?php
/**
 * @file
 * Based heavily on views_handler_field_date with the only major difference
 * being that Socrata stores this field type is returned in ISO 8601 format.
 */

class socrata_views_handler_field_calendar_date extends views_handler_field_date {
  /**
   * Generate output.
   */
  function render($values) {
    // Convert from milliseconds.
    // Casts are necessary as this came in as a string rep of an int64 value.
    $value = $this->get_value($values);
    $format = $this->options['date_format'];
    if (in_array($format, array(
      'custom',
      'raw time ago',
      'time ago',
      'raw time hence',
      'time hence',
      'raw time span',
      'time span',
      'raw time span',
      'inverse time span',
      'time span',
      ))) {
      $custom_format = $this->options['custom_date_format'];
    }

    if ($value) {
      $value = strtotime($value);
      $timezone = !empty($this->options['timezone']) ? $this->options['timezone'] : NULL;
      // Will be positive for a datetime in the past (ago),
      // and negative for a datetime in the future (hence).
      $time_diff = REQUEST_TIME - $value;
      switch ($format) {
        case 'raw time ago':
          return format_interval($time_diff, is_numeric($custom_format) ? $custom_format : 2);

        case 'time ago':
          return t('%time ago', array('%time' => format_interval($time_diff, is_numeric($custom_format) ? $custom_format : 2)));

        case 'raw time hence':
          return format_interval(-$time_diff, is_numeric($custom_format) ? $custom_format : 2);

        case 'time hence':
          return t('%time hence', array('%time' => format_interval(-$time_diff, is_numeric($custom_format) ? $custom_format : 2)));

        case 'raw time span':
          return ($time_diff < 0 ? '-' : '') . format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2);

        case 'inverse time span':
          return ($time_diff > 0 ? '-' : '') . format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2);

        case 'time span':
          return t(($time_diff < 0 ? '%time hence' : '%time ago'), array('%time' => format_interval(abs($time_diff), is_numeric($custom_format) ? $custom_format : 2)));

        case 'custom':
          if ($custom_format == 'r') {
            return format_date($value, $format, $custom_format, $timezone, 'en');
          }
          return format_date($value, $format, $custom_format, $timezone);

        default:
          return format_date($value, $format, '', $timezone);
      }
    }
  }
}
