<?php

/**
 * @file
 * Contains mock function definition of newrelic_record_custom_event() for the
 * sake of testing.
 */


/**
 * Mock implementation of the new relic custom event recording mechanism. If
 * called with no parameters, all previously recorded events will be returned.
 *
 * @param string $type
 *   The event type the given data represents.
 *
 * @param array $data
 *   The data to be stored.
 *
 * @param bool $reset
 *   TRUE if the statically stored values should be dumped and reset.
 *
 * @return array
 *   An array of associative arrays of data stored by callers.
 */
if (!function_exists('newrelic_record_custom_event')) {
  function newrelic_record_custom_event($type = FALSE, $data = FALSE, $reset = FALSE) {
    static $events = array();
    if ($reset) {
      $events = array();
    }
    if ($type !== FALSE && $data !== FALSE) {
      $events[$type][] = $data;
    }
    return $events;
  }

}
