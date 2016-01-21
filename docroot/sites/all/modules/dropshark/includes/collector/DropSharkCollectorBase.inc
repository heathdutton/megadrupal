<?php

/**
 * Class DropSharkCollectorBase.
 */
abstract class DropSharkCollectorBase implements DropSharkCollector {

  protected $definition;
  protected $events = array();

  /**
   * {@inheritdoc}
   */
  public function __construct($definition) {
    $this->definition = $definition;
  }

  /**
   * {@inheritdoc}
   */
  public function finalize() {
    // Default to performing no action.
  }

  /**
   * Determine if the collector will respond to an event.
   *
   * This is a simple check of the event compared to a white list consisting of
   * the collector's $events property plus the value 'all'.
   *
   * Each collector extending this class should add its applicable event types
   * to the $events property and utilize this method for checking applicability.
   *
   * Alternatively, an extending collector may override this method to perform
   * more sophisticated logic or elect to not use this check.
   *
   * @param $event
   *   The event or type of event for which the collector is being polled.
   *
   * @return bool
   *   A value indicating if the collector will respond.
   */
  protected function respond($event) {
    $types = $this->events;
    $types[] = 'all';
    return in_array($event, $types);
  }

  /**
   * @return string
   */
  public function getServer() {
    // TODO: make this dynamic, configurable.
    return 'default';
  }

}
