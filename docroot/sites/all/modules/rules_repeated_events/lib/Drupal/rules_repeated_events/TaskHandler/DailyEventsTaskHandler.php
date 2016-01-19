<?php

/**
 * @file
 * Contains DailyEventsTaskHandler.
 */

namespace Drupal\rules_repeated_events\TaskHandler;

/**
 * Event dispatcher for the Contextio contacts timeline of a given account.
 */
class DailyEventsTaskHandler extends RepeatedEventTaskHandlerBase {

  public function getIncrement() {
    return '+1 day';
  }

}
