<?php

/**
 * @file
 * Contains the ObserverEvent class.
 */

/**
 * Class ObserverEvent
 *
 * Base class for events that occur within an Observable.
 */
class ObserverEvent implements ObserverEventInterface {

  protected $type;
  protected $data;

  /**
   * {@inheritdoc}
   */
  public function __construct($type, $data) {
    $this->type = $type;
    $this->data = $data;
  }

  /**
   * {@inheritdoc}
   */
  public function getType() {
    return $this->type;
  }

  /**
   * {@inheritdoc}
   */
  public function getData() {
    return $this->data;
  }

}
