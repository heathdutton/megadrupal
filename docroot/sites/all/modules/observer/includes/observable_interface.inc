<?php

/**
 * @file
 * Contains the ObserverObservableInterface interface.
 */

/**
 * Interface ObserverObservableInterface
 *
 * Defines the Observable interface from the Observer design pattern.
 */
interface ObserverObservableInterface {

  /**
   * Registers an observer.
   *
   * @param ObserverObserverInterface $observer
   * @return ObserverObservableInterface
   */
  public function registerObserver(ObserverObserverInterface $observer);

  /**
   * Unregisters an observer.
   *
   * @param ObserverObserverInterface $observer
   * @return ObserverObservableInterface
   */
  public function unregisterObserver(ObserverObserverInterface $observer);

  /**
   * Notifies all observers of an event.
   *
   * @param ObserverEventInterface $event
   * @return ObserverObservableInterface
   */
  public function notifyObservers(ObserverEventInterface $event);
}
