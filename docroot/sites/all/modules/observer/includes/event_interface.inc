<?php

/**
 * @file
 * Contains the ObserverEventInterface interface.
 */

/**
 * Interface ObserverEventInterface
 *
 * The interface that defines the data wrapper that is passed to observers
 * on notification by an observable object.
 */
interface ObserverEventInterface {

  /**
   * Gets the event type.
   *
   * @return string
   */
  public function getType();

  /**
   * Gets the event data.
   *
   * @return mixed
   */
  public function getData();
}
