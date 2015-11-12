<?php

/**
 * @file
 * Class implementation for FBAutopostEvent
 */

/**
 * Special case for publication type Event
 */
class FBAutopostEvent extends FBAutopost {
  /**
   * Prepares the parameters to publish to Facebook, this means setting any
   * field or destination dependent configuration.
   */
  protected function publishParameterPrepare(&$publication) {
    parent::publishParameterPrepare($publication);
    $timezone = new DateTimeZone(variable_get('date_default_timezone', 'Europe/London'));
    if (is_numeric($publication['params']['start_time'])) {
      $start = new DateTime('@' . $publication['params']['start_time'], $timezone);
      $publication['params']['start_time'] = $start->format(DateTime::ISO8601);
    }
    if (is_numeric($publication['params']['end_time'])) {
      $end = new DateTime('@' . $publication['params']['end_time'], $timezone);
      $publication['params']['end_time'] = $end->format(DateTime::ISO8601);
    }
  }
}
