<?php
/**
 * Validates whether an argument is a date (range) or not.
 */
class availability_calendar_plugin_argument_validate_date_range extends views_plugin_argument_validate {
  function validate_argument($argument) {
    if (module_exists('date_api')) {
      module_load_include('inc', 'date_api', 'date_api_sql');
      $date_handler = new date_sql_handler(DATE_UNIX);
      $date_parts = $date_handler->arg_parts($argument);
      $min_date = new DateObject($date_handler->complete_date($date_parts[0]['date'], 'min'));

      if (count($min_date->errors) > 0) {
        return FALSE;
      }
      else {
        $now = new DateObject();
        $now->setTime(0, 0, 0);
        if (count($date_parts) === 1) {
          return $min_date >= $now;
        }
        else {
          $max_date = new DateObject($date_handler->complete_date($date_parts[1]['date'], 'min'));
          return count($max_date->errors) === 0 && $min_date >= $now && $min_date <= $max_date;
        }
      }
    }
    else {
      $date_parts = explode('--', $argument);
      $min_date = availability_calendar_parse_iso_date($date_parts[0]);
      if (!$min_date) {
        return FALSE;
      }
      else {
        $now = new DateTime();
        $now->setTime(0, 0, 0);
        if (count($date_parts) === 1) {
          return $min_date >= $now;
        }
        else {
          $max_date = availability_calendar_parse_iso_date($date_parts[1]);
          return $max_date !== FALSE && $min_date >= $now && $min_date <= $max_date;
        }
      }
    }
  }
}
