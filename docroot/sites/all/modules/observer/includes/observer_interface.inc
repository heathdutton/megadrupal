<?php

/**
 * @file
 * Contains the ObserverObserverInterface interface.
 */

/**
 * Interface ObserverObserverInterface
 *
 * Defines the Observer interface from the Observer design pattern.
 */
interface ObserverObserverInterface {
  public function onNotify(ObserverEventInterface $event);
}
