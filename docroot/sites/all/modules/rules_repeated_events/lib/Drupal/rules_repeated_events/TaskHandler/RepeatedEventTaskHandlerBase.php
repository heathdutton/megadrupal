<?php

/**
 * @file
 * Contains RepeatedEventTaskHandlerBase.
 */

namespace Drupal\rules_repeated_events\TaskHandler;

use Drupal\fluxservice\Rules\TaskHandler\RepetitiveTaskHandlerBase;

/**
 * Base class for Contextio task handlers that dispatch Rules events.
 */
class RepeatedEventTaskHandlerBase extends RepetitiveTaskHandlerBase {

  /**
   * Gets the configured event name to dispatch.
   */
  public function getEvent() {
    return $this->task['identifier'];
  }

  /**
   * Invokes a rules event.
   */
  protected function invokeEvent() {
    rules_invoke_event($this->getEvent());
  }

  /**
   * {@inheritdoc}
   */
  public function runTask() {
    $this->invokeEvent();
  }

  public function getIncrement() {
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function afterTaskQueued() {
    if ($this->getIncrement()) {
      $scheduled_time = strtotime($this->task['data']['time']) - rules_repeated_events_get_local_server_timestamp_diff();
      // Continuously reschedule the task.
      db_update('rules_scheduler')
        ->condition('tid', $this->task['tid'])
        // Never use math like 60*60*24*7 to add/subtract days (because of daylight time saving)
        ->fields(array('date' => strtotime($this->getIncrement(), $scheduled_time)))
        ->execute();
    }
  }
}
