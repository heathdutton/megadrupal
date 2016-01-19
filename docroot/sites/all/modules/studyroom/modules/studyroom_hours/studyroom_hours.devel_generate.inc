<?php

/**
 * @file
 * Module file for field generation for use with devel_generate
 */

/**
 * Support for processing hour fields in devel generate.
 */
function studyroom_hours_devel_generate($object, $field, $instance, $bundle) {
  $entity_field = array();

  for ($delta = 0; $delta < 7; $delta++) {
    if ($object->devel_random_time) {
      // @todo Apply site limits (and location limits) if they exists.
      $start_limit = studyroom_limit_start();
      $hour = mt_rand(substr($start_limit, 0, 2), 10);
      $date = new DateObject($hour . ':' . mt_rand(0, 59) . ':00');
      date_increment_round($date, variable_get('studyroom_time_increment', '15'));
      $entity_field[$delta]['start_time']['time'] = date_format($date, DATE_FORMAT_DATETIME);

      $max = 60 * (24 - $hour);
      $date->add(new DateInterval('PT' . mt_rand(240, $max) . 'M'));
      date_increment_round($date, variable_get('studyroom_time_increment', '15'));
      if (date_format_date($date, 'custom', 'H') < 12) {
        $date->add(new DateInterval("PT11H"));
      }

      $entity_field[$delta]['end_time']['time'] = date_format($date, DATE_FORMAT_DATETIME);
    }
    else {
      if (isset($instance['default_value'][$delta]['start_time'])) {
        $date = new DateObject($instance['default_value'][$delta]['start_time']);
        $entity_field[$delta]['start_time']['time'] = date_format($date, DATE_FORMAT_DATETIME);

        $date = new DateObject($instance['default_value'][$delta]['end_time']);
        $entity_field[$delta]['end_time']['time'] = date_format($date, DATE_FORMAT_DATETIME);
      }
      else {
        $entity_field[$delta]['start_time']['time'] = NULL;
        $entity_field[$delta]['end_time']['time'] = NULL;
      }
    }
  }
  return $entity_field;
}
